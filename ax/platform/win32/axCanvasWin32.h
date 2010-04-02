#ifndef axCanvasWin32_included
#define axCanvasWin32_included
//----------------------------------------------------------------------

//#include <math.h>
#include "core/axMath.h" // axMax

#include "platform/axContext.h"
#include "axDefines.h"
#include "base/axCanvasBase.h"
#include "base/axSurfaceBase.h"

class axCanvasWin32 : public axCanvasBase
{
  private:
		HDC     mDC;
    HFONT   mFont;
		HPEN    mPen, mOldPen, mOldPen2;
		HBRUSH  mBrush, mOldBrush, mOldBrush2;
		int     mWinHandle;
    RECT    R;
    HPEN    mNullPen;
  protected:
    int     mCurrentPen;
    int     mCurrentBrush;
    int     mCurrentFont;
    int     mXpos,mYpos;

  public:

    axCanvasWin32(axContext* aContext)
    : axCanvasBase(/*aContext*/)
      {
        mCurrentPen = 0;
        mCurrentBrush = 0;
        mCurrentFont = 0;
        mPen    = 0;
        mBrush  = 0;
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
      }

    //----------

    virtual ~axCanvasWin32()
      {
        DeleteObject(mNullPen);
        if (mPen)   DeleteObject(mPen);
        if (mBrush) DeleteObject(mBrush);
        if (mFont)  DeleteObject(mFont);
        if (mWinHandle) ReleaseDC((HWND)mWinHandle,mDC);
        else DeleteDC(mDC);
//        delete mNullPen;
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

    virtual axColor createColor(int aRed, int aGreen, int aBlue)
      {
        axColor color = RGB(aRed,aGreen,aBlue);
        return color;
      }

    //----------

    virtual int createPen(int r, int g, int b, int size=DEF_PENWIDTH)
      {
        axPen pen;
        axColor color = createColor(r,g,b);
        pen.mColor  = color;
        pen.mSize   = size;
        pen.mHandle = CreatePen(PS_SOLID,size,color.get());
        int num = mPens.size();
        mPens.append(pen);
        return num;
      }

    //----------

    virtual int createBrush(int r, int g, int b, int style=DEF_BRUSHSTYLE)
      {
        axBrush brush;
        axColor color = createColor(r,g,b);
        brush.mColor  = color;
        brush.mStyle  = style;
        brush.mHandle = CreateSolidBrush(color.get());
        int num = mBrushes.size();
        mBrushes.append(brush);
        return num;
      }

    //----------

    virtual int createFont(axString name, int r, int g, int b, int size=-1, int style=0)
      {
        axFont font;
        axColor color = createColor(r,g,b);
        font.mName    = name;
        font.mColor   = color;
        font.mSize    = size;
        font.mStyle   = style;
        int num = mFonts.size();
        mFonts.append(font);
        return num;
      }

    //----------

    virtual void deletePen(int aPen)
      {
        DeleteObject(mPens[aPen].mHandle);
      }

    virtual void deleteBrush(int aBrush)
      {
        DeleteObject(mBrushes[aBrush].mHandle);
      }

    virtual void deleteFont(int aFont)
      {
      }

    //----------

    virtual void selectPen(int aPen)
      {
        mCurrentPen = aPen;
        mOldPen2 = (HPEN)SelectObject( (HDC)mDC, mPens[aPen].mHandle );
//        //TODO: set null brush?
      }

    virtual void selectBrush(int aBrush)
      {
        mCurrentBrush = aBrush;
        mOldBrush2 = (HBRUSH)SelectObject((HDC)mDC, mBrushes[aBrush].mHandle);
//        //TODO: set null pen?
      }

    virtual void selectFont(int aFont)
      {
        mCurrentFont = aFont;
        //SetTextColor(mDC,mTextColor.mColor);
        SetTextColor(mDC,mFonts[aFont].mColor.get() );
      }

    //----------

    virtual void clearPen(void) { mOldPen2 = (HPEN)SelectObject( (HDC)mDC, mNullPen );
      }

    //----------

    virtual void resetPen(void) { SelectObject(mDC,mOldPen2); }
    virtual void resetBrush(void) { SelectObject(mDC,mOldBrush2); }

    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        HRGN hrgn = /*hrgn = */CreateRectRgn( aX1, aY1, aX2, aY2 );
        SelectClipRgn(mDC,hrgn);
      }

    //----------

    virtual void clearClipRect(void)
      {
        SelectClipRgn(mDC,NULL);
      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
      }

    //----------

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
        drawLine(aX1, aY1, aX2, aY1);
        drawLine(aX1, aY1, aX1, aY2);
        drawLine(aX1, aY2, aX2, aY2);
        drawLine(aX2, aY1, aX2, aY2);
      }

    //----------

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2+1;           // !!!
        R.bottom  = aY2+1;           // !!!
        FillRect(mDC,&R,mBrush);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        //Ellipse(mDC, aX1,aY1,aX2,aY2 );
        Arc( mDC,aX1,aY1,aX2+1,aY2+1,0,0,0,0);
      }

    //----------

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        Ellipse( mDC,aX1,aY1,aX2,aY2);
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

//    virtual void drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        HDC tempdc = CreateCompatibleDC(mDC);
//        SelectObject(tempdc,(HBITMAP)aBitmap->getHandle());
//        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
//        DeleteDC(tempdc);
//      }

    //--------------------------------------------------
    // canvas (surface[drawable])
    //--------------------------------------------------

//    //virtual void blit(int aHandle, axPoint aDst, axRect aSrc)
//    virtual void blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
        //HDC tempdc = (HBITMAP)mHandle->mDC;
        //axCanvasImpl* ci = (axCanvasImpl*)aCanvas;
        //BitBlt(mDC,aX,aY,aSrcW,aSrcH,ci->getDC(),aSrcX,aSrcY,SRCCOPY);
//      }

//    virtual void drawSurface(axSurface* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
//      {
//        axCanvas* canvas = aSurface->getCanvas();
//        HDC tempdc = (HDC)canvas->getHandle();
//        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
//      }
    virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = (HDC)aImage->getHandle();
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
      }


};

typedef axCanvasWin32 axCanvasImpl;

//----------------------------------------------------------------------
#endif
