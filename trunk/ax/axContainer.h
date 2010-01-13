#ifndef axContainer_included
#define axContainer_included
//----------------------------------------------------------------------

//#define AX_PAINTERS

#include "axWidget.h"

class axContainer : public axWidget,
                    public axWidgetListener
{
  //protected:
  public:
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
    // runtime
    axRect    mClient;                // current Client area
    int       mStackedX, mStackedY;   // where to put next wal_Stacked widget
    //int       mMinX,mMaxX,mMinY,mMaxY;
    axRect    mContent;
    int       mOffsetX,mOffsetY;

  public:

    axContainer(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axWidget(aListener, aID, aRect, aAlignment)
      {
        //if (!aListener) aListener = this;
        //clearAllFlags();
        //setFlag(wfl_Active);
        //setFlag(wfl_Visible);
        //setFlag(wfl_Capture);
        //setFlag(wfl_Align);
        //mWidgets.clear();
        mCapturedWidget = NULL;
        mClickedWidget  = NULL;
        //doRealign();
        mMarginX = 0;
        mMarginY = 0;
        mPaddingX = 0;
        mPaddingY = 0;
        mClient = mRect;
        mStackedX = 0;
        mStackedY = 0;
        setFlag(wfl_Align);
        //setFlag(wfl_Clip);
        mOffsetX = 0;
        mOffsetY = 0;
      }

    //----------

    virtual ~axContainer()
      {
        deleteWidgets();
      }

    //--------------------------------------------------

    virtual void setAlign(int aMarginX, int aMarginY, int aPaddingX=0, int aPaddingY=0)
      {
        mMarginX = aMarginX;
        mMarginY = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    virtual void initMouseState(void)
      {
        mCapturedWidget = NULL;
        mClickedWidget  = NULL;
      }

    //----------

    virtual void clearWidgets(void)
      {
        mWidgets.clear();
      }

    //----------

    virtual int numWidgets(void)
      {
        return mWidgets.size();
      }

    //----------

    virtual void appendWidget(axWidget* aWidget, bool aRealign=false)
      {
        aWidget->doMove( mRect.x + aWidget->mRect.x, mRect.y + aWidget->mRect.y );
        mWidgets.append(aWidget);
        //if(aRealign)
        //{
        //  aWidget->doMove( mRect.x + aWidget->mRect.x, mRect.y + aWidget->mRect.y );
          //doRealign();
        //}
      }

    //----------

    virtual void removeWidget(int aIndex)
      {
      }

    //----------

    virtual void deleteWidgets(void)
      {
        for( int i=0; i<numWidgets(); i++ ) delete mWidgets[i];
        mWidgets.clear();
      }

    //----------

    virtual axWidget* widget(int aIndex)
      {
        return mWidgets[aIndex];
      }

    //--------------------------------------------------

    // return first (active) widget that contains aX,aY
    virtual axWidget* findWidget(int aX, int aY)
      {
        //if( mHoverWidget && mHoverWidget->contains(aX,aY) ) return mHoverWidget;
        for( int i=0; i<mWidgets.size(); i++ )
        {
          axWidget* W = mWidgets[i];
          if(W->hasFlag(wfl_Active))
            if(W->contains(aX,aY))
              return W;
        }
        return NULL;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    //virtual void      doReset(void) {}
    //virtual void      doSetValue(float aValue) {} // 0..1
    //virtual float     doGetValue(void) { return 0; } // 0..1
    //virtual axString  doGetName(void) { return STR_EMPTY; }

    //----------

    virtual void doMove(int aX, int aY)
      {
        int dx = aX - mRect.x;                // how much we have moved
        int dy = aY - mRect.y;
        axWidget::doMove(aX,aY);              // move ourselves...
        for( int i=0;i<mWidgets.size(); i++ ) // ... and move sub-widgets same amount
        {
          mWidgets[i]->doMove( mWidgets[i]->mRect.x + dx, mWidgets[i]->mRect.y + dy );
        }
      }

    //----------

    virtual void doScroll(int dX, int dY)
      {
        for( int i=0;i<mWidgets.size(); i++ ) // move sub-widgets only
        {
          mWidgets[i]->doMove( mWidgets[i]->mRect.x + dX, mWidgets[i]->mRect.y + dY );
        }
      }

    //----------

    virtual void doResize(int aW, int aH)
      {
        //TRACE("axContainer doResize %i,%i\n",aW,aH);
        axWidget::doResize(aW,aH);
        //if( hasFlag(wfl_Align) )
        //{
          //TRACE("doRealign\n");
          doRealign();
        //}
      }

    //----------

    //TODO: clipping [hierarchial]
    //TODO: wal_Center
    virtual void doRealign(void)
      {
        if( hasFlag(wfl_Align ) )
        {
          //bool empty = false;
          axRect R = mRect;
          //mContent = R;
          mContent.set(R.x,R.y,0,0);
          R.add( mMarginX, mMarginY, -(mMarginX*2), -(mMarginY*2) ); // spacing (outer border)
          axRect C = R;
          int largest = 0;//-1;
          int stackx = R.x + mPaddingX;
          int stacky = R.y + mPaddingY;
          //mMinX = 999999;
          //mMaxX = -1;
          //mMinY = 999999;
          //mMaxY = -1;
          for( int i=0; i<mWidgets.size(); i++ )
          {
            axWidget* wdg = mWidgets[i];
            //int xx = wdg->mRect.x;// - mRect.x;
            //int yy = wdg->mRect.y;// - mRect.y;
            int ww = wdg->mRect.w;
            int hh = wdg->mRect.h;
            switch( wdg->mAlignment )
            {
              //case wal_None:
              //  wdg->doMove( R.x + xx, R.y+yy );
              //  break;
              case wal_Parent:
                wdg->doMove( R.x + wdg->mOrigin.x, R.y + wdg->mOrigin.y );
                break;
              //case wal_Fill:
              //  wdg->doMove( R.x, R.y );
              //  wdg->doResize( R.w, R.h );
              //  break;
              case wal_Client:
                wdg->doMove( C.x, C.y );
                wdg->doResize( C.w, C.h );
                //empty = true;
                break;
              case wal_Left:
                wdg->doMove( C.x, C.y );
                wdg->doResize( ww, C.h );
                C.x += (ww + mPaddingX);
                C.w -= (ww + mPaddingX);
                break;
              case wal_Right:
                wdg->doMove( C.x2()-ww+1, C.y );
                wdg->doResize( ww, C.h );
                C.w -= (ww + mPaddingX);
                break;
              case wal_Top:
                wdg->doMove( C.x, C.y );
                wdg->doResize( C.w, hh );
                C.y += (hh + mPaddingY);
                C.h -= (hh + mPaddingY);
                break;
              case wal_Bottom:
                wdg->doMove( C.x, C.y2()-hh+1 );
                wdg->doResize( C.w, hh );
                C.h -= (hh + mPaddingY);
                break;
              case wal_LeftTop:
                wdg->doMove( C.x, C.y );
                //wdg->doResize( ww, C.h );
                C.x += (ww + mPaddingX);
                C.w -= (ww + mPaddingX);
                break;
              case wal_RightTop:
                wdg->doMove( C.x2()-ww+1, C.y );
                //wdg->doResize( ww, C.h );
                C.w -= (ww + mPaddingX);
                break;
              case wal_LeftBottom:
                wdg->doMove( C.x, C.y2()-hh+1 );
                //wdg->doResize( C.w, hh );
                //C.y += hh;
                C.h -= (hh + mPaddingY);
                break;
              case wal_RightBottom:
                wdg->doMove( C.x2()-ww+1, C.y2()-hh+1 );
                //wdg->doResize( C.w, hh );
                C.h -= (hh + mPaddingY);
                break;
              case wal_Stacked:
                wdg->doMove( stackx, stacky );
                //TODO: fix
                int w = ww + mPaddingX;
                int h = hh + mPaddingY;

                // denne er feil
                // wrapper til neste linje/row, hvis current widget ikke får plass,

                if( mFlags&wfl_Vertical )
                {
                  int remain = R.y2() - stacky + 1 - hh;
                  if( remain>=h )
                  {
                    stacky+=h;
                    if(w>largest) largest=w;
                  }
                  else
                  {
                    stacky=R.y+mPaddingY;
                    if (largest>0) stackx+=largest;//w;                                        // largest
                    else stackx+=w;
                    largest = 0;//-1;
                  }
                } //vertical

                else
                {
                  int remain = R.x2() - stackx + 1 - ww;
                  if( remain>=w )
                  {
                    stackx+=w;
                    if(h>largest) largest=h;
                  }
                  else
                  {
                    stackx=R.x+mPaddingX;
                    if (largest>0) stacky+=largest;//h;                                        // largest
                    else stacky+=h;
                    largest = 0;//-1;
                  }
                } //horizontal

                break;
            } //switch alignment
            mContent.combine( wdg->mRect );
            //wdg->doRealign();
          } //for widgets
          //TRACE("minx:%i, miny:%i, maxx:%i, maxy:%i\n",mMinX,mMinY,mMaxX,mMaxY);
        } //wfl_Align
      }

    //----------

    //TODO: if w>0 && h>0
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //TRACE("axContainer.doPaint aRect %i,%i,%i,%i\n",aRect.x,aRect.y,aRect.w,aRect.h);
        //TRACE("                    mRect %i,%i,%i,%i\n",aRect.x,aRect.y,mRect.w,mRect.h);
        if( hasFlag(wfl_Visible) )
        {
          if( mRect.intersects(aRect) )
          {
            if( hasFlag(wfl_Clip) ) aCanvas->setClipRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
            axWidget::doPaint( aCanvas, aRect );
#ifdef AX_PAINTERS
            for( int i=mWidgets.size()-1; i>=0; i-- )
#else
            for( int i=0; i<mWidgets.size(); i++ )
#endif
            {
              axWidget* W = mWidgets[i];
              if( W->intersects(aRect) ) W->doPaint( aCanvas, mRect );
            }
            if( hasFlag(wfl_Clip) ) aCanvas->clearClipRect();
          } //intersect
        }
      }

    //----------

    //virtual void doEnter(void) {}
    //virtual void doLeave(void) {}
    //virtual void doTimer(void) {}

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if( hasFlag(wfl_Active) )
        {
          axWidget* wdg = findWidget(aX,aY);
          if(wdg)
          {
            wdg->doMouseDown(aX,aY,aB);
            mClickedWidget = wdg;
            mClickedX = aX;
            mClickedY = aY;
            mClickedB = aB;
            if(hasFlag(wfl_Capture)) mCapturedWidget = wdg;
          } //wdg
        } //active
      }

    //----------

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        if( hasFlag(wfl_Active) )
        {
          if( mCapturedWidget )
          {
            mCapturedWidget->doMouseUp(aX,aY,aB);
            mCapturedWidget = NULL;
            //releaseCursor();
          } //captured
          //else if (mHoverWidget) mHoverWidget->doMouseUp(aX,aY,aB);
          mClickedWidget = NULL;
        } //active
      }

    //----------

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if( hasFlag(wfl_Active) )
        {
          if( mCapturedWidget ) mCapturedWidget->doMouseMove(aX,aY,aB);
        } //active
      }

    //----------

    //virtual void doKeyDown(int aK, int aS) {}
    //virtual void doKeyUp(int aK, int aS) {}

    //----------------------------------------
    // widget listener
    //----------------------------------------

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

