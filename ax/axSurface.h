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
 * \file axSurface.h
 * \brief surface class
*/ 

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axSurface_included
#define axSurface_included
//----------------------------------------------------------------------

#include "axCanvas.h"
//#include "axSurfaceBase.h"

/**

base surface class.
hardware/os/platform surface classes inherit from this one.

*/

class axSurfaceBase
{
  public:

    //int       mFlags;
    int       mWidth;
    int       mHeight;
    int       mDepth;
    axCanvas* mCanvas;

  public:

    axSurfaceBase(int aWidth, int aHeight/*, int aFlags=0*/)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        //mFlags  = aFlags;
        mDepth  = 24;
        mCanvas = NULL;
      };

    virtual ~axSurfaceBase()
      {
      };

    //int getHandle(void) { return (int)mHandle; }
    inline int getWidth(void) { return mWidth; }
    inline int getHeight(void) { return mHeight; }
    inline int getDepth(void) { return mDepth; }
    inline axCanvas* getCanvas(void) { return mCanvas; }

};

//----------------------------------------------------------------------

//typedef axSurfaceBase axSurface;

#ifdef linux
  #include "axSurfaceX11.h"
#endif

#ifdef WIN32
  #include "axSurfaceW32.h"
#endif

//----------------------------------------------------------------------


class axSurface : public axSurfaceImpl
{
  public:
    axSurface(int aWidth, int aHeight/*, int aFlags=0*/)
    : axSurfaceImpl(aWidth,aHeight/*,aFlags*/)
      {
      }

    virtual ~axSurface()
      {
      }

    //int getHandle(void) { return (int)mHandle; }

};

//----------------------------------------------------------------------
#endif