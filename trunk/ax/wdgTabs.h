#ifndef wdgTabs_included
#define wdgTabs_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgSwitches.h"


class wdgTabs : public axContainer
{
  //protected:
  public:
    int mPage;
    wdgSwitches* mHeader;
    axContainer* mPages;

  public:

    wdgTabs(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener, aID, aRect, aAlignment)
      {
        mPage = 0;
        appendWidget( mHeader = new wdgSwitches(this,0,axRect(0,0, aRect.w,20),wal_Top ) );
        appendWidget( mPages  = new axContainer(this,1,axRect(0,20,aRect.w,aRect.h-20), wal_Client ) );
        //doRealign(); // hmmm...
        //mPages->setBackground( true, AX_GREY_LIGHT );
        //setBackground( true, AX_BLACK );
      }

    //virtual ~wdgTabs()
    //  {
    //  }

    void setup(int aNum, char** aStr)
      {
        mHeader->setup(aNum,aStr);
      }

    void appendPage( axContainer* aPage )
      {
        aPage->clearFlag(wfl_Active);
        aPage->clearFlag(wfl_Visible);
        //aPage->mRect = axRect(0,0,mPages->mRect.w,mPages->mRect.h);
        mPages->appendWidget( aPage );
      }

    void setPage(int aIndex, bool aRedraw=true)
      {
        //if( aIndex!=mPage)
        //{
          //axWidget* w = NULL;
          int num = mPages->mWidgets.size();

          for( int i=0; i<num; i++ )
          {
            axWidget* wdg = mPages->mWidgets[i];
            if( i==aIndex )
            {
              wdg->setFlag(wfl_Active);
              wdg->setFlag(wfl_Visible);
              //w = wdg;
            }
            else
            {
              wdg->clearFlag(wfl_Active);
              wdg->clearFlag(wfl_Visible);
            }
          } //for

          mHeader->setNum(aIndex);
          if(aRedraw) mListener->onRedraw(this);//(mPages); // double drawing of header...
          mPages->initMouseState();
          //initMouse();
          mPage = aIndex;
        //} //!=aIndex
        //TODO: fixup mouse hover/capture etc...
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    mHeader->doPaint(aCanvas,aRect);
    //    mPages->doPaint(aCanvas,aRect);
    //  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

//TODO: not mID, use mCNum (connection number)

//    // handle the header switches
//    virtual void onChange(axWidget* aWidget)
//      {
//        if( aWidget->mID==0 ) { setPage(mHeader->mSelected,true); }
//        mListener->onChange(aWidget);
//      }

    //----------

    //virtual void onRedraw(axWidget* aWidget)
    //  {
    //    mListener->onRedraw(aWidget);
    //  }

    //----------

    //virtual void onRedraw(axRect aRect)
    //  {
    //    mListener->onRedraw(aRect);
    //  }

    //----------

    //virtual void onRedrawAll(void)
    //  {
    //    mListener->onRedraw(this);
    //  }

    //----------

    //virtual void onMoved(int aX, int aY)
    //  {
    //    if(hasFlag(wfl_Align))
    //    {
    //      doRealign();
    //      mListener->onRedraw(this);
    //    }
    //  }

    //----------

    //virtual void onResized(int aW, int aH)
    //  {
    //    if(hasFlag(wfl_Align))
    //    {
    //      doRealign();
    //      mListener->onRedraw(this);
    //    }
    //  }

    //----------

    //virtual void onSetHint(axString aHint)
    //  {
    //    mListener->onSetHint(aHint);
    //  }

    //----------

};

//----------------------------------------------------------------------
#endif




