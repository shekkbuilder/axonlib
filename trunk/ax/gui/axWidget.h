#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------
/*

TODO:
- realign, take spacing & borders into account
- clipping, --"--

*/
//----------------------------------------------------------------------

//#include "core/axRect.h"
#include "gui/axCanvas.h"
#include "gui/axSkin.h"
//#include "axParameter.h"

//----------------------------------------------------------------------

// widget alignment
#define wa_None         0
#define wa_Client       1
#define wa_Left         2
#define wa_Right        3
#define wa_Top          4
#define wa_Bottom       5
#define wa_LeftTop      6
#define wa_RightTop     7
#define wa_LeftBottom   8
#define wa_RightBottom  9
#define wa_Stacked      10

//----------

// widget flags
#define wf_None         0
#define wf_Active       1
#define wf_Visible      2
#define wf_Capture      4
#define wf_Hover        8
#define wf_Align        16
#define wf_Vertical     32
#define wf_Clip         64
#define wf_Fill         128

//----------------------------------------------------------------------
//
// widget listener
//
//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

//----------

// used when you need to notify the 'owner' about something
class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
    virtual void onRedraw(axWidget* aWidget) {}
    virtual void onCursor(int aCursor=DEF_PENWIDTH) {}
    virtual void onHint(axString aHint) {}
    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY) {}
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class axWidget : public axWidgetListener
{
  protected:
    axString          mName;
    int               mFlags;
    axRect            mRect;
    axRect            mOrig;
    int               mAlignment;
    float             mValue;
    int               mConnection;            // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;             // direct access to the parameter (set in axEditor.connect)
    int               mMinWidth, mMinHeight;
    int               mMaxWidth, mMaxHeight;
    int               mMarginX,  mMarginY;    // container inner space (between outer border & widgets)
    int               mPaddingX, mPaddingY;   // space between wal_Stacked widgets
    axSkin*           mSkin;
  protected:
    axWidgetListener* mListener;
    axWidgets         mWidgets;
    axWidget*         mCapturedWidget;
    axWidget*         mHoverWidget;
    axWidget*         mModalWidget;
    axRect            mClient;                // current Client area
    axRect            mContent;               // rect encapsulating all sub-widgets (updated in doRealign)
    int               mStackedX, mStackedY;   // where to put next wal_Stacked widget
  public:
    int               mId;
    void*             mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mName           = "";
        mFlags          = wf_Active|wf_Visible|wf_Capture|wf_Align;
        mRect           = aRect;
        mOrig           = mRect;
        mAlignment      = aAlignment;
        mValue          = 0;
        mConnection     = -1;
        mParameter      = NULL;
        mMinWidth       = 0;
        mMinHeight      = 0;
        mMaxWidth       = 999999;
        mMaxHeight      = 999999;
        mMarginX        = 0;
        mMarginY        = 0;
        mPaddingX       = 0;
        mPaddingY       = 0;
        mSkin           = NULL;

        mListener       = aListener;
        mCapturedWidget = NULL;
        mHoverWidget    = this;
        mModalWidget    = NULL;
        mClient         = mRect;
        mContent        = NULL_RECT;
        mStackedX       = 0;
        mStackedY       = 0;

        mId             = 0;      //aId;
        mPtr            = NULL;   //aPtr;
      }

    virtual ~axWidget()
      {
        deleteWidgets();
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    inline void setName(axString aName) { mName=aName; }

    // flags

    inline void         setFlag(int aFlag)                    { mFlags |= aFlag; }
    inline void         setAllFlags(int aFlags)               { mFlags = aFlags; }
    inline void         clearFlag(int aFlag)                  { mFlags &= ~aFlag; }
    inline bool         hasFlag(int aFlag)                    { return (mFlags&aFlag); }
    inline axRect       getRect(void)                         { return mRect; }
    inline int          getFlags(void)                        { return mFlags; }

    inline bool         isActive(void)                        { return (mFlags&wf_Active); }
    inline bool         isVisible(void)                       { return (mFlags&wf_Visible); }
    inline bool         canCapture(void)                      { return (mFlags&wf_Capture); }

    // rect

    virtual bool        intersects(axRect aRect)              { return mRect.intersects(aRect); }
    virtual bool        contains(int aXpos, int aYpos)        { return mRect.contains(aXpos,aYpos); }

    // parameter

    inline float        getValue(void)                        { return mValue; }
    inline void         setValue(float aValue)                { mValue=aValue; }

    inline int          getConnection(void)                   { return mConnection; }
    inline void         setConnection(int aNum)               { mConnection=aNum; }

    inline axParameter* getParameter(void)                    { return mParameter; }
    inline void         setParameter(axParameter* aParameter) { mParameter=aParameter; }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    //virtual void initMouseState(void)
    //  {
    //    mCapturedWidget = NULL;
    //  }

    //----------

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

    virtual axRect getContent(void)
      {
        return mContent;
      }

    //----------

    virtual int getNumWidgets(void)
      {
        return mWidgets.size();
      }

    //----------

    virtual axWidget* getWidget(int aIndex)
      {
        return mWidgets[aIndex];
      }

    //--------------------------------------------------

    virtual int appendWidget(axWidget* aWidget)
      {
        int index = mWidgets.size();
        aWidget->setSkin(mSkin,false);
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

    //------------------------------

    virtual void setBorders(int aMarginX, int aMarginY, int aPaddingX, int aPaddingY)
      {
        mMarginX  = aMarginX;
        mMarginY  = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    //----------

    virtual void setLimits(int aMinW, int aMinH, int aMaxW, int aMaxH)
      {
        mMinWidth   = aMinW;
        mMinHeight  = aMinH;
        mMaxWidth   = aMaxW;
        mMaxHeight  = aMaxH;
      }

    //----------

    virtual void setSkin(axSkin* aSkin, bool aSub=false)
      {
        //wtrace("axWidget.setSkin");
        mSkin = aSkin;
        if (aSub) { for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->setSkin(aSkin,aSub); }
      }

    //--------------------------------------------------
    //
    // do...
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
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        if (aWidth  < mMinWidth)  aWidth  = mMinWidth;
        if (aWidth  > mMaxWidth)  aWidth  = mMaxWidth;
        if (aHeight < mMinHeight) aHeight = mMinHeight;
        if (aHeight > mMaxHeight) aHeight = mMaxHeight;
        mRect.setSize(aWidth,aHeight);
        //doRealign();
      }

    //----------

    // not used??

    //virtual void doMove(int aDeltaX, int aDeltaY)
    //  {
    //    //mRect.setPos(x,y);
    //    int x = mRect.x + aDeltaX;
    //    int y = mRect.y + aDeltaY;
    //    this->doSetPos(x,y);
    //  }

    //----------

    // only used in wdgScrollBox.doResize
    // (and that is not colled from anywhere???

    //virtual void doResize(int aDeltaX, int aDeltaY)
    //  {
    //    int w = mRect.w + aDeltaX;
    //    int h = mRect.h + aDeltaY;
    //    this->doSetSize(w,h);
    //    //doRealign();
    //  }

    //----------

    // move sub-widgets only

    virtual void doScroll(int dX, int dY)
      {
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
    - break up this (too large) function into smaller pieces
      to make it easier to follow and see any overview...

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
            wdg->doRealign();
          } //for widgets
          mContent.add(0,0,mMarginX,mMarginY);
        } //wfl_Align
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mFlags&wf_Visible)
        {
          if (mRect.intersects(aRect))
          {
            if (mFlags&wf_Clip)
            {
              aCanvas->setClipRect(mRect.x+mMarginX,mRect.y+mMarginY,mRect.x2()-mMarginX,mRect.y2()-mMarginY);
            }
            for (int i=0; i<mWidgets.size(); i++)
            {
              axWidget* wdg = mWidgets[i];
              //if (wdg->isVisible())
              {
                //if (/*wdg->intersects(aRect) &&*/ wdg->intersects(mRect))
                  wdg->doPaint(aCanvas,aRect);
              }
            } //for
            if (mFlags&wf_Clip) aCanvas->clearClipRect(); // resetClipRect();
          } //intersect
        } //visible
      }

    //----------

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
      }

    //----------

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        else
        if (mCapturedWidget) mCapturedWidget->doKeyDown(aKeyCode,aState);
      }

    //----------

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        else
        if (mCapturedWidget) mCapturedWidget->doKeyUp(aKeyCode,aState);
      }

    //----------

    virtual void doEnter(axWidget* aWidget)
      {
      }

    //----------

    virtual void doLeave(axWidget* aWidget)
      {
      }

    //--------------------------------------------------
    //
    // axWidgetListener
    //
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        mListener->onChange(aWidget);
      }

    //----------

    virtual void onRedraw(axWidget* aWidget)
      {
        mListener->onRedraw(aWidget);
      }

    //----------

    virtual void onCursor(int aCursor/*=DEF_PENWIDTH*/)
      {
        mListener->onCursor(aCursor);
      }

    //----------

    virtual void onHint(axString aHint)
      {
        mListener->onHint(aHint);
      }

    //----------

    // called from wdgSizer
    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY)
      {
        int w = mRect.w + aDeltaX;
        int h = mRect.h + aDeltaY;
        axWidget::doSetSize(w,h);
        mListener->onSize(aWidget,aDeltaX,aDeltaY);
      }

};

//----------------------------------------------------------------------
#endif
