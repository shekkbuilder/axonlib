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

