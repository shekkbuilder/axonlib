#ifndef axGlobals_included
#define axGlobals_included
//----------------------------------------------------------------------

#define AX_EMBEDDED   1
#define AX_WINDOWED   2
#define AX_BUFFERED   4

//----------------------------------------------------------------------

static char base_path[1024];

#ifdef linux
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  //#include <X11/cursorfont.h>
  #include <pthread.h>
  Display*  gDP = NULL;
  char* axPlatform(void)          { return (char*)"linux"; }
  char* axBasePath(void)          { return base_path; }
  void  axInitialize(int aFlags)  { XInitThreads(); if (aFlags&AX_WINDOWED) if(!gDP) gDP=XOpenDisplay(NULL); }
  void  axCleanup(int aFlags)     {                 if (aFlags&AX_WINDOWED) if (gDP) XCloseDisplay(gDP); }
#endif

//----------------------------------------------------------------------

#ifdef WIN32
  #include <windows.h>
  HINSTANCE gInstance;
  char* axPlatform(void)          { return (char*)"win32"; }
  char* axBasePath(void)          { GetModuleFileName(gInstance,base_path,1024); return base_path; }
  void  axInitialize(int aFlags)  { gInstance = (HINSTANCE)GetModuleHandle(NULL); }
  void  axCleanup(int aFlags)     { }
#endif

//----------------------------------------------------------------------
#endif
