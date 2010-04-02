#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------

/*
  a widget is the base class for the gui.
  it is mainly a rectangle in a window, and can receive messages
  (mouse, keys, paint, ..) from its 'owner' to do something, and can
  send messages back to notify its handler that something has changed.

  the rect coords are kept in screen-space, relative to the window.
  this simplifies quite a few calculations at runtime,
  but also mean that you have to be a little bit careful when setting
  up the editor/window...
*/

#include "core/axRect.h"
#include "gui/axCanvas.h"
#include "gui/axSkin.h"
#include "axParameter.h"

//----------------------------------------------------------------------

// widget alignment
#define wa_None 0

// widget flags
#define wf_Active   1
#define wf_Visible  2
#define wf_Capture  4


//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
};

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class axWidget : public axWidgetListener
{
  friend class axPlugin;
  friend class axContainer;

  protected:

    axWidgetListener* mListener;
    axRect            mRect;
    axFlags           mFlags;
    int               mAlignment;
    int               mConnection;    // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;     // direct access to the parameter (set in axEditor.connect)
    float             mValue;
    //axWidgets         mWidgets;
    //axWidget*         mCaptureWidget;
    //axWidget*         mHoverWidget;
    //axSkin*           mSkin;
  public:
    //int     mId;
    //void*   mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mListener = aListener;
        mRect = aRect;
        mFlags.mBits = wf_Active | wf_Visible | wf_Capture;
//        mSkin = NULL;
        mAlignment = aAlignment;
        mConnection = -1;
        //mCaptureWidget = NULL;
        //mHoverWidget = this;//NULL;
        mParameter = NULL;
        mValue = 0;
        //mId = aId;
        //mPtr = aPtr;
      }

    virtual ~axWidget()
      {
        //deleteWidgets();
      }

    //----------

    inline axRect   rect(void) { return mRect; }
    inline axFlags  flags(void) { return mFlags; }

    inline bool isActive(void)  { return mFlags.hasFlag(wf_Active); }
    inline bool isVisible(void) { return mFlags.hasFlag(wf_Visible); }
    inline bool doCapture(void) { return mFlags.hasFlag(wf_Capture); }

    virtual bool intersects(axRect aRect) { return mRect.intersects(aRect); }
    virtual bool contains(int aXpos, int aYpos) { return mRect.contains(aXpos,aYpos); }

    //----------

    inline float getValue(void) { return mValue; }
    inline void  setValue(float aValue) { mValue=aValue; }

    //virtual void  appendWidget(axWidget* aWidget) { mWidgets.append(aWidget); }
    //virtual void  deleteWidgets(void) { for (int i=0; i<mWidgets.size(); i++) delete mWidgets[i]; }

    inline int  getConnection(void)     { return mConnection; }
    inline void setConnection(int aNum) { mConnection = aNum; }

    inline axParameter* getParameter(void) { return mParameter; }
    inline void setParameter(axParameter* aParameter) { mParameter = aParameter; }

    //----------

//    virtual void setSkin(axSkin* aSkin, bool aChildren=false)
//      {
//        mSkin = aSkin;
//      }

    //----------

    // find first widget that contains x,y
    // or 'self' is no sub-widgets found or hit
    //
    // start searching from end of list, so that widgets that are painted last
    // (topmost) are found first.

//    virtual axWidget* findWidget(int aXpos, int aYpos)
//      {
//        axWidget* widget = this;
//        int num = mWidgets.size();
//        if (num>0)
//        {
//          for (int i=num-1; i>=0; i--)
//          {
//            axWidget* w = mWidgets[i];
//            if (w->isActive())
//            {
//              if (w->contains(aXpos,aYpos))
//              {
//                widget = w->findWidget(aXpos,aYpos);
//                if (widget!=w) return widget;
//              } //contains
//            } //active
//          } //for num
//        } //num>0
//        return widget;
//      }

    virtual axWidget* findWidget(int aXpos, int aYpos)
      {
        return this;
      }


    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual void doMove(int aXpos, int aYpos)
      {
//        int deltax = aXpos - mRect.x;
//        int deltay = aYpos - mRect.y;
//        for (int i=0; i<mWidgets.size(); i++)
//        {
//          axWidget* wdg = mWidgets[i];
//          int wx = wdg->rect().x;
//          int wy = wdg->rect().y;
//          wdg->doMove( wx+deltax, wy+deltay );
//        }
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doResize(int aWidth, int aHeight)
      {
        mRect.setSize(aWidth,aHeight);
      }

    //----------

    //TODO

    virtual void doRealign(void)
      {
      }

    //----------

    //TODO

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
//        for (int i=0; i<mWidgets.size(); i++)
//        {
//          axWidget* wdg = mWidgets[i];
//          if (wdg->isVisible())
//          {
//            // glitches on non-buffered display
//            //if (wdg->getRect().intersects(aRect))   // intersects update rect?
//            //{
//              wdg->doPaint(aCanvas,aRect);
//            //}
//          }
//        }
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
//        if (mCaptureWidget)
//        {
//          mCaptureWidget->doMouseDown(aXpos,aYpos,aButton);
//        } //capture
//        else
//        {
//          axWidget* hover = findWidget(aXpos,aYpos);
//          if (hover!=this)
//          {
//            if (mFlags.hasFlag(wf_Capture)) mCaptureWidget = hover;
//            hover->doMouseDown(aXpos,aYpos,aButton);
//          }
//        } //!capture
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
//        if (mCaptureWidget)
//        {
//          mCaptureWidget->doMouseUp(aXpos,aYpos,aButton);
//          mCaptureWidget = NULL;
//        } //capture
//        // send event to widget under mouse cursor
//        else
//        {
//          axWidget* hover = findWidget(aXpos,aYpos);
//          if (hover!=this)
//          {
//            hover->doMouseUp(aXpos,aYpos,aButton);
//          }
//        }
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
//        axWidget* hover = findWidget(aXpos,aYpos);
//        if (hover!=mHoverWidget)
//        {
//          mHoverWidget->doLeave(mCaptureWidget);
//          mHoverWidget = hover;
//          mHoverWidget->doEnter(mCaptureWidget);
//        }
//        if (mCaptureWidget)
//        {
//          mCaptureWidget->doMouseMove(aXpos,aYpos,aButton);
//        } //!capture
//        // send event to widget under mouse cursor
//        else
//        {
//          if (hover!=this)
//          {
//            hover->doMouseMove(aXpos,aYpos,aButton);
//          }
//        } //!capture
      }

    //----------

    virtual void doKeyDown(int aKeyCode, int aState)
      {
//        if (mCaptureWidget)
//        {
//          mCaptureWidget->doKeyDown(aKeyCode,aState);
//        }
      }

    //----------

    virtual void doKeyUp(int aKeyCode, int aState)
      {
//        if (mCaptureWidget)
//        {
//          mCaptureWidget->doKeyUp(aKeyCode,aState);
//        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture) {}
    virtual void doLeave(axWidget* aCapture) {}


    //----------------------------------------
    // axWidgetListener
    //----------------------------------------

    //virtual void onChange(axWidget* aWidget)
    //  {
    //    mListener->onChange(aWidget);
    //  }

};


//----------------------------------------------------------------------
#endif
