#ifndef wdgSizer_included
#define wdgSizer_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgSizer : public axWidget
{
  protected:
    int prevx,prevy;
    bool mIsDragging;
    axContainer* mTarget;
  public:
    wdgSizer(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,/*aId,*/aRect,aAlignment)
      {
        mTarget = NULL;
        mIsDragging = false;
      }

    inline void setTarget(axContainer* aTarget) { mTarget=aTarget; }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawSizer(aCanvas,mRect);
        //axWidget::doPaint(aCanvas,aRect);
      }

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        mIsDragging = true;
        prevx = aXpos;
        prevy = aYpos;
        //axWidget::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        mIsDragging = false;
        //axWidget::doMouseUp(aXpos,aYpos,aButton);
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mIsDragging)
        {
          int deltax = aXpos - prevx;
          int deltay = aYpos - prevy;
          //mListener->onSize(deltax,deltay);
          if (mTarget) mTarget->onSize(deltax,deltay);
          prevx = aXpos;
          prevy = aYpos;
        }
        //axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    virtual void doEnter(axWidget* aCapture)
      {
        mListener->onCursor(cu_ArrowLeftRight);
      }

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }


};

//----------------------------------------------------------------------
#endif
