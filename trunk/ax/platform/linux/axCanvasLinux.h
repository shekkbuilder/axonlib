#ifndef axCanvasLinux_included
#define axCanvasLinux_included
//----------------------------------------------------------------------

#include <math.h>

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axCanvasBase.h"
//#include "gui/axSurface.h"
#include "gui/axBitmap.h"

//----------------------------------------------------------------------

#define DEF_PENWIDTH 0

//----------------------------------------------------------------------

class axSurface;

// needs mDisplay & mWindow (Drawable) set in aContext

class axCanvasLinux : public axCanvasBase
{
  private:
    Display*      mDisplay;   // from context
    Drawable      mDrawable;  // from context
    GC            mGC;
    XGCValues     gcvalues;
    XFontStruct*  mFont;
  protected:
    int           mXpos;
    int           mYpos;

  public:

    axCanvasLinux(axContext* aContext)
    : axCanvasBase(/*aContext*/)
      {
        //wtrace("axCanvasLinux.constructor");
        mDisplay  = aContext->mDisplay;
        mDrawable = aContext->mWindow;
        mGC       = XCreateGC(mDisplay,mDrawable,0,&gcvalues);
        mFont     = XQueryFont(mDisplay,XGContextFromGC(mGC));
        mXpos     = 0;
        mYpos     = 0;
      }

    //----------

    virtual ~axCanvasLinux()
      {
        XFreeFontInfo(0,mFont,1);   // 1??
        XFreeGC(mDisplay,mGC);
      }

    //--------------------------------------------------

    //virtual int getHandle(void) { return (int)mDrawable; }

    //--------------------------------------------------
    // get / set
    //--------------------------------------------------

    virtual void setPos(int aX, int aY)
      {
        mXpos = aX;
        mYpos = aY;
      }

    //----------

    virtual axColor getColor(int aRed, int aGreen, int aBlue)
    {
      XColor xcol;
      xcol.red   = aRed << 8;
      xcol.green = aGreen << 8;
      xcol.blue  = aBlue << 8;
      xcol.flags = (DoRed or DoGreen or DoBlue);
      XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&xcol);
      return axColor(xcol.pixel);
    }

    virtual void    setPenColor(axColor aColor)   { XSetForeground(mDisplay,mGC,aColor.get()); }
    virtual void    setBrushColor(axColor aColor) { XSetForeground(mDisplay,mGC,aColor.get()); }
    virtual void    setTextColor(axColor aColor)  { XSetForeground(mDisplay,mGC,aColor.get()); }

    //virtual void    clearPen(void)    { mClearPen = true; }
    virtual void    resetPen(void)    { setPenWidth(DEF_PENWIDTH); }
    //virtual void    clearBrush(void)  { mClearBrush}
    //virtual void    resetBrush(void)  {}

    virtual void setPenWidth(int aWidth)
      {
        //mPenWidth = aWidth;
        XSetLineAttributes(mDisplay,mGC,aWidth,LineSolid,CapRound,JoinRound);
      };

    //virtual void resetPenWidth(void)
    //  {
    //    mPenWidth = DEF_PENWIDTH;
    //    XSetLineAttributes(gDP,mGC,mPenWidth,LineSolid,CapRound,JoinRound);
    //  };

    virtual void setPenStyle(int aStyle)
      {
        //TODO
      };

    //----------

    virtual void setBrushStyle(int aStyle)
      {
        XSetFillStyle(mDisplay,mGC,aStyle); // FillSolid
      };


    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        //mClipRect.set(aX1,aY1,(aX2-aX1+1),(aY2-aY1+1));
        XRectangle r;
        r.x      = aX1;
        r.y      = aY1;
        r.width  = (aX2-aX1)+2; // xlib seems to cut off one pixel to the right & bottom... ?
        r.height = (aY2-aY1)+2; // so we compensate by adding another pixel
        XSetClipRectangles(mDisplay,mGC,0,0,&r,1,Unsorted);
      }

    //----------

    virtual void clearClipRect(void)
      {
        XSetClipMask(mDisplay,mGC,None);
      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        XDrawPoint(mDisplay,mDrawable,mGC,aX,aY);
      }

    virtual void drawPoint(int aX, int aY, axColor aColor)
      {
        XSetForeground(mDisplay,mGC,aColor.get());
        XDrawPoint(mDisplay,mDrawable,mGC,aX,aY);
      }


    //----------

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawLine(mDisplay,mDrawable,mGC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawRectangle(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1);
      }

    //----------

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        XFillRectangle(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawArc(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XFillArc(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XDrawArc(mDisplay, mDrawable,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XFillArc(mDisplay, mDrawable,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    virtual int textWidth(axString aText)
      {
        return XTextWidth(mFont, aText.ptr(), aText.length());
      }

    //----------

    virtual int textHeight(axString aText)
      {
        return mFont->ascent + mFont->descent;
      }

    //----------

    virtual void drawText(int aX, int aY, axString aText)
      {
        XDrawString(mDisplay,mDrawable,mGC,aX,aY+mFont->ascent,aText.ptr(),aText.length());
      }

    //--------

    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        int x,y;
        if (aAlign & ta_Top) y = aY1 + mFont->ascent;
        else if (aAlign & ta_Bottom) y = aY2 - mFont->descent;
        else y = aY1 + ((mFont->ascent)>>1) + ((aY2-aY1)>>1);
        int width = textWidth(aText);
        if (aAlign & ta_Left) x = aX1;
        else if (aAlign & ta_Right) x = aX2 - width;
        else x = aX1 + ((aX2 - aX1) >> 1) - ( width >> 1);
        XDrawString(mDisplay,mDrawable,mGC,x,y,aText.ptr(),aText.length());
      }

    //--------------------------------------------------
    // bitmap
    //--------------------------------------------------

    //XPutImage(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height)
    //        Display *display;
    //        Drawable d;
    //        GC gc;
    //        XImage *image;
    //        int src_x, src_y;
    //        int dest_x, dest_y;
    //        unsigned int width, height;
    //
    //display Specifies the connection to the X server.
    //d       Specifies the drawable.
    //gc      Specifies the GC.
    //image   Specifies the image you want combined with the rectangle.
    //src_x   Specifies the offset in X from the left edge of the image defined by the XImage structure.
    //src_y   Specifies the offset in Y from the top edge of the image defined by the XImage structure.
    //dest_x, dest_y  Specify the x and y coordinates, which are relative to the origin of the drawable and are the coordinates of the subimage.
    //width, height 	Specify the width and height of the subimage, which define the dimensions of the rectangle.

    virtual void drawBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        XPutImage(mDisplay,mDrawable,mGC,(XImage*)aBitmap->getHandle(),aSrcX,aSrcY,aX,aY,aSrcW,aSrcH);
      }

    //--------------------------------------------------
    // canvas (surface[drawable])
    //--------------------------------------------------

    //XCopyArea(display, src, dest, gc, src_x, src_y, width, height,  dest_x, dest_y)
    //      Display *display;
    //      Drawable src, dest;
    //      GC gc;
    //      int src_x, src_y;
    //      unsigned int width, height;
    //      int dest_x, dest_y;
    //
    //display       Specifies the connection to the X server.
    //src, dest     Specify the source and destination rectangles to be combined.
    //gc            Specifies the GC.
    //src_x, src_y 	Specify the x and y coordinates, which are relative to the origin of the source rectangle and specify its upper-left corner.
    //width,height 	Specify the width and height, which are the dimensions of both the source and destination rectangles.
    //dest_x,dest_y Specify the x and y coordinates, which are relative to the origin of the destination rectangle and specify its upper-left corner

    virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //wtrace("blit");
        //XCopyArea(mDisplay,aCanvas->getHandle(),mDrawable,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
        XCopyArea(mDisplay,aImage->getHandle(),mDrawable,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
        //wtrace("blit ok");
      }

};

typedef axCanvasLinux axCanvasImpl;

//----------------------------------------------------------------------
#endif
