#ifndef axDemo_skin_included
#define axDemo_skin_included
//----------------------------------------------------------------------

//#include "gui/axSkin.h"
#include "gui/axSkinDefault.h"

class axDemo_skin : public axSkinDefault
{
  // let us access widgets, etc, directly
  friend class axDemo;

//  protected:
//    axColor mFillColor;
//    axColor mLightColor;
//    axColor mDarkColor;
//    axColor mTextColor;

  public:

    axDemo_skin(axCanvas* aCanvas)
    : axSkinDefault(aCanvas)
      {
//        mFillColor    = aCanvas->getColor(AX_GREY);
//        mLightColor   = aCanvas->getColor(AX_GREY_LIGHT);
//        mDarkColor    = aCanvas->getColor(AX_GREY_DARK);
//        mTextColor    = aCanvas->getColor(AX_WHITE);
      }

    virtual ~axDemo_skin()
      {
      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

};

//----------------------------------------------------------------------
#endif
