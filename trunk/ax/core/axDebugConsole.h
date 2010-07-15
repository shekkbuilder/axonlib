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

#ifndef axDebugConsole_included
#define axDebugConsole_included

///////////////////////////////////////////////////////////////////////////////
// axDebugConsole.h
///////////////////////////////////////////////////////////////////////////////

#include "axDefines.h"

#if defined AX_WIN32 && defined AX_DEBUG && defined AX_DEBUG_CONSOLE

//in axDefines.h
//#ifndef _WIN32_WINNT
//  #define _WIN32_WINNT 0x0501
//#endif

#include <windows.h>
#include <io.h>
#include <stdio.h>

class axDebugConsole
{
  private:
    FILE* axSfile;  // file stream

  public:

    axDebugConsole()
    {
      AllocConsole();
      SetConsoleTitle("axDebug");
      HWND hCw = GetConsoleWindow();
      if (hCw)
      {
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        // ENABLE_EXTENDED_FLAGS = 0x0080, ENABLE_QUICK_EDIT_MODE = 0x0040
        SetConsoleMode(hIn,0x0080|0x0040);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SMALL_RECT cDim = {0,0,70,20};
        SetConsoleWindowInfo(hOut,true,&cDim);
        SetConsoleCtrlHandler(NULL,true);
        SetWindowPos(hCw,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	      HMENU hMenu = GetSystemMenu(hCw,0);
	      if (hMenu)
	      {
          DeleteMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);
          DrawMenuBar(hCw);
	      }
        // _O_TEXT = 0x4000
        int axHcrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),
                        0x4000);
        if (axHcrt)
        {
          axSfile = _fdopen(axHcrt,"w");
          if (axSfile)
          {
            *stdout = *axSfile;
            setvbuf(stdout,NULL,_IONBF,0);
          }
        }
      }
    }

    ~axDebugConsole()
    {
      FreeConsole();
      if (axSfile)
        close((int)axSfile);
    }

};

#else // defined AX_WIN32 && defined AX_DEBUG && defined AX_DEBUG_AUTOSTD

class axDebugConsole
{
  // blank
};

#endif

#endif // axDebugConsole_included
