#ifndef axPlatformWin32_included
#define axPlatformWin32_included
//----------------------------------------------------------------------

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>

//----------------------------------------------------------------------

// from:
// http://old.nabble.com/build-dll-without-c-runtime-dependency-td22210375.html
//
// Here's what I used to link C++ code without a CRT under MinGW:
// compile: "-fno-exceptions -fno-rtti"
// link: "-mdll -entry=_DllMain@12 -lkernel32 -nostdlib"

// multiple definition of.. (dllcrt1.c)

//g++ -Wl,--subsystem,console -nodefaultlibs -nostartfiles -shared -s -o mydll.dll main.o

//extern "C" BOOL WINAPI DllMainCRTStartup(HINSTANCE hinstDLL, DWORD
//fdwReason, LPVOID lpvReserved)
//  {
//    printf("DllMainCRTStartup\n");
//    return TRUE;
//  }

//

////You could use the gcc way to handle dll init code. Declare two functions like:
//
//static void MyStartupFunc(void) __attribute__((constructor));
//void MyStartupFunc(void)
//{
//	// your code
//  printf("constructor\n");
//}
//static void MyExitFunc(void) __attribute__((destructor));
//void MyExitFunc(void)
//{
//	// your code
//  printf("destructor\n");
//}

//See __attribute__ in the gcc manual or in the mingw archives.
//
// ccernn: also called after global object initialization


//----------------------------------------------------------------------

// used in axUtils,getBasePath
static __thread HINSTANCE gWinInstance;

//----------------------------------------------------------------------

// The lpReserved parameter indicates whether the DLL is being loaded statically or dynamically.

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
      printf("gWinInstance = %i\n",(int)gWinInstance);
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
    default:
      trace("DllMain ???");
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
