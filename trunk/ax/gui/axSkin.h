#ifndef axSkin_included
#define axSkin_included
//----------------------------------------------------------------------

#include "gui/axCanvas.h"
#include "gui/axCanvas.h"

class axSkin
{
  protected:
    axColors  mColors;
  public:
    axSkin(axCanvas* aCanvas)
      {
      }
    virtual ~axSkin()
      {
      }
    virtual void appendColor(axColor aColor)
      {
        mColors.append(aColor);
      }
    virtual axColor getColor(int aIndex)
      {
        return mColors[aIndex];
      }
};

//----------------------------------------------------------------------

class axSkinDef : public axSkin
{
  public:
    //axColor mFillColor;
    //axColor mBorderColor;
    //axColor mTextColor;
  public:
    axSkinDef(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        //mFillColor   = aCanvas->getColor(AX_GREY);
        //mBorderColor = aCanvas->getColor(AX_GREY_DARK);
        //mTextColor   = aCanvas->getColor(AX_GREY_LIGHT);
        appendColor( aCanvas->getColor(AX_GREY_DARK) );
        appendColor( aCanvas->getColor(AX_GREY) );
        appendColor( aCanvas->getColor(AX_GREY_LIGHT) );
        appendColor( aCanvas->getColor(AX_WHITE) );
      }
    virtual ~axSkinDef()
      {
      }
};

//----------------------------------------------------------------------
#endif
