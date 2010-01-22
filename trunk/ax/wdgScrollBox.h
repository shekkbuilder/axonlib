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

//class wdgGroupHeader : public wdgPanel
//{
//  public:
//
//    wdgGroupHeader(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
//    : wdgPanel(aListener,aID,aRect,aAlignment)
//      {}
//
//    virtual void doMouseDown(int aX, int aY, int aB)
//      {
//        mListener->onChange(this);
//      }
//};

class wdgScrollBox : public axContainer
{
  //protected:
  public:
    //wdgPanel*     wTitleBar;
    axContainer*  wContainer;
    wdgScroller*  wScrollBar;
    //wdgScroller   *scr;
    //float prevscroll;
    float mPrevScroll;
    float mVisible;
    int mScrolledX,mScrolledY;

  public:

    wdgScrollBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener, aID, aRect, aAlignment)
      {
        axContainer::appendWidget( wScrollBar = new wdgScroller(this,0,axRect(0,0,10,0),wal_Left ) );
        axContainer::appendWidget( wContainer = new axContainer(this,1,NULL_RECT,       wal_Client ) );
        wContainer->setBackground(true, axColor(112,112,112) );
        mPrevScroll = 0;
        mVisible = 1;
        wScrollBar->setFlag(wfl_Vertical);
        wScrollBar->setThumbSize(0.25);
        mScrolledX = mScrolledY = 0;

      }

    //virtual ~wdgScrollBox()
    //  {
    //  }

    //----------

//    void recalc_scroll(void)
//      {
//        int over = mContent.h - mRect.h;
//        if (over>0)
//        {
//          float visi = (float)mRect.h / (float)mMaxY;
//          TRACE("over: %i\n",over);
//          TRACE("visi: %f\n",visi);
//          wScrollBar->setThumbSize( visi );
//        }
//        else
//        {
//          wScrollBar->doSetValue(0);
//          wScrollBar->setThumbSize(1);
//        }
//      }

    //----------

    virtual void appendWidget(axWidget* aWidget, bool aRealign=false)
      {
        wContainer->appendWidget(aWidget);
      }

    //----------

    virtual void doScroll(int dX, int dY)
      {
        //TRACE("wdgScrollBox.doScroll(%i,%i)\n",dX,dY);
        for( int i=0;i<wContainer->mWidgets.size(); i++ ) // move sub-widgets only
        {
          wContainer->mWidgets[i]->doMove( wContainer->mWidgets[i]->mRect.x - dX, wContainer->mWidgets[i]->mRect.y - dY );
        }
        mScrolledX += dX;
        mScrolledY += dY;
      }

    void unscroll(void)
      {
        doScroll(-mScrolledX,-mScrolledY);
        mScrolledX = 0;
        mScrolledY = 0;
      }

    //TODO: update scroll pos
    virtual void doResize(int aW, int aH)
      {
        //TRACE("wdgScrollBox.doResize\n");
        mVisible = 1;
        //TRACE("- pre doResize:  mContent: %i,%i,%i,%i\n",wContainer->mContent.x,wContainer->mContent.y,wContainer->mContent.w,wContainer->mContent.h);
        unscroll();
        axContainer::doResize(aW,aH);
        //TRACE("- post doResize:  mContent: %i,%i,%i,%i\n",wContainer->mContent.x,wContainer->mContent.y,wContainer->mContent.w,wContainer->mContent.h);
        float height = (float)wContainer->mRect.h;
        float content = (float) wContainer->mContent.h;
        //TRACE("- height : %f\n",height);
        //TRACE("- content: %f\n",content);
        if (content>height)
        {
          mVisible = height / content;
          //TRACE("visible: %f (thumbsize)\n",mVisible);
          wScrollBar->setThumbSize(mVisible);
            //
            float scrollable = (float)content * (1-mVisible);
            float val = wScrollBar->doGetValue();
            int i = (int)(scrollable*val);
            int dy = i;// - mPrevScroll;
            doScroll(0,dy);
            mPrevScroll = dy;
          //
        }
        else
        {
          wScrollBar->doSetValue(0);
          mScrolledX = 0;
          mScrolledY = 0;
          wScrollBar->setThumbSize( 1 );
        }
        //mListener->onChange(wScrollBar);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        //TRACE("wdgScrollBox.onChange\n");
        int id = aWidget->mID;
        float val = aWidget->doGetValue();
        if (id==0)
        {
          //float scrollable = (float)wScrollBar->mRect.h * (1-mVisible);
          float scrollable = (float)wContainer->mContent.h * (1-mVisible);
          //TRACE("- scrollable: %f\n",scrollable);
          int i = (int)(scrollable*val);
          //TRACE("- pixels to scroll: %i\n",i);
          int dy = i - (int)mPrevScroll;
          doScroll(0,dy);
          mListener->onChange(wContainer);
          mPrevScroll = i;
        }
        mListener->onChange(aWidget);
        //mListener->onResize(this,0,0);
      }

    //----------




};

//----------------------------------------------------------------------
#endif


