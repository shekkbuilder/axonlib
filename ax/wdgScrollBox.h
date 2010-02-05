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
 * \brief scrollbox widget
 */

/**
 * \brief scrollbox widget
 *
 * long desc
 *
 */

#ifndef wdgScrollBox_included
#define wdgScrollBox_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgScroller.h"

class wdgScrollBox : public axContainer
{
  //private:
  //  axContainer*  w_Container;
  protected:
    //wdgPanel*     wContainer;
    axContainer*  wContainer;
    //axContainer*  w_Container;
    wdgScroller*  wScrollBar;
    int           mScrolledX; // how much we've already scrolled
    int           mScrolledY; // -_"--
    float         mVisible;   // how much of the content is visible (calculated in doRealign)

  public:

    wdgScrollBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener, aID, aRect, aAlignment)
      {
        axContainer::appendWidget( wScrollBar  = new wdgScroller(this,-1,axRect(0,0,10,0),wal_Right ) );
        axContainer::appendWidget( wContainer = new axContainer(this,-1,NULL_RECT,       wal_Client ) );
        //wContainer = w_Container;
        doRealign();
        wScrollBar->setFlag(wfl_Vertical);
        wScrollBar->setThumbSize(1);
        mScrolledX = mScrolledY = 0;
        mVisible = 1;
      }

    //virtual ~wdgScrollBox()
    //  {
    //  }

    //--------------------------------------------------

    // accessors

    inline axContainer* getContainer(void) { return wContainer; }
    inline wdgScroller* getScrollBar(void) { return wScrollBar; }

    inline void setContainer(axContainer* c)
    {
      delete wContainer;
      wContainer = c;
      mWidgets[1] = c;
      doRealign();
    }

    //--------------------------------------------------

    void unscroll(void)
      {
        doScroll(-mScrolledX,-mScrolledY);
        wScrollBar->doSetValue(0);
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

    virtual void appendWidget(axWidget* aWidget)
      {
        wContainer->appendWidget(aWidget);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void doScroll(int dX, int dY)
      {
        for( int i=0;i<wContainer->numWidgets(); i++ )
        {
          axWidget* wdg = wContainer->getWidget(i);
          wdg->doMove( wdg->getRect().x - dX, wdg->getRect().y - dY );
        }
        mScrolledX += dX;
        mScrolledY += dY;
      }

    //----------

    virtual void doResize(int aW, int aH)
      {
        axContainer::doResize(aW,aH);
        calc_thumbsize();
      }

    //----------

    virtual void doRealign(void)
      {
        float y = wScrollBar->doGetValue();
        unscroll();
        axContainer::doRealign();
        calc_thumbsize();
        set_scrollpos(0,y);
        wScrollBar->doSetValue(y);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==wScrollBar)
        {
          float val = aWidget->doGetValue();
          float scrollable = (float)wContainer->getContent().h * (1-mVisible);
          int i = (int)(scrollable*val);
          int dy = i - mScrolledY;
          doScroll(0,dy);
          mListener->onChange(this);
        }
        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif


