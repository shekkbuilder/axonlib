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
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axSurfaceW32_included
#define axSurfaceW32_included
//----------------------------------------------------------------------
// pixmap

#include "axSurface.h"

class axSurfaceImpl : public axSurfaceBase
{
  public:

//    Pixmap    mHandle;
//    //axCanvas* mCanvas;
    HBITMAP mHandle, mOldHandle;

  public:

    //----------

    axSurfaceImpl(int aWidth, int aHeight/*, int aFlags=0*/)
    : axSurfaceBase(aWidth, aHeight/*, aFlags*/)
      {
        //mDepth = 0;
        HDC tempdc = GetDC(0);                                    // ...if this value is NULL, GetDC retrieves the device context for the entire screen...
        mHandle = CreateCompatibleBitmap(tempdc,mWidth,mHeight);  // creates a bitmap compatible with the device associated with the specified device context.
        ReleaseDC(0,tempdc);                                      // releases a device context (DC), freeing it for use by other applications.
        mCanvas = new axCanvas(0,cmo_buffer);
        mOldHandle = (HBITMAP)SelectObject(mCanvas->mDC,mHandle);
      }

    //----------

    virtual ~axSurfaceImpl()
      {
        SelectObject(mCanvas->mDC,mOldHandle);
        delete mCanvas;
        DeleteObject(mHandle);
      }

};

//----------------------------------------------------------------------
#endif

