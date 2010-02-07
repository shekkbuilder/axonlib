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
    const unsigned int axDwinW = 500;       // debug win width
    const unsigned int axDwinH = 300;       // debug win height
    const unsigned int axDtextLimit = 1000; // define maximum text length
    HWND axDtext;                           // edit control handle
           
    // ----------------
    // destroy window
    void axDwinDestroy(void) { DestroyWindow(axDtext); axDtext = NULL; }
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
      if (axDtext == NULL)
      {
      // attach window
      axDtext = CreateWindowEx
      (
        0, "EDIT", "axDebug\r\n",
        WS_SYSMENU|WS_MINIMIZEBOX|WS_POPUPWINDOW|WS_CAPTION|              
        WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
        ES_READONLY|ES_MULTILINE|ES_WANTRETURN|WS_SIZEBOX|
        ES_AUTOHSCROLL|ES_AUTOVSCROLL,
        400, 400, axDwinW-5, axDwinH-25, NULL, NULL, NULL, NULL
      );
      //set window properties
      // - on top
      SetWindowPos(axDtext, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
      // - default win32 font
      HFONT hfDefault;
      hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);      
      SendMessage(axDtext, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
      // - listener      
      SetWindowLong(axDtext, DWL_DLGPROC, (long)axDmsglistner);
      // - set limit
      Edit_LimitText(axDtext, axDtextLimit);
      
      }
    }
    // ----------------
    /** send text to debug window (inefficient).      
     */
    template <typename T0, typename T1>
    void wdebug(const T0 p0, const T1 p1, bool newline = true)
    {
      if (axDtext != NULL) // if window is created
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
        
        // kill focus
        SendMessage(axDtext, WM_KILLFOCUS, 0, 0);
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
