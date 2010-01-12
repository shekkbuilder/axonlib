#ifndef wdgGroupBox_included
#define wdgGroupBox_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgPanel.h"

class wdgGroupHeader : public wdgPanel
{
  public:

    wdgGroupHeader(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : wdgPanel(aListener,aID,aRect,aAlignment)
      {}

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        mListener->onChange(this);
      }
};

class wdgGroupBox : public axContainer
{
  //protected:
  public:
    //wdgPanel*     wTitleBar;
    wdgGroupHeader* wTitleBar;
    axContainer*  wContainer;
    bool mClosed;
    bool mClosable;
    int mHeight;


  public:

    wdgGroupBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener, aID, aRect, aAlignment)
      {
        axContainer::appendWidget( wTitleBar   = new wdgGroupHeader(   this,0,axRect(0,0,0,20), wal_Top ) );
        axContainer::appendWidget( wContainer  = new axContainer(this,1,NULL_RECT,        wal_Client ) );
        wContainer->setBackground(true, axColor(112,112,112) );
        //doRealign();
        mClosed = false;
        mClosable = false;
        mHeight = mRect.h;
      }

    //virtual ~wdgGroupBox()
    //  {
    //  }

    virtual void appendWidget(axWidget* aWidget, bool aRealign=false)
      {
        wContainer->appendWidget(aWidget);
      }

    void setup(axString aTitle, bool aClosed=false, bool aClosable=false)
      {
        //mTitleText = aTitle;
        mClosed = aClosed;
        mClosable = aClosable;
      }

    void toggle_closed(void)
      {
        if (mClosed)
        {
          mClosed = false;
          wContainer->setFlag(wfl_Active);
          wContainer->setFlag(wfl_Visible);
          mRect.h = mHeight;
        }
        else
        {
          mClosed = true;
          wContainer->clearFlag(wfl_Active);
          wContainer->clearFlag(wfl_Visible);
          mHeight = mRect.h;
          mRect.h = wTitleBar->mRect.h + 1;
        }
      }

    virtual void onChange(axWidget* aWidget)
      {
        if( aWidget->mID == 0 ) toggle_closed();
        mListener->onResize(this,0,0);
      }

};

//----------------------------------------------------------------------
#endif

