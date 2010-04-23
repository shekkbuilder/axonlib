#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

#include "gui/axCanvas.h"

//----------------------------------------------------------------------

// draw modes
#define dm_Normal   0
#define dm_Hover    1
#define dm_Active   2
#define dm_Disabled 4

//----------------------------------------------------------------------

class axSkin
{
  protected:
    axColor mFillColor;
    axColor mLightColor;
    axColor mDarkColor;
    axColor mTextColor;
  public:
    axSkin(axCanvas* aCanvas)
      {
        mFillColor  = aCanvas->getColor(AX_GREY);
        mLightColor = aCanvas->getColor(AX_GREY_LIGHT);
        mDarkColor  = aCanvas->getColor(AX_GREY_DARK);
        mTextColor  = aCanvas->getColor(AX_WHITE);
      }
    virtual ~axSkin() {}
    virtual axColor getFillColor(void)  { return mFillColor; }
    virtual axColor getDarkColor(void)  { return mDarkColor; }
    virtual axColor getLightColor(void) { return mLightColor; }
    virtual axColor getTextColor(void)  { return mTextColor; }
    virtual void drawPanel(     axCanvas* aCanvas, axRect aRect)  {}
    virtual void drawSizer(     axCanvas* aCanvas, axRect aRect)  {}
    virtual void drawLabel(     axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign) {}
    virtual void drawValue(     axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue) {}
    virtual void drawButton(    axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign, bool aState) {}
    virtual void drawSlider(    axCanvas* aCanvas, axRect aRect,  float aValue, axString aText1, axString aText2, bool aVertical) {}
    virtual void drawKnob(      axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue) {}
    virtual void drawScrollBar( axCanvas* aCanvas, axRect aRect,  float aValue, bool aVertical, float aThumbSize) {}
};

//----------------------------------------------------------------------
#endif
