/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file axBitmapLoader.h
  \brief bitmap loader & decoder
*/

#ifndef axBitmapLoad_included
#define axBitmapLoad_included
//----------------------------------------------------------------------

#include "axBitmap.h"
#include "axSurface.h"

//#include "../extern/lodepng.h"
//#include "../extern/lodepng.cpp"
#include "../extern/picopng.cpp"

//----------------------------------------------------------------------

////TODO: deprecate
//axSurface* loadPng(unsigned char* buffer, unsigned int buffersize)
//  {
//    axBitmap* bmp = NULL;
//    axSurface* srf = NULL;
//    //unsigned char*  buffer;
//    //unsigned int    buffersize;
//    unsigned char*  image;
//    unsigned int    imagesize;
//    LodePNG_Decoder decoder;
//    //LodePNG_loadFile(&buffer, &buffersize, filename);
//    //buffer     =  binaural_back;
//    //buffersize =  41373;
//    LodePNG_Decoder_init(&decoder);
//    LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
//    if (!decoder.error)
//    {
//      int width = decoder.infoPng.width;
//      int height = decoder.infoPng.height;
//      //int depth = decoder.infoPng.color.bitDepth;
//      //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
//      bmp = new axBitmap(width,height,image);
//      bmp->convertRgbaBgra();
//      bmp->setBackground(128,128,128);
//      srf = new axSurface(width,height/*,0*/);
//      srf->mCanvas->drawBitmap(bmp, 0,0, 0,0,width,height);
//      free(image);
//      delete bmp;
//    }
//    LodePNG_Decoder_cleanup(&decoder);
//    return srf;
//  }

//----------

//  axBitmap* bmp = decodePng(buffer,size);
//  bmp->convertRgbaBgra();
//  bmp->setBackground(128,128,128);
//  axSurface* srf = uploadBitmap(bmp);
//  delete bmp;

//----------

//axBitmap* decodePng(unsigned char* buffer, unsigned int buffersize)
//  {
//    unsigned char*  image;
//    unsigned int    imagesize;
//    axBitmap*       bmp = NULL;
//    LodePNG_Decoder decoder;
//    LodePNG_Decoder_init(&decoder);
//    LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
//    if (!decoder.error)
//    {
//      int width  = decoder.infoPng.width;
//      int height = decoder.infoPng.height;
//      bmp = new axBitmap(width,height,image);
//      //bmp->convertRgbaBgra();
//      free(image);
//    }
//    LodePNG_Decoder_cleanup(&decoder);
//    return bmp;
//  }

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// int decodePNG( std::vector<unsigned char>& out_image_32bit,
//                unsigned long& image_width,
//                unsigned long& image_height,
//                const unsigned char* in_png,
//                unsigned long in_size )

// decode a pre-loaded buffer containing png file
axBitmap* decodePng(unsigned char* buffer, unsigned int buffersize)
  {
    unsigned long width, height;
    std::vector<unsigned char> image;
    /*int error = */decodePNG(image, width, height, buffer, buffersize);
    axBitmap* bmp = new axBitmap(width,height,&image[0]);
    return bmp;
  }

//----------

// blit bitmap to surface
// (upload to vram/xserver)
axSurface* uploadBitmap(axBitmap* bmp)
{
  axSurface* srf = new axSurface(bmp->getWidth(),bmp->getHeight());
  //bmp->prepare();
  srf->mCanvas->drawBitmap(bmp,0,0,0,0,bmp->getWidth(),bmp->getHeight());
  return srf;
  //delete bmp;
}

//----------

// [helper] decode & upload
axSurface* loadPng(unsigned char* buffer, unsigned int buffersize)
  {
    axBitmap* bmp = decodePng(buffer,buffersize);
      //TODO: remove these!
      // caller's responsibility!
      // i just put them here (temporary) to not break every gui plugin :-/
      bmp->convertRgbaBgra();
      bmp->setBackground(128,128,128);
      //
    bmp->prepare();
    axSurface* srf = uploadBitmap(bmp);
    delete bmp;
    return srf;
  }

//----------

//untested!
//in:
//  aFilename = name (and path) of file
//out:
//  aBuffer = ptr to allocated buffer,
//            you must free it yourself, free(*aBuffer);
//  return  = size of buffer

// load file into memory
#include <stdio.h>
int loadFile(char* aFilename, unsigned char** aBuffer)
{
	//int size = 0;
	FILE *f = fopen(aFilename,"rb");
	if (f==NULL)
	{
		*aBuffer = NULL;
		return -1; // file opening fail
	}
	fseek(f,0,SEEK_END);
	unsigned int size = ftell(f);
	fseek(f,0,SEEK_SET);
	//*aBuffer = (char*)malloc(size+1);
	*aBuffer = (unsigned char*)malloc(size);
	if ( size != fread(*aBuffer,sizeof(char),size,f) )
	{
		free(*aBuffer);
		return -2; // file reading fail
	}
	fclose(f);
	//(*aBuffer)[size] = 0;
	return size;
}

//----------------------------------------------------------------------
#endif
