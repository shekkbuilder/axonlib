/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file axCanvasX11.h
 * \brief canvas for X11
 */

#ifndef axCanvasX11_included
#define axCanvasX11_included
//----------------------------------------------------------------------

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
//#include <pthread.h>
//#include <unistd.h>   // sleep
#include "axColor.h"
#include "axBitmap.h"
#include "axCanvas.h"

//----------------------------------------------------------------------

/// canvas for X11
class axCanvasImpl : public axCanvasBase
{
  protected:
  //public:
    GC            mGC;
    XGCValues     gcvalues;
    XFontStruct*  mFont;
    //int           mHandle; // Drawable (Window/Pixmap)

  public:

    // aHandle from:
    // - Window - axWindow
    // - Pixmap - axSurface
    /// constructor
    axCanvasImpl(int aHandle, int aMode=cmo_window)
    : axCanvasBase(aHandle,aMode)
      {
        //mHandle = aHandle;
        mGC     = XCreateGC( gDP,aHandle,0, &gcvalues );
        mFont   = XQueryFont( gDP, XGContextFromGC(mGC) );
      }

    virtual ~axCanvasImpl()
      {
        //XFreeFontInfo(char** names, XFontStruct* free_info, int actual_count);
        XFreeFontInfo(0,mFont,1);   // 1??
        XFreeGC(gDP,mGC);
      }

    //----------

    //virtual int getHandle(void) { return (int)mHandle; }

    //--------------------------------------------------
    // get / set
    //--------------------------------------------------

    /// pen selection
    virtual void selectPen(axPen* aPen)
      {
        XSetForeground(gDP,mGC,aPen->color().mColor);
        XSetLineAttributes(gDP,mGC,aPen->size(),LineSolid,CapButt,JoinBevel);
      }
      
    /// brush selection
    virtual void selectBrush(axBrush* aBrush)
      {
        XSetForeground(gDP,mGC,aBrush->color().mColor);
        XSetFillStyle(gDP,mGC,aBrush->style()); // FillSolid
      }
      
    /// font selection
    virtual void selectFont(axFont* aFont)
      {
        XSetForeground(gDP,mGC,aFont->color().mColor);
      }

    //--------------------------------------------------

    // internal
    /// set foreground color 
    void setforecolor(int aColor) { XSetForeground(gDP,mGC,aColor); };
    /// set background color
    void setbackcolor(int aColor) { XSetBackground(gDP,mGC,aColor); };

    //--------------------------------------------------
    /// set pen to color
    virtual void setPenColor(axColor aColor)
      {
        mPenColor = aColor;
        XSetForeground(gDP,mGC,aColor.mColor);
      };

    //----------
    /// set brush color
    virtual void setBrushColor(axColor aColor)
      {
        mBrushColor = aColor;
        setforecolor(mBrushColor.mColor);
      };

    //----------
    /// set text color
    virtual void setTextColor(axColor aColor)
      {
        mTextColor = aColor;
        setforecolor(mTextColor.mColor);
      };

    //----------
    /// set pen width
    virtual void setPenWidth(int aWidth)
      {
        mPenWidth = aWidth;
        XSetLineAttributes(gDP,mGC,aWidth,LineSolid,CapButt,JoinBevel);
      };
      
    /// reset pen width
    virtual void resetPenWidth(void)
      {
        mPenWidth = DEF_PENWIDTH;
        XSetLineAttributes(gDP,mGC,mPenWidth,LineSolid,CapButt,JoinBevel);
      };

    //----------
    /// set pen style
    virtual void setPenStyle(int aStyle)
      {
        //TODO
      };

    //----------
    /// set brush style
    virtual void setBrushStyle(int aStyle)
      {
        XSetFillStyle(gDP,mGC,aStyle); // FillSolid
      };

    //----------
    
    /// move to position [x, y]
    virtual void setPos(int aX, int aY)
      {
        mXpos = aX;
        mYpos = aY;
      };

    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------
    /// sets a rectangle region
    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        XRectangle r;
        r.x      = aX1;
        r.y      = aY1;
        r.width  = (aX2-aX1)+1;
        r.height = (aY2-aY1)+1;
        XSetClipRectangles(gDP,mGC,0,0,&r,1,Unsorted);
      };

    //----------
    /// clears rectangle region
    virtual void clearClipRect(void)
      {
        XSetClipMask(gDP,mGC,None);
      };

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------
    /// draws a point at [x,y]
    virtual void drawPoint(int aX, int aY)
      {
        XDrawPoint(gDP,mHandle,mGC,aX,aY);
      };

    /// draws a line from [x1,y1] to [x2,y2]
    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawLine(gDP,mHandle,mGC,aX1,aY1,aX2,aY2);
      };

    /// draws a rectangle from [x1,y1] to [x2,y2]
    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawRectangle(gDP,mHandle,mGC,aX1,aY1,aX2-aX1,aY2-aY1);
      };
      
    /// fills a rectangle from [x1,y1] to [x2,y2]
    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        XFillRectangle(gDP,mHandle,mGC,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
      };

    /// draws a circle from [x1,y1] to [x2,y2]
    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawArc(gDP,mHandle,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      };
      
    /// fills a circle from [x1,y1] to [x2,y2]
    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XFillArc(gDP,mHandle,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      };

    /// draws an arc    
    /// angle 1 = start angle, relative to 3 o'clock
    /// angle 2 = 'distance' 0..1, counter-clockwise    
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XDrawArc(gDP, mHandle,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }
    /// fills an arc 
    /// angle 1 = start angle, relative to 3 o'clock
    /// angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XFillArc(gDP, mHandle,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    /// get text width
    virtual int textWidth(axString aText)
      {
        return XTextWidth(mFont, aText.ptr(), aText.length());
      };

    //----------
    
    /// get text height
    virtual int textHeight(axString aText)
      {
        return mFont->ascent + mFont->descent;
      };

    //----------
    /// draw text at [x,y]
    virtual void drawText(int aX, int aY, axString aText)
      {
        XDrawString(gDP,mHandle,mGC,aX,aY+mFont->ascent,aText.ptr(),aText.length());
      };

    //--------
    /// draw text at [x,y] with background and alignment
    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        int x,y;
        if (aAlign & tal_Top) y = aY1 + mFont->ascent;
        else if (aAlign & tal_Bottom) y = aY2 - mFont->descent;
        else y = aY1 + ((mFont->ascent)>>1) + ((aY2-aY1)>>1);
        int width = textWidth(aText);
        if (aAlign & tal_Left) x = aX1;
        else if (aAlign & tal_Right) x = aX2 - width;
        else x = aX1 + ((aX2 - aX1) >> 1) - ( width >> 1);
        XDrawString(gDP,mHandle,mGC,x,y,aText.ptr(),aText.length());
      };

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

    /// draw bitmap
    virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        XPutImage(gDP,mHandle,mGC,(XImage*)aBitmap->getHandle(),aSrcX,aSrcY,aX,aY,aSrcW,aSrcH);
      };

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

    //--------------------------------------------------
    // canvas (surface[drawable])
    //--------------------------------------------------
        
    //virtual void blit(int aHandle, axPoint aDst, axRect aSrc)
    /// blit canvas
    virtual void blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        XCopyArea(gDP,aCanvas->getHandle(),mHandle,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
      };

    //--------------------------------------------------


};


//----------------------------------------------------------------------
#endif






