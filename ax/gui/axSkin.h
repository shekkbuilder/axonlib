#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

#include "gui/axCanvas.h"
#include "gui/axCanvas.h"

class axSkin
{
  public:
    axColor mFillColor;
    axColor mBorderColor;
    axColor mTextColor;
    axColor mLightColor;
    axColor mDarkColor;
  public:
    axSkin(axCanvas* aCanvas)
      {
      }
    virtual ~axSkin()
      {
      }
};

//----------------------------------------------------------------------

class axSkinDefault : public axSkin
{
  public:
    axSkinDefault(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        mFillColor    = aCanvas->getColor(AX_GREY);
        mBorderColor  = aCanvas->getColor(AX_GREY_DARK);
        mTextColor    = aCanvas->getColor(AX_GREY_LIGHT);
        mLightColor   = aCanvas->getColor(AX_GREY_LIGHT);
        mDarkColor    = aCanvas->getColor(AX_GREY_DARK);
      }
    virtual ~axSkinDefault()
      {
      }
};

//----------------------------------------------------------------------
#endif
