#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

#include "gui/axCanvas.h"

// draw modes
#define dm_Normal   0
#define dm_Hover    1
#define dm_Active   2
#define dm_Disabled 4

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------
/*

can a canvas change between creation of this class and use of it?
if not, we can cache the axCanvas* and reuse it instwead of
all this parameter passing

*/
//----------------------------------------------------------------------



class axSkin
{
  public:
    axSkin(axCanvas* aCanvas) {}
    virtual ~axSkin() {}
    virtual void drawPanel( axCanvas* aCanvas, axRect aRect) {}
    virtual void drawSizer( axCanvas* aCanvas, axRect aRect) {}
    virtual void drawLabel( axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign) {}
    virtual void drawValue(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue) {}
    virtual void drawButton(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, bool aState) {}
    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical) {}
    virtual void drawKnob(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue) {}
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axSkinDefault : public axSkin
{
  protected:
    axColor mFillColor;
    axColor mLightColor;
    axColor mDarkColor;
    axColor mTextColor;

  public:

    axSkinDefault(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        mFillColor    = aCanvas->getColor(AX_GREY);
        mLightColor   = aCanvas->getColor(AX_GREY_LIGHT);
        mDarkColor    = aCanvas->getColor(AX_GREY_DARK);
        mTextColor    = aCanvas->getColor(AX_WHITE);
      }
    virtual ~axSkinDefault()
      {
      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    void fill_back(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mFillColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    virtual void fill_dark(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mDarkColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    virtual void draw_frame(axCanvas* aCanvas, axRect aRect, bool aDown=false)
      {
        if (aDown) aCanvas->setPenColor(mDarkColor);
        else aCanvas->setPenColor(mLightColor);
        aCanvas->drawLine(aRect.x,aRect.y,aRect.x2()-1,aRect.y);
        aCanvas->drawLine(aRect.x,aRect.y,aRect.x,aRect.y2()-1);
        if (aDown) aCanvas->setPenColor(mLightColor);
        else aCanvas->setPenColor(mDarkColor);
        aCanvas->drawLine(aRect.x+1,aRect.y2(),aRect.x2(),aRect.y2());
        aCanvas->drawLine(aRect.x2(),aRect.y+1,aRect.x2(),aRect.y2());
        //aCanvas->setTextColor(mTextColor);
      }

    //----------

    virtual void draw_text(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign)
      {
        aCanvas->setTextColor(mTextColor);
        aCanvas->drawText(aRect.x,aRect.y,aRect.x2(),aRect.y2(),aText,aTextAlign);
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
      {
        fill_back(aCanvas,aRect);
        draw_frame(aCanvas,aRect);
      }

    //----------

    virtual void drawSizer(axCanvas* aCanvas, axRect aRect)
      {
        fill_dark(aCanvas,aRect);
      }

    //----------

    virtual void drawLabel(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign)
      {
        draw_text(aCanvas,aRect,aText,aTextAlign);
      }

    //----------

    virtual void drawValue(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue)
      {
        fill_dark( aCanvas,aRect);
        axRect r = aRect;
        r.add(3,3,-6,-6);
        draw_text(aCanvas,r,aName,ta_Left);
        draw_text(aCanvas,r,aDisp,ta_Right);
      }

    //----------

    virtual void drawButton(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, bool aState)
      {
        fill_back( aCanvas,aRect);
        draw_frame(aCanvas,aRect,aState);
        draw_text( aCanvas,aRect,aText,aTextAlign);
      }

    //----------

    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical)
      {
        if (aVertical)
        {
          int size = (float)aRect.h * (1-aValue);
          int y1 = aRect.y;
          int y2 = aRect.y+size;
          int h1 = size;
          int h2 = aRect.h-size;
          fill_dark( aCanvas,axRect(aRect.x, y1, aRect.w, h1 ));
          fill_back( aCanvas,axRect(aRect.x, y2, aRect.w, h2 ));
          draw_frame(aCanvas,axRect(aRect.x, y2, aRect.w, h2 ));
        }
        else
        {
          int size = (float)aRect.w * aValue;
          fill_back( aCanvas,axRect(aRect.x,     aRect.y, size,        aRect.h));
          draw_frame(aCanvas,axRect(aRect.x,     aRect.y, size,        aRect.h));
          fill_dark( aCanvas,axRect(aRect.x+size,aRect.y, aRect.w-size,aRect.h));
          axRect r = aRect;
          r.add(3,3,-6,-6);
          draw_text(aCanvas,r,aText1,ta_Left);
          draw_text(aCanvas,r,aText2,ta_Right);
        }
      }

    //----------

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue)
      {
        int x  = aRect.x;
        int y  = aRect.y;
        int size = axMin(aRect.w,aRect.h);
        aCanvas->setPenColor(mDarkColor);
        aCanvas->setPenWidth(5);
        aCanvas->drawArc(x+2,y+2,x+(size-2),y+(size-2),0.6,0.8);
        aCanvas->resetPen();
        aCanvas->setPenColor(mLightColor);
        aCanvas->setPenWidth(5);
        aCanvas->drawArc(x+2,y+2,x+(size-2),y+(size-2),0.6,aValue*0.8);
        aCanvas->resetPen();
        //aCanvas->clearPen();
        aCanvas->setTextColor(mTextColor);
        aCanvas->drawText(x+size+5,y,aRect.x2(),aRect.y2(),aDisp,ta_Left);
      }


};

//----------------------------------------------------------------------
#endif
