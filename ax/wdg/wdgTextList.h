#ifndef wdgTextList_included
#define wdgTextList_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgTextList : public axWidget
{
  protected:
  public:
    wdgTextList(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
      }

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    //if (mSkin) mSkin->drawTextList(aCanvas,mRect);
    //    //axWidget::doPaint(aCanvas,aRect);
    //  }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

