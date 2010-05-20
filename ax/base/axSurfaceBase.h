#ifndef axSurfaceBase_included
#define axSurfaceBase_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "base/axImage.h"
//#include "base/axCanvasBase.h"
#include "gui/axCanvas.h"

class axCanvas;

class axSurfaceBase : public axImage
{
  protected:
    int       mWidth;
    int       mHeight;
    int       mDepth;
    axCanvas* mCanvas;
  public:
    axSurfaceBase(int aWidth, int aHeight, int aDepth/*=24*/)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mDepth  = aDepth;
        mCanvas = NULL;
      }
    virtual ~axSurfaceBase() {}

    inline int        getWidth(void) { return mWidth; }
    inline int        getHeight(void) { return mHeight; }
    inline int        getDepth(void) { return mDepth; }
    virtual axCanvas* getCanvas(void) { return NULL; }

};

//----------------------------------------------------------------------
#endif
