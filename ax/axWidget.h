#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------
//TODO: mSurface in here, from axWindow
//  axWindow: double buffering
//  axWidget: bitmap for gui
//  (axContainer?)

#include "axString.h"
#include "axRect.h"
#include "axMath.h"
#include "axCanvas.h"

#define wfl_Active    1
#define wfl_Visible   2
#define wfl_Clear     4
#define wfl_Clip      8
#define wfl_Capture   16
#define wfl_Vertical  32
#define wfl_Align     64
#define wfl_All       0xffff

//widget alignment
#define wal_None        0
#define wal_Client      1
#define wal_Left        2
#define wal_Top         3
#define wal_Right       4
#define wal_Bottom      5
#define wal_LeftTop     6
#define wal_RightTop    7
#define wal_LeftBottom  8
#define wal_RightBottom 9
#define wal_Stacked     10
#define wal_Parent      11

//#define MAX_TEXT_LEN 16

#include "axParameter.h"

class axWidget;
#include "axArray.h"
typedef axArray<axWidget*> axWidgets;

//----------------------------------------------------------------------

// Listener -> widget

class axWidgetHandler
{
  public:
    virtual void      doReset(void)                             {}
    virtual void      doSetValue(float aValue)                  {} // 0..1
    virtual float     doGetValue(void)                          { return 0; } // 0..1
    virtual axString  doGetName(void)                           { return STR_EMPTY; }
    virtual void      doMove(int aX, int aY)                    {}
    virtual void      doResize(int aW, int aH)                  {}
    virtual void      doRealign(void)                           {}
    virtual void      doPaint(axCanvas* aCanvas, axRect aRect)  {}
    virtual void      doEnter(void)                             {}
    virtual void      doLeave(void)                             {}
    virtual void      doTimer(void)                             {}
    virtual void      doMouseDown(int aX, int aY, int aB)       {}
    virtual void      doMouseUp(int aX, int aY, int aB)         {}
    virtual void      doMouseMove(int aX, int aY, int aB)       {}
    virtual void      doKeyDown(int aK, int aS)                 {}
    virtual void      doKeyUp(int aK, int aS)                   {}

};

//----------------------------------------------------------------------

// widget -> Listener

class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget)  {}
    virtual void onRedraw(axWidget* aWidget)  {}
    virtual void onRedraw(axRect aRect)       {}
    virtual void onRedrawAll(void)            {}
    virtual void onMove(axWidget* aWidget, int dX,int dY)   {}
    virtual void onResize(axWidget* aWidget, int dX,int dY) {}
    virtual void onHint(axString aHint)       {}
    virtual void onHover(axWidget* aWidget)   {}
    virtual void onCursor(int aCursor)        {}
};

//----------------------------------------------------------------------

class axWidget : public axWidgetHandler
{
  //protected:
  public:
    axParameter *mParameter;
    axWidgetListener* mListener;
    int       mID;
    axRect    mRect;
    int       mFlags;
    axRect    mOrigin;
    axColor   mFillColor;
    int       mAlignment;
    float     mValue;
    void*     mUser;
    int       mCNum;

  public:

    // TODO: remove aParameter, setup things in connect(wdg,par) instead?

    //axWidget(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, axParameter* aParameter=NULL)
    axWidget(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
      {
        mListener   = aListener;
        mID         = aID;
        mRect       = aRect;
        mOrigin     = aRect;
        mParameter  = NULL;//aParameter; // link to parameter (for painting only)
        clearAllFlags();
        setFlag(wfl_Active);
        setFlag(wfl_Visible);
        setFlag(wfl_Capture);
        //setFlag(wfl_Align);
        mFillColor  = AX_GREY;
        mAlignment  = aAlignment;
        mValue      = 0;
        mUser       = NULL;
        mCNum       = -1;
      }

    //virtual ~axWidget() {}

    //--------------------------------------------------

    virtual void setBackground(bool aFill, axColor aColor=AX_GREY)
      {
        if( aFill) setFlag(wfl_Clear);
        else clearFlag(wfl_Clear);
        mFillColor = aColor;
      }

    // --- flags ---

    inline void   clearAllFlags(void)       { mFlags=0; }
    inline void   setFlag(int aFlag)        { /*if (aFlag==0) mFlags=0; else*/ mFlags|=aFlag; }
    inline void   clearFlag(int aFlag)      { mFlags &= ~aFlag; }
    inline bool   hasFlag(int aFlag)        { return (mFlags&aFlag); }

    // --- rect ---

    inline axRect getRect(void)             { return mRect; }
    inline bool   intersects(axRect aRect)  { return (mRect.intersects(aRect)); }
    inline bool   contains(int aX, int aY)  { return (mRect.contains(aX,aY));  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    //virtual void      doReset(void)             { mValue = 0; }
    virtual void      doSetValue(float aValue)  { mValue = aValue; }
    virtual float     doGetValue(void)          { return mValue; }
    virtual axString  doGetName(void)           { return STR_EMPTY; }

    virtual void doMove(int aX, int aY)
      {
        //int dx = aX - mRect.x;
        //int dy = aY - mRect.y;
        //mRect.x += dx;
        //mRect.y += dy;
        mRect.x = aX;
        mRect.y = aY;
      }

    //----------

    virtual void doResize(int aW, int aH)
      {
        //mRect.w( aW );
        //mRect.h( aH );
        mRect.w = aW;
        mRect.h = aH;
      }

    //virtual void doRealign(void)
    //  {
    //  }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( hasFlag(wfl_Visible) )
        {
          if( hasFlag(wfl_Clear) )
          {
            aCanvas->setBrushColor( mFillColor );
            aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
            //aCanvas->drawPanel( mRect.x, mRect.y, mRect.x2(), mRect.y2(), 0);
          }
        }
      }

};

#undef MAX_TEXT_LEN

//----------------------------------------------------------------------
#endif


