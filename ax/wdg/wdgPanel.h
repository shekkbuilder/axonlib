#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"

class wdgPanel : public axContainer
{
  protected:
    axColor   mPenColor;
    axColor   mBrushColor;
    axColor   mTextColor;
    axImage*  mImage;
    axString  mText;
    int       mTextAlign;

  public:

    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axContainer(aListener,aRect,aAlignment)
      {
        mFlags.setFlag(wf_Fill);
        mFlags.setFlag(wf_Border);
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mFlags.hasFlag(wf_Fill))
        {
          aCanvas->setBrushColor(mBrushColor);
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        }
        if (mFlags.hasFlag(wf_Image))
        {
          aCanvas->drawImage(mImage,0,0,mRect.x,mRect.y,mRect.w,mRect.h);
        }
        if (mFlags.hasFlag(wf_Border))
        {
          aCanvas->setPenColor(mPenColor);
          aCanvas->drawRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        }
        if (mFlags.hasFlag(wf_Text))
        {
          aCanvas->setPenColor(mPenColor);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
        }
        axContainer::doPaint(aCanvas,aRect);
      }

     //virtual void setColors(axColor aPenColor, axColor aBrushColor, axColor aTextColor)
     // {
     //   mPenColor   = aPenColor;
     //   mBrushColor = aBrushColor;
     //   mTextColor  = aTextColor;
     // }

     virtual void setSkin(axSkin* aSkin, bool aSubWidgets=false)
      {
        mPenColor = aSkin->getColor(0);
        mBrushColor = aSkin->getColor(2);
        axContainer::setSkin(aSkin,aSubWidgets);
      }


};

//----------------------------------------------------------------------
#endif
