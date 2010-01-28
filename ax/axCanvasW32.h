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
 * \file axCanvasW32.h
 * \brief canvas for win32
 */

#ifndef axCanvasW32_included
#define axCanvasW32_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "axColor.h"
#include "axBitmap.h"
#include "axCanvas.h"

//----------------------------------------------------------------------
/// canvas for win32
class axCanvasImpl : public axCanvasBase
{
  protected:
  //public:
		HDC     mDC;
    HFONT   mFont;
		HPEN    mPen, mOldPen;
		HBRUSH  mBrush, mOldBrush;
    RECT    R;
		int     mWinHandle;

  public:

    /// constructor
    // aHandle = owner (HWND or NULL)    
    axCanvasImpl(int aHandle, int aMode=cmo_window)
    : axCanvasBase(aHandle,aMode)
      {
        mPen    = 0;
        mBrush  = 0;
        HDC tempdc = GetDC(0);
        mWinHandle = aHandle;
        if (mMode==cmo_window) mDC = GetDC((HWND)aHandle);    // window
        else mDC = CreateCompatibleDC(tempdc);      // bitmap
        LOGFONT LFont;
        memset(&LFont,0,sizeof(LFont));
        strcpy(LFont.lfFaceName,"Arial");
        LFont.lfHeight = -MulDiv(8,GetDeviceCaps(tempdc,LOGPIXELSY),72);
        mFont = CreateFontIndirect(&LFont);
        SelectObject(mDC,mFont);
        ReleaseDC(0,tempdc);
      }

    virtual ~axCanvasImpl()
      {
        if (mPen)   DeleteObject(mPen);
        if (mBrush) DeleteObject(mBrush);
        if (mFont)  DeleteObject(mFont);
        if (mMode==cmo_window) ReleaseDC((HWND)mWinHandle,mDC);
        else DeleteDC(mDC);
      }

    //----------

    //virtual int getHandle(void) { return (int)mHandle; }
    virtual HDC getDC(void) { return mDC; }

    //--------------------------------------------------
    // get / set
    //--------------------------------------------------

    /// pen selection
    virtual void selectPen(axPen* aPen)
      {
        /*mOldPen = (HPEN)*/SelectObject((HDC)mDC, (HPEN)aPen->handle());
      }

    /// brush selection
    virtual void selectBrush(axBrush* aBrush)
      {
        /*mOldBrush = (HBRUSH)*/SelectObject((HDC)mDC, (HBRUSH)aBrush->handle());
      }
    
    /// font selection
    virtual void selectFont(axFont* aFont)
      {
        SetTextColor(mDC,mTextColor.mColor);
      }

    //--------------------------------------------------

    // internal
    /// set pen to color / width   
    void setpen(int aColor, int aWidth=DEF_PENWIDTH)
      {
        if (mPen)
        {
          SelectObject(mDC,mOldPen);
          DeleteObject(mPen);
        }
        mPen = CreatePen(PS_SOLID,aWidth,aColor);
        mOldPen = (HPEN)SelectObject((HDC)mDC, mPen);
      }

    //----------

    /// set brush with color
    // internal
    void setbrush(int aColor)
      {
        if (mBrush)
        {
          SelectObject(mDC,mOldBrush);
          DeleteObject(mBrush);
        }
        mBrush = CreateSolidBrush(aColor);
        mOldBrush = (HBRUSH)SelectObject((HDC)mDC, mBrush);
      }

    //--------------------------------------------------
    /// set pen to color
    virtual void setPenColor(axColor aColor)
      {
        mPenColor = aColor;
        setpen(mPenColor.mColor,mPenWidth);
        //SetDCPenColor(mDC,mPenColor.mColor);
      }

    //----------
    /// set brush to color
    virtual void setBrushColor(axColor aColor)
      {
        mBrushColor = aColor;
        setbrush(mBrushColor.mColor);
      }

    //----------
    /// set the text color
    virtual void setTextColor(axColor aColor)
      {
        mTextColor = aColor;
        SetTextColor(mDC,mTextColor.mColor);
      }

    //----------
    /// set pen width
    virtual void setPenWidth(int aWidth)
      {
        mPenWidth = aWidth;
        setpen(mPenColor.mColor,mPenWidth);
      }
    
    /// reset pen width to default
    virtual void resetPenWidth(void)
      {
        mPenWidth = DEF_PENWIDTH;
        setpen(mPenColor.mColor,mPenWidth);
      }

    //----------

    //virtual void setPenStyle(int aStyle)
    //  {
    //  };

    //----------

    //virtual void setBrushStyle(int aStyle)
    //  {
    //  };

    //----------
    
    /// move to position [x, y]
    virtual void setPos(int aX, int aY)
      {
        MoveToEx(mDC,aX,aY,0);
        mXpos = aX;
        mYpos = aY;
      }

    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------
    /// sets a rectangle region
    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        HRGN hrgn = /*hrgn = */CreateRectRgn( aX1, aY1, aX2, aY2 );
        SelectClipRgn(mDC,hrgn);
      }

    //----------
    /// clears rectangle region
    virtual void clearClipRect(void)
      {
        SelectClipRgn(mDC,NULL);
      }

    //----------

//    axRect getClipRect(void)
//      {
//        axRect R;
//        RECT rect;
//        HRGN hrgn;
//        int res1 = GetClipRgn(mDC,hrgn); // res = 0=no region, 1=region, -1=error
//        int res2 = GetRgnBox(hrgn,&rect); //
//        R.set(rect.left, rect.top, rect.right-rect.left+1, rect.bottom-rect.top+1);
//        return R;
//      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------
    /// draw a point at coordinates x, y
    virtual void drawPoint(int aX, int aY)
      {
        SetPixel(mDC,aX,aY,mPenColor.mColor);
      }
    
    /// draw line from point A[x,y] to B[x,y]
    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(mDC,aX2,aY2);
        drawPoint(aX2,aY2);         // !!!
      }

    /// draw a rectangle A{x,y] to B[x,y]
    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        // todo setpos + 4 lineto
        drawLine(aX1, aY1, aX2, aY1);
        drawLine(aX1, aY1, aX1, aY2);
        drawLine(aX1, aY2, aX2, aY2);
        drawLine(aX2, aY1, aX2, aY2);
      }

    /// fill rectangle A{x,y] to B[x,y]
    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2+1;           // !!!
        R.bottom  = aY2+1;           // !!!
        FillRect(mDC,&R,mBrush);
      }
    
    /// draw a circle from A{x,y] to B[x,y] 
    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        //Ellipse(mDC, aX1,aY1,aX2,aY2 );
        Arc( mDC,aX1,aY1,aX2+1,aY2+1,0,0,0,0);
      }

    /// fill circle from A{x,y] to B[x,y]
    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        Ellipse( mDC,aX1,aY1,aX2,aY2);
      }

    /// draw an arch
    /// angle 1 = start angle, relative to 3 o'clock
    /// angle 2 = 'distance' 0..1, counter-clockwise
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
    

    /// fill arch
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
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
          Pie(mDC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------
    
    /// get text width
    /// \param aText axString input text
    virtual int textWidth(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cx;
      }

    //----------
    
    /// get text height
    virtual int textHeight(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cy;
      }

    //----------
    

    /// draw text at [x,y]  
    virtual void drawText(int aX, int aY, axString aText)
      {
        //SetBkMode(mDC,TRANSPARENT);
        //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
        drawText( aX,aY, aX+1000,aY+1000, aText, tal_Left|tal_Top);
      }

    //--------
    
    /// draw text at [x,y] with background and alignment
    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        SetBkMode(mDC,TRANSPARENT);
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2;
        R.bottom  = aY2;
        int flags = 0;
        if (aAlign & tal_Left) flags = flags | DT_LEFT;
        else if (aAlign & tal_Right) flags = flags | DT_RIGHT;
        else flags = flags | DT_CENTER;
        if (aAlign & tal_Top) flags = flags | DT_TOP;
        else if (aAlign & tal_Bottom) flags = flags | DT_BOTTOM;
        else flags = flags | DT_VCENTER;
        HFONT oldfont = (HFONT)SelectObject(mDC, mFont);
        DrawText(mDC, aText.ptr(), -1, &R, flags | DT_NOCLIP | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(mDC, oldfont);
      }

    //--------------------------------------------------
    // bitmap
    //--------------------------------------------------
    /// draw bitmap
    virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = CreateCompatibleDC(mDC);
        SelectObject(tempdc,(HBITMAP)aBitmap->getHandle());
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
        DeleteDC(tempdc);
      }

    //--------------------------------------------------
    // canvas (surface[drawable])
    //--------------------------------------------------
    /// blit canvas
    virtual void blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HBITMAP)mHandle->mDC;
        axCanvasImpl* ci = (axCanvasImpl*)aCanvas;
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,ci->getDC(),aSrcX,aSrcY,SRCCOPY);
      }

};


//----------------------------------------------------------------------
#endif






