#ifndef wdgScrollBox_included
#define wdgScrollBox_included
//----------------------------------------------------------------------
/*
  currently uses a ScrollBar and a panel
  todo: let user decide container instead of wdgPanel
*/

//#include "gui/axContainer.h"
#include "gui/axWidget.h"
#include "wdg/wdgScrollBar.h"

class wdgScrollBox : public axWidget
{
  protected:
    wdgScrollBar* wScrollBar;
    wdgPanel*     wContainer;
    int           mScrolledX;   // how much we've already scrolled
    int           mScrolledY;   // -_"--
    float         mVisible;     // how much of the content is visible (calculated in doRealign)

  public:

    wdgScrollBox(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener, aRect, aAlignment)
      {
        axWidget::appendWidget( wScrollBar  = new wdgScrollBar(this,axRect(0,0,15,0),wa_Right ) );
        axWidget::appendWidget( wContainer  = new wdgPanel( this,NULL_RECT,       wa_Client ) );
        wScrollBar->setFlag(wf_Vertical);
        wScrollBar->setThumbSize(1);
        mScrolledX = mScrolledY = 0;
        mVisible = 1;
      }

    //virtual ~wdgScrollBox()
    //  {
    //  }

    //--------------------------------------------------

    // accessors
    inline axWidget*      getContainer(void) { return wContainer; }
    inline wdgScrollBar*  getScrollBar(void) { return wScrollBar; }

    //inline void setContainer(axWidget* c)
    //{
    //  if (wContainer) delete wContainer;
    //  wContainer = c;
    //  mWidgets[1] = c;
    //  doRealign();
    //}

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    void unscroll(void)
      {
        doScroll(-mScrolledX,-mScrolledY);
        wScrollBar->setValue(0);
      }

    //----------

    void calc_thumbsize(void)
      {
        float height  = (float)wContainer->getRect().h;       // size of container
        float content = (float)wContainer->getContent().h;    // size of content
        if (content>height)
        {
          mVisible = height / content;
          wScrollBar->setThumbSize(mVisible);
        }
        else
        {
          mVisible = 1;
          wScrollBar->setThumbSize(1);
        }
      }

    //----------

    void set_scrollpos(float x, float y)
      {
        float scrollable = (float)wContainer->getContent().h * (1-mVisible);
        int yscroll = (int)(scrollable*y);
        int dy = yscroll - mScrolledY;
        doScroll(0,dy);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual int appendWidget(axWidget* aWidget)
      {
        int index = wContainer->appendWidget(aWidget);
        return index;

      }

    //--------------------------------------------------
    // do...
    //--------------------------------------------------

    virtual void doScroll(int dX, int dY)
      {
        for( int i=0; i<wContainer->getNumWidgets(); i++ )
        {
          axWidget* wdg = wContainer->getWidget(i);
          wdg->doSetPos( wdg->getRect().x - dX, wdg->getRect().y - dY );
        }
        mScrolledX += dX;
        mScrolledY += dY;
        //axWidget::doScroll(dX,dY);
      }

    //----------

    //virtual void doResize(int aW, int aH)
    //  {
    //    axWidget::doResize(aW,aH);
    //    calc_thumbsize();
    //  }

    //----------

    virtual void doRealign(void)
      {
        float y = wScrollBar->getValue();
        unscroll();
        axWidget::doRealign();
        calc_thumbsize();
        set_scrollpos(0,y);
        wScrollBar->setValue(y);
      }

    //----------

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    if (mSkin) mSkin->drawPanel(aCanvas,mRect);
    //    axWidget::doPaint(aCanvas,aRect);
    //  }

    //--------------------------------------------------
    // on...
    //--------------------------------------------------

    // if scrollbar has changed, we need to move sub-widgets

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==wScrollBar)
        {
          float val = aWidget->getValue();
          float scrollable = (float)wContainer->getContent().h * (1-mVisible);
          int i = (int)(scrollable*val);
          int dy = i - mScrolledY;
          doScroll(0,dy);
          mListener->onChange(wContainer);
        }
        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif

