#ifndef axContainer_included
#define axContainer_included
//----------------------------------------------------------------------

#include "core/axRect.h"
//#include "gui/axCanvas.h"
//#include "gui/axSkin.h"
#include "gui/axWidget.h"
#include "axParameter.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class axContainer : public axWidget
{
  //friend class axPlugin;
  //friend class axContainer;

  protected:

    axWidgets         mWidgets;
    axWidget*         mCaptureWidget;
    axWidget*         mHoverWidget;

  public:

    axContainer(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mCaptureWidget = NULL;
        mHoverWidget = this;//NULL;
      }

    virtual ~axContainer()
      {
        deleteWidgets();
      }

    //----------

    virtual void  appendWidget(axWidget* aWidget) { mWidgets.append(aWidget); }
    virtual void  deleteWidgets(void) { for (int i=0; i<mWidgets.size(); i++) delete mWidgets[i]; }

    //----------

    virtual void setSkin(axSkin* aSkin, bool aChildren=false)
      {
        //mSkin = aSkin;
        axWidget::setSkin(aSkin,aChildren);
        if (aChildren)
        {
          for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->setSkin(aSkin,aChildren);
        }
      }


    // find first widget that contains x,y
    // or 'self' is no sub-widgets found or hit
    //
    // start searching from end of list, so that widgets that are painted last
    // (topmost) are found first.

    virtual axWidget* findWidget(int aXpos, int aYpos)
      {
        axWidget* widget = this;
        int num = mWidgets.size();
        if (num>0)
        {
          for (int i=num-1; i>=0; i--)
          {
            axWidget* w = mWidgets[i];
            if (w->isActive())
            {
              if (w->contains(aXpos,aYpos))
              {
                widget = w->findWidget(aXpos,aYpos);
                if (widget!=w) return widget;
              } //contains
            } //active
          } //for num
        } //num>0
        return widget;
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual void doMove(int aXpos, int aYpos)
      {
        int deltax = aXpos - mRect.x;
        int deltay = aYpos - mRect.y;
        for (int i=0; i<mWidgets.size(); i++)
        {
          axWidget* wdg = mWidgets[i];
          int wx = wdg->rect().x;
          int wy = wdg->rect().y;
          wdg->doMove( wx+deltax, wy+deltay );
        }
        //mRect.setPos(aXpos,aYpos);
        axWidget::doMove(aXpos,aYpos);
      }

    //----------

    virtual void doResize(int aWidth, int aHeight)
      {
        //mRect.setSize(aWidth,aHeight);
        axWidget::doResize(aWidth,aHeight);
      }

    //----------

    //TODO

    virtual void doRealign(void)
      {
        axWidget::doRealign();
      }

    //----------

    //TODO

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        for (int i=0; i<mWidgets.size(); i++)
        {
          axWidget* wdg = mWidgets[i];
          if (wdg->isVisible())
          {
            // glitches on non-buffered display
            //if (wdg->getRect().intersects(aRect))   // intersects update rect?
            //{
              wdg->doPaint(aCanvas,aRect);
            //}
          }
        }
        axWidget::doPaint(aCanvas,aRect);
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mCaptureWidget)
        {
          mCaptureWidget->doMouseDown(aXpos,aYpos,aButton);
        } //capture
        else
        {
          axWidget* hover = findWidget(aXpos,aYpos);
          if (hover!=this)
          {
            if (mFlags.hasFlag(wf_Capture)) mCaptureWidget = hover;
            hover->doMouseDown(aXpos,aYpos,aButton);
          }
        } //!capture
        axWidget::doMouseDown(aXpos,aYpos,aButton);

      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (mCaptureWidget)
        {
          mCaptureWidget->doMouseUp(aXpos,aYpos,aButton);
          mCaptureWidget = NULL;
        } //capture
        // send event to widget under mouse cursor
        else
        {
          axWidget* hover = findWidget(aXpos,aYpos);
          if (hover!=this)
          {
            hover->doMouseUp(aXpos,aYpos,aButton);
          }
        }
        axWidget::doMouseUp(aXpos,aYpos,aButton);
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        axWidget* hover = findWidget(aXpos,aYpos);
        if (hover!=mHoverWidget)
        {
          mHoverWidget->doLeave(mCaptureWidget);
          mHoverWidget = hover;
          mHoverWidget->doEnter(mCaptureWidget);
        }
        if (mCaptureWidget)
        {
          mCaptureWidget->doMouseMove(aXpos,aYpos,aButton);
        } //!capture
        // send event to widget under mouse cursor
        else
        {
          if (hover!=this)
          {
            hover->doMouseMove(aXpos,aYpos,aButton);
          }
        } //!capture
        axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    //----------

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mCaptureWidget)
        {
          mCaptureWidget->doKeyDown(aKeyCode,aState);
        }
        axWidget::doKeyDown(aKeyCode,aState);
      }

    //----------

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        if (mCaptureWidget)
        {
          mCaptureWidget->doKeyUp(aKeyCode,aState);
        }
        axWidget::doKeyUp(aKeyCode,aState);
      }

    //----------

    //virtual void doEnter(axWidget* aCapture)
    //  {
    //    axWidget::doEnter(aCapture);
    //  }

    //----------

    //virtual void doLeave(axWidget* aCapture)
    //  {
    //    axWidget::doEnter(aCapture);
    //  }

    //----------------------------------------
    // axWidgetListener
    //----------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        mListener->onChange(aWidget);
      }

};


//----------------------------------------------------------------------
#endif
