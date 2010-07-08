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

#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

// get file name from a path string
#include "axStdlib.h"
inline const char* axGetFileName(const char* path)
{
  if (path)
  {
    const char *slash, *backslash;
    slash = axStrrchr(path, '/');
    backslash = axStrrchr(path, '\\') + 1;
    if (slash)
      return slash + 1;
    else if (backslash)
      return backslash;
  }
  return "NULL";
}

// case: debug enabled
#ifdef AX_DEBUG

  #include <iostream>
  #include <fstream>
  using namespace std;

  // printf / cout / fstream / macros
  #define axDprintf(x) \
    printf("[%s|%ui] %s\n", axGetFileName(__FILE__), __LINE__, x)
  #define axDcout(x) \
    cout << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; \
    cout.flush()
  #define _axDcout(x) \
    cout << x << endl; \
    cout.flush()

  // log file
  #ifdef AX_DEBUG_LOG
    fstream axDlog;
    #define axDfstream(x) \
      axDlog << "[" << axGetFileName(__FILE__) << "|" << __LINE__ << "] " << x << endl; \
      axDlog.flush();
    #define _axDfstream(x) \
      axDlog << x << "\n"; \
      axDlog.flush();
  #else
    static __thread bool axDlog;
    #define axDfstream(x) (void(0))
    #define _axDfstream(x) (void(0))
  #endif

  // case: windows
  #ifdef AX_WIN32
  
    #define _WIN32_WINNT 0x0501
    #include <windows.h>
    #include <io.h>
    #include <stdio.h>            // gcc-4.4.1-tdm

    /*
     * create a console debugger window (only one instance per process, fast)
     * allocates console and routes stdout as seen in example:
     * http://support.microsoft.com/kb/105305
     *
     */
    static __thread unsigned int  axHcrt = 0;          // crt handle
    static __thread FILE*         axSfile;             // file stream

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
      if (!axHcrt)
      {
        // allocate console
        AllocConsole();
        // set title
        SetConsoleTitle("axDebug");
        // get handle for console
        HWND hCw = GetConsoleWindow();
        if(hCw)
	      {
          // ENABLE_EXTENDED_FLAGS: 0x0080, ENABLE_QUICK_EDIT_MODE = 0x0040
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
		      if(hMenu)
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

    // print macros
    #define _trace(x) \
      { if (axHcrt) { _axDcout(x); }  if (axDlog) { _axDfstream(x); } }
    #define trace(x)  \
      { if (axHcrt) { axDcout(x); }   if (axDlog) { axDfstream(x); } }
    #define msg(x)    \
      { if (axHcrt) { axDprintf(x); } if (axDlog) axDfstream(x); } }
    #define wtrace(x) \
      { axDcout(x); if (axDlog) { axDfstream(x); } }

  #endif // AX_WIN32


  #ifdef AX_LINUX
  
    #define _trace(x) { _axDcout(x);  if (axDlog) { _axDfstream(x); } }
    #define trace(x)  { axDcout(x);   if (axDlog) { axDfstream(x); } }
    #define msg(x)    { axDprintf(x); if (axDlog) { axDfstream(x); } }
    inline void axDstdCreate(void) {}
    inline void axDstdDestroy(void) {}
    
  #endif // AX_LINUX

#else // !AX_DEBUG

  #define NDEBUG
  #define _trace(x)             ((void)0)
  #define trace(x)              ((void)0)
  #define msg(x)                ((void)0)
  #define axAssert(x)           ((void)0)
  #define axStaticAssert(x)     ((void)0)
  inline void axDstdCreate(void) {}
  inline void axDstdDestroy(void) {}
  
#endif

//----------------------------------------------------------------------
#endif // axDebug_included

/**
 * @file axDebug.h
 * \brief debugger methods
 *
 * if AX_DEBUG is defined, library debug mode will be enabled. usually using the
 * compile flag -DAX_DEBUG <br><br>
 *
 * while you can observe program output in linux by running a program directly
 * from a terminal, on win32 you have to create a attach a console/or a text
 * window to the
 * process.
 *
 * axonlib will do that for you if you define AX_DEBUG_AUTO_STD or
 * AX_DEBUG_AUTO_WIN (deprecated). the same definitions do not weight on linux.
 *
 * debug methods:
 * \code
 * msg("text");                \\ write text to stdout
 * trace(variable << "text");  \\ write to stdout. simillar to cout
 * axAssert(expression);       \\ runtime assertion
 * axStaticAssert(expression); \\ statatic assertion
 * \endcode
 *
 * <br>
 *
 * file logging: <br>
 * you can log all output that is passed to stdout into a text file located in
 * a relative path to the path of the binary by using:
 * \code
 * #define AX_DEBUG_LOG "./logs/filename.log"
 * \endcode
 * <br>
 * on win32, due to fact that a console window is closed with a program,
 * the only way to see the complete debug output is to log it using
 * this method.
 *
 * <br><br>
 *
 * <i>other notes: <br> - on win32, text windows are shared between plugin
 * instances and only one console can be allocated per process.<i><br>
 *
*/
