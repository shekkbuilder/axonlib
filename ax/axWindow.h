#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axContainer.h"
#include "axSurface.h"

// mouse buttons and mod keys
#define but_None    0
#define but_Left    1
#define but_Right   2
#define but_Middle  4
#define but_Shift   8
#define but_Ctrl    16
#define but_Alt     32
//const int buDouble  = 1 << 6;
//const int buHover   = 1 << 7;

//----------------------------------------------------------------------

class axWindowBase : public axContainer
{
  protected:
    int         mParent;
    int         mWinFlags;
    axCanvas*   mCanvas;
    axSurface*  mSurface;

  public:

    axWindowBase(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    : axContainer(aListener,aID,aRect/*,aWinFlags*/)
      {
        mParent = 0;
        mWinFlags = aWinFlags;
        mCanvas = NULL;
        mSurface = NULL;
        //mDirtyList.clear();
      }

    virtual ~axWindowBase()
      {
      }

    //----------

    virtual void show(void) {}
    virtual void hide(void) {}
    virtual void setPos(int aX, int aY) {}
    virtual void setSize(int aWidth, int aHeight) {}
    virtual void setParentSize(int aWidth, int aHeight) {}
    virtual void resizeBuffer(int aWidth, int aHeight) {}
    virtual void setTitle(axString aTitle) {}
    virtual void reparent(int aParent) {}
    virtual void invalidate(int aX, int aY, int aW, int aH) {}
    virtual void flush(void) {}
    virtual void sync(void) {}
    virtual void lock(void) {}
    virtual void unlock(void) {}
    virtual void beginPaint(void) {}
    virtual void endPaint(void) {}
    virtual void startTimer(int ms) {}
    virtual void stopTimer(int ms) {}
    virtual void setCursor(int aCursor) {}
    virtual void setCursorPos(int aX, int aY) {}
    virtual void showCursor(void) {}
    virtual void hideCursor(void) {}
    virtual void grabCursor(void) {}
    virtual void releaseCursor(void) {}
    virtual void sendEvent(unsigned int aValue) {}
    virtual void eventLoop(void) {}

    //

};

//----------------------------------------------------------------------

#ifdef linux
  #include "axWindowX11.h"
#endif

#ifdef WIN32
  #include "axWindowW32.h"
#endif

//----------------------------------------------------------------------

class axWindow : public axWindowImpl
{
  public:
    //axSurface* mSurface;

  public:

    axWindow(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    : axWindowImpl(aWinName, aListener,aID,aRect,aWinFlags)
      {
        mCanvas = new axCanvas( getHandle(), cmo_window );
        if (aWinFlags&AX_BUFFERED) mSurface = new axSurface(aRect.w,aRect.h,cmo_buffer); // aFlags...
        setBackground( true, AX_GREY );
      }

    virtual ~axWindow()
      {
        //if( mCanvas ) delete mCanvas;
      }

    //----------------------------------------
    //
    //----------------------------------------


    //----------------------------------------

    void redrawRect( int aX, int aY, int aW, int aH )
      {
        //("redrawRect %i,%i - %i,%i\n",aX,aY,aW,aH);
        invalidate(aX,aY,aW,aH);
      }

    void redrawRect( axRect aRect )
      {
        redrawRect( aRect.x, aRect.y, aRect.w, aRect.h );
      }

    void redrawWidget(axWidget* aWidget)
      {
        redrawRect( aWidget->mRect );
      }

    void redraw(void)
      {
        //TRACE("axWindow redraw %i,%i,%i,%i\n",mRect.x,mRect.y,mRect.w,mRect.h);
        redrawRect( mRect );
      }

    //----------------------------------------
    // widget handler
    //----------------------------------------

    //virtual void      doReset(void)                             {}
    //virtual void      doSetValue(float aValue)                  {} // 0..1
    //virtual float     doGetValue(void)                          { return 0; } // 0..1
    //virtual axString  doGetName(void)                           { return STR_EMPTY; }
    //virtual void      doMove(int aX, int aY)                    {}
    //virtual void      doResize(int aW, int aH)                  {}
    //virtual void      doRealign(void)                           {}
    //virtual void      doPaint(axCanvas* aCanvas, axRect aRect)  {}
    //virtual void      doEnter(void)                             {}
    //virtual void      doLeave(void)                             {}
    //virtual void      doTimer(void)                             {}
    //virtual void      doMouseDown(int aX, int aY, int aB)       {}
    //virtual void      doMouseUp(int aX, int aY, int aB)         {}
    //virtual void      doMouseMove(int aX, int aY, int aB)       {}
    //virtual void      doKeyDown(int aK, int aS)                 {}
    //virtual void      doKeyUp(int aK, int aS)                   {}

    //virtual void doResize(int aW, int aH)
    //  {
    //    TRACE("doResize %i,%i\n",aW,aH);
    //    axContainer::doResize(aW,aH);
    //  }

    //virtual void      doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    TRACE("axContainer.doPaint %i,%i\n",aRect.w,aRect.h);
    //    TRACE("              mRect %i,%i\n",mRect.w,mRect.h);
    //    axContainer::doPaint(aCanvas, aRect);
    //  }




    //----------------------------------------
    // widget listener
    //----------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        redrawWidget(aWidget);
      }

    virtual void onRedraw(axWidget* aWidget)
      {
        redrawWidget(aWidget);
      }

    virtual void onRedraw(axRect aRect)
      {
        redrawRect(aRect);
      }

    virtual void onRedrawAll(void)
      {
        redraw();
      }

    virtual void onSetHint(axString aHint)
      {
        //mListener->onSetHint(aHint);
        //setTitle(aHint);
      }

    //----------

    //virtual void onSetCursor(int aCursor)
    //  {
    //    mListener->onSetCursor(aCursor);
    //  }

};

//----------------------------------------------------------------------

//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
