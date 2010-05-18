#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------
//TODO: a lot

#include "axBitmap.h"
#include "axSurface.h"

//#include "../extern/picopng.cpp"
// lii: try to use picopng.c instead
#include "../extern/picopng.h"

//----------------------------------------------------------------------

//// decode a pre-loaded buffer containing png file
//axBitmap* decodePng(unsigned char* buffer, unsigned int buffersize)
//  {
//    unsigned long width, height;
//    std::vector<unsigned char> image;
//    /*int error = */decodePNG(image, width, height, buffer, buffersize);
//    axBitmap* bmp = new axBitmap(width,height,&image[0]);
//    return bmp;
//  }

//----------

//// blit bitmap to surface
//// (upload to vram/xserver)
//axSurface* uploadBitmap(axBitmap* bmp)
//{
//  axSurface* srf = new axSurface(bmp->getWidth(),bmp->getHeight());
//  //bmp->prepare();
//  srf->mCanvas->drawBitmap(bmp,0,0,0,0,bmp->getWidth(),bmp->getHeight());
//  return srf;
//  //delete bmp;
//}

//----------

//// [helper] decode & upload
//axSurface* loadPng(unsigned char* buffer, unsigned int buffersize)
//  {
//    axBitmap* bmp = decodePng(buffer,buffersize);
//      //TODO: remove these!
//      // caller's responsibility!
//      // i just put them here (temporary) to not break every gui plugin :-/
//      bmp->convertRgbaBgra();
//      bmp->setBackground(128,128,128);
//      //
//    bmp->prepare();
//    axSurface* srf = uploadBitmap(bmp);
//    delete bmp;
//    return srf;
//  }

//----------

//untested!
//in:
//  aFilename = name (and path) of file
//out:
//  aBuffer = ptr to allocated buffer,
//            you must free it yourself, free(*aBuffer);
//  return  = size of buffer

//// load file into memory
//#include <stdio.h>
//int loadFile(char* aFilename, unsigned char** aBuffer)
//{
//	//int size = 0;
//	FILE *f = fopen(aFilename,"rb");
//	if (f==NULL)
//	{
//		*aBuffer = NULL;
//		return -1; // file opening fail
//	}
//	fseek(f,0,SEEK_END);
//	unsigned int size = ftell(f);
//	fseek(f,0,SEEK_SET);
//	//*aBuffer = (char*)malloc(size+1);
//	*aBuffer = (unsigned char*)malloc(size);
//	if ( size != fread(*aBuffer,sizeof(char),size,f) )
//	{
//		free(*aBuffer);
//		return -2; // file reading fail
//	}
//	fclose(f);
//	//(*aBuffer)[size] = 0;
//	return size;
//}

//----------------------------------------------------------------------

class axBitmapLoader
{
  private:
    axPngInfo*     mPngInfo;
  public:
    unsigned long   mWidth;
    unsigned long   mHeight;
    unsigned char*  mImage;
    //std::vector<unsigned char> image;
    // lii: pass this pointer when using getImage instead
    //unsigned char* local_buffer;

  public:

    axBitmapLoader()
      {
        mPngInfo  = NULL;
        mWidth    = 0;
        mHeight   = 0;
        mImage    = NULL;
      }

    virtual ~axBitmapLoader()
      {
        if (mPngInfo) axFree(mPngInfo);
      }

    //----------

    inline int            getWidth(void)  { return mWidth; }
    inline int            getHeight(void) { return mHeight; }
    inline unsigned char* getImage(void)  { return &mImage[0]; }
    //inline unsigned char* getImage(void)  { return local_buffer; }

    //----------

    int decode(unsigned char* buffer, unsigned int buffersize)
      {
        //local_buffer = buffer; // (test) set address here
        //int error = decodePNG(image, width, height, buffer, buffersize);
        //int error = (int)PNG_decode(buffer, buffersize);
        if (mPngInfo) axFree(mPngInfo);
        PNG_info_t* mPngInfo = axPngDecode(buffer, buffersize);
        mWidth  = mPngInfo->width;
        mHeight = mPngInfo->height;
        mImage  = mPngInfo->image->data;
        return (int)mPngInfo;
      }

    //----------

    //axSurface* upload(void) { return NULL;}
    //axSurface* load(unsigned char* buffer, unsigned int buffersize) { return NULL; }

};

//----------------------------------------------------------------------
#endif
