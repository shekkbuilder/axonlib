#ifndef axCanvasWin32_included
#define axCanvasWin32_included
//----------------------------------------------------------------------

//#include <math.h>
#include "core/axMath.h" // axMax

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axCanvasBase.h"
//#include "base/axSurfaceBase.h"
#include "gui/axBitmap.h"

//----------------------------------------------------------------------

#define DEF_PENWIDTH 0

//----------------------------------------------------------------------

class axCanvasWin32 : public axCanvasBase
{
  private:
		HDC     mDC;
    HFONT   mFont;
		HPEN    mPen, mOldPen;      // set when clearPen, use with resetPen()
    HPEN    mNullPen;     // empty pen (shapes without borders)
		HBRUSH  mBrush, mOldBrush;
    HBRUSH  mNullBrush;
    RECT    R;            // temp
		//HPEN    mPen, mOldPen, mOldPen2;
		//HBRUSH  mBrush, mOldBrush, mOldBrush2;
  protected:
		int     mWinHandle;   // from context
    axColor mPenColor;
    axColor mBrushColor;
    axColor mTextColor;
    //bool    mIsPenClear;
    int     mCurrentPen;
    int     mCurrentBrush;
    int     mXpos,mYpos;

  public:

    axCanvasWin32(axContext* aContext)
    : axCanvasBase(/*aContext*/)
      {
        mPenColor   = getColor(AX_GREY_LIGHT);
        mBrushColor = getColor(AX_GREY);
        mTextColor  = getColor(AX_WHITE);
        mCurrentPen = 0;
        mCurrentBrush = 0;
        //mIsPenClear = false;
        //mPen    = 0;
        //mBrush  = 0;
        HDC tempdc = GetDC(0);
        if (aContext->mWindow)
        { // window
          mWinHandle = (int)aContext->mWindow;
          mDC = GetDC(aContext->mWindow);
        }
        else
        {
          // surface
          mWinHandle = 0;
          mDC = CreateCompatibleDC(tempdc);
        }
        LOGFONT LFont;
        memset(&LFont,0,sizeof(LFont));
        strcpy(LFont.lfFaceName,"Arial");
        LFont.lfHeight = -MulDiv(8,GetDeviceCaps(tempdc,LOGPIXELSY),72);
        mFont = CreateFontIndirect(&LFont);
        SelectObject(mDC,mFont);
        ReleaseDC(0,tempdc);

        mNullPen = CreatePen(PS_NULL,0,0);
        //http://msdn.microsoft.com/en-us/library/dd145035%28v=VS.85%29.aspx
        LOGBRUSH logbrush;
          logbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ++
          logbrush.lbColor = 0;       // ignored if null
          logbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
        mNullBrush = CreateBrushIndirect(&logbrush);

        mPen   = (HPEN)GetStockObject(DC_PEN);
        mBrush = (HBRUSH)GetStockObject(DC_BRUSH);

        // /*mOldPen = (HPEN)*/    SelectObject(mDC, GetStockObject(DC_PEN));
        // /*mOldBrush = (HBRUSH)*/SelectObject(mDC, GetStockObject(DC_BRUSH));
        SelectObject(mDC,mPen);
        SelectObject(mDC,mBrush);

      }

    //----------

    virtual ~axCanvasWin32()
      {
        DeleteObject(mNullPen);
        DeleteObject(mNullBrush);

        //DeleteObject(mOldPen);
        //DeleteObject(mOldBrush);

        //if (mPen)   DeleteObject(mPen);
        //if (mBrush) DeleteObject(mBrush);
        //if (mFont)  DeleteObject(mFont);

        if (mWinHandle) ReleaseDC((HWND)mWinHandle,mDC);
        else DeleteDC(mDC);

      }

    //----------

    virtual int getHandle(void) { return (int)mDC; }
    //virtual HDC getDC(void) { return mDC; }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void setPos(int aX, int aY)
      {
        MoveToEx(mDC,aX,aY,0);
        mXpos = aX;
        mYpos = aY;
      }

    //--------------------

    //virtual axColor getColor(int aRed, int aGreen, int aBlue)
    //  {
    //    axColor color = RGB(aRed,aGreen,aBlue);
    //    return color;
    //  }

    virtual axColor getColor(int aRed, int aGreen, int aBlue)
      {
        axColor color = RGB(aRed,aGreen,aBlue);
        return color;
      }

    virtual void setPenColor(axColor aColor)
      {
        mPenColor = aColor;
        SetDCPenColor(mDC, aColor.get());
      }

    virtual void setBrushColor(axColor aColor)
      {
        mBrushColor = aColor;
        SetDCBrushColor(mDC, aColor.get());
      }

    virtual void setTextColor(axColor aColor)
      {
        mTextColor = aColor;
        SetTextColor(mDC,aColor.get());
      }

    virtual void clearPen(void)
      {
        mOldPen = (HPEN)SelectObject( (HDC)mDC, mNullPen );
      }

    virtual void resetPen(void)
      {
        SelectObject(mDC,mOldPen);
      }

    virtual void clearBrush(void)
      {
        mOldBrush = (HBRUSH)SelectObject( (HDC)mDC, mNullBrush );
      }

    virtual void resetBrush(void)
      {
        SelectObject(mDC,mOldBrush);
      }

    // internal

    void setPen(int aColor, int aWidth=DEF_PENWIDTH)
      {
        //if (mPen)
        //{
        //  SelectObject(mDC,mOldPen);
        //  DeleteObject(mPen);
        //}
        HPEN pen = CreatePen(PS_SOLID,aWidth,aColor);
        mOldPen = (HPEN)SelectObject((HDC)mDC, pen);
      };

    void setBrush(int aColor)
      {
        if (mBrush)
        {
          SelectObject(mDC,mOldBrush);
          DeleteObject(mBrush);
        }
        mBrush = CreateSolidBrush(aColor);
        mOldBrush = (HBRUSH)SelectObject((HDC)mDC, mBrush);
      };


    // internal

    virtual void setPenWidth(int aWidth)
      {
        //mPenWidth = aWidth;
        setPen(mPenColor.mColor,aWidth);
      };

    //virtual void resetPenWidth(void)
    //  {
    //    mPenWidth = DEF_PENWIDTH;
    //    setpen(mPenColor.mColor,mPenWidth);
    //  };

    virtual void setPenStyle(int aStyle)
      {
      };

    virtual void setBrushStyle(int aStyle)
      {
      };


    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------

    // The CreateRectRgn function creates a rectangular region.

    // Regions created by the Create<shape>Rgn methods
    // (such as CreateRectRgn and CreatePolygonRgn) only include
    // the interior of the shape; the shape's outline is excluded
    // from the region. This means that any point on a line between
    // two sequential vertices is not included in the region.
    // If you were to call PtInRegion for such a point, it would return zero as the result.

    // The SelectClipRgn function selects a region as the current clipping region for the specified device context.

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        HRGN hrgn = /*hrgn = */CreateRectRgn( aX1-1, aY1-1, aX2+1, aY2+1 );
        SelectClipRgn(mDC,hrgn);
      }

    //----------

    // To remove a device-context's clipping region, specify a NULL region handle.

    virtual void clearClipRect(void)
      {
        SelectClipRgn(mDC,NULL);
      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(mDC,aX,aY,mPenColor.get());
      }

    //----------

    virtual void drawPoint(int aX, int aY, axColor aColor)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(mDC,aX,aY,aColor.get());
      }

    //----------

    // The LineTo function draws a line from the current position up to, but not including, the specified point.

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(mDC,aX2,aY2);
        drawPoint(aX2,aY2);         // !!!
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        // todo setpos + 4 lineto
        //drawLine(aX1, aY1, aX2, aY1);
        //drawLine(aX1, aY1, aX1, aY2);
        //drawLine(aX1, aY2, aX2, aY2);
        //drawLine(aX2, aY1, aX2, aY2);
        setPos(aX1,aY1);
        LineTo(mDC,aX2,aY1);
        LineTo(mDC,aX2,aY2);
        LineTo(mDC,aX1,aY2);
        LineTo(mDC,aX1,aY1);
        //drawPoint(aX2,aY2);         // !!!
      }

    //----------

    // The FillRect function fills a rectangle by using the specified brush.
    // This function includes the left and top borders,
    // but excludes the right and bottom borders of the rectangle.

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2+1;           // !!!
        R.bottom  = aY2+1;           // !!!
        FillRect(mDC,&R,mBrush);
        //Rectangle(mDC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        //Ellipse(mDC, aX1,aY1,aX2,aY2 );
        Arc( mDC,aX1,aY1,aX2+1,aY2+1,0,0,0,0);
      }

    //----------

    // The Ellipse function draws an ellipse. The center of the ellipse is the
    // center of the specified bounding rectangle. The ellipse is outlined by
    // using the current pen and is filled by using the current brush.

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        clearPen();
        Ellipse( mDC,aX1,aY1,aX2,aY2);
        resetPen();
      }

    //----------


    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = axMax(w,h);
          float x1 = x + cos(a1*PI2) * size;
          float y1 = y + sin(a1*PI2) * size;
          float x2 = x + cos(a2*PI2) * size;
          float y2 = y + sin(a2*PI2) * size;
          Arc(mDC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( fabs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          clearPen();
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = axMax(w,h);
          float x1 = x + cos(a1*PI2) * size;
          float y1 = y + sin(a1*PI2) * size;
          float x2 = x + cos(a2*PI2) * size;
          float y2 = y + sin(a2*PI2) * size;
          Pie(mDC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
          resetPen();
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    virtual int textWidth(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cx;
      }

    //----------

    virtual int textHeight(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cy;
      }

    //----------

    // hack alert!
    // draw to a rect size 1000x1000, and align to upper left

    virtual void drawText(int aX, int aY, axString aText)
      {
        //SetBkMode(mDC,TRANSPARENT);
        //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
        drawText( aX,aY, aX+1000,aY+1000, aText, ta_Left|ta_Top);
      }

    //--------

    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        SetBkMode(mDC,TRANSPARENT);
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2;
        R.bottom  = aY2;
        int flags = 0;
        if (aAlign & ta_Left) flags = flags | DT_LEFT;
        else if (aAlign & ta_Right) flags = flags | DT_RIGHT;
        else flags = flags | DT_CENTER;
        if (aAlign & ta_Top) flags = flags | DT_TOP;
        else if (aAlign & ta_Bottom) flags = flags | DT_BOTTOM;
        else flags = flags | DT_VCENTER;
        HFONT oldfont = (HFONT)SelectObject(mDC, mFont);
        DrawText(mDC, aText.ptr(), -1, &R, flags | DT_NOCLIP | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(mDC, oldfont);
      }

    //--------------------------------------------------
    // bitmap
    //--------------------------------------------------

    virtual void drawBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = CreateCompatibleDC(mDC);
        SelectObject(tempdc,(HBITMAP)aBitmap->getHandle());
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
        DeleteDC(tempdc);
      }

    //--------------------------------------------------
    // image
    //--------------------------------------------------

    virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = (HDC)aImage->getHandle();
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
      }

};

typedef axCanvasWin32 axCanvasImpl;

//----------------------------------------------------------------------
#endif
