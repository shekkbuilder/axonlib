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
 * @file axWindowW32.h
 * \brief window for win32
 */

#ifndef axWindowW32_included
#define axWindowW32_included
//----------------------------------------------------------------------

//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
//#include <pthread.h>

#include <unistd.h> // sleep
#include <windows.h>

#include "axRect.h"
#include "axMath.h"
#include "axUtils.h"
#include "axGlobals.h"
//#include "axColor.h"
#include "axWindow.h"
#include "axDebug.h"

//----------------------------------------------------------------------

//void eventProc(XEvent* ev);
//void* timerProc(void* data);
//static char noData[] = { 0,0,0,0,0,0,0,0 };

LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------

// mouse cursor shapes
#define cuArrow           ((int)IDC_ARROW)
#define cuArrowUp         ((int)IDC_UPARROW)
#define cuArrowDown       -1
#define cuArrowLeft       -1
#define cuArrowRight      -1
#define cuArrowUpDown     ((int)IDC_SIZENS)
#define cuArrowLeftRight  ((int)IDC_SIZEWE)
#define cuMove            ((int)IDC_SIZEALL)
#define cuWait            ((int)IDC_WAIT)
#define cuArrowWait       ((int)IDC_IDC_APPSTARTING)
#define cuHand            ((int)IDC_HAND)
#define cuFinger          -1
#define cuCross           ((int)IDC_CROSS)
#define cuPencil          -1
#define cuPlus            -1
#define cuQuestion        -1
#define cuIbeam           -1
#define DEF_CURSOR    cuArrow

//----------------------------------------------------------------------

class axWindowImpl : public axWindowBase
{
  public:

    axString mWinName;
    HWND mHandle;
    PAINTSTRUCT mPS;
    HCURSOR mWinCursor;
    int mPrevCursor;
    //int btn;
    int clicked_but;
    //HDC mDC;
    //HDC mPaintDC;

  public:

    axWindowImpl(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    : axWindowBase(aWinName,aListener,aID,aRect,aWinFlags)
      {
        mWinName = aWinName;
        //TRACE("eventProc=%x\n",(int)&eventProc);
        mWinCursor = LoadCursor(NULL,IDC_ARROW);
        mPrevCursor = -666;
//----- register window class
        /*
          should this be moved to axGlobal? and where should we unregister it?
          all instances share the same global data in a dll, so we could register the window class once
          and all instances uses that class
        */
        char* classname = mWinName.ptr();//(char*)"axonlib";
        WNDCLASS wc;
        memset(&wc,0,sizeof(wc));
        wc.style          = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc    = &eventProc;
        wc.hInstance      = gInstance;
        wc.lpszClassName  = classname;
        wc.hCursor        = mWinCursor;//LoadCursor(NULL, IDC_ARROW);
        RegisterClass(&wc);
//----- create window (windowed)
        if (aWinFlags&AX_WINDOWED)
        {
          mHandle = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
            classname,                // lpClassName
            0,                        // lpWindowName
            WS_OVERLAPPEDWINDOW,      // dwStyle
            mRect.x,mRect.y,          // x,y
            mRect.w,mRect.h,          // w,h
            0,                        // hWndParent
            0,                        // hMenu
            gInstance,                // hInstance
            0                         // lpParam
          );
        } //AX_WINDOWED
//----- create window (embedded)
        else if (aWinFlags&AX_EMBEDDED)
        {
          mHandle = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            classname,
            0,
            WS_POPUP,
            mRect.x,mRect.y,
            mRect.w,mRect.h,
            0,
            0,
            gInstance,
            0
          );
        }
//-----
        SetWindowLong(mHandle,GWL_USERDATA,(int)this);
        //DragAcceptFiles(mHandle,true);
        //mCanvas = new axCanvas((int)mHandle,cmo_window);
        //if (aWinFlags&AX_BUFFERED) mSurface = new axSurface(mRect.w,mRect.h);
      }

    //----------

    virtual ~axWindowImpl()
      {
        //hide();
        //if (mSurface) delete mSurface;
        //if (mCanvas) delete mCanvas;
        DestroyWindow(mHandle);
        UnregisterClass( mWinName.ptr(), gInstance);
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
        SetWindowPos(mHandle,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
      }

    //----------

    virtual void hide(void)
      {
        ShowWindow(mHandle,SW_HIDE);
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        SetWindowPos(mHandle,0,aX,aY,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
      }

    //----------

    virtual void setSize(int aWidth, int aHeight)
      {
        SetWindowPos(mHandle,HWND_TOP,0,0,aWidth,aHeight,SWP_NOMOVE);
        //SetWindowPos(mHandle,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
      }

      //----------

      //internal
      void GetWindowSize(HWND pWnd, int* pW, int* pH)
        {
          if (pWnd)
          {
            RECT r;
            GetWindowRect(pWnd, &r);
            *pW = r.right - r.left;
            *pH = r.bottom - r.top;
          }
          else
          {
            *pW = *pH = 0;
          }
        }

      //internal
      bool IsChildWindow(HWND pWnd)
        {
          if (pWnd)
          {
            int style = GetWindowLong(pWnd, GWL_STYLE);
            int exStyle = GetWindowLong(pWnd, GWL_EXSTYLE);
            return ((style & WS_CHILD) && !(exStyle & WS_EX_MDICHILD));
          }
          return false;
        }


      //----------

    #define SETPOS_FLAGS SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE

    virtual void setParentSize(int aWidth, int aHeight)
      {
        int dw = aWidth - mRect.w, dh = aHeight - mRect.h;
        HWND pParent = 0, pGrandparent = 0;
        int w=0, h=0, parentW=0, parentH=0, grandparentW=0, grandparentH=0;
        GetWindowSize(mHandle,&w,&h);
        if (IsChildWindow(mHandle))
        {
          pParent = GetParent(mHandle);
          GetWindowSize(pParent, &parentW, &parentH);
          if (IsChildWindow(pParent))
          {
            pGrandparent = GetParent(pParent);
            GetWindowSize(pGrandparent, &grandparentW, &grandparentH);
          }
        }
        SetWindowPos(mHandle, 0, 0, 0, w + dw, h + dh, SETPOS_FLAGS);
        if (pParent)
        {
          SetWindowPos(pParent, 0, 0, 0, parentW + dw, parentH + dh, SETPOS_FLAGS);
        }
        if (pGrandparent)
        {
          SetWindowPos(pGrandparent, 0, 0, 0, grandparentW + dw, grandparentH + dh, SETPOS_FLAGS);
        }
      }

    #undef SETPOS_FLAGS

    //----------

    //TODO: locking?

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
              srf = mSurface;
              mSurface = NULL;
              delete srf;
            }
            srf = new axSurface(aWidth,aHeight/*,mWinFlags*/);
            mSurface = srf;
            mSurfaceMutex.unlock();
          }
          //mRect.w = aWidth;
          //mRect.h = aHeight;
          //doResize(aWidth,h);
        //} //newsize
      }

    //----------

    //virtual void setTitle(axString aTitle)
    //  {
    //  }

    //----------

    virtual void reparent(int aParent)
      {
        mParent = aParent;
        SetWindowLong(mHandle,GWL_STYLE,(GetWindowLong(mHandle,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
        SetParent(mHandle, (HWND)aParent);
      }

    //----------

    // The invalidated areas accumulate in the update region until the region is processed when the next
    // WM_PAINT message occurs or until the region is validated by using the ValidateRect or ValidateRgn function.
    // The system sends a WM_PAINT message to a window whenever its update region is not empty
    // and there are no other messages in the application queue for that window.

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        RECT R;
        R.left   = aX;
        R.top    = aY;
        R.right  = aX + aW - 1;
        R.bottom = aY + aH - 1;
        InvalidateRect(mHandle,&R,false);
        //UpdateWindow(mHandle);
      }

    //----------

    virtual void flush(void)
      {
        GdiFlush();
      }

    //----------

    //virtual void sync(void)
    //  {
    //  }

    //----------

    //virtual void lock(void)
    //  {
    //  }

    //----------

    //virtual void unlock(void)
    //  {
    //  }

    //----------

    // The BeginPaint function prepares the specified window for painting
    // and fills a PAINTSTRUCT structure with information about the painting
    //
    // The BeginPaint function automatically sets the clipping region of the device context
    // to exclude any area outside the update region. The update region is set by the InvalidateRect
    // or InvalidateRgn function and by the system after sizing, moving, creating, scrolling,
    // or any other operation that affects the client area. If the update region is marked for erasing,
    // BeginPaint sends a WM_ERASEBKGND message to the window.
    //
    // An application should not call BeginPaint except in response to a WM_PAINT message

    virtual void beginPaint(void)
      {
        /*mPaintDC = */BeginPaint(mHandle,&mPS);
      }

    //----------

    virtual void endPaint(void)
      {
        EndPaint(mHandle,&mPS);
      }

    //----------

    virtual void startTimer(int ms)
      {
        /*mTimer = */SetTimer(mHandle,666,ms,NULL/*timerProc*/);
      }

    //----------

    virtual void stopTimer(void)
      {
        KillTimer(mHandle,666);
      }

    //------------------------------

    virtual void setCursor(int aCursor/*, bool aHide=false*/)
      {
        if( aCursor<0 )
        {
          hideCursor();// aCursor = DEF_CURSOR;
          mPrevCursor = aCursor;
        } //-1
        else
        {
          if( mPrevCursor<0 ) showCursor();
          if( aCursor != mPrevCursor )
          {
            //mWinCursor = LoadCursor(NULL,(char*)aCursor);
            //SetCursor( mWinCursor );
            SetCursor( LoadCursor(NULL,(char*)aCursor) );
            mPrevCursor = aCursor;
          }
        } //>0
      }

    //----------

    virtual void setCursorPos(int aX, int aY)
      {
        SetCursorPos(aX,aY);
      }

    //----------

    virtual void showCursor(void)
      {
        ShowCursor(true);
      }

    //----------

    virtual void hideCursor(void)
      {
        ShowCursor(false);
      }

    //----------

    virtual void grabCursor(void)
      {
        SetCapture(mHandle);
      }

    //----------

    virtual void releaseCursor(void)
      {
        ReleaseCapture();
      }

    //------------------------------

    //virtual void sendEvent(unsigned int aValue)
    //  {
    //  };

    //----------

    virtual void eventLoop(void)
      {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }

    //--------------------------------------------------

    int mod_keys(int wParam)
      {
        int result = but_None;
        if (wParam & MK_SHIFT) result |= but_Shift;
        if (wParam & MK_CONTROL) result |= but_Ctrl;
        if (GetKeyState(VK_MENU) < 0) result |= but_Alt;
        return result;
      };

    //----------

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
        //MSG msg2;
        //HDC dc;
        LRESULT result = 0;
        int btn;
        axRect rc;
        int w,h;

        //TRACE("win32 eventHandler. msg=%x\n",message);
        switch (message)
        {
          case WM_GETDLGCODE:
            result = DLGC_WANTALLKEYS;
            break;
          case WM_PAINT:
            //TRACE("WM_PAINT\n");
            beginPaint();
            rc = axRect(  mPS.rcPaint.left,
                          mPS.rcPaint.top,
                          mPS.rcPaint.right -  mPS.rcPaint.left + 1,
                          mPS.rcPaint.bottom - mPS.rcPaint.top  + 1);
            if (mWinFlags&AX_BUFFERED && mSurface)
            {
              mSurfaceMutex.lock();
              axCanvas* can = mSurface->mCanvas;
              //can->setClipRect(rc);
              doPaint(can,rc);
              mCanvas->blit( can, rc.x,rc.y, rc.x,rc.y,rc.w,rc.h );
              //can->clearClipRect();
              mSurfaceMutex.unlock();

            }
            else
            {
              // http://msdn.microsoft.com/en-us/library/dd183439%28VS.85%29.aspx
              // if you obtain a device context handle from the  BeginPaint function,
              // the DC contains a predefined rectangular clipping region that corresponds
              // to the invalid rectangle that requires repainting
              //mCanvas->setClipRect(rc);
              doPaint(mCanvas,rc);
              //mCanvas->clearClipRect();
            }
            endPaint();
            break;
          case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
            switch (message)
            {
              case WM_LBUTTONDOWN: btn = but_Left;   break;
              case WM_MBUTTONDOWN: btn = but_Middle; break;
              case WM_RBUTTONDOWN: btn = but_Right;  break;
            }
            clicked_but = btn;
            doMouseDown(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | mod_keys(wParam));
            if( mCapturedWidget ) grabCursor();
            break;
          case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
            switch (message)
            {
              case WM_LBUTTONUP: btn = but_Left;   break;
              case WM_MBUTTONUP: btn = but_Middle; break;
              case WM_RBUTTONUP: btn = but_Right;  break;
            }
            clicked_but = but_None;
            doMouseUp(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | mod_keys(wParam));
            if( !mCapturedWidget ) releaseCursor();
            break;
          case WM_MOUSEMOVE:
            doMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), clicked_but | mod_keys(wParam));
            //return 1;
            break;
//          case WM_CHAR:
//            doKeyChar(wParam,lParam);
//            break;
          case WM_KEYDOWN:
            doKeyDown(wParam,lParam);
            break;
          case WM_KEYUP:
            doKeyDown(wParam,lParam);
            break;
          case WM_SIZE:
            //TRACE("WM_SIZE\n");

            //lParam:
            //  The low-order word of lParam specifies the new width of the client area.
            //  The high-order word of lParam specifies the new height of the client area.
            //TODO: resize surface, if any
            //int x = ev->xconfigure.x;
            //int y = ev->xconfigure.y;
            w = short(LOWORD(lParam));
            h = short(HIWORD(lParam));

            //if (w!=mRect.w || h!=mRect.h)
            //{

            //// hack: ignore this if there is other WM_SIZE messages in the queue
            //if ( PeekMessage(&msg2,mHandle,WM_SIZE,WM_SIZE,PM_NOREMOVE) )
            //{
            //  TRACE("there are oher WN_SIZE messages in the queue, so we're ignoring this one\n");
            //}
            //else
            //{
              //flush();
              resizeBuffer(w,h);
              doResize(w,h);
            //}
            //}

            result = 0;
            break;
          case WM_DESTROY:
            if (mWinFlags&AX_WINDOWED) PostQuitMessage(0);
            break;
          case WM_TIMER:
            if (wParam==666) doTimer();
            result = 0;
            break;
          case WM_SETCURSOR:
            SetCursor( mWinCursor );
            break;
          default:
            result = DefWindowProc(hWnd,message,wParam,lParam);
        }
        return result;
      }

};

//----------------------------------------------------------------------
// event handler
//----------------------------------------------------------------------

LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  axWindowImpl* wnd = (axWindowImpl*)GetWindowLong(hWnd,GWL_USERDATA);
	if (wnd==0) return DefWindowProc(hWnd,message,wParam,lParam);
  return wnd->eventHandler(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------
#endif

