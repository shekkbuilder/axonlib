#ifndef axCanvas_included
#define axCanvas_included
//----------------------------------------------------------------------

//#include "axString.h"
#include "axColor.h"
#include "axBitmap.h"

#include "axPen.h"
#include "axBrush.h"
#include "axFont.h"

// text align
#define tal_Center        0
#define tal_Left          1
#define tal_Top           2
#define tal_Right         4
#define tal_Bottom        8
#define tal_TopLeft       16
#define tal_TopRight      32
#define tal_BottomLeft    64
#define tal_BottomRight   128
#define tal_RightStretch  256
#define tal_BottomStretch 512
#define tal_Horz          1024
#define tal_Vert          2048

// canvas mode
#define cmo_window        0
#define cmo_buffer        1

//#define DEF_PENWIDTH      0

// max depth of cliprect stack
#define MAX_CLIPRECTS     32

//----------------------------------------------------------------------

class axCanvasBase
{
  //protected:
  public:

    int mHandle;
    int mMode;
    axColor mPenColor;
    axColor mBrushColor;
    axColor mTextColor;
    int     mPenWidth;
    int     mXpos, mYpos;

    axPen*    defaultPen;
    axBrush*  defaultBrush;
    axFont*   defaultFont;

    int mNumClipRects;
    axRect mClipRects[MAX_CLIPRECTS];
    //axArray<axRect*> mClipRects;

  public:

    axCanvasBase(int aHandle, int aMode=cmo_window)
      {
        mHandle     = aHandle;
        mMode       = aMode;
        mPenColor   = axColor(192,192,192);
        mBrushColor = axColor(128,128,128);
        mTextColor  = axColor(255,255,255);
        mPenWidth   = 0;
        mXpos=mYpos = 0;
        mNumClipRects = 0;
      }

    virtual ~axCanvasBase()
      {
      }

    virtual int getHandle(void) { return mHandle; }

    //----------

    //TODO
    virtual axPen*    createPen(axColor, int aWidth, int aStyle) { return NULL; }
    virtual axBrush*  createBrush(axColor, int aStyle) { return NULL; }
    virtual axFont*   createFont(axString aName, int aSize, int aStyle) { return NULL; }
    virtual void      selectPen(axPen* aPen) {}
    virtual void      selectBrush(axBrush* aBrush) {}
    virtual void      selectFont(axFont* aFont) {}
    virtual void      resetPen(void) {}
    virtual void      resetBrush(void) {}
    virtual void      resetFont(void) {}

    virtual void setPenColor(axColor aColor) {}
    virtual void setBrushColor(axColor aColor) {}
    virtual void setTextColor(axColor aColor) {}
    virtual void setPenWidth(int aWidth) {}
    virtual void resetPenWidth(void) {}
    virtual void setPenStyle(int aStyle) {}
    virtual void setBrushStyle(int aStyle) {}

    virtual void setPos(int aX, int aY) {}
    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void clearClipRect(void) {}

    virtual void drawPoint(int aX, int aY) {}
    virtual void drawLine(int aX1, int aY1, int aX2, int aY2) {}
    virtual void drawRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void fill() {}
    virtual void fillRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}

    virtual int  textWidth(axString aText) { return 0; };
    virtual int  textHeight(axString aText) { return 0; };
    virtual void drawText(int aX, int aY, axString aText) {}
    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign) {}

    virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}
    virtual void blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}


    //----------

    //virtual void setClipRect(axRect aRect) { setClipRect(aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    //virtual void drawLine(axPoint aP1, axPoint aP2) {}
    //virtual void drawRect(axRect aRect) {}
    //virtual void fillRect(axRect aRect) { fillRect( aRect.x, aRect.y, aRect.x+aRect.w-1, aRect.y+aRect.h-1 ); }
    //virtual void drawCircle(axRect aRect) {}
    //virtual void fillCircle(axRect aRect) {}
    //virtual void drawText(axPoint aPoint,axString aText) {}
    //virtual void drawText(axRect aRect,axString aText, int aAlign=tal_Center) {}
    //virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, axRect aSrc) {}
    //virtual void blit(int aHandle, int aX, int aY, axRect aSrc) {}
    //virtual void blit(axCanvasBase* aCanvas, int aX, int aY, axRect aSrc) {}

};

//----------------------------------------------------------------------

#ifdef linux
  #include "axCanvasX11.h"
#endif

#ifdef WIN32
  #include "axCanvasW32.h"
#endif

//----------------------------------------------------------------------

class axCanvas : public axCanvasImpl
{
  public:

    axCanvas(int aHandle, int aMode=cmo_window)
    : axCanvasImpl(aHandle,aMode)
      {
      }

    virtual ~axCanvas()
      {
      }

    //void drawAngle(int x, int y, int w, int h, float a)
    //  {
    //  }

//    void drawSmallKnob(int x, int y, int w, int h, float v)
//      {
//        setPenColor( AX_GREY_DARK );
//        setPenWidth( 3 );
//        drawArc(x,y,x+15,y+15,0.6,0.8);
//        setPenColor( AX_WHITE );
//        drawArc(x,y,x+15,y+15,0.6,v*0.8);
//        resetPenWidth();
//        setBrushColor( axColor(192,192,192) );
//        fillArc(x+3,y+3,x+12,y+12,0.6,v*0.8);
//        char buf[32];
//        sprintf(buf,"%.2f %%",v*100);
//        setTextColor( AX_GREY_DARK );
//        drawText(x+22,y,x+w-1,y+15,buf,tal_Left);
//      }

    void drawPanel(int x, int y, int w, int h, int mode)
      {
        int x2 = x+w-1;
        int y2 = y+h-1;
        setPenColor( AX_GREY_LIGHT );
        drawLine(x,y,x2,y);
        drawLine(x,y,x,y2);
        setPenColor( AX_GREY_DARK );
        drawLine(x2,y,x2,y2);
        drawLine(x,y2,x2,y2);
        setBrushColor( AX_GREY );
        fillRect(x+1,y+1,x2-1,y2-1);
      }

    //----------

//    void pushClipRect(int aX1, int aY1, int aX2, int aY2)
//      {
//        setClipRect(aX1,aY1,aX2,aY2);
//        if (mNumClipRects<MAX_CLIPRECTS)
//        {
//          mClipRects[mNumClipRects].set(aX1,aY1,aX2,aY2);
//          mNumClipRects++;
//        }
//        //axRect* R = new axRect(aX1,aY1,aX2,aY2);
//        //mClipRects.push(R);
//      }
//
//    void popClipRect(void)
//      {
//        if (mNumClipRects>0)
//        {
//          mNumClipRects--;
//          setClipRect(mClipRects[mNumClipRects].x,
//                      mClipRects[mNumClipRects].y,
//                      mClipRects[mNumClipRects].x2(),
//                      mClipRects[mNumClipRects].y2());
//        }
//      }

};

//----------------------------------------------------------------------
#endif

