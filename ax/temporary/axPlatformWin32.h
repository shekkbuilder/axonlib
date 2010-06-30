#ifndef axPlatformWin32_included
#define axPlatformWin32_included
//----------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

//----------------------------------------------------------------------

static HINSTANCE g_HInstance; // __thread?

//----------

extern "C"
BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved)
{
  trace("DllMain");
  switch(reason)
  {
    case DLL_PROCESS_ATTACH:
      g_HInstance = hModule;
      //register_winclass(hModule);
      break;
    case DLL_PROCESS_DETACH:
      //unregister_winclass(hModule);
      break;
  }
  return TRUE;
}

//----------------------------------------------------------------------

class axPlatform
{
  private:
    HINSTANCE mHInstance;

  public:

    axPlatform(axFormat* aFormat)
      {
        mHInstance = g_HInstance;
        trace("axPlatform win32");
        // hmodule, hinstance
      }

    virtual ~axPlatform()
      {
      }

    inline HINSTANCE getHInstance(void) { return mHInstance; }
};

//

//void register_winclass(HINSTANCE aInstance)
//  {
//    //trace("register...");
//  }
//
//void unregister_winclass(HINSTANCE aInstance)
//  {
//    //trace("unregister...");
//  }
//

//----------------------------------------------------------------------
#endif


