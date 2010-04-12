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
  public:
    axSkin(axCanvas* aCanvas) {}
    virtual ~axSkin() {}
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
