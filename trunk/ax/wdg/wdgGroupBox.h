#ifndef wdgGroupBox_included
#define wdgGroupBox_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"
#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------

class wdgGroupHeader : public wdgPanel
{
  public:

    wdgGroupHeader(axWidgetListener* aListener, /*int aID,*/ axRect aRect, int aAlignment)
    : wdgPanel(aListener,/*aID,*/aRect,aAlignment)
      {}

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        mListener->onChange(this);
      }
};

//----------------------------------------------------------------------

class wdgGroupBox : public axContainer
{
  //protected:
  public:
    wdgGroupHeader* wTitleBar;
    axContainer*    wContainer;
    bool            mClosed;
    bool            mClosable;
    int             mHeight;
    //axBrush*  mFillBrush;

  public:

    wdgGroupBox(axWidgetListener* aListener, /*int aID, */axRect aRect, int aAlignment=wa_None)
    : axContainer(aListener, /*aID, */aRect, aAlignment)
      {
        axContainer::appendWidget( wTitleBar   = new wdgGroupHeader(this,axRect(0,0,0,20), wa_Top ) );
        axContainer::appendWidget( wContainer  = new axContainer(   this,NULL_RECT,        wa_Client ) );
        //wContainer->setBackground(true,axColor(112,112,112));
        mClosed = false;
        mClosable = false;
        mHeight = mRect.h;
        doRealign();
      }

    //virtual ~wdgGroupBox()
    //  {
    //  }

    virtual int appendWidget(axWidget* aWidget)
      {
        //wContainer->appendWidget(aWidget);
        aWidget->doSetSkin(mSkin,false); // inherit skin
        int num = wContainer->appendWidget(aWidget);
        return num;

      }

    void setup(axString aTitle, bool aClosed=false, bool aClosable=false)
      {
        //wTitleBar->appendWidget( new wdgLabel( this,-1,NULL_RECT,wal_Left,"title"));
        mClosed = aClosed;
        mClosable = aClosable;
      }

    void toggle_closed(void)
      {
        if (mClosed)
        {
          mClosed = false;
          wContainer->setFlag(wf_Active);
          wContainer->setFlag(wf_Visible);
          mRect.h = mHeight;
        }
        else
        {
          mClosed = true;
          wContainer->clearFlag(wf_Active);
          wContainer->clearFlag(wf_Visible);
          mHeight = mRect.h;
          mRect.h = wTitleBar->getRect().h + 1;
        }
      }

    //virtual void doScroll(int dX, int dY)
    //  {
    //    for( int i=0;i<wContainer->mWidgets.size(); i++ ) // move sub-widgets only
    //    {
    //      //mWidgets[i]->doMove( mWidgets[i]->mRect.x + dX, mWidgets[i]->mRect.y + dY );
    //      wContainer->mWidgets[i]->doMove( dX, dY );
    //    }
    //  }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawPanel(aCanvas,wTitleBar->getRect());
        if (mSkin) mSkin->drawPanel(aCanvas,wContainer->getRect());
        axContainer::doPaint(aCanvas,aRect);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        //if (aWidget->mID==0) toggle_closed();
        if (aWidget==wTitleBar) toggle_closed();
        mListener->onSize(this,0,0);
//        if (aWidget->mID==0)
//        {
//          toggle_closed();
//          mListener->onResize(aWidget,0,0);
//        }
//        //else
//        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif

