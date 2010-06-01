#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

//#include "gui/axCanvas.h"

//----------------------------------------------------------------------

// draw modes
#define dm_Normal   0
#define dm_Hover    1
#define dm_Active   2
#define dm_Disabled 4

//----------------------------------------------------------------------

//class axSkinBase
class axSkin
{
  protected:
    axColor mFillColor;
    axColor mLightColor;
    axColor mDarkColor;
    axColor mTextColor;

  public:

    //axSkin(axCanvas* aCanvas) {}
    //virtual ~axSkin() {}

    inline axColor getFillColor(void)   { return mFillColor; }
    inline axColor getLightColor(void)  { return mLightColor; }
    inline axColor getDarkColor(void)   { return mDarkColor; }
    inline axColor getTextColor(void)   { return mTextColor; }

    virtual void drawPanel(     axCanvas* aCanvas, axRect aRect, int aMode=0)  {}
    virtual void drawSizer(     axCanvas* aCanvas, axRect aRect, int aMode=0)  {}
    virtual void drawLabel(     axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign, int aMode=0) {}
    virtual void drawValue(     axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue, int aMode=0) {}
    virtual void drawButton(    axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign, bool aState, int aMode=0) {}
    virtual void drawSlider(    axCanvas* aCanvas, axRect aRect,  float aValue, axString aText1, axString aText2, bool aVertical, int aMode=0) {}
    virtual void drawKnob(      axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue, int aMode=0) {}
    virtual void drawScrollBar( axCanvas* aCanvas, axRect aRect,  float aValue, bool aVertical, float aThumbSize, int aMode=0) {}
};

//----------------------------------------------------------------------
#endif
