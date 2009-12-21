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

    axSurfaceImpl(int aWidth, int aHeight, int aFlags=0)
    : axSurfaceBase(aWidth, aHeight, aFlags)
      {
        //mDepth = 0;
        HDC tempdc = GetDC(0);
        mHandle = CreateCompatibleBitmap(tempdc,mWidth,mHeight);
        ReleaseDC(0,tempdc);
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

