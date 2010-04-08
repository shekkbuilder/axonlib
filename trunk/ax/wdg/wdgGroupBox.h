//#ifndef wdgGroupBox_included
//#define wdgGroupBox_included
////----------------------------------------------------------------------
//
//#include "wdg/wdgPanel.h"
//
//class wdgGroupBox : public wdgPanel
//{
//  protected:
//    axColor   mPenColor;
//    axColor   mBrushColor;
//    axColor   mTextColor;
//    axImage*  mImage;
//    axString  mText;
//    int       mTextAlign;
//
//  public:
//
//    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
//    : axContainer(aListener,aRect,aAlignment)
//      {
//        mFlags |= wf_Fill;
//        mFlags |= wf_Border;
//      }
//
//    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
//      {
//        if (mFlags&wf_Fill)
//        {
//          aCanvas->setBrushColor(mBrushColor);
//          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
//        }
//        if (mFlags&wf_Image)
//        {
//          aCanvas->drawImage(mImage,0,0,mRect.x,mRect.y,mRect.w,mRect.h);
//        }
//        if (mFlags&wf_Border)
//        {
//          aCanvas->setPenColor(mPenColor);
//          aCanvas->drawRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
//        }
//        if (mFlags&wf_Text)
//        {
//          aCanvas->setPenColor(mPenColor);
//          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
//        }
//        axContainer::doPaint(aCanvas,aRect);
//      }
//
//     virtual void setSkin(axSkin* aSkin, bool aSubWidgets=false)
//      {
//        mPenColor = aSkin->getColor(0);
//        mBrushColor = aSkin->getColor(2);
//        axContainer::setSkin(aSkin,aSubWidgets);
//      }
//
//
//};
//
////----------------------------------------------------------------------
//#endif
