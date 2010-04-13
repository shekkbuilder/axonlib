#ifndef wdgImage_included
#define wdgImage_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"

class wdgImage : public axWidget//axContainer
{
  protected:
    axImage* mImage;
  public:
    wdgImage(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None, axImage* aImage=NULL)
    //: axContainer(aListener,/*aId,*/aRect,aAlignment)
    : axWidget(aListener,/*aId,*/aRect,aAlignment)
      {
        //clearFlag(wf_Active);
        mImage = aImage;
      }

    inline void setImage(axImage* aImage) { mImage = aImage; }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int dstx = mRect.x;
        int dsty = mRect.y;
        int srcx = 0;
        int srcy = 0;
        int srcw = mRect.w;
        int srch = mRect.h;
        aCanvas->drawImage(mImage,dstx,dsty, srcx,srcy,srcw,srch);
        //axContainer::doPaint(aCanvas,aRect);
        axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

