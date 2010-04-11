#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"

class wdgPanel : public axContainer
{
  public:
    wdgPanel(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None)
    : axContainer(aListener,/*aId,*/aRect,aAlignment)
      {
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //if (mSkin)
        //{
        //  mSkin->fillBack(aCanvas,mRect);
        //  mSkin->drawFrame(aCanvas,mRect);
        //}
        if (mSkin) mSkin->drawPanel(aCanvas,mRect);
        axContainer::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture)
    //  {
    //    mListener->onCursor(cu_ArrowLeftRight);
    //  }

    //virtual void doLeave(axWidget* aCapture)
    //  {
    //    mListener->onCursor(DEF_CURSOR);
    //  }

};

//----------------------------------------------------------------------
#endif


