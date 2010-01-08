#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------

#include "axBitmap.h"
#include "axSurface.h"

#include "../extern/lodepng.h"
#include "../extern/lodepng.cpp"

//TODO: split decoding & buffer preparation into separate function

axSurface* loadPng(unsigned char* buffer, unsigned int buffersize)
  {
    axBitmap* bmp = NULL;
    axSurface* srf = NULL;
    //unsigned char*  buffer;
    //unsigned int    buffersize;
    unsigned char*  image;
    unsigned int    imagesize;
    LodePNG_Decoder decoder;
    //LodePNG_loadFile(&buffer, &buffersize, filename);
    //buffer     =  binaural_back;
    //buffersize =  41373;
    LodePNG_Decoder_init(&decoder);
    LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
    if (!decoder.error)
    {
      int width = decoder.infoPng.width;
      int height = decoder.infoPng.height;
      //int depth = decoder.infoPng.color.bitDepth;
      //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
      bmp = new axBitmap(width,height,image);
      bmp->convertRgbaBgra();
      bmp->setBackground(128,128,128);
      srf = new axSurface(width,height,0);
      srf->mCanvas->drawBitmap(bmp, 0,0, 0,0,width,height);
      free(image);
      delete bmp;
    }
    LodePNG_Decoder_cleanup(&decoder);
    return srf;
  }


//----------------------------------------------------------------------
#endif
