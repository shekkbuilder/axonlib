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
    int           mWidth;
    int           mHeight;
    int           mDepth;
    //axCanvasBase* mCanvas;
    axCanvas* mCanvas;
  public:
    axSurfaceBase(int aWidth, int aHeight)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mDepth  = 24;
        mCanvas = NULL;
      }
    virtual ~axSurfaceBase() {}
    //virtual axCanvas* createCanvas(void) { return NULL; }
    //virtual int getHandle(void) { return 0; }
    //virtual axSurfaceBase* getSurface(void) { return NULL; }

    inline int getWidth(void) { return mWidth; }
    inline int getHeight(void) { return mHeight; }
    inline int getDepth(void) { return mDepth; }
    //virtual axCanvasBase* getCanvas(void) { return NULL; }
    virtual axCanvas* getCanvas(void) { return NULL; }

};

//----------------------------------------------------------------------
#endif
