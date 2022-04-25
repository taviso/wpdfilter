// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <new>
#include <stack>
#include <string>
#include <utility>
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <xmllite.h>

#include "FilterBase.h"
#include "libwpd/libwpd.h"
#include "libwpd-stream/libwpd-stream.h"
#include "callbacks.h"
#include "stream.h"

void DllAddRef();
void DllRelease();

class CWordperfectFilter : public CFilterBase
{
public:
    CWordperfectFilter() : m_cRef(1), m_pReader(NULL)
    {
        DllAddRef();
    }

    ~CWordperfectFilter()
    {
        if (m_pReader)
        {
            m_pReader->Release();
        }
        DllRelease();
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CWordperfectFilter, IInitializeWithStream),
            QITABENT(CWordperfectFilter, IFilter),
            {0, 0 },
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

    virtual HRESULT OnInit();
    virtual HRESULT GetNextChunkValue(CChunkValue &chunkValue);

private:
    long m_cRef;
    FilterStream *m_wpdStream;
    FilterCallbacks *m_wpdCallbacks;
    CWordperfectFilter *m_pReader;
};

HRESULT CWordperfectFilter_CreateInstance(REFIID riid, void **ppv)
{
    HRESULT hr = E_OUTOFMEMORY;
    fprintf(stderr, "CWordperfectFilter_CreateInstance()\n");
    CWordperfectFilter *pFilter = new (std::nothrow) CWordperfectFilter();
    if (pFilter)
    {
        hr = pFilter->QueryInterface(riid, ppv);
        pFilter->Release();
    }
    return hr;
}

// This is called after the stream (m_pStream) has been setup and is ready for use
// This implementation of this filter passes that stream to the xml reader
HRESULT CWordperfectFilter::OnInit()
{
    WPDConfidence confidence;
    WPDResult error;

    fprintf(stderr, "CWordperfectFilter::OnInit()\n");
    this->m_wpdStream = new FilterStream(this->m_pStream);
    this->m_wpdCallbacks = new FilterCallbacks();

    confidence = WPDocument::isFileFormatSupported(this->m_wpdStream);

    if (confidence != WPD_CONFIDENCE_EXCELLENT) {
        return E_FAIL;
    }

    // Parse the file.
    error = WPDocument::parse(this->m_wpdStream, this->m_wpdCallbacks, NULL);

    if (error != WPD_OK) {
        return E_FAIL;
    }

    return S_OK;
}


// When GetNextChunkValue() is called we fill in the ChunkValue by calling
// SetXXXValue() with the property and value (and other parameters that you
// want) example:  chunkValue.SetTextValue(PKEY_ItemName, L"example text");
// return FILTER_E_END_OF_CHUNKS when there are no more chunks
HRESULT CWordperfectFilter::GetNextChunkValue(CChunkValue &chunkValue)
{
    if (!this->m_wpdCallbacks->props.empty()) {
        chunkValue.SetTextValue(this->m_wpdCallbacks->props.top().first,
                                this->m_wpdCallbacks->props.top().second.c_str());
        this->m_wpdCallbacks->props.pop();
        return S_OK;
    }

    if (!this->m_wpdCallbacks->searchtext.empty()) {
        chunkValue.SetTextValue(PKEY_Search_Contents,
                                this->m_wpdCallbacks->searchtext.c_str(),
                                CHUNK_TEXT);
        this->m_wpdCallbacks->searchtext.clear();
        return S_OK;
    }

    return FILTER_E_END_OF_CHUNKS;
}
