#ifndef axPlatformWin32_included
#define axPlatformWin32_included
//----------------------------------------------------------------------

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>

//----------------------------------------------------------------------
//----------------------------------------------------------------------

// used in axUtils,getBasePath
static __thread HINSTANCE gWinInstance;

//----------------------------------------------------------------------

__externc BOOL APIENTRY
DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved)
{
  //trace("win32 DllMain");
  //g_Instance = hModule;
  switch(reason)
  {
    case DLL_PROCESS_ATTACH:
      trace("DllMain DLL_PROCESS_ATTACH");
      gWinInstance = hModule;
      break;
    case DLL_PROCESS_DETACH:
      trace("DllMain DLL_PROCESS_DETACH");
      break;
    case DLL_THREAD_ATTACH:
      trace("DllMain DLL_THREAD_ATTACH");
      break;
    case DLL_THREAD_DETACH:
      trace("DllMain DLL_THREAD_DETACH");
      break;
  }
  return TRUE;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//class axDebugWindow
//{
//  public:
//    axDebugWindow(axBase* aBase) {}
//    ~axDebugWindow() {}
//};
//
//class axLogFile
//{
//  public:
//    axLogFile(axBase* aBase) {}
//    ~axLogFile() {}
//};

//----------------------------------------------------------------------
//----------------------------------------------------------------------

class axPlatformWin32 : public axPlatform
{
  private:
    HINSTANCE mWinInstance;
  public:
    axPlatformWin32(axBase* aBase) : axPlatform(aBase)
      {
        //trace("axPlatformWin32.constructor");
        mWinInstance = gWinInstance;
      }
    virtual ~axPlatformWin32()
      {
        //trace("axPlatformWin32.destructor");
      }
    //----------

    virtual char* getPlatformName(void) { return (char*)"win32"; }
    virtual void* getHandle(void) { return (void*)mWinInstance; }

};

//----------

typedef axPlatformWin32 AX_PLATFORM;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const char* axGetBasePath(char* path)
  {
    char* path_init = path;
    char filepath[AX_MAX_PATH] = "";
    GetModuleFileName(gWinInstance, filepath, MAX_PATH);
    const char* slash = axStrrchr(filepath, '\\') + 1;
    int len = (axStrrchr(filepath, '\\') + 1) - (char*)filepath;
    if (slash) axStrncpy(path, filepath, len);
    else axStrcpy(path, (char*)".\\");
    //axStrcat(path, '\0');
    path[len] = 0;
    return path_init;
  }

//----------------------------------------------------------------------
#endif



































//#ifndef axPlatformWin32_included
//#define axPlatformWin32_included
////----------------------------------------------------------------------
//
//#include <windows.h>
//#include <stdio.h>
//
////----------------------------------------------------------------------
//
//#ifndef AX__NOGUI
//
//static HINSTANCE g_WinInstance; // __thread?
//
//extern "C"
//BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved)
//{
//  switch(reason)
//  {
//    case DLL_PROCESS_ATTACH:
//      wtrace("DllMain DLL_PROCESS_ATTACH");
//      g_WinInstance = hModule;
//      //register_winclass(hModule);
//      break;
//    case DLL_PROCESS_DETACH:
//      wtrace("DllMain DLL_PROCESS_DETACH");
//      //unregister_winclass(hModule);
//      break;
//    case DLL_THREAD_ATTACH:
//      wtrace("DllMain DLL_THREAD_ATTACH");
//      break;
//    case DLL_THREAD_DETACH:
//      wtrace("DllMain DLL_THREAD_DETACH");
//      break;
//  }
//  return TRUE;
//}
//
//#endif
//
////----------------------------------------------------------------------
//
//class axPlatform
//{
//  private:
//    HINSTANCE mWinInstance;
//
//  public:
//
//    axPlatform(axFormat* aFormat)
//      {
//        wtrace("axPlatform win32");
//        #ifdef AX__NOGUI
//        mWinInstance = (HINSTANCE)GetModuleHandle(NULL);
//        #else
//        mWinInstance = g_WinInstance;
//        #endif
//      }
//
//    virtual ~axPlatform()
//      {
//      }
//
//    inline HINSTANCE getWinInstance(void) { return mWinInstance; }
//};
//
////----------------------------------------------------------------------
//#endif
//
//