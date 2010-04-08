#ifndef axWindowWin32_included
#define axWindowWin32_included
//----------------------------------------------------------------------

#include <windows.h>

//----------------------------------------------------------------------

LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------

// mouse cursor shapes
#define cu_Arrow           ((int)IDC_ARROW)
#define cu_ArrowUp         ((int)IDC_UPARROW)
#define cu_ArrowDown       cu_Arrow
#define cu_ArrowLeft       cu_Arrow
#define cu_ArrowRight      cu_Arrow
#define cu_ArrowUpDown     ((int)IDC_SIZENS)
#define cu_ArrowLeftRight  ((int)IDC_SIZEWE)
#define cu_Move            ((int)IDC_SIZEALL)
#define cu_Wait            ((int)IDC_WAIT)
#define cu_ArrowWait       ((int)IDC_APPSTARTING)
#define cu_Hand            ((int)IDC_HAND)
#define cu_Finger          cu_Arrow
#define cu_Cross           ((int)IDC_CROSS)
#define cu_Pencil          cu_Arrow
#define cu_Plus            cu_Arrow
#define cu_Question        cu_Arrow
#define cu_Ibeam           cu_Arrow

#define DEF_CURSOR    cu_Arrow

//----------------------------------------------------------------------

class axWindowWin32 : public axWindowBase
{
  private:
    HINSTANCE   mInstance;
    HWND        mWindow;
    axString    mWinName;
    PAINTSTRUCT mPS;
    //int mWinCursor,mPrevCursor;
    //HCURSOR mWinCursor;
    HCURSOR     mWinCursor;
    int         mPrevCursor;
    int         mClickedButton;
    int         mParent;

  public:
    axWindowWin32(axContext* aContext, axRect aRect, int aWinFlags)
    : axWindowBase(aContext,aRect,aWinFlags)
      {
        //wtrace("axWindowWin32.constructor()");
        mInstance   = aContext->mInstance;
        mWinName    = aContext->mWinClassName;
        mParent     = (int)aContext->mWindow;
        mWinCursor  = LoadCursor(NULL,IDC_ARROW);
        mPrevCursor = NULL;
        //wtrace(mWinName.ptr());
        mClickedButton = bu_None;

        // --- register window class ---

        //http://msdn.microsoft.com/en-us/library/ms633586%28v=VS.85%29.aspx
        //RegisterClass:
        //  All window classes that an application registers are unregistered when it terminates.
        //  Windows NT/2000/XP: No window classes registered by a DLL are unregistered when the DLL is unloaded.
        //  A DLL must explicitly unregister its classes when it is unloaded.

        // multiple instances:
        // what happens when we try to register a window with similar name
        // and shouldn't we unregister it?
        // (or is it done automatically when dll is unloaded?)

        char* classname = mWinName.ptr();//(char*)"axonlib";
        //wtrace("window class name:" << classname);
        WNDCLASS wc;
        memset(&wc,0,sizeof(wc));
        wc.style          = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc    = &eventProc;
        wc.hInstance      = mInstance;
        wc.lpszClassName  = classname;
        wc.hCursor        = (HICON)mWinCursor;//LoadCursor(NULL, IDC_ARROW);
        RegisterClass(&wc);

        //RECT rc = {mRect.x,mRect.y,mRect.x2(),mRect.y2()};
        RECT rc = { mRect.x, mRect.y, mRect.w, mRect.h };

        // --- embedded ---

        if (mWinFlags&AX_WIN_EMBEDDED)
        {
          AdjustWindowRect(&rc,WS_POPUP,FALSE);
          mWindow = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            classname,
            0,
            WS_POPUP,
            //mRect.x,mRect.y,
            //mRect.w,mRect.h,
            rc.left, rc.top,
            rc.right-rc.left+1, rc.bottom-rc.top+1,
            0,
            0,
            mInstance,
            0
          );
          reparent(mParent);
        } //embedded

        // --- windowed ---

        else
        {
          AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW,FALSE);
          mWindow = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
            classname,                // lpClassName
            0,                        // lpWindowName
            WS_OVERLAPPEDWINDOW,      // dwStyle
            //mRect.x,mRect.y,          // x,y
            //mRect.w,mRect.h,          // w,h
            rc.left, rc.top,
            rc.right-rc.left+1, rc.bottom-rc.top+1,
            0,                        // hWndParent
            0,                        // hMenu
            mInstance,                // hInstance
            0                         // lpParam
          );
        }

        // ---

        SetWindowLong(mWindow,GWL_USERDATA,(int)this);
        //DragAcceptFiles(mWindow,true);

        // --- canvas ---

        //axContext ctx((int)mWindow);
        //mCanvas = new axCanvas(&ctx);
        mCanvas = createCanvas();

        // --- surface ---

        if (aWinFlags & AX_WIN_BUFFERED)
        {
          //mSurface = new axSurface(aContext,mRect.w,mRect.h);
          mSurface = createSurface(mRect.w,mRect.h);
        }

      }

    //----------------------------------------

    ~axWindowWin32()
      {
        DestroyWindow(mWindow);
        // can this be dangerous if moutlple plugin instances uses the plugin?
        // or is there some reference-counting going on?

        //http://msdn.microsoft.com/en-us/library/ms644899%28v=VS.85%29.aspx
        //UnregisterClass:
        //  If the function succeeds, the return value is nonzero.
        //  If the class could not be found or if a window still exists
        //  that was created with the class, the return value is zero

        // unregister window?
        // what if multiple instances is using the same window?
        UnregisterClass( mWinName.ptr(), mInstance);
      }

    //----------------------------------------

    //virtual int getHandle(void) { return (int)mWindow; }

    virtual axCanvas* createCanvas(void)
      {
        axContext ctx(mWindow);
        return new axCanvas(&ctx);
      }

    virtual axSurface* createSurface(int aWidth, int aHeight)
      {
        //axContext ctx(mParent);
        axContext ctx(mWindow);
        return new axSurface(&ctx,aWidth,aHeight);
      }

    virtual axBitmap* createBitmap(int aWidth, int aHeight)
      {
        axContext ctx(mWindow);
        return new axBitmap(&ctx,aWidth,aHeight);
      }


    //----------------------------------------
    // low level
    //----------------------------------------

    virtual void flush(void)
      {
        //GdiFlush();
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

    virtual void show(void)
      {
        SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
      }

    //----------

    virtual void hide(void)
      {
        ShowWindow(mWindow,SW_HIDE);
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        SetWindowPos(mWindow,0,aX,aY,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
      }

    //----------

    virtual void setSize(int aWidth, int aHeight)
      {
        SetWindowPos(mWindow,HWND_TOP,0,0,aWidth,aHeight,SWP_NOMOVE);
        //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
      }

    //------------------------------

      //internal
      void getWindowSize(HWND pWnd, int* pW, int* pH)
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

      //----------

      //internal
      bool isChildWindow(HWND pWnd)
        {
          if (pWnd)
          {
            int style = GetWindowLong(pWnd, GWL_STYLE);
            int exStyle = GetWindowLong(pWnd, GWL_EXSTYLE);
            return ((style & WS_CHILD) && !(exStyle & WS_EX_MDICHILD));
          }
          return false;
        }

    //------------------------------

    #define SETPOS_FLAGS SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE

    virtual void setParentSize(int aWidth, int aHeight)
      {
        int dw = aWidth - mRect.w, dh = aHeight - mRect.h;
        HWND pParent = 0, pGrandparent = 0;
        int w=0, h=0, parentW=0, parentH=0, grandparentW=0, grandparentH=0;
        getWindowSize(mWindow,&w,&h);
        if (isChildWindow(mWindow))
        {
          pParent = GetParent(mWindow);
          getWindowSize(pParent, &parentW, &parentH);
          if (isChildWindow(pParent))
          {
            pGrandparent = GetParent(pParent);
            getWindowSize(pGrandparent, &grandparentW, &grandparentH);
          }
        }
        SetWindowPos(mWindow, 0, 0, 0, w + dw, h + dh, SETPOS_FLAGS);
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

    virtual void setTitle(axString aTitle)
      {
      }

    //----------

    virtual void reparent(int aParent)
      {
        mParent = aParent;
        SetWindowLong(mWindow,GWL_STYLE,(GetWindowLong(mWindow,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
        SetParent(mWindow, (HWND)aParent);
      }

    //virtual void resetCursor(void)
    //  {
    //  }

    //----------

    virtual void setCursor(int aCursor)
      {
        if (aCursor<0)
        {
          hideCursor();// aCursor = DEF_CURSOR;
          mPrevCursor = aCursor;
        } //-1
        else
        {
          if (mPrevCursor<0) showCursor();
          if (aCursor!=mPrevCursor)
          {
            mWinCursor = LoadCursor(NULL,(char*)aCursor);
            SetCursor( mWinCursor );
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
        SetCapture(mWindow);
      }

    //----------

    virtual void releaseCursor(void)
      {
        ReleaseCapture();
      }

    //----------------------------------------
    // medium level
    //----------------------------------------

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
        InvalidateRect(mWindow,&R,false);
        //UpdateWindow(mWindow);
      }

    //----------

    virtual void resizeBuffer(int aWidth, int aHeight)
      {
        //if( aWidth!=mRect.w || aHeight!=mRect.h )
        //{
          if (mWinFlags&AX_WIN_BUFFERED)
          {
            //mSurfaceMutex.lock();
            axSurface* srf;
            if (mSurface)
            {
              srf = mSurface;
              mSurface = NULL;
              delete srf;
            }
            //srf = new axSurface(aWidth,aHeight/*,mWinFlags*/);
            srf = createSurface(aWidth,aHeight);
            mSurface = srf;
            //mSurfaceMutex.unlock();
          }
          //mRect.w = aWidth;
          //mRect.h = aHeight;
          //doResize(aWidth,aHeight);
        //} //newsize
      }

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
        //wtrace("axWindowWin32.beginPaint");
        /*mPaintDC = */BeginPaint(mWindow,&mPS);
      }

    //----------

    virtual void endPaint(void)
      {
        EndPaint(mWindow,&mPS);
        //wtrace("axWindowWin32.endPaint");
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void startTimer(int ms)
      {
        /*mTimer = */SetTimer(mWindow,667,ms,NULL/*timerProc*/);
      }

    //----------

    virtual void stopTimer(void)
      {
        KillTimer(mWindow,667);
      }

    //----------------------------------------
    // events
    //----------------------------------------

    virtual void sendEvent(unsigned int aValue=0)
    {
    }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void eventLoop(void)
      {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
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
        int result = bu_None;
        if (aKey & MK_SHIFT) result |= bu_Shift;
        if (aKey & MK_CONTROL) result |= bu_Ctrl;
        if (GetKeyState(VK_MENU) < 0) result |= bu_Alt;
        return result;
      }

      //------------------------------

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
        //MSG msg2;
        //HDC dc;
        LRESULT result = 0;
        int btn = 0;
        axRect rc;
        int w,h;

        //TRACE("win32 eventHandler. msg=%x\n",message);

        switch (message)
        {
          case WM_GETDLGCODE:
            result = DLGC_WANTALLKEYS;
            break;
          case WM_PAINT:
            //wtrace("WM_PAINT\n");
            beginPaint();
            rc = axRect(  mPS.rcPaint.left,
                          mPS.rcPaint.top,
                          mPS.rcPaint.right -  mPS.rcPaint.left + 2,
                          mPS.rcPaint.bottom - mPS.rcPaint.top  + 2);
            //mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            if ((mWinFlags&AX_WIN_BUFFERED) && mSurface )
            {
              //mSurfaceMutex.lock();
              axCanvas* can = mSurface->getCanvas();
              can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(can,rc);
              //mCanvas->blit( can, rc.x,rc.y, rc.x,rc.y,rc.w,rc.h );
              mCanvas->drawImage(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              can->clearClipRect();
              //mSurfaceMutex.unlock();
            }
            else
            {
              // http://msdn.microsoft.com/en-us/library/dd183439%28VS.85%29.aspx
              // if you obtain a device context handle from the  BeginPaint function,
              // the DC contains a predefined rectangular clipping region that corresponds
              // to the invalid rectangle that requires repainting
              mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(mCanvas,rc);
              mCanvas->clearClipRect();
            }
            //mCanvas->clearClipRect();
            endPaint();
            break;
          case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
            switch (message)
            {
              case WM_LBUTTONDOWN: btn = bu_Left;   break;
              case WM_MBUTTONDOWN: btn = bu_Middle; break;
              case WM_RBUTTONDOWN: btn = bu_Right;  break;
            }
            mClickedButton = btn;
            doMouseDown(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            if( mCapturedWidget ) grabCursor();
            break;
          case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
            switch (message)
            {
              case WM_LBUTTONUP: btn = bu_Left;   break;
              case WM_MBUTTONUP: btn = bu_Middle; break;
              case WM_RBUTTONUP: btn = bu_Right;  break;
            }
            mClickedButton = bu_None;
            doMouseUp(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            if( !mCapturedWidget ) releaseCursor();
            break;
          case WM_MOUSEMOVE:
            doMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), mClickedButton | remapKey(wParam));
            //return 1;
            break;
          //case WM_CHAR:
          //  doKeyChar(wParam,lParam);
          //  break;
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
            //if ( PeekMessage(&msg2,mWindow,WM_SIZE,WM_SIZE,PM_NOREMOVE) )
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
            if (!(mWinFlags&AX_WIN_EMBEDDED))
            {
              //trace("Quit");
              PostQuitMessage(0);
            }
            break;
          case WM_TIMER:
            if (wParam==667)
            {
              //wtrace("axWIndowWin32.eventHandler :: timer");
              doTimer();
            }
            result = 0;
            break;
          case WM_SETCURSOR:
            //wtrace("WM_SETCURSOR");
            SetCursor(mWinCursor);
            break;
          default:
            result = DefWindowProc(hWnd,message,wParam,lParam);
        }
        return result;
      }

};

//----------------------------------------------------------------------
//
// eventProc
//
//----------------------------------------------------------------------

LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  axWindowWin32* wnd = (axWindowWin32*)GetWindowLong(hWnd,GWL_USERDATA);
	if (wnd==0) return DefWindowProc(hWnd,message,wParam,lParam);
  return wnd->eventHandler(hWnd, message, wParam, lParam);
}
//----------------------------------------------------------------------

typedef axWindowWin32 axWindowImpl;

//----------------------------------------------------------------------
#endif
