#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

//#include "gui/axContainer.h"
#include "gui/axWidget.h"

class wdgPanel : public axWidget//axContainer
{
  public:
    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (isVisible() && mSkin)
        {
          mSkin->drawPanel(aCanvas,mRect/*,mSkinMode*/);
          axWidget::doPaint(aCanvas,aRect);
        }
      }
    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }
};

//----------------------------------------------------------------------
#endif


