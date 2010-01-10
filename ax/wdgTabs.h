#ifndef wdgTabs_included
#define wdgTabs_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgSwitches.h"


class wdgTabs : public axContainer
{
  //protected:
  public:
    int           mActivePage;
    axContainer*  mPages;

  public:

    wdgTabs(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener, aID, aRect, aAlignment)
      {
        mActivePage = 0;
        appendWidget( mPages = new axContainer(/*this*/aListener,-1,NULL_RECT/*axRect(0,0,aRect.w,aRect.h)*/, wal_Client ) );
        mPages->setFlag(wfl_Align);
      }

    //virtual ~wdgTabs()
    //  {
    //  }

    void appendPage( axContainer* aPage )
      {
        aPage->clearFlag(wfl_Active);
        aPage->clearFlag(wfl_Visible);
        aPage->doMove(mRect.x,mRect.y);
        aPage->doResize(mRect.w,mRect.h);
        mPages->appendWidget( aPage );
      }

    void setPage(int aPage)
      {
        //if( aPage!=mActivePage)
        //{
          for( int i=0; i<mPages->mWidgets.size(); i++ )
          {
            axWidget* wdg = mPages->mWidgets[i];
            if (i==aPage)
            {
              wdg->setFlag(wfl_Active);
              wdg->setFlag(wfl_Visible);
            }
            else
            {
              wdg->clearFlag(wfl_Active);
              wdg->clearFlag(wfl_Visible);
            }
          } //for
          //mPages->initMouseState();
          mActivePage = aPage;
          mListener->onRedraw(this);
        //} // !=aIndex
      }


};

//----------------------------------------------------------------------
#endif




