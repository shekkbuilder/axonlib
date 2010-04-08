//#ifndef wdgPanel_included
//#define wdgPanel_included
////----------------------------------------------------------------------
//
//#include "gui/axContainer.h"
//#include "gui/axSkin.h"
//
//
//class wdgPanel : public axContainer
//{
//  protected:
//
//  public:
//
//    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
//    : axContainer(aListener,aRect,aAlignment)
//      {
//        //mFlags |= wf_Fill;
//        //mFlags |= wf_Border;
//        mOptions = 0;
//        mImage = NULL;
//        mText = "wdgPanel";
//        mTextAlign = ta_Center;
//        mSkin = NULL;
//      }
//
//
//    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
//      {
//        axContainer::doPaint(aCanvas,aRect);
//      }
//
//    //----------
//
//     virtual void setSkin(axSkin* aSkin, bool aSubWidgets=false)
//      {
//        //mSkin = aSkin;
//        mLightColor = aSkin->getColor(3);
//        mDarkColor  = aSkin->getColor(1);
//        mFillColor  = aSkin->getColor(2);
//        mTextColor  = aSkin->getColor(4);
//        axContainer::setSkin(aSkin,aSubWidgets);
//      }
//
//};
//
////----------------------------------------------------------------------
//#endif
