#ifndef axSurfaceLinux_included
#define axSurfaceLinux_included
//----------------------------------------------------------------------

//#ifdef AX_XRENDER
#ifdef AX_ALPHA
#include <X11/extensions/Xrender.h>
#endif

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axSurfaceBase.h"
#include "gui/axCanvas.h"
//#include "axonlib.h"

//#include "gui/axCanvas.h"
class axCanvas;

// platform (linux) dependent things

class axSurfaceLinux : public axSurfaceBase
{
  private:
    Display*  mDisplay;
    Drawable  mDrawable;
    Pixmap    mPixmap;

    int       mPicture;

  //protected:
  //  int       mWidth;
  //  int       mHeight;
  //  int       mDepth;
  //  axCanvas* mCanvas;
  public:

    //Pixmap XCreatePixmap(display, d, width, height, depth)
    //  Display *display;           // Specifies the connection to the X server.
    //  Drawable d;                 // Specifies which screen the pixmap is created on.
    //  unsigned int width, height; // 	Specify the width and height, which define the dimensions of the pixmap.
    //  unsigned int depth; // Specifies the depth of the pixmap.

    axSurfaceLinux(axContext* aContext, int aWidth, int aHeight, int aDepth)
    //: axSurfaceBase(aContext)
    //: axSurfaceBase(/*aContext,*/aWidth,aHeight)
    : axSurfaceBase(aWidth,aHeight,aDepth)
      {
        trace("axSurfaceLinux constructor");
        mDisplay  = aContext->mDisplay;
        mDrawable = aContext->mWindow;  // XDefaultRootWindow(mDisplay);
        mWidth    = aWidth;
        mHeight   = aHeight;
        //mDepth = DefaultDepth(mDisplay,DefaultScreen(mDisplay));
        mDepth = aDepth;
        mPixmap  = XCreatePixmap(mDisplay,mDrawable,mWidth,mHeight,mDepth);

        //axContext ctx(mDisplay,mPixmap); // display, drawable
        //mCanvas = new axCanvas(&ctx);
        mCanvas = createCanvas();

        //#ifdef AX_XRENDER
        #ifdef AX_ALPHA
          XRenderPictFormat* fmt;
          if (mDepth==24)
            fmt = XRenderFindStandardFormat(mDisplay,PictStandardRGB24);
          else
            fmt = XRenderFindStandardFormat(mDisplay,PictStandardARGB32);
          XRenderPictureAttributes pict_attr;
          pict_attr.poly_edge = PolyEdgeSmooth;
          pict_attr.poly_mode = PolyModeImprecise;
          //pict_attr.component_alpha = true;
          int pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
          mPicture = XRenderCreatePicture(mDisplay,/*mDrawable*/mPixmap,fmt,pict_bits,&pict_attr);

          trace("  mPicture: " << mPicture);

          mCanvas->setPicture(mPicture);

        #endif


      }

    //----------

    virtual ~axSurfaceLinux()
      {
        delete mCanvas;
        #ifdef AX_ALPHA
        XRenderFreePicture(mDisplay,mPicture);
        #endif
        // The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
        // Then, the X server frees the pixmap storage when there are no references to it.
        XFreePixmap(mDisplay,mPixmap);
      }

    //----------

    // called from axCanvas.blit
    virtual int getHandle(void)  { return mPixmap; }
    virtual int getPicture(void) { return mPicture; }

    //----------

    virtual  int          getWidth(void)  { return 0; }
    virtual  int          getHeight(void) { return 0; }
    virtual  int          getDepth(void)  { return 0; }
    virtual axCanvas*     getCanvas(void) { return mCanvas; }

    //----------

    virtual axCanvas* createCanvas(void)
      {
        axContext ctx(mDisplay,mPixmap);
        axCanvas* can = new axCanvas(&ctx);
        //can->setPicture(mPicture);
        return can;
      }

};

typedef axSurfaceLinux axSurfaceImpl;

#endif
//----------------------------------------------------------------------

