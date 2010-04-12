#ifndef axContainer_included
#define axContainer_included
//----------------------------------------------------------------------

/*

  essentially a widget with sub-widgets. these can have various alignment
  or positions within its parent. the subwidget rects have coordinates
  relative to the window, not its parent, and the container can
  (automatically) align, resize and move its subwidgets.

*/

#include "core/axRect.h"
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

  protected:
    axRect    mContent;               // rect encapsulating all sub-widgets (updated in doRealign)
    //int       mOffsetX,mOffsetY;

  protected:

    axWidgets mWidgets;
    axWidget* mCapturedWidget;
    axWidget* mHoverWidget;
    axWidget* mModalWidget;
    //layout
    int       mMarginX,  mMarginY;      // container inner space (between outer border & widgets)
    int       mPaddingX, mPaddingY;   // space between wal_Stacked widgets

  public:

    axContainer(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {

        mClient     = mRect;
        mStackedX   = 0;
        mStackedY   = 0;
        //mOffsetX    = 0;
        //mOffsetY    = 0;

        mCapturedWidget  = NULL;
        mHoverWidget    = this;//NULL;
        mModalWidget = NULL;
        mMarginX = 0;
        mMarginY = 0;
        mPaddingX = 0;
        mPaddingY = 0;

        setFlag(wf_Align);
        //mFlags    |= wf_Align;
        //mOptions  |= wo_Fill | wo_Border;
        //mFlags |= wf_Clip;

      }

    virtual ~axContainer()
      {
        deleteWidgets();
      }

    //--------------------------------------------------

    inline axRect     getContent(void)      { return mContent; }

    //inline int        getNumWidgets(void)   { return mWidgets.size(); }
    //inline axWidget*  getWidget(int aIndex) { return mWidgets[aIndex]; }
    virtual int        getNumWidgets(void)   { return mWidgets.size(); }
    virtual axWidget*  getWidget(int aIndex) { return mWidgets[aIndex]; }

    //--------------------------------------------------

    virtual int appendWidget(axWidget* aWidget)
      {
        int index = mWidgets.size();
        aWidget->doSetSkin(mSkin,false); // inherit skin
        aWidget->doSetPos( mRect.x + aWidget->getRect().x, mRect.y + aWidget->getRect().y );
        mWidgets.append(aWidget);
        return index;
      }

    //----------

    virtual void removeWidget(int aIndex)
      {
        mWidgets.remove(aIndex);
      }

    //----------

    virtual void deleteWidgets(void)
      {
        for (int i=0; i<mWidgets.size(); i++) delete mWidgets[i];
        mWidgets.clear();
      }

    //----------

    virtual void setBorders(int aMarginX, int aMarginY, int aPaddingX=0, int aPaddingY=0)
      {
        mMarginX  = aMarginX;
        mMarginY  = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    //----------

    // reset mouse capture & click info
    virtual void initMouseState(void)
      {
        mCapturedWidget = NULL;
//        mClickWidget  = NULL;
      }

    virtual void goModal(axWidget* aWidget)
      {
        mModalWidget = aWidget;
        //mModal = true;
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    // find first widget that contains x,y
    // or 'self' is no sub-widgets found or hit
    //
    // start searching from end of list, so that widgets that are painted last
    // (topmost) are found first.

    virtual axWidget* doFindWidget(int aXpos, int aYpos)
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
                widget = w->doFindWidget(aXpos,aYpos);
                if (widget!=w) return widget;
              } //contains
            } //active
          } //for num
        } //num>0
        return widget;
      }

    //----------

    virtual void doSetSkin(axSkin* aSkin, bool aSubWidgets=false)
      {
        //mSkin = aSkin;
        //axWidget::setSkin(aSkin,aSubWidgets);
        if (aSubWidgets)
        {
          for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->doSetSkin(aSkin,aSubWidgets);
        }
        axWidget::doSetSkin(aSkin,aSubWidgets);
      }

    //----------

    virtual void doSetPos(int aXpos, int aYpos)
      {
        int deltax = aXpos - mRect.x;
        int deltay = aYpos - mRect.y;
        for (int i=0; i<mWidgets.size(); i++)
        {
          axWidget* wdg = mWidgets[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->doSetPos( wx+deltax, wy+deltay );
        }
        //mRect.setPos(aXpos,aYpos);
        axWidget::doSetPos(aXpos,aYpos);
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        //trace("axContaier.doSetSize");
        //mRect.setSize(aWidth,aHeight);
        axWidget::doSetSize(aWidth,aHeight);
        doRealign();
      }

    //----------

    //virtual void doMove(int aDeltaX, int aDeltaY)
    //  {
    //  }

    virtual void doResize(int aDeltaX, int aDeltaY)

      {
        axWidget::doResize(aDeltaX,aDeltaY);
        //mListener->onSize(aDeltaX,aDeltaY);
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
          mWidgets[i]->doSetPos( wx + dX, wy + dY );
        }
      }

    //----------

    // realign sub-widgets according to their alignment setting.
    // also, it keeps track of a mContent rectangle, that encapsulates all the sub-widgets

    //TODO. update stackx,stacky when we update the available client area
    //      so we can interleave them...

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
        if (mFlags&wf_Align)
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

              // position relative to container
              case wa_None:
                wdg->doSetPos(parent.x+ox,parent.y+oy);
                break;
              //case wal_Fill:
              //  wdg->doSetPos(  parent.x, parent.y );
              //  wdg->doSetSize( parent.w, parent.h );
              //  break;

              // stretch to fill entire client area
              case wa_Client:
                wdg->doSetPos( client.x, client.y );
                wdg->doSetSize( client.w, client.h );
                break;

              //--- stretch ---

              // move to left edge, stretch vertical to client area height
              case wa_Left:
                wdg->doSetPos( client.x, client.y );
                wdg->doSetSize( ww, client.h );
                //ww = wdg->getRect().w;
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              // move to right edge, stretch vertical to client area height
              case wa_Right:
                wdg->doSetPos( client.x2()-ww+1, client.y );
                wdg->doSetSize( ww, client.h );
                //ww = wdg->getRect().w;
                client.w -= (ww + mPaddingX);
                break;
              // move to top, stretch horizontal to client area width
              case wa_Top:
                wdg->doSetPos( client.x, client.y );
                wdg->doSetSize( client.w, wh );
                //wh = wdg->getRect().h;
                client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                break;
              // move to bottom, stretch horizontal to client area width
              case wa_Bottom:
                wdg->doSetPos( client.x, client.y2()-wh+1 );
                wdg->doSetSize( client.w, wh );
                //wh = wdg->getRect().w;
                client.h -= (wh + mPaddingY);
                break;

              //--- move ---

              // move to top-left, no resize
              case wa_LeftTop:
                wdg->doSetPos( client.x, client.y );
                //wdg->doSetSize( ww, C.h );
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                break;
              // move to top-right, no resize
              case wa_RightTop:
                wdg->doSetPos( client.x2()-ww+1, client.y );
                //wdg->doSetSize( ww, C.h );
                client.w -= (ww + mPaddingX);
                break;
              // move to bottom-left, no resize
              case wa_LeftBottom:
                wdg->doSetPos( client.x, client.y2()-wh+1 );
                //wdg->doSetSize( C.w, wh );
                //C.y += wh;
                client.h -= (wh+mPaddingY);
                break;
              // move to bottom-right, no resize
              case wa_RightBottom:
                wdg->doSetPos( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doSetSize( C.w, wh );
                client.h -= (wh+mPaddingY);
                break;

              //TODO: TopLeft,TopRight, etc?
              // update client the 'other' direction
              // wa_LeftTop, clips available client rect from left,
              // wa_TopLeft should be from top?

              //--- stacked ---
              // stacked widgets does not affect the available client rect

              // stacked horizontal or vertical (depending on wf_Vertical flag)
              case wa_Stacked:
                wdg->doSetPos(stackx,stacky);
                //TODO: fix
                int w = ww + mPaddingX;
                int h = wh + mPaddingY;

                if (mFlags&wf_Vertical)
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
                break; // stacked

              //---

            } //switch alignment
            mContent.combine( wdg->getRect() ); // keep track of outer boundary
            //wdg->doRealign();
          } //for widgets
          mContent.add(0,0,mMarginX,mMarginY);
        } //wfl_Align
      }

    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mFlags&wf_Visible)
        {
          if (mRect.intersects(aRect))
          {
            //axWidget::doPaint(aCanvas,aRect);
            if (mFlags&wf_Clip)
            {
              aCanvas->setClipRect(mRect.x+mMarginX,mRect.y+mMarginY,mRect.x2()-mMarginX,mRect.y2()-mMarginY);
            }
            for (int i=0; i<mWidgets.size(); i++)
            {
              axWidget* wdg = mWidgets[i];
              if (wdg->isVisible())
              {
                if (wdg->intersects(aRect) && wdg->intersects(mRect)) wdg->doPaint(aCanvas,aRect);
              }
            } //for

            if (mFlags&wf_Clip) aCanvas->clearClipRect(); // resetClipRect();
          }
        }
      }

    //--------------------------------------------------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
        else
        if (mCapturedWidget) mCapturedWidget->doMouseDown(aXpos,aYpos,aButton);
        else
        {
          if (isActive())
          {
          axWidget* hover = doFindWidget(aXpos,aYpos);
          if (hover!=this)
          {
            if (mFlags&wf_Capture) mCapturedWidget = hover;
            hover->doMouseDown(aXpos,aYpos,aButton);
          } //!hover
          } //active
        } //!capture
        //axWidget::doMouseDown(aXpos,aYpos,aButton);
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
        else
        if (mCapturedWidget)
        {
          mCapturedWidget->doMouseUp(aXpos,aYpos,aButton);
          mCapturedWidget = NULL;
        } //capture
        else
        {
          if (isActive())
          {
            axWidget* hover = doFindWidget(aXpos,aYpos);
            if (hover!=this) hover->doMouseUp(aXpos,aYpos,aButton);
          } //active
        }
        //axWidget::doMouseUp(aXpos,aYpos,aButton);
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
        else
        if (mCapturedWidget) mCapturedWidget->doMouseMove(aXpos,aYpos,aButton);
        else
        {
          axWidget* hover = doFindWidget(aXpos,aYpos);
          if (hover!=mHoverWidget)
          {
            mHoverWidget->doLeave(mCapturedWidget);
            mHoverWidget = hover;
            mHoverWidget->doEnter(mCapturedWidget);
          }
          if (hover!=this) hover->doMouseMove(aXpos,aYpos,aButton);
        }
        //axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    //--------------------------------------------------

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        else
        if (mCapturedWidget) mCapturedWidget->doKeyDown(aKeyCode,aState);
        //axWidget::doKeyDown(aKeyCode,aState);
      }

    //----------

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        else
        if (mCapturedWidget) mCapturedWidget->doKeyUp(aKeyCode,aState);
        //axWidget::doKeyUp(aKeyCode,aState);
      }

    //----------

    //virtual void doEnter(axWidget* aCapture)
    //  {
    //    if (mModalWidget) mModalWidget->doEnter(axWidget* aCapture);
    //    else
    //    axWidget::doEnter(aCapture);
    //  }

    //----------

    //virtual void doLeave(axWidget* aCapture)
    //  {
    //    axWidget::doEnter(aCapture);
    //  }

    //--------------------------------------------------
    // axWidgetListener
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        mListener->onChange(aWidget);
      }

    //virtual void onRedraw(axWidget* aWidget)
    //  {
    //    mListener->onRedraw(aWidget);
    //  }

    virtual void onCursor(int aCursor)
      {
        mListener->onCursor(aCursor);
      }

    //virtual void onHint(axString aHint) {}

    // called from wdgSizer
    virtual void onSize(int aDeltaX, int aDeltaY)
      {
        //wtrace("axContainer.onSize");
        int w = mRect.w + aDeltaX;
        int h = mRect.h + aDeltaY;
        axWidget::doSetSize(w,h);
        //doSetSize(w,h);
        mListener->onSize(aDeltaX,aDeltaY);
        //mListener->onRedraw(this);
        //doRealign();
      }


};


//----------------------------------------------------------------------
#endif
