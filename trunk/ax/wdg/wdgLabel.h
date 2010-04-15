#ifndef wdgLabel_included
#define wdgLabel_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgLabel : public axWidget
{
  protected:
    axString  mText;
    int       mTextAlign;
  public:
    wdgLabel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
             axString aText="", int aTextAlign=ta_Center)
    : axWidget(aListener,aRect,aAlignment)
      {
        clearFlag(wf_Active);
        mText = aText;
        mTextAlign = aTextAlign;
      }

    inline void setText(axString aText, int aTextAlign=ta_Center)
      {
        mText = aText;
        mTextAlign = aTextAlign;
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawLabel(aCanvas,mRect,mText,mTextAlign);
        //axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

