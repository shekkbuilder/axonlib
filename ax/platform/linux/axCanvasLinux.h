#ifndef axCanvasLinux_included
#define axCanvasLinux_included
//----------------------------------------------------------------------

#include <math.h>

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axCanvasBase.h"

class axCanvasLinux : public axCanvasBase
{
  private:
    XGCValues     gcvalues;
  //protected:
    Display*      mDisplay;
    Window        mWindow;
    GC            mGC;
    XFontStruct*  mFont;
  protected:
    //int           mXpos,mYpos;
    //axRect        mClipRect;

  public:
    axCanvasLinux(axContext* aContext)
    : axCanvasBase()
      {
        mDisplay  = aContext->mDisplay;
        mWindow   = aContext->mWindow;
        mGC       = XCreateGC(mDisplay,mWindow,0,&gcvalues);
        mFont     = XQueryFont(mDisplay,XGContextFromGC(mGC));
        //mXpos     = 0;
        //mYpos     = 0;
        //mClipRect = NULL_RECT;
      }

    //----------

    virtual ~axCanvasLinux()
      {
        //XFreeFontInfo(char** names, XFontStruct* free_info, int actual_count);
        XFreeFontInfo(0,mFont,1);   // 1??
        XFreeGC(mDisplay,mGC);
      }

    //--------------------------------------------------
    // get / set
    //--------------------------------------------------

    //virtual axColor createColor(int aRed, int aGreen, int aBlue)
    //  {
    //    XColor xcol;
    //    xcol.red   = aRed << 8;
    //    xcol.green = aGreen << 8;
    //    xcol.blue  = aBlue << 8;
    //    xcol.flags = (DoRed or DoGreen or DoBlue);
    //    XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&xcol);
    //    return axColor(xcol.pixel);
    //  }

    //----------

    //virtual axPen createPen(axColor aColor, int aWidth)
    //  {
    //    return new axPen();
    //  }

    //----------

    //virtual axBrush createBrush(axColor aColor, int aStyle)
    //  {
    //    return new axPen();
    //  }

    //----------

    //virtual axFont createFont(axString aName, axColor aColor, int aSize)
    //  {
    //    return new axPen();
    //  }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //virtual void setPos(int aX, int aY)
    //  {
    //    mXpos = aX;
    //    mYpos = aY;
    //  }

    //----------

    virtual axColor createColor(int aRed, int aGreen, int aBlue)
    {
      XColor xcol;
      xcol.red   = aRed << 8;
      xcol.green = aGreen << 8;
      xcol.blue  = aBlue << 8;
      xcol.flags = (DoRed or DoGreen or DoBlue);
      XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&xcol);
      return axColor(xcol.pixel);
    }

    //----------

    virtual axPen createPen(int r, int g, int b, int size=DEF_PENWIDTH)
      {
        axColor col = createColor(r,g,b);
        return axPen(col,size);
      }

    //----------

    virtual axBrush createBrush(int r, int g, int b, int style=DEF_BRUSHSTYLE)
      {
        axColor col = createColor(r,g,b);
        return axBrush(col,style);
      }

    //----------

    virtual axFont createFont(axString name, int r, int g, int b, int size=-1, int style=0)
      {
        axColor col = createColor(r,g,b);
        return axFont(name,col,size,style);
      }

    //virtual void deletePen(axPen* aPen) {}
    //virtual void deleteBrush(axBrush* aBrush) {}
    //virtual void deleteFont(axFont* aFont) {}

    //----------

    //virtual void clearPen(void) {}

    //----------

    virtual void selectPen(axPen* aPen)
      {
        XSetForeground(mDisplay,mGC,aPen->getColor());
        XSetLineAttributes(mDisplay,mGC,aPen->getSize(),LineSolid,CapButt,JoinBevel);
      }

    //----------

    virtual void selectBrush(axBrush* aBrush)
      {
        XSetForeground(mDisplay,mGC,aBrush->getColor());
        XSetFillStyle(mDisplay,mGC,aBrush->getStyle()); // FillSolid
      }

    //----------

    virtual void selectFont(axFont* aFont)
      {
        XSetForeground(mDisplay,mGC,aFont->getColor());
      }

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

    //virtual void pushClipRect(axRect aRect)
    //  {
    //    axRect* R = new axRect(aRect);
    //    mClipRects.push(R);
    //    setClipRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
    //  }

    //virtual void popClipRect(void)
    //  {
    //    axRect* R = mClipRects.pop();
    //    setClipRect(R->x,R->y,R->x2(),R->y2());
    //    delete R;
    //  }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        XDrawPoint(mDisplay,mWindow,mGC,aX,aY);
      }

    //----------

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawLine(mDisplay,mWindow,mGC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawRectangle(mDisplay,mWindow,mGC,aX1,aY1,aX2-aX1,aY2-aY1);
      }

    //----------

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        XFillRectangle(mDisplay,mWindow,mGC,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawArc(mDisplay,mWindow,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XFillArc(mDisplay,mWindow,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
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
          XDrawArc(mDisplay, mWindow,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
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
          XFillArc(mDisplay, mWindow,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
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
        XDrawString(mDisplay,mWindow,mGC,aX,aY+mFont->ascent,aText.ptr(),aText.length());
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
        XDrawString(mDisplay,mWindow,mGC,x,y,aText.ptr(),aText.length());
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

//    virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        XPutImage(mDisplay,mWindow,mGC,(XImage*)aBitmap->getHandle(),aSrcX,aSrcY,aX,aY,aSrcW,aSrcH);
//      }

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

//    //virtual void blit(int aHandle, axPoint aDst, axRect aSrc)
//    virtual void blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        XCopyArea(mDisplay,aCanvas->getHandle(),mWindow,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
//      }

};

typedef axCanvasLinux axCanvasImpl;

#endif
//----------------------------------------------------------------------



#if 0

//    // internal
//    void setforecolor(int aColor) { XSetForeground(gDP,mGC,aColor); };
//    void setbackcolor(int aColor) { XSetBackground(gDP,mGC,aColor); };
//
//    //--------------------------------------------------
//
//    virtual void setPenColor(axColor aColor)
//      {
//        mPenColor = aColor;
//        XSetForeground(gDP,mGC,aColor.mColor);
//      };
//
//    //----------
//
//    virtual void setBrushColor(axColor aColor)
//      {
//        mBrushColor = aColor;
//        setforecolor(mBrushColor.mColor);
//      };
//
//    //----------
//
//    virtual void setTextColor(axColor aColor)
//      {
//        mTextColor = aColor;
//        setforecolor(mTextColor.mColor);
//      };
//
//    //----------
//
//    virtual void setPenWidth(int aWidth)
//      {
//        mPenWidth = aWidth;
//        XSetLineAttributes(gDP,mGC,aWidth,LineSolid,CapButt,JoinBevel);
//      };
//
//    virtual void resetPenWidth(void)
//      {
//        mPenWidth = DEF_PENWIDTH;
//        XSetLineAttributes(gDP,mGC,mPenWidth,LineSolid,CapButt,JoinBevel);
//      };
//
//    //----------
//
//    virtual void setPenStyle(int aStyle)
//      {
//        //TODO
//      };
//
//    //----------
//
//    virtual void setBrushStyle(int aStyle)
//      {
//        XSetFillStyle(gDP,mGC,aStyle); // FillSolid
//      };

    //----------



    //--------------------------------------------------


};



#endif
