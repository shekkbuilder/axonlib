#ifndef axWindowLinux_included
#define axWindowLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
#include <pthread.h>
//#include <unistd.h>   // sleep

#include "platform/axContext.h"
#include "axDefines.h"
#include "core/axRect.h"
#include "core/axString.h"
#include "base/axWindowBase.h"
#include "gui/axWidget.h"
#include "gui/axCanvas.h"
#include "gui/axSurface.h"

//----------------------------------------------------------------------

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
#define DEF_CURSOR      -1

//----------

// thread signals
#define ts_Kill   666
#define ts_Timer  667

//----------------------------------------------------------------------

void* threadProc(void* data);
void eventProc(XEvent* ev);

// empty/blank mouse cursor
static char noData[] = { 0,0,0,0,0,0,0,0 };

//char* x11_event_names[] =
//{
//  (char*)"0",
//  (char*)"1",
//  (char*)"KeyPress",
//  (char*)"KeyRelease",
//  (char*)"ButtonPress",
//  (char*)"ButtonRelease",
//  (char*)"MotionNotify",
//  (char*)"EnterNotify",
//  (char*)"LeaveNotify",
//  (char*)"FocusIn",
//  (char*)"FocusOut",
//  (char*)"KeymapNotify",
//  (char*)"Expose",
//  (char*)"GraphicsExpose",
//  (char*)"NoExpose",
//  (char*)"VisibilityNotify",
//  (char*)"CreateNotify",
//  (char*)"DestroyNotify",
//  (char*)"UnmapNotify",
//  (char*)"MapNotify",
//  (char*)"MapRequest",
//  (char*)"ReparentNotify",
//  (char*)"ConfigureNotify",
//  (char*)"ConfigureRequest",
//  (char*)"GravityNotify",
//  (char*)"ResizeRequest",
//  (char*)"CirculateNotify",
//  (char*)"CirculateRequest",
//  (char*)"PropertyNotify",
//  (char*)"SelectionClear",
//  (char*)"SelectionRequest",
//  (char*)"SelectionNotify",
//  (char*)"ColormapNotify",
//  (char*)"ClientMessage",
//  (char*)"MappingNotify",
//  (char*)"GenericEvent"
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axWindowLinux : public axWindowBase
{
  friend void* threadProc(void* data);

  private:
    Display*    mDisplay;
    Window      mParent;
    long        mEventMask;
    Atom        mDeleteWindowAtom;
    pthread_t   mThreadHandle;
    Window      mHandle;
    Atom        mCustomEventAtom;
    Cursor      mInvisibleCursor;
    Pixmap      mBitmapNoData;
    XColor      mBlack;
  protected:
    // ptr to external axContext (used when creating surfaces)
    //axContext*  mContext;
    int         mWinCursor;

    //Cursor    invisibleCursor;
    //Pixmap    bitmapNoData;
    //int       mWinCursor;
    //XColor    mBlack;


  public:

    //----------------------------------------
    //
    //----------------------------------------

    axWindowLinux(axContext* aContext, axRect aRect, int aWinFlags)
    : axWindowBase(aContext,aRect,aWinFlags)
      {

        //mContext  =  aContext;
        //mRect     = aRect;
        //mWinFlags = aWinFlags;

        mDisplay  = aContext->mDisplay;
        mParent   = aContext->mWindow;

        mEventMask  = ExposureMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | PointerMotionMask
                    | KeyPressMask
                    | KeyReleaseMask
                    | StructureNotifyMask
                    | PropertyChangeMask
                    | ClientMessage;

        XSetWindowAttributes    swa;
        swa.override_redirect = true;       // http://tronche.com/gui/x/xlib/window/attributes/override-redirect.html
        swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
        swa.colormap          = 0;          // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
        swa.event_mask        = mEventMask; // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html

        // --- create window ---

        mHandle = XCreateWindow(
          mDisplay,
          mParent,
          aRect.x,aRect.y,aRect.w,aRect.h,
          CopyFromParent,
          CopyFromParent,
          InputOutput,
          CopyFromParent,
          CWBackPixmap|CWEventMask,
          &swa
        );

        // --- WM_DELETE_WINDOW ClientMessage ---

        if (mWinFlags.hasFlag(AX_WIN_MSGDELETE))
        {
          mDeleteWindowAtom = XInternAtom(mDisplay,"WM_DELETE_WINDOW",True);
          XSetWMProtocols(mDisplay,mHandle,&mDeleteWindowAtom,1);
        }

        // --- remove title-bar, borders ---

        if (mWinFlags.hasFlag(AX_WIN_EMBEDDED))
        {
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
          Atom prop = XInternAtom(mDisplay,"_MOTIF_WM_HINTS",True);
          XChangeProperty(mDisplay,mHandle,prop,prop,32,PropModeReplace,(unsigned char *)&motif_hints,PROP_MOTIF_WM_HINTS_ELEMENTS);
          #undef MWM_HINTS_DECORATIONS
          #undef PROP_MOTIF_WM_HINTS_ELEMENTS
        }

        // --- eventProc ---

        if (mWinFlags.hasFlag(AX_WIN_MSGPROC))
        {
          Atom atom;
          void* data;
          // --- eventproc ---
          data = (void*)(int)&eventProc;  // the (int) to make the compiler shut up its complaining!
          atom = XInternAtom(mDisplay,"_XEventProc",false);
          XChangeProperty(mDisplay,mHandle,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
          // --- this ---
          data = this;
          atom = XInternAtom(mDisplay, "_this", false);
          XChangeProperty(mDisplay,mHandle,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
        }
        // --- (invisible) mouse cursor ---

        mBitmapNoData = XCreateBitmapFromData(mDisplay,mHandle,noData,8,8);
        mWinCursor    = -1;
        mBlack.red    = 0;
        mBlack.green  = 0;
        mBlack.blue   = 0;
        mBlack.flags  = (DoRed or DoGreen or DoBlue);
        XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&mBlack);

        // --- canvas ---

        axContext ctx;
        ctx.mDisplay = mDisplay;
        ctx.mWindow = mHandle;
        mCanvas = new axCanvas(&ctx);

        // --- surface ---

        //mSurface = NULL;
        if (aWinFlags & AX_WIN_BUFFERED)
        {
          mSurface = new axSurface(aContext, mRect.w,mRect.h);
        }

        // --- event handler thread ---

        if (mWinFlags.hasFlag(AX_WIN_MSGTHREAD))
        {
          pthread_create(&mThreadHandle,NULL,&threadProc,this);
        }

      }

    //----------------------------------------

    ~axWindowLinux()
      {
        if (mWinFlags.hasFlag(AX_WIN_MSGTHREAD))
        {
          sendEvent(ts_Kill);
          void* ret;
          pthread_join(mThreadHandle,&ret);
        }
        if (mCanvas) delete mCanvas;
        if (mSurface) delete mSurface;
        XFreePixmap(mDisplay, mBitmapNoData);
        if (mWinCursor>=0) XFreeCursor(mDisplay,mWinCursor);
        XDestroyWindow(mDisplay,mHandle);
        XFlush(mDisplay);
      }

    //----------------------------------------

    virtual int getHandle(void) { return (int)mHandle; }

    //----------------------------------------
    // low level
    //----------------------------------------

    virtual void flush(void)
      {
        XFlush(mDisplay);
      }

    //----------

    virtual void sync(void)
      {
        XSync(mDisplay,false);
      }

    //----------

    virtual void lock(void)
      {
        XLockDisplay(mDisplay);
      }

    //----------

    virtual void unlock(void)
      {
        XUnlockDisplay(mDisplay);
      }

    //----------

    virtual void show(void)
      {
        XMapWindow(mDisplay,mHandle);
        XFlush(mDisplay);
      }

    //----------

    virtual void hide(void)
      {
        XUnmapWindow(mDisplay,mHandle);
        XFlush(mDisplay);
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        XWindowChanges attr;
        attr.x = aX;
        attr.y = aY;
        XConfigureWindow(mDisplay,mHandle,CWX|CWY,&attr);
        XFlush(mDisplay);
      }

    //----------

    virtual void setSize(int aW, int aH)
      {
        XResizeWindow(mDisplay,mHandle,aW,aH);
        XFlush(mDisplay);
      }

    //----------

//    //TODO
//    virtual void setParentSize(int aWidth, int aHeight)
//      {
//        //if (mParent!=0)
//        //  XResizeWindow(gDP, mParent, aWidth, aHeight);
//        setSize(aWidth,aHeight);
//      }

    //----------

    virtual void setTitle(axString aTitle)
      {
        XTextProperty window_title_property;
        char* window_title = aTitle.ptr();
        XStringListToTextProperty(&window_title,1,&window_title_property);
        XSetWMName(mDisplay,mHandle,&window_title_property);
        XFlush(mDisplay);
      }

    //----------

    virtual void reparent(int aParent)
      {
        mParent = aParent;
        XReparentWindow(mDisplay,mHandle,mParent,0,0);
        XFlush(mDisplay);
      }

    virtual void resetCursor(void)
      {
        XUndefineCursor(mDisplay,mHandle);
        XFreeCursor(mDisplay,mWinCursor);
        mWinCursor=-1;
      }

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
          mWinCursor = XCreateFontCursor(mDisplay, aCursor);
          XDefineCursor(mDisplay, mHandle, mWinCursor);
        }
      }

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
        mWinCursor = XCreatePixmapCursor( mDisplay,mBitmapNoData,mBitmapNoData,&mBlack,&mBlack,0,0 );
        XDefineCursor(mDisplay,mHandle,mWinCursor);
      }

    //----------

    virtual void grabCursor(void)
      {
        int which = ButtonPressMask|ButtonReleaseMask|PointerMotionMask;
        XGrabPointer(mDisplay,mHandle,false,which,GrabModeSync,GrabModeAsync,mHandle,/*cursor*/None,CurrentTime);
      }

    //----------

    virtual void releaseCursor(void)
      {
        XUngrabPointer(mDisplay, CurrentTime);
      }


    //----------------------------------------
    // medium level
    //----------------------------------------

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        static XExposeEvent ev;
        ev.type     = Expose;
        ev.display  = mDisplay;
        ev.window   = mHandle; // mParent;
        ev.x        = aX;
        ev.y        = aY;
        ev.width    = aW;
        ev.height   = aH;
        ev.count    = 0;
        XSendEvent(mDisplay,mHandle,false,ExposureMask,(XEvent*)&ev);
        XFlush(mDisplay);
      }

    //----------

    virtual void resizeBuffer(int aWidth, int aHeight)
      {
        if (mWinFlags.hasFlag(AX_WIN_BUFFERED))
        {
          //mSurfaceMutex.lock();
          axSurface* srf;
          if (mSurface)
          {
            srf = mSurface;
            mSurface = NULL;
            delete srf;
          }
          //srf = new axSurface(mContext,aWidth,aHeight);
          axContext ctx;
          ctx.mDisplay = mDisplay;
          ctx.mWindow  = mHandle;
          ctx.mAudio   = NULL;
          srf = new axSurface(&ctx,aWidth,aHeight);
          mSurface = srf;
          //mSurfaceMutex.unlock();
        }
      }

    //----------

    virtual void beginPaint(void)
      {
      }

    //----------

    virtual void endPaint(void)
      {
        XFlush(mDisplay);
      }



    //----------------------------------------
    //
    //----------------------------------------

//    virtual void startTimer(int ms)
//      {
//        mTimerSpeed = ms;
//        mTimerRunning = true;
//        /*int ret = */pthread_create( &mTimerThread, NULL, &timerProc, this );
//      }

    //----------

//    virtual void stopTimer(void)
//      {
//        void* retval;
//        mTimerRunning = false;
//        /*int res = */pthread_join(mTimerThread, &retval);
//      }

    //----------------------------------------
    // events
    //----------------------------------------

    virtual void sendEvent(unsigned int aValue=0)
    {
      static XClientMessageEvent event;
      //memset(&event,0,sizeof(event));
      event.type          = ClientMessage;
      event.message_type  = mCustomEventAtom;
      event.display       = mDisplay;
      event.window        = mHandle;
      event.format        = 32;
      event.data.l[0]     = aValue;
      XSendEvent(mDisplay,mHandle,False,NoEventMask,(XEvent*)&event);
      XFlush(mDisplay);
    }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void eventLoop(void)
      {
        XEvent ev;
        while (1)
        {
          XNextEvent(mDisplay,&ev);
          unsigned int data = ev.xclient.data.l[0];
          if (ev.type==ClientMessage && data==mDeleteWindowAtom)
          {
            //trace("exit signal");
            break;
          }
          else eventHandler(&ev);
        }
      }

    //----------------------------------------
    //
    //----------------------------------------

    int remapButton(int aButton)
      {
        return aButton;
      }

    //----------

    int remapKey(int aKey)
      {
        return aKey;
      }

    //----------------------------------------
    // event handler
    //----------------------------------------

    void eventHandler(XEvent* ev)
      {
        //trace("axWindowX11.eventHandler: " << ev->type << " : " << x11_event_names[ev->type]);
        axRect rc;
        int but,key,val;
        int w,h;
        //int x,y;
        switch (ev->type)
        {
          case ConfigureNotify:
            w = ev->xconfigure.width;
            h = ev->xconfigure.height;
            while (XCheckTypedWindowEvent(mDisplay, ev->xconfigure.window, ConfigureNotify, ev))
            {
              w = ev->xconfigure.width;
              h = ev->xconfigure.height;
            }
            //trace("axWindowX11.eventHandler :: ConfigureNotify " << w << "," << h);
            resizeBuffer(w,h);
            doResize(w,h);
            break;
          case Expose:
            beginPaint();
            rc = axRect(ev->xexpose.x,
                        ev->xexpose.y,
                        ev->xexpose.width,
                        ev->xexpose.height);
            while (XCheckTypedWindowEvent(mDisplay, ev->xexpose.window, Expose, ev))
            {
              rc.combine( ev->xexpose.x, ev->xexpose.y, ev->xexpose.width, ev->xexpose.height );
            }
            //trace("axWindowX11.eventHandler :: Expose " << rc.x << "," << rc.y << "," << rc.w << "," << rc.h);
            mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            //if (mSurface)
            //{
            //  mSurfaceMutex.lock();
            //  axCanvas* can = mSurface->mCanvas;
            //  doPaint(can,rc);
            //  mCanvas->blit(can,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
            //  mSurfaceMutex.unlock();
            //}
            //else
            //{
              doPaint(mCanvas,rc);
              //XFlush(mDisplay);
            //}
            mCanvas->clearClipRect();
            endPaint();
            break;
          case ClientMessage:
            val = ev->xclient.data.l[0];
            //trace("axWindowX11.eventHandler :: ClientMessage " << val);
            //if (val==ts_Timer) doTimer();
            break;
          case ButtonPress:
            but = remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: ButtonPress " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseDown(ev->xbutton.x, ev->xbutton.y, but|key );
            break;
          case ButtonRelease:
            but = remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: ButtonRelease " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseUp(ev->xbutton.x, ev->xbutton.y, but|key);
            break;
          case MotionNotify:
            but = 0;//remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: MotionNotify " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseMove(ev->xbutton.x, ev->xbutton.y, but|key);
            break;
          case KeyPress:
            //trace("axWindowX11.eventHandler :: KeyPress " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyDown(ev->xkey.keycode, ev->xkey.state);
            break;
          case KeyRelease:
            //trace("axWindowX11.eventHandler :: KeyRelease " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyUp(ev->xkey.keycode, ev->xkey.state);
            break;
//          default:
//            trace("axWindowX11.eventHandler :: unhandled event :: " << ev->type << " " << x11_event_names[ev->type]);
//            break;
        }

      }

};

//----------------------------------------------------------------------
//
// thread proc
//
//----------------------------------------------------------------------

void* threadProc(void* data)
  {
    //trace("threadProc enter");
    axWindowLinux* win = (axWindowLinux*)data;
    if (win)
    {
      XEvent ev;
      while (1)
      {
        XNextEvent(win->mDisplay,&ev);
        //trace("threadProc event");
        if (ev.type==ClientMessage)
        {
          XClientMessageEvent *cev = (XClientMessageEvent *)&ev;
          unsigned int data = ev.xclient.data.l[0];
          if (cev->message_type == win->mCustomEventAtom && data==ts_Kill)
          {
            //trace("threadProc kill");
            pthread_exit(NULL); //break;
          }
        } //ClientMessage
        else win->eventHandler(&ev);
      }
    }
    //trace("threadProc return");
    return NULL;
  }

//----------------------------------------------------------------------
//
// event proc
//
//----------------------------------------------------------------------

//[internal]
bool xerror;
int errorHandler(Display *dp, XErrorEvent *e)
{
  xerror = true;
  return 0;
}
//----------
//[internal]
int getProperty(Window win, Display* disp, Atom atom)
{
  int result = 0, userSize;
  unsigned long bytes, userCount;
  unsigned char *data;
  Atom userType;
  xerror = false;
  XErrorHandler olderrorhandler = XSetErrorHandler(errorHandler);
  XGetWindowProperty(
    disp,             // display
    win,              // window
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

//typedef struct
//{
//	int type;
//	unsigned long serial; /* # of last request processed by server */
//	Bool send_event;      /* true if this came from a SendEvent request */
//	Display *display;     /* Display the event was read from */
//	Window window;
//} XAnyEvent;

void eventProc(XEvent* ev)
{
  //trace("eventProc");
  Window xw   = ev->xany.window;
  Display* xd = ev->xany.display;
  axWindowLinux* win = (axWindowLinux*)getProperty(xw,xd,XInternAtom(xd,"_this",false));
  if (win==0) return;
  win->eventHandler(ev);
}

//----------------------------------------------------------------------

typedef axWindowLinux axWindowImpl;

//----------------------------------------------------------------------
#endif
