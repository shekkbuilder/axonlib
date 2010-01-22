/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * \brief tabs widged
 */

/**
 * \brief tabs widged
 *
 * long desc
 *
 */

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
        appendWidget( mPages = new axContainer(this/*aListener*/,-1,NULL_RECT/*axRect(0,0,aRect.w,aRect.h)*/, wal_Client ) );
        mPages->setFlag(wfl_Align);
      }

    //virtual ~wdgTabs()
    //  {
    //  }

    void appendPage( axContainer* aPage )
      {
        // initially hidden
        aPage->clearFlag(wfl_Active);
        aPage->clearFlag(wfl_Visible);
        // put it on top of self & all others
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




