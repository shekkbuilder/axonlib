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
 * wdebug() is a method which uses the win32api to draw a text window. 
 * very inefficient to be called per sample on older machines (possible freeze). <br>
 * examples:
 * \code
 * // minimum 2 parameters of different or same type 
 * wdebug("var = ", myvar);
 * wdebug(var1, var2);
 * wdebug(var, " <- text message");
 * wdebug("text", "more text", false); // no new line
 * wdebug(var, "");
 * wdebug("", var);
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
     * creates a winapi debugger window (unsafe)
     */

    /*
    NOTES:
    working under reaper, vstpa. much more stable and useable version.
    #issues:
    - crashes energyxt with c0000005 (access violation) in:
    "..!ZN8myPlugin15doProcessSampleEPPfS1_"
    - plugin instances share a axDebug window and crash the host if multiple windows are created
    and then all plugins are unloaded (case: reaper). 
    - the debugger window has to be controlled from a plugin with doProcessState(..)
    best case for users will be to only include axDebug.h, define AX_DEBUG
    and be ready to call wdebug(..)
    */    
    #include <windows.h>
    #include <windowsx.h>
    #include <sstream>
    // ----------------
    using namespace std;
    const unsigned int axDtext_id = 0x3039; // text window id
    const unsigned int axDwinW = 500;       // debug win width
    const unsigned int axDwinH = 300;       // debug win height
    const unsigned int axDtextLimit = 1000; // define maximum text length
    HWND axDdialog;                         // parent window handle
    HWND axDtext;                           // edit control handle
    bool axDwopen = false;                  
    // ----------------
    // destroy window
    void axDwinDestroy(void) { PostQuitMessage(0); DestroyWindow(axDdialog); axDwopen = false; }
    // ----------------
    // message listener
    bool WINAPI axDmsglistner(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
      // destroy
      if (message == WM_DESTROY || message == WM_CLOSE) { axDwinDestroy(); }      
      return false;
    }
    // ----------------
    // create window
    void axDwinCreate(void)
    {
      // attach parent window
      axDdialog = CreateWindowEx
      (
      	0, WC_DIALOG, "axDebug", WM_KILLFOCUS|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|
        WS_MINIMIZEBOX|WS_EX_TOPMOST|WS_VISIBLE|WS_EX_STATICEDGE,
      	400, 400, axDwinW, axDwinH, NULL, NULL, NULL, NULL
      );
      // set always on top
      SetWindowPos(axDdialog, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
      // create edit control
      axDtext = CreateWindow
      (
        "edit", "# init\r\n", WM_KILLFOCUS|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|
        WS_HSCROLL|ES_READONLY|ES_LOWERCASE|ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|
        ES_AUTOVSCROLL, 0, 0, axDwinW-5, axDwinH-25, axDdialog, (HMENU)axDtext_id,
        NULL, NULL
      );
      // attach listener to parent window
      SetWindowLong(axDdialog, DWL_DLGPROC, (long)axDmsglistner);
      // limit text in edit control
      Edit_LimitText(axDtext, axDtextLimit);
      // set flag: window is created
      axDwopen = true;
    }
    // ----------------
    /** send text to debug window (inefficient).      
     */
    template <typename T0, typename T1>
    void wdebug(const T0 p0, const T1 p1, bool newline = true)
    {
      if (axDwopen) // if window is created
      {
        // use a string stream to cast input vars to type std::string
        ostringstream oss;
        oss << p0 << " " << p1;
        if (newline) oss << "\r\n";
        string s2 = oss.str();
        
        // cast std::string to char* then to LPCTSTR (win32)
        const char* string = s2.c_str();
        LPCTSTR text = (LPCTSTR)string;

        // get length of new string & old text
        int str_len = s2.length();
        int len = Edit_GetTextLength(axDtext);

        // clear a portion (str_len*2) of text if too much already exist
        if ((len + str_len) >= axDtextLimit)
        {
          Edit_SetSel(axDtext, 0, str_len << 1);
          Edit_ReplaceSel(axDtext, "");
          len = Edit_GetTextLength(axDtext);
        }

        // append the string at the end of the entire text
        Edit_SetSel(axDtext, len, len);
        Edit_ReplaceSel(axDtext, text);
      }
    }
    // ----------------
  #else
    #define wdebug(x,y) ((void)0)
  #endif
#else
  #define NDEBUG
  #define trace(x) ((void)0)
  #define warn(x) ((void)0)
  #define assert(x) ((void)0)
  #define wdebug(x,y) ((void)0)
#endif

//----------------------------------------------------------------------
#endif
