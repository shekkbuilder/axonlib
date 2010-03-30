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
 * @file axWindowX11.h
 * \brief window for x11
 */

#ifndef axWindowX11_included
#define axWindowX11_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
#include <pthread.h>
#include <unistd.h>   // sleep

#include "axRect.h"
#include "axMath.h"
#include "axUtils.h"
#include "axGlobals.h"
//#include "axColor.h"
#include "axWindow.h"
//#include "axThread.h"

//----------------------------------------------------------------------

void eventProc(XEvent* ev);
void* timerProc(void* data);
#ifdef AX_THREAD_GUI
void* threadProc(void* data);
#endif
static char noData[] = { 0,0,0,0,0,0,0,0 };

// http://tronche.com/gui/x/xlib/appendix/b/
// mouse cursor shapes
#define cuArrow           2
#define cuArrowUp         114
#define cuArrowDown       106
#define cuArrowLeft       110
#define cuArrowRight      112
#define cuArrowUpDown     116
#define cuArrowLeftRight  108
#define cuMove            52
#define cuWait            150
#define cuArrowWait       -1
#define cuHand            58
#define cuFinger          60
#define cuCross           0
#define cuPencil          86
#define cuPlus            90
#define cuQuestion        99
#define cuIbeam           152

//#define DEF_CURSOR    cuArrow
#define DEF_CURSOR    -1

//----------------------------------------------------------------------

char* x11_event_names[] =
{
  (char*)"0",
  (char*)"1",
  (char*)"KeyPress",
  (char*)"KeyRelease",
  (char*)"ButtonPress",
  (char*)"ButtonRelease",
  (char*)"MotionNotify",
  (char*)"EnterNotify",
  (char*)"LeaveNotify",
  (char*)"FocusIn",
  (char*)"FocusOut",
  (char*)"KeymapNotify",
  (char*)"Expose",
  (char*)"GraphicsExpose",
  (char*)"NoExpose",
  (char*)"VisibilityNotify",
  (char*)"CreateNotify",
  (char*)"DestroyNotify",
  (char*)"UnmapNotify",
  (char*)"MapNotify",
  (char*)"MapRequest",
  (char*)"ReparentNotify",
  (char*)"ConfigureNotify",
  (char*)"ConfigureRequest",
  (char*)"GravityNotify",
  (char*)"ResizeRequest",
  (char*)"CirculateNotify",
  (char*)"CirculateRequest",
  (char*)"PropertyNotify",
  (char*)"SelectionClear",
  (char*)"SelectionRequest",
  (char*)"SelectionNotify",
  (char*)"ColormapNotify",
  (char*)"ClientMessage",
  (char*)"MappingNotify",
  (char*)"GenericEvent"
};

//----------------------------------------------------------------------


class axWindowImpl : public axWindowBase
{
  private:
    long eventmask;

  public:

    Window    mHandle;
    Atom      wmDeleteMessage;
    pthread_t mTimerThread;
    bool      mTimerRunning;
    int       mTimerSpeed;
    Cursor    invisibleCursor;
    Pixmap    bitmapNoData;
    int       mWinCursor;
    XColor    mBlack;

    Atom      mCustomEventAtom;
    #ifdef AX_THREAD_GUI
    pthread_t mThreadHandle;
    //bool      mIsGuiThreadRunning;
    #endif

  public:

    //axWindowImpl(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    axWindowImpl(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags, int aParent)
    : axWindowBase(aWinName,aListener,aID,aRect,aWinFlags,aParent)
      {
    /*long*/ eventmask  = ExposureMask
                        | ButtonPressMask
                        | ButtonReleaseMask
                        | PointerMotionMask
                        | KeyPressMask
                        | KeyReleaseMask
                        | StructureNotifyMask
                        | PropertyChangeMask
                        | ClientMessage;


//-----
        if (aWinFlags&AX_WINDOWED) // || aParent=0;
        {
//          mHandle = XCreateSimpleWindow(
//            gDP,                      // display
//            XDefaultRootWindow(gDP),  // parent
//            aRect.x,aRect.y,aRect.w,aRect.h,
//            CopyFromParent,//0,                        // border width
//            CopyFromParent,//0,                        // border col
//            CopyFromParent //0                         // backcol
//          );
//          XSelectInput(gDP,mHandle,eventmask);
          XSetWindowAttributes swa;
          swa.override_redirect = true;       // http://tronche.com/gui/x/xlib/window/attributes/override-redirect.html
          swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
          swa.colormap          = 0;          // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
          swa.event_mask        = eventmask;  // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html
          mHandle = XCreateWindow(
            gDP,
            XDefaultRootWindow(gDP),
            aRect.x,aRect.y,aRect.w,aRect.h,
            CopyFromParent,
            CopyFromParent,
            InputOutput,
            CopyFromParent,
            CWBackPixmap|CWEventMask,
            &swa
          );
          // --- WM_DELETE_WINDOW ClientMessage
          wmDeleteMessage = XInternAtom(gDP,"WM_DELETE_WINDOW",True);
          XSetWMProtocols(gDP,mHandle,&wmDeleteMessage,1);
        } //AX_WINDOWED
//-----
        else if (aWinFlags&AX_EMBEDDED)
        {

          XSetWindowAttributes swa;
          swa.override_redirect = true;       // http://tronche.com/gui/x/xlib/window/attributes/override-redirect.html
          swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
          swa.colormap          = 0;          // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
          swa.event_mask        = eventmask;  // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html
          //trace("axWindowX11: XDefaultRootWindow(gDP)=" << (int)XDefaultRootWindow(gDP) << " aParent=" << aParent << " gDP=" << (int)gDP );
          trace("axWindowX11.constructor :: gDP=" << hex << (int)gDP << " aParent=" << (int)aParent << dec);
          //trace("> XDefaultRootWindow(gDP) = " << hex << (int)XDefaultRootWindow(gDP) << dec);
          mHandle = XCreateWindow(
            gDP,
            // here you must supply the parent window that as been given to you in the effEditOpen call
            // Juce checks, that there is a child window of this parent after the return from effEditOpen,
            // and if there is not, it will ignore your _eventproc. This is what happens.
            XDefaultRootWindow(gDP),
            (Window)aParent,
            aRect.x,aRect.y,aRect.w,aRect.h,
            CopyFromParent,
            CopyFromParent,
            InputOutput,
            CopyFromParent,
            CWBackPixmap|CWEventMask,
            &swa
          );

//          Atom atom;
//          void* data;
          // --- title-bar, borders
          #define MWM_HINTS_DECORATIONS (1L << 1)
          #define PROP_MOTIF_WM_HINTS_ELEMENTS  5
          typedef struct
          {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long          inputMode;
            unsigned long status;
          } PropMotifWmHints;
          PropMotifWmHints motif_hints;
          motif_hints.flags = MWM_HINTS_DECORATIONS;
          motif_hints.decorations = 0;
          Atom prop = XInternAtom(gDP,"_MOTIF_WM_HINTS",True);
          XChangeProperty(gDP,mHandle,prop,prop,32,PropModeReplace,(unsigned char *)&motif_hints,PROP_MOTIF_WM_HINTS_ELEMENTS);
#ifndef AX_THREAD_GUI
          Atom atom;
          void* data;
          // --- eventproc
          data = (void*)(int)&eventProc;  // the (int) to make the compiler shut up its complaining!
          atom = XInternAtom(gDP,"_XEventProc",false);
          XChangeProperty(gDP,mHandle,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
          // --- this
          data = this;
          atom = XInternAtom(gDP, "_this", false);
          XChangeProperty(gDP,mHandle,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
#endif
        } //AX_EMBEDDED
//-----
        bitmapNoData = XCreateBitmapFromData(gDP,mHandle,noData,8,8);
        mWinCursor = -1;
        //mBlack = axColor(0,0,0);
        mBlack.red   = 0;
        mBlack.green = 0;
        mBlack.blue  = 0;
        mBlack.flags = (DoRed or DoGreen or DoBlue);
        XAllocColor(gDP,XDefaultColormap(gDP,0),&mBlack);
        //mCanvas = new axCanvas(mHandle,cmo_window);
        //if (aWinFlags&AX_BUFFERED) mSurface = new axSurface(aRect.w,aRect.h);
        #ifdef AX_THREAD_GUI
        trace("axWindowX11.constructor :: pthread_create...");
        //mIsGuiThreadRunning = false;
        pthread_create(&mThreadHandle,NULL,&threadProc,this);
        //mIsGuiThreadRunning = true;
        trace("axWindowX11.constructor :: ...thread up and running?");
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual ~axWindowImpl()
      {
        //hide(); // will also kill the (eventual) message thread
        //if (mSurface) delete mSurface;
        //if (mCanvas) delete mCanvas;
        #ifdef AX_THREAD_GUI
        //mIsGuiThreadRunning = false;
        //pthread_cancel(mThreadHandle);
        sendEvent(667); // signal we want to quit
        void* ret;
        trace("axWindowX11.destructor :: waiting for pthread_join...");
        pthread_join(mThreadHandle,&ret);
        trace("axWindowX11.destructor :: ...returned from pthread_join");
        #endif
        XDestroyWindow(gDP,mHandle);
        XFreePixmap(gDP, bitmapNoData);
        if (mWinCursor>=0) XFreeCursor(gDP,mWinCursor);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual int getHandle(void) { return (int)mHandle; }

//    //----------------------------------------
//    // widget handler
//    //----------------------------------------
//
//    virtual void doTimer(void) {}
//    virtual void doSetSize(int aW, int aH) {}
//    virtual void doMouseUp(int aX, int aY, int aB) {}
//    virtual void doMouseDown(int aX, int aY, int aB) {}
//    virtual void doMouseMove(int aXW, int aY, int aB) {}
//    virtual void doKeyDown(int aK, int aS) {}
//    virtual void doKeyUp(int aK, int aS) {}

    //----------------------------------------

    virtual void show(void)
      {
        //trace("axWindowX11.show");
//        #ifdef AX_THREAD_GUI
//        mIsGuiThreadRunning = true;
//        pthread_create(&mThreadHandle,NULL,&threadProc,this);
//        #endif
        XMapWindow(gDP,mHandle);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void hide(void)
      {
        //trace("axWindowX11.hide");
//        #ifdef AX_THREAD_GUI
//        mIsGuiThreadRunning = false;
//        void* ret;
//        //pthread_cancel(mThreadHandle);
//        sendEvent(/*667*/);
//        pthread_join(mThreadHandle,&ret);
//        #endif
        XUnmapWindow(gDP,mHandle);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        XWindowChanges attr;
        attr.x = aX;
        attr.y = aY;
        XConfigureWindow(gDP, mHandle, CWX | CWY, &attr);
        //XMoveWindow(gDP,mHandle,x,y);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void setSize(int aWidth, int aHeight)
      {
        XResizeWindow(gDP, mHandle, aWidth, aHeight);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void setParentSize(int aWidth, int aHeight)
      {
        //if (mParent!=0)
        //  XResizeWindow(gDP, mParent, aWidth, aHeight);
        setSize(aWidth, aHeight);
      }

    //----------

    //TODO: safe-checking
    //- be careful with messing with the mSurface pointer
    //  multi-core / multi-thread, ...
    //- perhaps create the new surface first
    //  then exchange the pointers (danger!)
    //- do a sync() first, to empty the event queue??

    virtual void resizeBuffer(int aWidth, int aHeight)
      {
        //if( aWidth!=mRect.w || aHeight!=mRect.h )
        //{
          if (mWinFlags&AX_BUFFERED)
          {
            mSurfaceMutex.lock();
            axSurface* srf;
            if (mSurface)
            {
              // hopefully, the compiler won't optimize away this!
              // we want to set the mSurface member to NULL before deleting the class
              // in case some late-coming things are trying to access it
              // they check for NULL
              // todo: LOCK
              srf = mSurface;
              mSurface = NULL;
              delete srf;
            }
            srf = new axSurface(aWidth,aHeight/*,mWinFlags*/);
            mSurface = srf;
            mSurfaceMutex.unlock();
          }
          //mRect.w = aWidth;  //mRect.w(w);
          //mRect.h = aHeight;  //mRect.h(h);
          //doResize(aWidth,aHeight);
        //} //newsize
      }

    //----------

    virtual void setTitle(axString aTitle)
      {
        XTextProperty window_title_property;
        char* window_title = aTitle.ptr();
        XStringListToTextProperty(&window_title,1,&window_title_property);
        XSetWMName(gDP,mHandle,&window_title_property);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void reparent(int aParent)
      {
        trace("axWindowX11.reparent :: aParent=" << hex << (int)aParent << dec);
        mParent = aParent;
        XReparentWindow(gDP,mHandle,aParent,0,0);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

    //----------

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        static XExposeEvent ev;
        ev.type     = Expose;
        ev.display  = gDP;
        ev.window   = mHandle; // mParent;
        ev.x        = aX;
        ev.y        = aY;
        ev.width    = aW;
        ev.height   = aH;
        ev.count    = 0;
        XSendEvent(gDP,mHandle,false,ExposureMask,(XEvent*)&ev);
        //eventhandler( (XEvent*)&ev);
        //XFlush(gDP);
        //XClearArea(gDP, mHandle, aX, aY, aW, aH, true);
        //XSync(gDP,false);
        #ifdef AX_THREAD_GUI
        flush();//XFlush(gDP);
        #endif
      }

//    virtual void resizewindow(int aW, int aH)
//      {
//        //TRACE("invalidate (winx11)\n");
//        static XConfigureEvent ev;
//        ev.type     = ConfigureNotify;
//        ev.display  = gDP;
//        ev.window   = mHandle;
//        ev.width    = aW;
//        ev.height   = aH;
//        //ev.count    = 0;
//        //TODO: test true
//        XSendEvent(gDP,mHandle,false, StructureNotifyMask,(XEvent*)&ev);
//        //eventhandler( (XEvent*)&ev);
//        //XFlush(gDP);
//        //XClearArea(gDP, mHandle, aRect.x1, aRect.y1, aRect.width(), aRect.height(), true);
//        //XSync(gDP,false);
//      }

    //----------

    // The XFlush function flushes the output buffer. Most client applications need not use this function
    // because the output buffer is automatically flushed as needed by calls to XPending, XNextEvent,
    // and XWindowEvent. Events generated by the server may be enqueued into the library's event queue.
    //
    // The XSync function flushes the output buffer and then waits until all requests have been received
    // and processed by the X server. Any errors generated must be handled by the error handler.
    // For each protocol error received by Xlib, XSync calls the client application's error handling routine
    // (see section 11.8.2). Any events generated by the server are enqueued into the library's event queue.
    //
    // Finally, if you passed False, XSync does not discard the events in the queue. If you passed True,
    // XSync discards all events in the queue, including those events that were on the queue before XSync
    // was called. Client applications seldom need to call XSync.

    virtual void flush(void)
      {
        XFlush(gDP);
        //XSync(gDP,false);
      }

    virtual void sync(void)
      {
        XSync(gDP,false);
      }

    //----------

    virtual void lock(void)
      {
        XLockDisplay(gDP);
      }

    //----------

    virtual void unlock(void)
      {
        XUnlockDisplay(gDP);
      }

    //----------

    //virtual void beginPaint(void)
    //  {
    //  }

    //----------

    //virtual void endPaint(void)
    //  {
    //    flush();
    //    //XFlush(gDP);
    //    //XSync(gDP,false);
    //  }

    //----------

    virtual void startTimer(int ms)
      {
        mTimerSpeed = ms;
        mTimerRunning = true;
        /*int ret = */pthread_create( &mTimerThread, NULL, &timerProc, this );
      }

    //----------

    virtual void stopTimer(void)
      {
        void* retval;
        mTimerRunning = false;
        /*int res = */pthread_join(mTimerThread, &retval);
      }

    //------------------------------

    virtual void resetCursor(void)
      {
        XUndefineCursor(gDP,mHandle);
        XFreeCursor(gDP,mWinCursor);
        mWinCursor=-1;
      };

    //----------

    virtual void setCursor(int aCursor)
      {
        //if( aCursor<0 ) aCursor = DEF_CURSOR;
        if (mWinCursor>=0) resetCursor();
        //{
        //  XUndefineCursor(gDP,mHandle);
        //  XFreeCursor(gDP,mWinCursor);
        //}
        //if (aCursor<0)
        //mWinCursor = XCreatePixmapCursor( gDP,bitmapNoData,bitmapNoData,&mBlack,&mBlack,0,0 );
        //else
        if (aCursor>=0)
        {
          mWinCursor = XCreateFontCursor(gDP, aCursor);
          XDefineCursor(gDP, mHandle, mWinCursor);
        }
      };

    //----------

    //virtual void setCursorPos(int aX, int aY)
    //  {
    //  }

    //----------

    virtual void showCursor(void)
      {
        setCursor(DEF_CURSOR);
      }

    //----------

    virtual void hideCursor(void)
      {
        //setCursor(-1);
        if (mWinCursor>=0) resetCursor();
        //{
        //  XUndefineCursor(gDP,mHandle);
        //  XFreeCursor(gDP,mWinCursor);
        //}
        mWinCursor = XCreatePixmapCursor( gDP,bitmapNoData,bitmapNoData,&mBlack,&mBlack,0,0 );
        XDefineCursor(gDP, mHandle, mWinCursor);
      }

    //----------

    virtual void grabCursor(void)
      {
        int which = ButtonPressMask|ButtonReleaseMask|PointerMotionMask;
        XGrabPointer(gDP,mHandle,false,which,GrabModeSync,GrabModeAsync,mHandle,/*cursor*/None,CurrentTime);
      }

    //----------

    virtual void releaseCursor(void)
      {
        XUngrabPointer(gDP, CurrentTime);
      }

    //------------------------------

    //virtual void sendEvent(unsigned int aValue)
    //  {
    //  };

    //----------

    //typedef struct {
    //	int type;			          /* ClientMessage */
    //	unsigned long serial;		/* # of last request processed by server */
    //	Bool send_event;		    /* true if this came from a SendEvent request */
    //	Display *display;		    /* Display the event was read from */
    //	Window window;
    //	Atom message_type;
    //	int format;
    //	union {
    //		char b[20];
    //		short s[10];
    //		long l[5];
    //	        } data;
    //} XClientMessageEvent;

    virtual void sendEvent(unsigned int aValue=0)
    {
//      trace("sendEvent");
      static XClientMessageEvent event;
      memset(&event,0,sizeof(event));
      event.type          = ClientMessage;
      event.message_type  = mCustomEventAtom;
      event.display       = gDP;
      event.window        = mHandle;
      event.format        = 32;
      event.data.l[0]     = aValue;
      XSendEvent(gDP,mHandle,False,NoEventMask,(XEvent*)&event);
      #ifdef AX_THREAD_GUI
      flush();//XFlush(gDP);
      #endif
    }

    virtual void eventLoop(void)
      {
        XEvent ev;
        while( 1 )
        {
          XNextEvent(gDP,&ev);
          unsigned int data = ev.xclient.data.l[0];
          if (ev.type==ClientMessage && data==wmDeleteMessage) break;
          else eventHandler(&ev);
        }
      }

    //--------------------------------------------------

    int buttons(int btn)
      {
        int res = but_None;
        switch (btn)
        {
          case 1: res = but_Left; break;
          case 2: res = but_Middle; break;
          case 3: res = but_Right; break;
        }
        return res;//buNone;
      }

    //----------

    int keys(int state)
      {
        int result = 0;
        if (state & 1) result += but_Shift;
        // 2 CapsLock
        if (state & 4) result += but_Ctrl;
        if (state & 8) result += but_Alt;
        // 128 Alt Gr
        return result;
      }

    //----------

// http://tronche.com/gui/x/xlib/events/processing-overview.html

    //virtual void eventHandler(int aEvent)
    virtual void eventHandler(XEvent* ev)
      {
        //XEvent* ev = (XEvent*)aEvent;
        axRect rc;
        int but,key,val;
        //int x,y;
        int w,h;
        switch (ev->type)
        {
          case ConfigureNotify:
            w = ev->xconfigure.width;
            h = ev->xconfigure.height;
            while (XCheckTypedWindowEvent(gDP, ev->xconfigure.window, ConfigureNotify, ev))
            {
              //rc.combine(ev->xexpose.x, ev->xexpose.y,ev->xexpose.width,ev->xexpose.height);
              w = ev->xconfigure.width;
              h = ev->xconfigure.height;
            }
            trace("axWindowX11.eventHandler :: ConfigureNotify " << w << "," << h);
           //flush();
            //if ((w!=mRect.w) || (h!=mRect.h))
            //{
            resizeBuffer(w,h);
            doResize(w,h);
            //}
            //onRedraw(mRect);
            break;
          case Expose:
            beginPaint(); // should this be inside the lock() ?
            rc = axRect(ev->xexpose.x,
                        ev->xexpose.y,
                        ev->xexpose.width,
                        ev->xexpose.height);
            while (XCheckTypedWindowEvent(gDP, ev->xexpose.window, Expose, ev))
            {
              rc.combine( ev->xexpose.x, ev->xexpose.y, ev->xexpose.width, ev->xexpose.height );
            }
            trace("axWindowX11.eventHandler :: Expose " << rc.x << "," << rc.y << "," << rc.w << "," << rc.h);
            if (mWinFlags&AX_BUFFERED && mSurface)
            {
              mSurfaceMutex.lock();
              //axCanvasBase* can = mSurface->mCanvas;
              axCanvas* can = mSurface->mCanvas;
              //can->setClipRect(rc);
              //doPaint(mSurface->mCanvas,rc);
              doPaint(can,rc);
              //mCanvas->blit(mSurface->mHandle,rc.x1,rc.y1,rc.x1,rc.y1,rc.w(),rc.h());
              mCanvas->blit(can,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              //can->clearClipRect();
              mSurfaceMutex.unlock();
            }
            else
            {
              //mCanvas->setClipRect(rc);
              doPaint(mCanvas,rc);
              //mCanvas->clearClipRect();
            }
            endPaint();
            break;
          case ClientMessage:
            val = ev->xclient.data.l[0];
            trace("axWindowX11.eventHandler :: ClientMessage " << val);
            if (val==666) doTimer();  //TODO: fix?
            break;
          case ButtonPress:
            but = buttons(ev->xbutton.button);
            key = keys(ev->xbutton.state);
            trace("axWindowX11.eventHandler :: ButtonPress " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseDown(ev->xbutton.x, ev->xbutton.y, but|key );
            //mClickedButton = but;
            break;
          case ButtonRelease:
            but = buttons(ev->xbutton.button);
            key = keys(ev->xbutton.state);
            trace("axWindowX11.eventHandler :: ButtonRelease " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseUp(ev->xbutton.x, ev->xbutton.y, but|key);
            //mClickedButton = 0;
            break;
          case MotionNotify:
            but = 0;//buttons(ev->xbutton.button);
            key = keys(ev->xbutton.state);
            trace("axWindowX11.eventHandler :: MotionNotify " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseMove(ev->xbutton.x, ev->xbutton.y, but|key);
            break;
          case KeyPress:
            trace("axWindowX11.eventHandler :: KeyPress " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyDown(ev->xkey.keycode, ev->xkey.state);
            break;
          case KeyRelease:
            trace("axWindowX11.eventHandler :: KeyRelease " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyUp(ev->xkey.keycode, ev->xkey.state);
            break;
          default:
            trace("axWindowX11.eventHandler :: unhandled event :: " << ev->type << " " << x11_event_names[ev->type]);
            break;
//
//
//if(ev.type == KeyPress  ev.type == KeyRelease)
//        {
//            /* again the lock key issues -- we just fire if alt is being
//             * pressed, regardless if other modifiers are being pressed or not.
//             */
//            if(ev.xkey.keycode == F1 && ev.xkey.state & Mod1Mask)
//            {
//                if(ev.type == KeyPress)
//                    fprintf(stderr, "got Alt+F1\n");
//
//                swallow_keystroke(dpy, &ev);
//            }
//            else
//            {
//                fprintf(stderr, "got (something else)+F1, passing through\n");
//                passthru_keystroke(dpy, &ev);
//            }
//        }
//
//
//
        } //switch
      }

    //----------------------------------------

};

//----------------------------------------------------------------------
// event handler
//----------------------------------------------------------------------

bool xerror;

//----------

int errorHandler(Display *dp, XErrorEvent *e)
{
  xerror = true;
  return 0;
}

//----------

// !!!!!

int getProperty(Window handle, Atom atom)
{
  int result = 0, userSize;
  unsigned long bytes, userCount;
  unsigned char *data;
  Atom userType;
  xerror = false;
  XErrorHandler olderrorhandler = XSetErrorHandler(errorHandler);
  XGetWindowProperty(
    gDP,              // display
    handle,           // window
    atom,             // property
    0, 1,             // offset, length
    false,            // delete?
    AnyPropertyType,  // req type
    &userType,        // actual type return
    &userSize,        // actual format return
    &userCount,       // n items return
    &bytes,           // bytes after return
    &data             // prop return
  );
  if (xerror==false && userCount==1) result = *(int*)data;
  XSetErrorHandler(olderrorhandler);
  return result;
}

//--------------------------------------------------

void eventProc(XEvent* ev)
{
  //trace("eventProc");
  axWindowImpl* win = (axWindowImpl*)getProperty(ev->xany.window, XInternAtom(gDP,"_this",false));
  if (win==0) return;
  win->eventHandler(ev);
}

//----------------------------------------------------------------------
// timer handler
//----------------------------------------------------------------------
//TODO: sendEvent
void* timerProc(void* data)
{
  axWindowImpl* win = (axWindowImpl*)data;
  if (win)
  {
    while( win->mTimerRunning==true )
    {
      //win->sendEvent(666);
      win->doTimer();
      usleep( win->mTimerSpeed * 1000 ); //ms*1000;
    }
  }
  return NULL;
}

//----------------------------------------------------------------------

#ifdef AX_THREAD_GUI
void* threadProc(void* data)
  {
    trace("> threadProc :: entry");
    axWindowImpl* win = (axWindowImpl*)data;
    if (win)
    {
      XEvent ev;
      while (1)
      {
        //if (!win->mIsGuiThreadRunning) pthread_exit(NULL);  //break;
        //if (XPending(gDP))
        //{
          XNextEvent(gDP,&ev);
          //trace("> threadProc :: event");

          //unsigned int data = ev.xclient.data.l[0];
          //if (ev.type==ClientMessage && data==123) break;
          //else win->eventHandler(&ev);
          if (ev.type==ClientMessage)
          {
            trace("> threadProc :: ClientMessage");
            XClientMessageEvent *cev = (XClientMessageEvent *)&ev;
            unsigned int data = ev.xclient.data.l[0];
            if (cev->message_type == win->mCustomEventAtom && data==667)
            {
              trace("> threadProc :: pthread_exit");
              pthread_exit(NULL); //break;
            }
          }
          else
          {
            //trace("> calling win->eventHandler..");
            win->eventHandler(&ev);
          }
        //}
      }
    }
    trace("> threadProc :: return");
    return NULL;
  }
#endif

//----------------------------------------------------------------------
#endif
