/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file axContainer.h
  \brief container widget
*/

#ifndef axContainer_included
#define axContainer_included
//----------------------------------------------------------------------

//#define AX_PAINTERS

#include "axWidget.h"

class axContainer : public axWidget,
                    public axWidgetListener
{
  private:
    axRect    mClient;                // current Client area
    int       mStackedX, mStackedY;   // where to put next wal_Stacked widget
    axRect    mContent;
    int       mOffsetX,mOffsetY;
  protected:
    axWidgets mWidgets;
    //mouse
    axWidget* mCapturedWidget;
    axWidget* mClickedWidget;
    int       mClickedX;
    int       mClickedY;
    int       mClickedB;
    //layout
    int       mMarginX,  mMarginY;      // container inner space (between outer border & widgets)
    int       mPaddingX, mPaddingY;   // space between wal_Stacked widgets

  public:

    /// constructor
    /**
      \param aListener listener that will receive response-events from this
      \param aID widget it, use for whatever
      \param aRect position (relative to parent) and size
      \param aAlignment alignment mode
    */

    //TODO:
    // class wdgLayout : public axContainer
    // -> doRealign

    //TODO: clipping [hierarchial]
    //TODO: wal_Center
    axContainer(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axWidget(aListener, aID, aRect, aAlignment)
      {
        mCapturedWidget = NULL;
        mClickedWidget  = NULL;
        mMarginX = 0;
        mMarginY = 0;
        mPaddingX = 0;
        mPaddingY = 0;
        mClient = mRect;
        mStackedX = 0;
        mStackedY = 0;
        setFlag(wfl_Align);
        mOffsetX = 0;
        mOffsetY = 0;
        //mClipStackPos = 0;
      }

    //----------

    virtual ~axContainer()
      {
        deleteWidgets();
      }

    //--------------------------------------------------
    // accessors
    //--------------------------------------------------

    inline axWidget* getWidget(int i) { return mWidgets[i]; }
    inline axRect getContent(void) { return mContent; } // set up in doRealign

    //--------------------------------------------------
    // set
    //--------------------------------------------------

    /// set alignment borders

    virtual void setAlign(int aMarginX, int aMarginY, int aPaddingX=0, int aPaddingY=0)
      {
        mMarginX = aMarginX;
        mMarginY = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    /// reset mouse capture & click info

    virtual void initMouseState(void)
      {
        mCapturedWidget = NULL;
        mClickedWidget  = NULL;
      }

    //--------------------------------------------------
    // widgets
    //--------------------------------------------------

    /// clear subwidgets list

    virtual void clearWidgets(void)
      {
        mWidgets.clear();
      }

    //----------

    /// number of subwidgets

    virtual int numWidgets(void)
      {
        return mWidgets.size();
      }

    //----------

    /// append widget
    /**
      append widget to subwidgets list,
      and move new widget to correct place inside the container.
      (widget position relative to container)
    */

    virtual void appendWidget(axWidget* aWidget/*, bool aRealign=false*/)
      {
        aWidget->doMove( mRect.x + aWidget->getRect().x, mRect.y + aWidget->getRect().y );
        mWidgets.append(aWidget);
      }

    //----------

    /// remove specified widget
    /**
      [not implemented]
    */
    virtual void removeWidget(int aIndex)
      {
      }

    //----------

    /// delete all widgets

    virtual void deleteWidgets(void)
      {
        for( int i=0; i<numWidgets(); i++ ) delete mWidgets[i];
        mWidgets.clear();
      }

    //----------

    ///// return specified widget
    //
    //virtual axWidget* widget(int aIndex)
    //  {
    //    return mWidgets[aIndex];
    //  }

    //--------------------------------------------------

    /// return first (active) widget that contains aX,aY

    virtual axWidget* findWidget(int aX, int aY)
      {
        #ifdef AX_PAINTERS
        for (int i=0; i<mWidgets.size(); i++)
        #else
        for (int i=mWidgets.size()-1; i>=0; i--)
        #endif
        {
          axWidget* W = mWidgets[i];
          if(W->hasFlag(wfl_Active))
            if (W->contains(aX,aY))
              return W;
        }
        return NULL;
      }

    //----------

    //TODO: redo (combine with findWidget?
    virtual axWidget* findHover(int aX, int aY)
      {
        //axWidget* W = findWidget(aX,aY);
        axWidget* W = NULL;
        #ifdef AX_PAINTERS
        for (int i=0; i<mWidgets.size(); i++)
        #else
        for (int i=mWidgets.size()-1; i>=0; i--)
        #endif
        {
          axWidget* SW = mWidgets[i];
          if (SW->contains(aX,aY))
          {
            W = SW;
            break;
          }
        }
        if (W) return W->findHover(aX,aY);
        else return this;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //void push_rect(axRect aRect)
    //  {
    //    mClipStack[mClipStackPos++] = aRect;
    //  }

    //axRect pop_rect(void)
    //  {
    //    return mClipStack[--mClipStackPos];
    //  }

    //--------------------------------------------------
    // widget base
    //--------------------------------------------------

    //virtual void      doReset(void) {}
    //virtual void      doSetValue(float aValue) {} // 0..1
    //virtual float     doGetValue(void) { return 0; } // 0..1
    //virtual axString  doGetName(void) { return STR_EMPTY; }

    //----------

    /// move self & subwidgets

    virtual void doMove(int aX, int aY)
      {
        int dx = aX - mRect.x;                // how much we have moved
        int dy = aY - mRect.y;
        axWidget::doMove(aX,aY);              // move ourselves...
        for( int i=0;i<mWidgets.size(); i++ ) // ... and move sub-widgets same amount
        {
          mWidgets[i]->doMove( mWidgets[i]->getRect().x + dx, mWidgets[i]->getRect().y + dy );
        }
      }

    //----------

    /// scroll subwidgets

    virtual void doScroll(int dX, int dY)
      {
        // move sub-widgets only
        for( int i=0;i<mWidgets.size(); i++ )
        {
          mWidgets[i]->doMove( mWidgets[i]->getRect().x + dX, mWidgets[i]->getRect().y + dY );
        }
      }

    //----------

    /// resize self & realign subwidgets

    virtual void doResize(int aW, int aH)
      {
        axWidget::doResize(aW,aH);
        //if( hasFlag(wfl_Align) )
        //{
          doRealign();
        //}
      }

    //----------

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

*/

    virtual void doRealign(void)
      {
        if( hasFlag(wfl_Align ) )
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
              case wal_None:
                wdg->doMove(parent.x+ox,parent.y+oy);
                break;
              //case wal_Fill:
              //  wdg->doMove( parent.x, parent.y );
              //  wdg->doResize( parent.w, parent.h );
              //  break;
              case wal_Client:
                wdg->doMove( client.x, client.y );
                wdg->doResize( client.w, client.h );
                break;
              case wal_Left:
                wdg->doMove( client.x, client.y );
                wdg->doResize( ww, client.h );
                //ww = wdg->getRect().w;
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              case wal_Right:
                wdg->doMove( client.x2()-ww+1, client.y );
                wdg->doResize( ww, client.h );
                //ww = wdg->getRect().w;
                client.w -= (ww + mPaddingX);
                break;
              case wal_Top:
                wdg->doMove( client.x, client.y );
                wdg->doResize( client.w, wh );
                //wh = wdg->getRect().h;
                client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                break;
              case wal_Bottom:
                wdg->doMove( client.x, client.y2()-wh+1 );
                wdg->doResize( client.w, wh );
                //wh = wdg->getRect().w;
                client.h -= (wh + mPaddingY);
                break;
              case wal_LeftTop:
                wdg->doMove( client.x, client.y );
                //wdg->doResize( ww, C.h );
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              case wal_RightTop:
                wdg->doMove( client.x2()-ww+1, client.y );
                //wdg->doResize( ww, C.h );
                client.w -= (ww + mPaddingX);
                break;
              case wal_LeftBottom:
                wdg->doMove( client.x, client.y2()-wh+1 );
                //wdg->doResize( C.w, wh );
                //C.y += wh;
                client.h -= (wh+mPaddingY);
                break;
              case wal_RightBottom:
                wdg->doMove( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doResize( C.w, wh );
                client.h -= (wh+mPaddingY);
                break;
              case wal_Stacked:
                wdg->doMove(stackx,stacky);
                //TODO: fix
                int w = ww + mPaddingX;
                int h = wh + mPaddingY;

                if ( hasFlag(wfl_Vertical))
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

    /**
      paints the container, and its children.
      does some intersection testing, and skips widgets that are not visible,
      or outside the update area (aRect).
      if AX_PAINTERS is defined, paints the widgets from last to first,
      else from first to last.
      additionally, if wfl_CLip is set, before drawing, setup the clip rectangle.

    */

    //TODO: if w>0 && h>0
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( hasFlag(wfl_Visible) )
        {
          if( mRect.intersects(aRect) )
          {
            if (hasFlag(wfl_Clip))
            {
              aCanvas->setClipRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
              //aCanvas->push_rect(mRect);
            }
            //axWidget::doPaint( aCanvas, aRect );
            #ifdef AX_PAINTERS
            for( int i=mWidgets.size()-1; i>=0; i-- )
            #else
            for( int i=0; i<mWidgets.size(); i++ )
            #endif
            {
              axWidget* W = mWidgets[i];
              if (W->intersects(aRect)) W->doPaint(aCanvas, mRect);
            }
            // this fails for recursive clip rectangles,
            // the innermost rect will clear the clip rect on its way out,
            // but it should reset to what it was 'on enrty'

            if (hasFlag(wfl_Clip))
            {
              //axRect CR = aCanvas->pop_rect();
              //aCanvas->setClipRect( CR.x, CR.y, CR.x2(), CR.y2() );
              aCanvas->clearClipRect();
            }
          } //intersect
        }
      }

    //----------

    //virtual void doEnter(void) {}
    //virtual void doLeave(void) {}
    //virtual void doTimer(void) {}

    //----------

    /**
      called (by owner/parent) when a mouse button is pressed,
      finds which sub-widget the mouse is inside, and call doMouseDown for that widget.
      also, if wfl_capture is set in this container,
      it keeps track of that widget, and sends later mouse move and release events/calls
      directly to that widget only.
    */

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if( hasFlag(wfl_Active) )
        {
          axWidget* wdg = findWidget(aX,aY);
          if (wdg)
          {
            wdg->doMouseDown(aX,aY,aB);
            mClickedWidget = wdg;
            mClickedX = aX;
            mClickedY = aY;
            mClickedB = aB;
            if (hasFlag(wfl_Capture))
            {
              //trace("captured");
              mCapturedWidget = wdg;
            }
          } //wdg
        } //active
      }

    //----------

    /**
      if any widget were 'captured' during an earlier mouse down event,
      that widget is sent a mouse up event (doMouseUp is called),
      and the capture is released.
      this function does nothing if wfl_Active flag is not set.
    */

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        if( hasFlag(wfl_Active) )
        {
          if( mCapturedWidget )
          {
            //trace("released");
            mCapturedWidget->doMouseUp(aX,aY,aB);
            mCapturedWidget = NULL;
            //releaseCursor();
          } //captured
          //else if (mHoverWidget) mHoverWidget->doMouseUp(aX,aY,aB);
          mClickedWidget = NULL;
        } //active
      }

    //----------

    /**
      if any widget were 'captured' during an earlier mouse down event,
      that widget's doMouseMouse is called.
      this function does nothing if wfl_Active flag is not set.
    */

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        //axWidget* W = findWidget(aX,aY);
        //if (W) mListener->onHover(W);
        if( hasFlag(wfl_Active) )
        {
          if( mCapturedWidget ) mCapturedWidget->doMouseMove(aX,aY,aB);
        } //active

        //TODO: find hovering widget?

        //axWidget* wdg = findHover(aX,aY);
        //if (wdg)
        //  trace("hover id: " << wdg->mID );
        //if (wdg != mHoverWidget)
        //{
        //  if (mHoverWidget) mHoverWidget->onLeave();
        //  mHoverWidget = wdg;
        //  mHoverWidget->onEnter();
        //}

      }

    //----------

    //virtual void doKeyDown(int aK, int aS) {}
    //virtual void doKeyUp(int aK, int aS) {}

  //protected:

    //----------------------------------------
    // widget listener
    //----------------------------------------

    // widgets responding to containers

    virtual void onChange(axWidget* aWidget)                  { mListener->onChange(aWidget); }
    virtual void onRedraw(axWidget* aWidget)                  { mListener->onRedraw(aWidget); }
    virtual void onRedraw(axRect aRect)                       { mListener->onRedraw(aRect); }
    virtual void onRedrawAll(void)                            { mListener->onRedraw(this); }
    virtual void onMove(axWidget* aWidget, int dX, int dY)    { mListener->onMove(aWidget,dX,dY); }   // delta x/y
    virtual void onResize(axWidget* aWidget, int dX, int dY)  { mListener->onResize(aWidget,dX,dY); } // delta x/y
    virtual void onHint(axString aHint)                       { mListener->onHint(aHint); }
    virtual void onHover(axWidget* aWidget)                   { mListener->onHover(aWidget); }
    virtual void onCursor(int aCursor)                        { mListener->onCursor(aCursor); }

};

//----------------------------------------------------------------------
#endif

