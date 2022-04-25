#include <new>
#include <stack>
#include <string>
#include <utility>
#include <stdio.h>
#include <windows.h>
#include <strsafe.h>
#include <shlwapi.h>

#include "libwpd/libwpd.h"
#include "libwpd-stream/libwpd-stream.h"
#include "stream.h"

// This is a quick attempt at mapping wpd streams onto an IStream.

bool FilterStream::atEOS()
{
    return this->tell() == this->getSize();
}

FilterStream::FilterStream(IStream* input)
{
    this->m_pInput = input;
    this->buf = NULL;
    this->m_pInput->Stat(&this->stat, STATFLAG_NONAME);
}

FilterStream::~FilterStream()
{
    free(this->buf);
}

// This is ridiculous, there is no way to signal error, and who owns the
// pointer I return? I'm going to assume the caller is only allowed to use it
// until next call?????
const unsigned char * FilterStream::read(unsigned long numBytes,
                                   unsigned long &numBytesRead)
{
    numBytesRead = 0;
    this->buf = realloc(this->buf, numBytes);

    if (numBytes == 0 || this->buf == NULL)
        return NULL;

    memset(this->buf, 0, numBytes);

    if (SUCCEEDED(this->m_pInput->Read(this->buf, numBytes, &numBytesRead)))
        return (const unsigned char *) this->buf;

    return NULL;
}

int FilterStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
    ULARGE_INTEGER pos;
    LARGE_INTEGER off;
    DWORD dwOrigin;

    // Guess what, these are not compatible, so translate them.
    switch (seekType) {
        case WPX_SEEK_CUR: dwOrigin = STREAM_SEEK_CUR; break;
        case WPX_SEEK_SET: dwOrigin = STREAM_SEEK_SET; break;
        case WPX_SEEK_END: dwOrigin = STREAM_SEEK_END; break;
        default: return -1;
    }

    off.QuadPart = offset;

    if (SUCCEEDED(this->m_pInput->Seek(off, dwOrigin, &pos))) {
        return pos.QuadPart;
    }

    return -1;
}

unsigned long FilterStream::getSize()
{
    return this->stat.cbSize.QuadPart;
}

long FilterStream::tell()
{
    ULARGE_INTEGER pos = {0};
    LARGE_INTEGER off = {0};

    if (SUCCEEDED(this->m_pInput->Seek(off, STREAM_SEEK_CUR, &pos)))
        return pos.QuadPart;

    return -1;
}
