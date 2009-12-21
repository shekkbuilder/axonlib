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

    axSurfaceImpl(int aWidth, int aHeight, int aFlags=0)
    : axSurfaceBase(aWidth, aHeight, aFlags)
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

