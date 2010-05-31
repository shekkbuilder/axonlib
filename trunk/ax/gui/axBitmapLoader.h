#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------
/*
  TODO:

  rework this into a more generic decoder class.
  and specific decoders being sub-clases of this

*/
#include "axBitmap.h"
#include "axSurface.h"
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

        trace("image size:  " << png->image->size ); // 256k
        trace("image alloc: " << png->image->allocsize );  // 2mb
        mImage = (unsigned char*)axMalloc(png->image->size);
        axMemcpy(mImage,png->image->data,png->image->size);
        axPngFreeAll();                                                 // CHECK THIS !!!
        //axFree(png);
        //trace("png: " << png );
        //trace("png image: " << png->image );
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

    //--------------------------------------------------

    //axSurface* upload(void) { return NULL;}
    //axSurface* load(unsigned char* buffer, unsigned int buffersize) { return NULL; }

};

//----------------------------------------------------------------------
#endif

/*

// can this be used for samples too? like wav, like an 1d picture
// width  = length in samples
// height = 1
// depth  = (format?)
// buffer = cast to SPL*

class axDecoderBase
{
  protected:
    int   mWidth;
    int   mHeight;
    int   mDepth;
    char* mBuffer;
  public:
    axDecoderBase()
    {
      mWidth  = 0;
      mHeight = 0;
      mDepth  = 32;
      mBuffer = NULL;
    }
    virtual ~axDecoderBase()
    {
      if (mBuffer) delete mBuffer;
    }
    inline int   getWidth(void)   { return mWidth; }
    inline int   getHeight(void)  { return mHeight; }
    inline int   getDepth(void)   { return mDepth; }
    inline char* getBuffer(void)  { return mBuffer; }
    virtual bool canDecode(char* aBuffer, int aSize) { return false; }
    virtual int  decode(char* aBuffer, int aSize) { return -1; }
};

//----------

class axDecoderPng : public axDecoderBase
{
  public:
    axDecoderPng() : axDecoderBase() {}
};

// tga, pcx, bmp, raw

*/

//----------------------------------------------------------------------
//
//
//
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

/*

9. error values
---------------

The meanings of the LodePNG error values:

*) 0: no error, everything went ok
*) 1: the Encoder/Decoder has done nothing yet, so error checking makes no sense yet
*) 10: while huffman decoding: end of input memory reached without endcode
*) 11: while huffman decoding: error in code tree made it jump outside of tree
*) 13: problem while processing dynamic deflate block
*) 14: problem while processing dynamic deflate block
*) 15: problem while processing dynamic deflate block
*) 16: unexisting code while processing dynamic deflate block
*) 17: while inflating: end of out buffer memory reached
*) 18: while inflating: invalid distance code
*) 19: while inflating: end of out buffer memory reached
*) 20: invalid deflate block BTYPE encountered while decoding
*) 21: NLEN is not ones complement of LEN in a deflate block
*) 22: while inflating: end of out buffer memory reached.
   This can happen if the inflated deflate data is longer than the amount of bytes required to fill up
   all the pixels of the image, given the color depth and image dimensions. Something that doesn't
   happen in a normal, well encoded, PNG image.
*) 23: while inflating: end of in buffer memory reached
*) 24: invalid FCHECK in zlib header
*) 25: invalid compression method in zlib header
*) 26: FDICT encountered in zlib header while it's not used for PNG
*) 27: PNG file is smaller than a PNG header
*) 28: incorrect PNG signature (the first 8 bytes of the PNG file)
   Maybe it's not a PNG, or a PNG file that got corrupted so that the header indicates the corruption.
*) 29: first chunk is not the header chunk
*) 30: chunk length too large, chunk broken off at end of file
*) 31: illegal PNG color type or bpp
*) 32: illegal PNG compression method
*) 33: illegal PNG filter method
*) 34: illegal PNG interlace method
*) 35: chunk length of a chunk is too large or the chunk too small
*) 36: illegal PNG filter type encountered
*) 37: illegal bit depth for this color type given
*) 38: the palette is too big (more than 256 colors)
*) 39: more palette alpha values given in tRNS, than there are colors in the palette
*) 40: tRNS chunk has wrong size for greyscale image
*) 41: tRNS chunk has wrong size for RGB image
*) 42: tRNS chunk appeared while it was not allowed for this color type
*) 43: bKGD chunk has wrong size for palette image
*) 44: bKGD chunk has wrong size for greyscale image
*) 45: bKGD chunk has wrong size for RGB image
*) 46: value encountered in indexed image is larger than the palette size (bitdepth == 8). Is the palette too small?
*) 47: value encountered in indexed image is larger than the palette size (bitdepth < 8). Is the palette too small?
*) 48: the input data is empty. Maybe a PNG file you tried to load doesn't exist or is in the wrong path.
*) 49: jumped past memory while generating dynamic huffman tree
*) 50: jumped past memory while generating dynamic huffman tree
*) 51: jumped past memory while inflating huffman block
*) 52: jumped past memory while inflating
*) 53: size of zlib data too small
*) 55: jumped past tree while generating huffman tree, this could be when the
       tree will have more leaves than symbols after generating it out of the
       given lenghts. They call this an oversubscribed dynamic bit lengths tree in zlib.
*) 56: given output image colorType or bitDepth not supported for color conversion
*) 57: invalid CRC encountered (checking CRC can be disabled)
*) 58: invalid ADLER32 encountered (checking ADLER32 can be disabled)
*) 59: conversion to unexisting or unsupported color type or bit depth requested by encoder or decoder
*) 60: invalid window size given in the settings of the encoder (must be 0-32768)
*) 61: invalid BTYPE given in the settings of the encoder (only 0, 1 and 2 are allowed)
*) 62: conversion from non-greyscale color to greyscale color requested by encoder or decoder. LodePNG
       leaves the choice of RGB to greyscale conversion formula to the user.
*) 63: length of a chunk too long, max allowed for PNG is 2147483647 bytes per chunk (2^31-1)
*) 64: the length of the "end" symbol 256 in the Huffman tree is 0, resulting in the inability of a deflated
       block to ever contain an end code. It must be at least 1.
*) 66: the length of a text chunk keyword given to the encoder is longer than the maximum 79 bytes.
*) 67: the length of a text chunk keyword given to the encoder is smaller than the minimum 1 byte.
*) 68: tried to encode a PLTE chunk with a palette that has less than 1 or more than 256 colors
*) 69: unknown chunk type with "critical" flag encountered by the decoder
*) 71: unexisting interlace mode given to encoder (must be 0 or 1)
*) 72: while decoding, unexisting compression method encountering in zTXt or iTXt chunk (it must be 0)
*) 73: invalid tIME chunk size
*) 74: invalid pHYs chunk size
*) 75: no null termination char found while decoding any kind of text chunk, or wrong length
*) 76: iTXt chunk too short to contain required bytes
*) 77: integer overflow in buffer size happened somewhere
*) 78: file doesn't exist or couldn't be opened for reading
*) 79: file couldn't be opened for writing
*) 80: tried creating a tree for 0 symbols
*) 9900-9999: out of memory while allocating chunk of memory somewhere



*/
