#ifndef axCanvasBase_included
#define axCanvasBase_included
//----------------------------------------------------------------------

#include "gui/axColor.h"
#include "gui/axPen.h"
#include "gui/axBrush.h"
#include "gui/axFont.h"
#include "gui/axBitmap.h"

//----------------------------------------------------------------------

// text align
#define ta_Center        0
#define ta_Left          1
#define ta_Top           2
#define ta_Right         4
#define ta_Bottom        8
#define ta_TopLeft       16
#define ta_TopRight      32
#define ta_BottomLeft    64
#define ta_BottomRight   128
#define ta_RightStretch  256
#define ta_BottomStretch 512
#define ta_Horz          1024
#define ta_Vert          2048

//----------------------------------------------------------------------

class axCanvasBase
{
  protected:
    axPens    mPens;
    axBrushes mBrushes;
    axFonts   mFonts;

  public:

    axCanvasBase() {}

    virtual ~axCanvasBase()
      {
        int i;
        for (i=0; i<mPens.size(); i++) deletePen(i);
        for (i=0; i<mBrushes.size(); i++) deleteBrush(i);
        for (i=0; i<mFonts.size(); i++) deleteFont(i);;
      }

    // get / set

    virtual void    setPos(int aX, int aY) {}

    // pen/brush/font

//    virtual axPen*  createPen(int r, int g, int b, int size=DEF_PENWIDTH) { return NULL; }
//    virtual axBrush createBrush(int r, int g, int b, int style=DEF_BRUSHSTYLE) { return axBrush(); }
//    virtual axFont  createFont(axString name, int r, int g, int b, int size=-1, int style=0) { return axFont(); }

    virtual axColor createColor(int aRed, int aGreen, int aBlue) { return axColor(0); }
    virtual int     createPen(int r, int g, int b, int size=DEF_PENWIDTH) { return 0; }
    virtual int     createBrush(int r, int g, int b, int style=DEF_BRUSHSTYLE) { return 0; }
    virtual int     createFont(axString name, int r, int g, int b, int size=-1, int style=0) { return 0; }

    //virtual int     createPen(axColor aColor, int size=DEF_PENWIDTH) { return 0; }
    //virtual int     createBrush(axColor aColor, int style=DEF_BRUSHSTYLE) { return 0; }
    //virtual int     createFont(axString aName, axColor aColor, int size=DEF_PENWIDTH, int style=DEF_BRUSHSTYLE) { return 0; }

    virtual void    deletePen(int aPen) {}
    virtual void    deleteBrush(int aBrush) {}
    virtual void    deleteFont(int aFont) {}

    virtual void    clearPen(void) {}
    virtual void    selectPen(int aPen) {}
    virtual void    selectBrush(int aBrush) {}
    virtual void    selectFont(int aFont) {}

    // clip rect

    virtual void    setClipRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    clearClipRect(void) {}

    // shapes

    virtual void    drawPoint(int aX, int aY) {}
    virtual void    drawLine(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    fillRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    fillCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}
    virtual void    fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}

    // text

    virtual int     textWidth(axString aText) {return 0;}
    virtual int     textHeight(axString aText) {return 0;}
    virtual void    drawText(int aX, int aY, axString aText) {}
    virtual void    drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign) {}

    // bitmap

    //virtual void    drawBitmap(axBitmapBase* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}
    //virtual void    blit(axCanvasBase* aCanvas, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}

    //----------

};

//----------------------------------------------------------------------
#endif
