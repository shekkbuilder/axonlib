#ifndef wdgBitmap_included
#define wdgBitmap_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgBitmap : public axWidget
{
  protected:
    axBitmap* mBitmap;
  public:
    wdgBitmap(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axBitmap* aBitmap=NULL)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
        mBitmap = aBitmap;
      }

    inline void setBitmap(axBitmap* aBitmap) { mBitmap = aBitmap; }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int dstx = mRect.x;
        int dsty = mRect.y;
        int srcx = 0;
        int srcy = 0;
        int srcw = mRect.w;
        int srch = mRect.h;
        aCanvas->drawBitmap(mBitmap, dstx,dsty, srcx,srcy,srcw,srch);
        axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

