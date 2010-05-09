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
 * msg() examples:
 * \code
 * msg("message");         // show a message/warning
 * \endcode
 * trace() examples:
 * \code
 * trace(var);              // trace a variable
 * trace("message");        // trace a message
 * trace(var << "message"); // combined
 * \endcode
 * wtrace() (linux/wine) example:
 * \code
 * wtrace(var);              // trace a variable
 * wtrace("message");        // trace a message
 * wtrace(var << "message"); // combined
 * \endcode
 * wdebug() (win32) examples:
 * wdebug(var);              // trace a variable
 * wdebug("message");        // trace a message
 * wdebug(var << "message"); // combined
 * \endcode
 * <br>
 * linux specific methods: <br>
 * -------------------------------- <br>
 * wtrace() - trace for wine <br>
 *
 * win32 specific methods: <br>
 * -------------------------------- <br>
 * on windows you can manually create a debug window (console or gui) and
 * destroy it when the dll/exe is beeing closed or you can use
 * the flags AX_DEBUG_AUTO_STD (auto create/destroy debug console)
 * or AX_DEBUG_AUTO_WIN (auto create/destroy debug gui window).
 * <br>
 * <br>
 * <b>winapi window (inefficient):</b> <br>
 * axDwinCreate() - create a winapi debug window <br>
 * axDwinDestroy() - destroy the winapi debug window  <br>
 * wdebug() is a method which is used to append text into the gui debug
 * window. <br>
 * not recommended to be called per sample or block on older machines
 * (possible freeze). <br>
 * <br>
 * <b>console window (efficient):</b> <br>
 * uses the standard trace(), msg() macros. <br>
 * axDstdCreate() - create a debug console and route stdout to it <br>
 * axDstdDestroy() - destroy the debug console <br>
 * <br>
 * example in a vst plugin: <br>
 * \code
 * // automatic
 * #define AX_DEBUG           // or call from command line: g++ ... -DAX_DEBUG
 * #define AX_DEBUG_AUTO_WIN
 * #define AX_DEBUG_AUTO_STD
 *
 * // manual
 * virtual void doProcessState(int aState) // overwride default method
    {
      #if defined(AX_DEBUG) && defined(WIN32) // check for win32 and ax_debug
        switch(aState)
        {
          case pst_Open:                                // - on plugin open
            axDstdCreate();                             // create console
            axDwinCreate();                             // create debug gui
            trace("axLogf(2.24)=" << axLogf(2.24));     // test trace()
            wdebug("axLogf(2.24)=" << axLogf(2.24));    // test wdebug()
          break;
          case pst_Close:                               // - on plugin close
            axDstdDestroy();                            // destroy console
            axDwinDestroy();                            // destroy debug gui
          break;
        }
      #endif
    }
 * \endcode
 * <i>notes: debug windows are shared between plugin instances and
 * only one console can be allocated per process.<i><br><br>
*/

#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

// case: debug enabled
#ifdef AX_DEBUG

  #define _WIN32_WINNT 0x0501

  #include <assert.h>
  #include <iostream>
  #include "core/axUtils.h"
  using namespace std;

  // case: windows
  #ifdef WIN32
    /*
     * creates a winapi debugger window (slow)
     *
    */
    #include <windows.h>
    #include <windowsx.h>         // macros
    #include <io.h>
    #include <stdio.h>            // gcc-4.4.1-tdm
    #include <sstream>

    HWND axDtext;                 // edit control handle
    ostringstream axDoss;         // string stream for window

    // ----------------
    // destroy window
    #define axDwinDestroy(void) \
    { \
      DestroyWindow(axDtext); axDtext = NULL; \
      axDoss.flush(); \
    }

    // ----------------
    // message listener
    bool WINAPI axDwinListner(HWND hwnd, UINT message)
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
          400, 400, 440-5, 320-25, NULL, NULL, NULL, NULL
        );
        //set window properties
        // - on top
        SetWindowPos(axDtext, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
        // - default win32 font
        HFONT hfDefault;
        hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(axDtext, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
        //icon
        HICON hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), "axicon");
        if (hIcon) SendMessage(axDtext, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        // - listener
        SetWindowLong(axDtext, DWL_DLGPROC, (long)axDwinListner);
        // - set limit
        Edit_LimitText(axDtext, 1000);
      }
    }

    // -------------------
    // window debug write
    inline void wdebug_write(void)
    {
        // cast stream to string
        const string s2 = axDoss.str();
        // clear
        axDoss.str("");
        axDoss.flush();
        // cast std::string to char* then to LPCTSTR (win32)
        const char* string = s2.c_str();
        const LPCTSTR text = (LPCTSTR)string;
        // get length of new string & old text
        const int str_len = s2.length();
        unsigned int len = Edit_GetTextLength(axDtext);
        // clear a portion (str_len*2) of text if too much already exist
        if ((len + str_len) >= 1000)
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

    // -------------------
    // window debug macro
    #define wdebug(x) \
      if (axDtext != NULL) \
      { \
        axDoss << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << "\r\n"; \
        wdebug_write(); \
      }

    /*
     * create a console debugger window (only one instance per process, fast)
     * allocates console and routes stdout as seen in example:
     * http://support.microsoft.com/kb/105305
     *
     */
    unsigned int axHcrt = 0;                  // crt handle
    FILE *axSfile;                            // file stream

    // ----------------
    // destroy console
    #define axDstdDestroy(void) \
    { \
      FreeConsole(); \
      close((int)axSfile); \
      axHcrt = 0; \
    }

    // ----------------
    // create console
    void axDstdCreate(void)
    {
      if (axHcrt == 0)
      {
        // allocate console
        AllocConsole();
        // set title
        SetConsoleTitle("axDebug");
        // get handle for console
        // requires _WIN32_WINNT >= 0x0500 (set before windows.h)
        HWND hCw = GetConsoleWindow();
        if(hCw != NULL)
	      {
          // ENABLE_EXTENDED_FLAGS = 0x0080
          // ENABLE_QUICK_EDIT_MODE = 0x0040
          HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
          SetConsoleMode(hIn, 0x0080|0x0040);
          // size & color
          HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
          //// SetConsoleTextAttribute(hOut, 15);
          SMALL_RECT cDim = {0, 0, 50, 20};
          SetConsoleWindowInfo(hOut, true, &cDim);
          // disable ctrl+c
          SetConsoleCtrlHandler(NULL, true);
          // set always on top
          SetWindowPos(hCw, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
          // disable close button
		      HMENU hMenu = GetSystemMenu(hCw, 0);
		      if(hMenu != NULL)
		      {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DrawMenuBar(hCw);
		      }
        }
        // get std handle for text output, _O_TEXT = 0x4000
        axHcrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
        // open stream
        axSfile = _fdopen(axHcrt, "w");
        // link stdout to stream
        *stdout = *axSfile;
        // set buffer length to 0
        setvbuf(stdout, NULL, _IONBF, 0);
      }
    }

    // --------------
    #define trace(x) { if (axHcrt != 0) { cout << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; cout.flush(); } }
    #define msg(x) { if (axHcrt != 0) { printf("[%s|%ui] %s\n", axGetFileName(__FILE__), __LINE__, x); } }
    #define wtrace(x) { cout << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; cout.flush(); }
    // ---------
  #endif

  // case: linux
  #ifdef linux
    #define trace(x) { cout << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; cout.flush(); }
    #define msg(x) { printf("[%s|%ui] %s\n", axGetFileName(__FILE__), __LINE__, x); }
    #define wtrace(x) { cout << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; cout.flush(); }
    #define wdebug(x) ((void)0)
    inline void axDstdCreate(void) {}
    inline void axDstdDestroy(void) {}
    inline void axDwinCreate(void) {}
    inline void axDwinDestroy(void) {}
  #endif

// case: no debug
#else
  #define NDEBUG
  #define trace(x) ((void)0)
  #define msg(x) ((void)0)
  #define assert(x) ((void)0)
  #define wtrace(x) ((void)0)
  #define wdebug(x) ((void)0)
  inline void axDstdCreate(void) {}
  inline void axDstdDestroy(void) {}
  inline void axDwinCreate(void) {}
  inline void axDwinDestroy(void) {}
#endif

//----------------------------------------------------------------------
#endif

/*

// from: http://www.skynet.ie/~caolan/Fragments/C++StaticAssert.html
// google search term: "static assert"
//
//C++ Static Assert
// StaticAssert is like assert for C++, except that the test is done at compile time.
// Of course this can only be done when the test is possible at compile time.
//
// const char *stiName = { "Normal" };
// StaticAssert((sizeof(stiName) / sizeof(stiName)) == 100, WrongSizeOfArray);
//
// Here the array is one element in size, and we have a StaticAssert that will fail
// if the array is not 100 in size with an error of WrongSizeOfArray.
//
// ----------
//
// Lifted direct from:
// Modern C++ Design: Generic Programming and Design Patterns Applied
// Section 2.1
// by Andrei Alexandrescu

namespace ww // change this? (ccernn)
{
  template<bool> class compile_time_check
    {
      public:
        compile_time_check(...) {}
    };
  template<> class compile_time_check<false>
    {
    };
}

// Similiar to assert, StaticAssert is only in operation when NDEBUG is not
// defined. It will test its first argument at compile time and on failure
// report the error message of the second argument, which must be a valid c++
// classname. i.e. no spaces, punctuation or reserved keywords.

#ifndef NDEBUG // todo: if AX_DEBUG? (ccernn)
#define StaticAssert(test, errormsg) \
do \
{ \
  struct ERROR_##errormsg {}; \
  typedef ww::compile_time_check< (test)!=0 > tmplimpl; \
  tmplimpl aTemp = tmplimpl(ERROR_##errormsg()); \
  sizeof(aTemp); \
} while (0)
#else
#define StaticAssert(test, errormsg) \
  do {} while (0)
#endif
#endif

*/
