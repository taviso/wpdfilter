#ifndef __STREAM_H
#define __STREAM_H

class FilterStream: public WPXInputStream
{
    public:
        FilterStream(IStream* input);
        virtual ~FilterStream();
        virtual const unsigned char *read(unsigned long numBytes,
                                          unsigned long &numBytesRead);
        virtual int seek(long offset, WPX_SEEK_TYPE seekType);
        virtual long tell();
        virtual bool atEOS();
        virtual unsigned long getSize();
        virtual bool isOLEStream() {
            return false;
        }
        virtual FilterStream *getDocumentOLEStream(const char *) {
            return NULL;
        }

    private:
        IStream* m_pInput;
        void *buf;
        STATSTG stat;
};

#endif
