#ifndef axContainer_included
#define axContainer_included
//----------------------------------------------------------------------

/*
  essentially a widget with sub-widgets. these can have various alignment
  or positions within its parent.
  the subwidget rects have coordinates relative to the window, not its parent,
  and the container can automatically align, resize and move its subwidgets,
  depending on some flags and defines..

*/

#include "core/axRect.h"
//#include "gui/axCanvas.h"
#include "gui/axSkin.h"
#include "gui/axWidget.h"
#include "axParameter.h"

//----------------------------------------------------------------------
/*

TODO:
- tune hovering, vs captured
- modal, send events directly, and only, to specific widget (popup window)

*/
//----------------------------------------------------------------------

class axContainer : public axWidget
{
  //friend class axPlugin;
  //friend class axContainer;

  private:
    axRect    mClient;                // current Client area
    int       mStackedX, mStackedY;   // where to put next wal_Stacked widget
    axRect    mContent;
    int       mOffsetX,mOffsetY;

  protected:

    axWidgets mWidgets;
    axWidget* mCaptureWidget;
    axWidget* mHoverWidget;
    //mouse
//    axWidget* mClickWidget;
//    int       mClickX;
//    int       mClickY;
//    int       mClickB;
    //layout
    int       mMarginX,  mMarginY;      // container inner space (between outer border & widgets)
    int       mPaddingX, mPaddingY;   // space between wal_Stacked widgets

  public:

    axContainer(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mCaptureWidget  = NULL;
        mHoverWidget    = this;//NULL;
//        mClickWidget    = NULL;
        mMarginX = 0;
        mMarginY = 0;
        mPaddingX = 0;
        mPaddingY = 0;

        mClient     = mRect;
        mStackedX   = 0;
        mStackedY   = 0;
        mOffsetX    = 0;
        mOffsetY    = 0;
        mFlags.setFlag(wf_Align);

      }

    virtual ~axContainer()
      {
        deleteWidgets();
      }

    //----------

    virtual void  appendWidget(axWidget* aWidget)
      {
        ////TODO: copy std stuff (colors, etc)
        //mWidgets.append(aWidget);
        aWidget->doMove( mRect.x + aWidget->getRect().x, mRect.y + aWidget->getRect().y );
        mWidgets.append(aWidget);

      }

    virtual void  deleteWidgets(void)
      {
        for (int i=0; i<mWidgets.size(); i++) delete mWidgets[i];
        mWidgets.clear();
      }

    //----------

    virtual void setAlignment(int aMarginX, int aMarginY, int aPaddingX=0, int aPaddingY=0)
      {
        mMarginX  = aMarginX;
        mMarginY  = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    // reset mouse capture & click info

    virtual void initMouseState(void)
      {
        mCaptureWidget = NULL;
//        mClickWidget  = NULL;
      }

    //----------

    virtual void setSkin(axSkin* aSkin, bool aSubWidgets=false)
      {
        //mSkin = aSkin;
        //axWidget::setSkin(aSkin,aSubWidgets);
        if (aSubWidgets)
        {
          for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->setSkin(aSkin,aSubWidgets);
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
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->doMove( wx+deltax, wy+deltay );
        }
        //mRect.setPos(aXpos,aYpos);
        axWidget::doMove(aXpos,aYpos);
      }

    //----------

    virtual void doResize(int aWidth, int aHeight)
      {
        //trace("axContaier.doResize");
        //mRect.setSize(aWidth,aHeight);
        axWidget::doResize(aWidth,aHeight);
        doRealign();
      }

    //----------

    virtual void doScroll(int dX, int dY)
      {
        // move sub-widgets only
        for( int i=0;i<mWidgets.size(); i++ )
        {
          axWidget* wdg = mWidgets[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          mWidgets[i]->doMove( wx + dX, wy + dY );
        }
      }

    //----------

    //virtual void doRealign(void)
    //  {
    //    axWidget::doRealign();
    //  }

    /**
      realign sub-widgets according to their alignment setting.
      also, it keeps track of a mContent rectangle, that encapsulates all the sub-widgets
    */

/*

    M = margin
    P = padding (between widgets when stacked)

     ________________________________________ _ _          _ ____
    |          ^                                                  |
    |          M                                                  |
    |      ____v___       ________       ____ _            _      |
    |     |        |     |        |     |                   |     |
    |<-M->|        |<-P->|        |<-P->|                   |<-M->|
    |     |________|     |________|     |______ _       _ __|     |
    |          ^                                                  :
    |          P
    |      ____v____      ___ _
    |     |         |    |
    |     :         .
    |

    TODO:
    - skip widget if not enough space for it?
      (null or negative space left)

*/

    virtual void doRealign(void)
      {
        if( mFlags.hasFlag(wf_Align ) )
        {
          axRect parent = mRect;
          mContent.set(parent.x,parent.y,0,0);
          parent.add( mMarginX, mMarginY, -(mMarginX*2), -(mMarginY*2) ); // spacing (outer border)
          axRect client = parent;
          int largest = 0;
          int stackx = parent.x + mPaddingX;
          int stacky = parent.y + mPaddingY;
          for( int i=0; i<mWidgets.size(); i++ )
          {
            axWidget* wdg = mWidgets[i];
            int ox = wdg->mOrig.x;// getOrig().x;//wdg->getRect().x;
            int oy = wdg->mOrig.y;//getOrig().y;//wdg->getRect().y;
            int ww = wdg->getRect().w;
            int wh = wdg->getRect().h;
            switch (wdg->mAlignment)//getAlignment() )
            {
              case wa_None:
                wdg->doMove(parent.x+ox,parent.y+oy);
                break;
              //case wal_Fill:
              //  wdg->doMove( parent.x, parent.y );
              //  wdg->doResize( parent.w, parent.h );
              //  break;
              case wa_Client:
                wdg->doMove( client.x, client.y );
                wdg->doResize( client.w, client.h );
                break;
              case wa_Left:
                wdg->doMove( client.x, client.y );
                wdg->doResize( ww, client.h );
                //ww = wdg->getRect().w;
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              case wa_Right:
                wdg->doMove( client.x2()-ww+1, client.y );
                wdg->doResize( ww, client.h );
                //ww = wdg->getRect().w;
                client.w -= (ww + mPaddingX);
                break;
              case wa_Top:
                wdg->doMove( client.x, client.y );
                wdg->doResize( client.w, wh );
                //wh = wdg->getRect().h;
                client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                break;
              case wa_Bottom:
                wdg->doMove( client.x, client.y2()-wh+1 );
                wdg->doResize( client.w, wh );
                //wh = wdg->getRect().w;
                client.h -= (wh + mPaddingY);
                break;
              case wa_LeftTop:
                wdg->doMove( client.x, client.y );
                //wdg->doResize( ww, C.h );
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              case wa_RightTop:
                wdg->doMove( client.x2()-ww+1, client.y );
                //wdg->doResize( ww, C.h );
                client.w -= (ww + mPaddingX);
                break;
              case wa_LeftBottom:
                wdg->doMove( client.x, client.y2()-wh+1 );
                //wdg->doResize( C.w, wh );
                //C.y += wh;
                client.h -= (wh+mPaddingY);
                break;
              case wa_RightBottom:
                wdg->doMove( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doResize( C.w, wh );
                client.h -= (wh+mPaddingY);
                break;
              case wa_Stacked:
                wdg->doMove(stackx,stacky);
                //TODO: fix
                int w = ww + mPaddingX;
                int h = wh + mPaddingY;

                if ( mFlags.hasFlag(wf_Vertical))
                {
                  int remain = parent.y2() - stacky + 1 - wh;
                  if (remain>=h)
                  {
                    stacky+=h;
                    if(w>largest) largest=w;
                  }
                  else
                  {
                    stacky = parent.y + mPaddingY;
                    if (largest>0) stackx += largest;//w;         // largest
                    else stackx+=w;
                    largest = 0;//-1;
                  }
                } //vertical
                else
                {
                  int remain = parent.x2() - stackx + 1 - ww;
                  if( remain>=w )
                  {
                    stackx+=w;
                    if(h>largest) largest=h;
                  }
                  else
                  {
                    stackx = parent.x + mPaddingX;
                    if (largest>0) stacky += largest;//h;         // largest
                    else stacky += h;
                    largest = 0;//-1;
                  }
                } //horizontal
                break;
            } //switch alignment
            mContent.combine( wdg->getRect() ); // keep track of outer boundary
            //wdg->doRealign();
          } //for widgets
          mContent.add(0,0,mMarginX,mMarginY);
        } //wfl_Align
      }


    //----------

    //TODO

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( mFlags.hasFlag(wf_Visible) )
        {
          if( mRect.intersects(aRect) )
          {
            if (mFlags.hasFlag(wf_Clip)) aCanvas->setClipRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
            //wtrace("axContainer.doPaint");
            for (int i=0; i<mWidgets.size(); i++)
            {
              axWidget* wdg = mWidgets[i];
              if (wdg->isVisible())
              {
                if (wdg->intersects(aRect))
                {
                  wdg->doPaint(aCanvas,aRect);
                }
              }
            } //for
            if (mFlags.hasFlag(wf_Clip)) aCanvas->clearClipRect();
            axWidget::doPaint(aCanvas,aRect);
          }
        }
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
        // default action, pass the message on to the 'owner'
        mListener->onChange(aWidget);
      }

    //virtual void onRedraw(axWidget* aWidget)
    //  {
    //    // default action, pass the message on to the 'owner'
    //    mListener->onRedraw(aWidget);
    //  }

};


//----------------------------------------------------------------------
#endif
