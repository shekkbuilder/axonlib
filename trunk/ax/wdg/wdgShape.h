#ifndef wdgShape_included
#define wdgShape_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgShape : public axWidget
{
  protected:
  public:
    wdgShape(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
      }

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    //if (mSkin) mSkin->drawShape(aCanvas,mRect);
    //    //axWidget::doPaint(aCanvas,aRect);
    //  }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

