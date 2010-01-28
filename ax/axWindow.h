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
 * @file axWindow.h
 * \brief os/host window
 */

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axContainer.h"
#include "axSurface.h"
#include "axMutex.h"

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
    axMutex     mSurfaceMutex;

  public:

    axWindowBase(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    : axContainer(aListener,aID,aRect/*,aWinFlags*/) // alignment
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

//    int         mParent;
//    int         mWinFlags;
//    axCanvas*   mCanvas;
//    axSurface*  mSurface;

  inline axCanvas* getCanvas(void) { return mCanvas; }


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
        if (aWinFlags&AX_BUFFERED)
        {
          //mSurfaceMutex.lock();
          mSurface = new axSurface(aRect.w,aRect.h/*,cmo_buffer*/);
          //mSurfaceMutex.unlock();
        }
        //setFlag(wfl_DefaultDraw);
      }

    virtual ~axWindow()
      {
        if (mSurface)
          //mSurfaceMutex.lock();
        {
          delete mSurface;
          //mSurfaceMutex.unlock();
        }
        if (mCanvas) delete mCanvas;
      }

    //----------------------------------------
    //
    //----------------------------------------


    //----------------------------------------

    void redrawRect(int aX, int aY, int aW, int aH) { invalidate(aX,aY,aW,aH); }
    void redrawRect(axRect aRect)                   { redrawRect(aRect.x,aRect.y,aRect.w,aRect.h); }
    void redrawWidget(axWidget* aWidget)            { redrawRect(aWidget->getRect()); }
    void redraw(void)                               { redrawRect(mRect ); }

    //----------------------------------------
    // widget handler
    //----------------------------------------

    //----------------------------------------
    // widget listener
    //----------------------------------------

    virtual void onChange(axWidget* aWidget)  { redrawWidget(aWidget); }
    virtual void onRedraw(axWidget* aWidget)  { redrawWidget(aWidget); }
    virtual void onRedraw(axRect aRect)       { redrawRect(aRect); }
    virtual void onRedrawAll(void)            { redraw(); }
    virtual void onCursor(int aCursor)        { setCursor(aCursor); }

    virtual void onMove(axWidget* aWidget, int dX,int dY) {}
    virtual void onResize(axWidget* aWidget, int dX,int dY) {}


};

//----------------------------------------------------------------------

//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
