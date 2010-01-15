#ifndef axSurface_included
#define axSurface_included
//----------------------------------------------------------------------

#include "axCanvas.h"
//#include "axSurfaceBase.h"

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
