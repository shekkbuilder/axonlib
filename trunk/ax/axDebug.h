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
 * @file axDebug.h
 * \brief debugger methods
 *
 * if AX_DEBUG is defined, 'assert.h' and 'iostream' are included for debugging purposes. <br><br>
 * assert() examples:
 * \code
 * assert(var);   // if 'var' is false the compiler will trow an error and abort
 * assert(num>5); // same as above but if 'num' is not greater than 5
 * \endcode
 * warn() examples:
 * \code
 * warn("message"); // show a warning but does not break
 * \endcode
 * trace() examples:
 * \code
 * trace(var);              // trace a variable
 * trace("message");        // trace a message
 * trace(var << "message"); // combined
 * \endcode
 */

#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

#ifdef AX_DEBUG
  #include <assert.h>
  #include <iostream>
  using namespace std;
  #define warn(x) { printf("WARN | LINE: %i | %s\n", __LINE__, x); }
  #define trace(x) { cout << "TRC | LINE: " << __LINE__ << " | " << x << endl; cout.flush(); }

  #ifdef WIN32
    /**
     * # creates a winapi debugger window
     */
    #include <windows.h>
    #include <sstream>
    #include <stdarg.h>
    // ----------------
    using namespace std;
    const unsigned int axDtext_id = 0x3039;
    const unsigned int axDwinW = 500;
    const unsigned int axDwinH = 300;
    ostringstream oss;
    HWND axDdialog;
    HWND axDtext;
    MSG axDmsg;
    bool axDfirstopen = false;
    // ----------------
    /// message listener:
    bool WINAPI axDmsglistner(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
      if(message == WM_CLOSE)
      {
        PostQuitMessage(0);
        /*
        LPCTSTR text = "# history cleared\r\n";
        SendMessage(axDtext, EM_SETSEL, 0, -1);
        SendMessage(axDtext, EM_REPLACESEL, (WPARAM)true, (LPARAM)text);
        */
      }
      return false;
    }
    /// create window after first call
    void axDwin_create(void)
    {
      if (!axDfirstopen)
      {
        /// attach parent window
        axDdialog = CreateWindowEx
        (
        	0, WC_DIALOG, "axDebug", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|
          WS_MINIMIZEBOX|WS_EX_TOPMOST|WS_EX_STATICEDGE,
        	400, 400, axDwinW, axDwinH, NULL, NULL, NULL, NULL
        );
        /// create edit control
        axDtext = CreateWindow
        (
          "edit", "# init\r\n", WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|
          WS_HSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
          0, 0, axDwinW-5, axDwinH-25, axDdialog, (HMENU)axDtext_id, NULL, NULL
        );
        /// attach listener
        SetWindowLong(axDdialog, DWL_DLGPROC, (long)axDmsglistner);
        while(GetMessage(&axDmsg,NULL,0,0))
        {
        	TranslateMessage(&axDmsg);
        	DispatchMessage(&axDmsg);
        }
      axDfirstopen = false;
      }
    }
    // ----------------
    /** # usage:
     * debugw("myvar = ", myvar);
     * debugw(myvar, " <- message");
     * debugw("message", "");
     * debugw(myvar, "");
     */
    template <typename T0, typename T1>
    void wdebug(const T0 p0, const T1 p1)
    {
      oss << p0 << p1 << "\r\n";
      string s2 = oss.str();
      const char *string = s2.c_str();
      LPCTSTR text = (LPCTSTR)string;
      oss.str().clear();
      int nLength = GetWindowTextLength(axDtext);
      SendMessage(axDtext, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
      SendMessage(axDtext, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)text);
    }
    // ----------------
    // where to put

  #else
    //#define wdebug(x,x) ((void)0)
    #define wdebug(x,y) ((void)0)
  #endif
#else
  #define NDEBUG
  #define trace(x) ((void)0)
  #define warn(x) ((void)0)
  #define assert(x) ((void)0)
  #define wdebug(x,x) ((void)0)
#endif

//----------------------------------------------------------------------
#endif
