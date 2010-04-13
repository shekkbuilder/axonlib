#ifndef wdgGroupBox_included
#define wdgGroupBox_included
//----------------------------------------------------------------------

/*
  currently uses a button (title) and panel (content)
  todo: let user decide container instead of wdgPanel
*/

#include "gui/axWidget.h"
//#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------

class wdgGroupBox : public axWidget
{
  //protected:
  protected:
    wdgButton*  wTitleBar;
    wdgPanel*   wContainer;
    bool        mClosed;
    bool        mClosable;

    int prev_size;

  public:

    wdgGroupBox(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener, aRect, aAlignment)
      {
        axWidget::appendWidget( wTitleBar   = new wdgButton(  this,axRect(0,0,0,20),wa_Top,false,"group box","group box",ta_Center,bm_Spring ) );
        axWidget::appendWidget( wContainer  = new wdgPanel(   this,NULL_RECT,       wa_Client ) );
        mClosed   = false;
        mClosable = false;
      }

    //virtual ~wdgGroupBox()
    //  {
    //  }

    //--------------------------------------------------

    // accessors
    inline axWidget*  getContainer(void) { return wContainer; }
    inline wdgButton* getHeader(void) { return wTitleBar; }

    //--------------------------------------------------

    virtual int appendWidget(axWidget* aWidget)
      {
        return  wContainer->appendWidget(aWidget); // !!!
      }


    //--------------------------------------------------

    //TODO: setup heights etc

    void setup(axString aTitle, bool aClosed=false, bool aClosable=false)
      {
        mClosed = aClosed;
        mClosable = aClosable;
      }

    //----------

    void toggle_closed(void)
      {
        if (mClosed)
        {
          mClosed = false;
          wContainer->setFlag(wf_Active);
          wContainer->setFlag(wf_Visible);
          mRect.h = prev_size;
        }
        else
        {
          mClosed = true;
          wContainer->clearFlag(wf_Active);
          wContainer->clearFlag(wf_Visible);
          prev_size = mRect.h;
          mRect.h = wTitleBar->getRect().h;
        }
      }

    //--------------------------------------------------

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    if (mSkin)
    //    {
    //      aCanvas->setPenColor( aCanvas->getColor(0,0,0) );
    //      aCanvas->drawRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
    //    }
    //    axWidget::doPaint(aCanvas,aRect);
    //  }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==wTitleBar)
        {
          toggle_closed();
          mListener->onSize(this,0,0);
        }
        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif

