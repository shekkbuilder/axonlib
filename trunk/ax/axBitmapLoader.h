/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
  \file axBitmapLoad.h
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

axBitmap* decodePng(unsigned char* buffer, unsigned int buffersize)
  {
    std::vector<unsigned char> image;
    //loadFile(buffer, filename);
    unsigned long width, height;
    /*int error = */decodePNG(image, width, height, buffer, buffersize);
    //if (error!=0) std::cout << "error: " << error << std::endl;
    //the pixels are now in the vector "image", use it as texture, draw it, ...
    //if (image.size() > 4)
    //  std::cout << "width: " << w << " height: " << h << " first pixel: " << std::hex << int(image[0]) << int(image[1]) << int(image[2]) << int(image[3]) << std::endl;
    axBitmap* bmp = new axBitmap(width,height,&image[0]);
    return bmp;
  }

//----------

axSurface* uploadBitmap(axBitmap* bmp)
{
  axSurface* srf = new axSurface(bmp->getWidth(),bmp->getHeight()/*,0*/);
  srf->mCanvas->drawBitmap(bmp,0,0,0,0,bmp->getWidth(),bmp->getHeight());
  return srf;
  //delete bmp;
}

//----------

axSurface* loadPng(unsigned char* buffer, unsigned int buffersize)
  {
    axBitmap*  bmp = decodePng(buffer,buffersize);
    //TODO: remove these!
    // caller's responsibility!
    // just ut them here to not break every gui plugin :-/
      bmp->convertRgbaBgra();
      bmp->setBackground(128,128,128);
    //
    axSurface* srf = uploadBitmap(bmp);
    delete bmp;
    return srf;
  }

//----------------------------------------------------------------------
#endif
