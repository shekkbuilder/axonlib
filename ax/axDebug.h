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
 * if AX_DEBUG is defined, 'assert.h' and 'iostream' are included for
 * debugging purposes. <br><br>
 * assert() examples:
 * \code
 * assert(var);             // if 'var' is false the compiler will throw an error and abort
 * assert(num>5);           // same as above but if 'num' is not greater than 5
 * \endcode
 * warn() examples:
 * \code
 * warn("message");         // show a warning but does not break
 * \endcode
 * trace() examples:
 * \code
 * trace(var);              // trace a variable
 * trace("message");        // trace a message
 * trace(var << "message"); // combined
 * \endcode
 * wdebug() (win32) examples:
 * \code
 * // minimum 2 parameters of different or same type
 * wdebug("var = ", myvar);
 * wdebug(var1, var2);
 * wdebug(var, " <- text message");
 * wdebug("text", "more text", false, false); // "don't show line number", "no new line"
 * wdebug(var, "");
 * wdebug("", var);
 * \endcode
 * <br>
 * win32 specific methods: <br>
 * -------------------------------- <br>
 * on windows you should manually create a debug window (console or gui) and
 * destroy it when the plugin is beeing closed.
 * <br>
 * <br>
 * <b>gui window (inefficient):</b> <br>
 * axDwinCreate() - create a winapi debug window <br>
 * axDwinDestroy() - destroy the winapi debug window  <br>
 * wdebug() is a method which is used to append text into the gui debug
 * window. <br>
 * not recommended to be called per sample or block on older machines
 * (possible freeze). <br>
 * <br>
 * <b>console window (efficient):</b> <br>
 * uses the standart trace(), warn() macros. <br>
 * axDstdCreate() - create a debug console and route stdout to it <br>
 * axDstdDestroy() - destroy the debug console <br>
 * <br>
 * example in a vst plugin: <br>
 * \code
 * virtual void doProcessState(int aState) // overwride default method
    {
      #if defined(AX_DEBUG) && defined(WIN32) // check for win32 and ax_debug
        switch(aState)
        {
          case pst_Open:                                // - on plugin open
            axDstdCreate();                             // create console
            axDwinCreate();                             // create debug gui
            trace("axLogf(2.24)=" << axLogf(2.24));     // test trace()
            wdebug("axLogf(2.24)=", axLogf(2.24));      // test wdebug()
          break;
          case pst_Close:                               // - on plugin close
            axDstdDestroy();                            // destroy console
            axDwinDestroy();                            // destroy debug gui
          break;
        }
      #endif
    }
 * \endcode
*/

#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

// case: debug enabled
#ifdef AX_DEBUG
  #include <assert.h>
  #include <iostream>
  using namespace std;
  // trace() & warn() defined bellow

  // case: windows
  #ifdef WIN32
    /**
     * creates a winapi debugger window (unsafe / slow)
     *
    NOTES:
    much more stable and useable version. works under reaper3, enrgyxt2 and vstpa.
    #issues:
    - the debugger window has to be controlled from a plugin with doProcessState(..)
    best case for users will be to only include axDebug.h, define AX_DEBUG
    and be ready to call wdebug(..)
    */
    #define _WIN32_WINNT 0x0501   // add this before window.h to use GetConsoleWindow()
    #include <windows.h>
    #include <windowsx.h>         // macros
    #include <io.h>
    #include <fcntl.h>            // for _O_TEXT
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
    bool WINAPI axDwinListner(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        SetWindowLong(axDtext, DWL_DLGPROC, (long)axDwinListner);
        // - set limit
        Edit_LimitText(axDtext, axDtextLimit);
      }
    }

    // ----------------
    // send text to debug window
    template <typename T0, typename T1>
    void wdebug(const T0 p0, const T1 p1, bool lineN = true, bool newline = true)
    {
      if (axDtext != NULL) // if window is created
      {
        // use a string stream to cast input vars to type std::string
        ostringstream oss;
        if (lineN) oss << __LINE__ << " | ";
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

    // allocate console and route stdout as seen in example:
    // http://support.microsoft.com/kb/105305
    // --------------------------------------------------------
    unsigned int hCrt = 0;                  // crt handle
    FILE *sfile;                            // file stream

    // ----------------
    // destroy console
    void axDstdDestroy(void)
    {
      FreeConsole();
      close((int)sfile);
      hCrt = 0;
    }

    // ----------------
    // create console
    void axDstdCreate(void)
    {
      if (hCrt == 0)
      {
        // allocate console
        AllocConsole();
        // set title
        SetConsoleTitle("axDebug");
        // get handle for console
        // requires _WIN32_WINNT >= 0x0500 (see above, before windows.h)
        HWND hCw = GetConsoleWindow();
        if(hCw != NULL)
	      {
          // set always on top
          SetWindowPos(hCw, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
          // disable close button (otherwise crash if clicked)
		      HMENU hMenu = GetSystemMenu(hCw, 0);
		      if(hMenu != NULL)
		      {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DrawMenuBar(hCw);
		      }
        }
        // get std handle for text output
        hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        // open stream
        sfile = _fdopen(hCrt, "w");
        // link stdout to stream
        *stdout = *sfile;
        // set buffer length to 0
        setvbuf(stdout, NULL, _IONBF, 0);
      }
      // define trace() and warn() to check for hCrt;
      #define trace(x) { if (hCrt != 0) { cout << "TRC | " << __LINE__ << " | " << x << endl; cout.flush(); } }
      #define warn(x) { if (hCrt != 0) { printf("WARN | %i | %s\n", __LINE__, x); } }
    }

  #endif
  // case: linux
  #ifdef linux
    #define trace(x) { cout << "TRC | " << __LINE__ << " | " << x << endl; cout.flush(); }
    #define warn(x) { printf("WARN | %i | %s\n", __LINE__, x); }
    #define wdebug(...) ((void)0)
  #endif
// case: no debug
#else
  #define NDEBUG
  #define trace(x) ((void)0)
  #define warn(x) ((void)0)
  #define assert(x) ((void)0)
  #define wdebug(...) ((void)0)
#endif

//----------------------------------------------------------------------
#endif
