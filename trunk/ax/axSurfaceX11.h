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
 * \file axSurfaceX11.h
 * \brief surface class x11
*/ 

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axSurfaceX11_included
#define axSurfaceX11_included
//----------------------------------------------------------------------
// pixmap

#include "axSurface.h"

class axSurfaceImpl : public axSurfaceBase
{
  public:

    Pixmap    mHandle;
    //axCanvas* mCanvas;

  public:

    //----------

    axSurfaceImpl(int aWidth, int aHeight/*, int aFlags=0*/)
    : axSurfaceBase(aWidth, aHeight/*, aFlags*/)
      {
        mDepth  = DefaultDepth(gDP,DefaultScreen(gDP));
        mHandle = XCreatePixmap(gDP,XDefaultRootWindow(gDP),mWidth,mHeight,mDepth);
        mCanvas = new axCanvas(mHandle,cmo_buffer);
      }

    //----------

    virtual ~axSurfaceImpl()
      {
        delete mCanvas;
        // The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
        // Then, the X server frees the pixmap storage when there are no references to it.
        XFreePixmap(gDP,mHandle);
      }

};

//----------------------------------------------------------------------
#endif

