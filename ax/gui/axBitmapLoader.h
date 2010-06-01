#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------
/*
  TODO:

  rework this into a more generic decoder class.
  and specific decoders being sub-clases of this

*/

//#include "axBitmap.h"
//#include "axSurface.h"

#include "../extern/picopng.h"
#include "stdio.h" // fread()

//----------------------------------------------------------------------

class axBitmapLoader
{
  public:
    unsigned long   mWidth;
    unsigned long   mHeight;
    unsigned char*  mImage;

  public:

    axBitmapLoader()
      {
        mWidth    = 0;
        mHeight   = 0;
        mImage    = NULL;
      }

    //----------

    virtual ~axBitmapLoader()
      {
        if (mImage) axFree(mImage);
      }

    //----------

    inline int            getWidth(void)  { return mWidth; }
    inline int            getHeight(void) { return mHeight; }
    inline unsigned char* getImage(void)  { return mImage; } //&mImage[0]; }

    //--------------------------------------------------

    int decode(unsigned char* buffer, unsigned int buffersize)
      {
        int res = 0;
        //if (mPngInfo) axFree(mPngInfo);
        axPngInfo* png = axPngDecode(buffer, buffersize);
        res = (int)png;
        mWidth  = png->width;
        mHeight = png->height;
        //mImage  = mPngInfo->image->data;
        // allocate our own chunk of data, and free all temporary png decoder things
        //trace("image size:  " << png->image->size ); // 256k
        //trace("image alloc: " << png->image->allocsize );  // 2mb
        mImage = (unsigned char*)axMalloc(png->image->size);
        axMemcpy(mImage,png->image->data,png->image->size);
        axPngFreeAll();                                                 // CHECK THIS !!!
        //axFree(png);
        //png->image->data[0] = 0;
        return res;//(int)mPngInfo;
      }

    //--------------------------------------------------

    int decodeLoad(const char* _file)
      {
        #ifdef WIN32
          // get dll/exe path on windows
          char path [MAX_PATH] = "";
          char file [MAX_PATH] = "";
          #ifdef AX_FORMAT_VST
            GetModuleFileName(gInstance, path, MAX_PATH);
          #endif
          #ifdef AX_FORMAT_EXE
            GetModuleFileName(NULL, path, MAX_PATH);
          #endif
          axStrncpy(file, path, (axStrrchr(path, '\\') + 1) - (char*)path);
          axStrcat(file, (char*)_file);
        #endif
        #ifdef linux
          // should work ok with relative paths on linux
          char* file = (char*)_file;
        #endif
        FILE* f = fopen(file, "rb");
        if (!f)
        {
          trace("decodeLoad(), #ERR missing: " << file);
          return 0;
        }
        fseek(f, 0, SEEK_END);
        unsigned long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        if (!size)
        {
          trace("decodeLoad(), #ERR null sized: " << file);
          return 0;
        }
        unsigned char* b = (unsigned char*)axMalloc(size);
        unsigned int res = fread(b, size, 1, f);
        if (!res)
        {
          trace("decodeLoad(), #ERR file read: " << file);
          return 0;
        }
        if (b[0] != 0x89 || b[1] != 0x50 || b[2] != 0x4E || b[3] != 0x47)
        {
          trace("decodeLoad(), #ERR not a png: " << file);
          return 0;
        }
        else
        {
          fclose(f);
          decode(b, size);
          axFree(b);
          trace("decodeLoad(), " << file << ", " << size);
          return 1;
        }
      }

};

//----------------------------------------------------------------------
#endif

