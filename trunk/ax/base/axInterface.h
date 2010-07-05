#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

#include "base/axBase.h"
//#include "base/test_Window.h"

//----------------------------------------------------------------------

#ifdef AX_NOGUI
#include "base/axInterfaceNone.h"
#else

  #ifdef AX_LINUX
  #include "base/axInterfaceLinux.h"
  #endif

  #ifdef AX_WIN32
  #include "base/axInterfaceWin32.h"
  #endif

#endif // !NOGUI

//----------------------------------------------------------------------
#endif



























//#ifndef axInterface_included
//#define axInterface_included
////----------------------------------------------------------------------
//
//#include "core/axRect.h"
//#include "base/axPlatform.h"
//
//
//#define DEFAULT_INTERFACE axInterface
//
////----------
//
//#ifdef AX_FORMAT_VST
//  #define AX_WIN_DEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGTHREAD | AX_WIN_EMBEDDED )
//#endif
//
//#ifdef AX_FORMAT_EXE
//  #define AX_WIN_DEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGDELETE )
//#endif
//
////----------------------------------------------------------------------
//
//class axInterfaceBase
//{
//  public:
//    axInterfaceBase(axPlatform* aPlatform) {}
//    virtual ~axInterfaceBase() {}
//    virtual void*   getHandle(void) { trace("axInterface.getHandle: NULL"); return NULL; } // linux: Display*
//    //virtual void    notifyParamChanged(axParameter* aParameter) { trace("axInterfaceBase.notifyParamChanged"); }
//    //virtual void    notifyResizeEditor(int aWidth, int aHeight) { trace("axInterfaceBase.notifyResizeEditor"); }
//    //virtual axRect  getEditorRect(void) { return axRect(0,0,100,100); }
//    virtual void* createWindow(void* aParent, axRect aRect, int aFlags=0) { return NULL; }
//};
//
////----------
//
//#ifdef AX_NOGUI
//#include "base/axInterfaceNone.h"
//#else
//
//  #ifdef AX_LINUX
//  #include "base/axInterfaceLinux.h"
//  #endif
//
//  #ifdef AX_WIN32
//  #include "base/axInterfaceWin32.h"
//  #endif
//
//#endif // !NOGUI
//
////----------------------------------------------------------------------
//#endif
