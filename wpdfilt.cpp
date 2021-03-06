#include <windows.h>
#include <new>
#include <shlwapi.h>

#define SZ_WPD_CLSID L"{a7599c83-4272-470d-b107-1ccc47711d6b}"
#define SZ_WPD_HANDLER L"{1a19048a-7a86-494a-b49e-12958b6eed4f}"

HRESULT CWordperfectFilter_CreateInstance(REFIID riid, void **ppv);

// Handle to the DLL's module
HINSTANCE g_hInst = NULL;

// Module Ref count
long c_cRefModule = 0;

void DllAddRef()
{
    InterlockedIncrement(&c_cRefModule);
}

void DllRelease()
{
    InterlockedDecrement(&c_cRefModule);
}

class CClassFactory : public IClassFactory
{
public:
    CClassFactory(REFCLSID clsid) : m_cRef(1), m_clsid(clsid)
    {
        DllAddRef();
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CClassFactory, IClassFactory),
            { 0 }
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0)
        {
            delete this;
        }
        return cRef;
    }

    // IClassFactory
    IFACEMETHODIMP CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv)
    {
        *ppv = NULL;
        HRESULT hr;
        if (punkOuter)
        {
            hr = CLASS_E_NOAGGREGATION;
        }
        else
        {
            CLSID clsid;
            if (SUCCEEDED(CLSIDFromString(SZ_WPD_CLSID, &clsid)) && IsEqualCLSID(m_clsid, clsid))
            {
                hr = CWordperfectFilter_CreateInstance(riid, ppv);
            }
            else
            {
                hr = CLASS_E_CLASSNOTAVAILABLE;
            }
        }
        return hr;
    }

    IFACEMETHODIMP LockServer(BOOL bLock)
    {
        if (bLock)
        {
            DllAddRef();
        }
        else
        {
            DllRelease();
        }
        return S_OK;
    }

private:
    ~CClassFactory()
    {
        DllRelease();
    }

    long m_cRef;
    CLSID m_clsid;
};

// Standard DLL functions
STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void *)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        g_hInst = hInstance;
        DisableThreadLibraryCalls(hInstance);
    }
    return TRUE;
}

STDAPI DllCanUnloadNow()
{
    return (c_cRefModule == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    *ppv = NULL;
    CClassFactory *pClassFactory = new (std::nothrow) CClassFactory(clsid);
    HRESULT hr = pClassFactory ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        hr = pClassFactory->QueryInterface(riid, ppv);
        pClassFactory->Release();
    }
    return hr;
}

// A struct to hold the information required for a registry entry
struct REGISTRY_ENTRY
{
    HKEY    hkeyRoot;
    PCWSTR pszKeyName;
    PCWSTR pszValueName;
    PCWSTR pszData;
};

// Creates a registry key (if needed) and sets the default value of the key
HRESULT CreateRegKeyAndSetValue(const REGISTRY_ENTRY *pRegistryEntry)
{
    HRESULT hr;
    HKEY hKey;

    LONG lRet = RegCreateKeyExW(pRegistryEntry->hkeyRoot, pRegistryEntry->pszKeyName,
                                0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (lRet != ERROR_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(lRet);
    }
    else
    {
        lRet = RegSetValueExW(hKey, pRegistryEntry->pszValueName, 0, REG_SZ,
                            (LPBYTE) pRegistryEntry->pszData,
                            ((DWORD) wcslen(pRegistryEntry->pszData) + 1) * sizeof(WCHAR));

        hr = HRESULT_FROM_WIN32(lRet);

        RegCloseKey(hKey);
    }

    return hr;
}

// Registers this COM server
STDAPI DllRegisterServer()
{
    HRESULT hr;

    WCHAR szModuleName[MAX_PATH];

    if (!GetModuleFileNameW(g_hInst, szModuleName, ARRAYSIZE(szModuleName)))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    else
    {
        // List of registry entries we want to create

        const REGISTRY_ENTRY rgRegistryEntries[] =
        {
            // RootKey             KeyName                                                                                  ValueName           Data
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_CLSID,                                     NULL,               L"WordPerfect DOS"},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_CLSID L"\\InProcServer32",                 NULL,               szModuleName},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_CLSID L"\\InProcServer32",                 L"ThreadingModel",  L"Both"},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_HANDLER,                                   NULL,               L"WordPerfect DOS Persistent Handler"},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_HANDLER L"\\PersistentAddinsRegistered",   NULL,               L""},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\CLSID\\" SZ_WPD_HANDLER L"\\PersistentAddinsRegistered\\{89BCB740-6119-101A-BCB7-00DD010655AF}", NULL, SZ_WPD_CLSID},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\.wpd",                                                     NULL,               L"WordPerfect DOS Document"},
            {HKEY_LOCAL_MACHINE,   L"Software\\Classes\\.wpd\\PersistentHandler",                                  NULL,               SZ_WPD_HANDLER},
        };

        hr = S_OK;

        for (int i = 0; i < ARRAYSIZE(rgRegistryEntries) && SUCCEEDED(hr); i++)
        {
            hr = CreateRegKeyAndSetValue(&rgRegistryEntries[i]);
        }
    }
    return hr;
}

// Unregisters this COM server
STDAPI DllUnregisterServer()
{
    HRESULT hr = S_OK;

    const PCWSTR rgpszKeys[] =
    {
        L"Software\\Classes\\CLSID\\" SZ_WPD_CLSID,
        L"Software\\Classes\\CLSID\\" SZ_WPD_HANDLER,
        L"Software\\Classes\\.wpd"
    };

    // Delete the registry entries
    for (int i = 0; i < ARRAYSIZE(rgpszKeys) && SUCCEEDED(hr); i++)
    {
        DWORD dwError = RegDeleteTree(HKEY_LOCAL_MACHINE, rgpszKeys[i]);
        if (ERROR_FILE_NOT_FOUND == dwError)
        {
            // If the registry entry has already been deleted, say S_OK.
            hr = S_OK;
        }
        else
        {
            hr = HRESULT_FROM_WIN32(dwError);
        }
    }
    return hr;
}
