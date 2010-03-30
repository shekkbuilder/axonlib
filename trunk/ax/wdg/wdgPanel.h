#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

//#include "gui/axWidget.h"
#include "gui/axContainer.h"

class wdgPanel : public axContainer//axWidget
{
  protected:
    int mMode;

  public:
    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, int aMode=0)
    //: axWidget(aListener,aRect,aAlignment)
    : axContainer(aListener,aRect,aAlignment)
      {
        mMode=aMode;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          switch(mMode)
          {
            case 0:
              aCanvas->selectBrush(mSkin->getBrush(1)); // assume default-brush?
              aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
              break;
          }
        }
        axContainer::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
#endif