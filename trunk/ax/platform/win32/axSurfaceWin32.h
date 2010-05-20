#ifndef axSurfaceWin32_included
#define axSurfaceWin32_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axSurfaceBase.h"

class axSurfaceWin32 : public axSurfaceBase
{
  private:
    HWND    mWindow;
    HBITMAP mBitmap, mOldHandle;
  protected:
//    int       mWidth;
//    int       mHeight;
//    int       mDepth;
//    axCanvas* mCanvas;
  public:
    axSurfaceWin32(axContext* aContext, int aWidth, int aHeight, int aDepth)
    : axSurfaceBase(aWidth,aHeight,aDepth)
      {
        mWindow   = aContext->mWindow;
        //mWidth    = aWidth;
        //mHeight   = aHeight;
        //mDepth    = 24;
        HDC tempdc = GetDC(0);                                    // ...if this value is NULL, GetDC retrieves the device context for the entire screen...
        mBitmap = CreateCompatibleBitmap(tempdc,mWidth,mHeight);  // creates a bitmap compatible with the device associated with the specified device context.
        ReleaseDC(0,tempdc);                                      // releases a device context (DC), freeing it for use by other applications.
        mCanvas = createCanvas();
        mOldHandle = (HBITMAP)SelectObject((HDC)mCanvas->getHandle(),mBitmap);
      }

    virtual ~axSurfaceWin32()
      {
        SelectObject((HDC)mCanvas->getHandle(),mOldHandle);
        delete mCanvas;
        DeleteObject(mBitmap);
      }

    //----------

    // called by: axCanvasLinux.drawImage
    virtual int getHandle(void) { return (int)mCanvas->getHandle(); }
    virtual int getBitmap(void) { return (int)mBitmap; }
    virtual axCanvas* getCanvas(void) { return mCanvas; }

    virtual axCanvas* createCanvas(void)
      {
        //wtrace("axSurfaceWin32.createCanvas");
        //axContext ctx(mWindow);
        axContext ctx(0);
        axCanvas* can = new axCanvas(&ctx);
        return can;
      }

};

typedef axSurfaceWin32 axSurfaceImpl;


//----------------------------------------------------------------------
#endif

