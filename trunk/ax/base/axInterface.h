#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

#include "core/axRect.h"

#define DEFAULT_INTERFACE axInterface

//----------

#ifdef AX_FORMAT_VST
  #define AX_WIN_DEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGTHREAD | AX_WIN_EMBEDDED )
#endif

#ifdef AX_FORMAT_EXE
  #define AX_WIN_DEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGDELETE )
#endif

//----------------------------------------------------------------------

#include "base/axPlatform.h"

class axInterfaceBase
{
  public:
    axInterfaceBase(axPlatform* aPlatform) {}
    virtual ~axInterfaceBase() {}
    virtual void*   getHandle(void) { trace("axInterface.getHandle: NULL"); return NULL; } // linux: Display*
    virtual void    notifyResizeEditor(int aWidth,int aHeight) {}
    virtual void    notifyParamChanged(axParameter* par) {}
    virtual axRect  getEditorRect(void) { return axRect(0,0,256,256); }

};

//----------

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
