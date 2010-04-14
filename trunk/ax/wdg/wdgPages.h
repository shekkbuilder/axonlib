#ifndef wdgPages_included
#define wdgPages_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgPages : public axWidget
{
  protected:
  public:
    wdgPages(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
      }

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    //if (mSkin) mSkin->drawPages(aCanvas,mRect);
    //    //axWidget::doPaint(aCanvas,aRect);
    //  }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

