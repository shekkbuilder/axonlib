#ifndef axDebugConsole_included
#define axDebugConsole_included
//----------------------------------------------------------------------

#include <io.h>
//#include <stdio.h>
//#include <sstream>

class axDebugConsole
{
  private:
    unsigned int  axHcrt;   // crt handle
    FILE*         axSfile;  // file stream

  public:

    axDebugConsole()
      {
        AllocConsole();
        SetConsoleTitle("axDebug");
        HWND hCw = GetConsoleWindow();
        if (hCw)
	      {
          HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
          SetConsoleMode(hIn,0x0080|0x0040);
          HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
          SMALL_RECT cDim = {0,0,50,20};
          SetConsoleWindowInfo(hOut,true,&cDim);
          SetConsoleCtrlHandler(NULL,true);
          SetWindowPos(hCw,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		      HMENU hMenu = GetSystemMenu(hCw,0);
		      if (hMenu)
		      {
            DeleteMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);
            DrawMenuBar(hCw);
		      }
        }
        axHcrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),0x4000);
        axSfile = _fdopen(axHcrt,"w");
        *stdout = *axSfile;
        setvbuf(stdout,NULL,_IONBF,0);
      }

    ~axDebugConsole()
      {
        FreeConsole();
        close((int)axSfile);
      }

};

#define _axDcout(x) cout << x << endl; cout.flush()
#define trace(x) _axDcout(x)

// in axPlatformWin32 constructor, something like
// #ifdef AX_DEBUG_LOG
// mDebugWin = new axDebugWindow();
// #endif
// and similar in the destructor to auto-delete it

/*
//#include <iostream>
//using namespace std;
//#define _axDcout(x) cout << x << endl; cout.flush()
//#define trace(x) _axDcout(x)
*/

//----------------------------------------------------------------------
#endif
