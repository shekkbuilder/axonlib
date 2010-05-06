#ifndef axPlugin_included
#define axPlugin_included
//----------------------------------------------------------------------

//TODO: updateList (dirtyParameters)

#include "axConfig.h"
#include "axDefines.h"
#include "core/axDebug.h" // < - should be define before Utils because of _WINNT
#include "axStdlib.h"
#include "core/axMalloc.h"
//#include "core/axMemory.h"
#include "core/axUtils.h"
#include "platform/axContext.h"
#include "base/axPluginBase.h"

#include "axParameter.h"
//#include "gui/axWidget.h"
#include "gui/axWindow.h"

//----------

#ifdef AX_FORMAT_VST
  #include "platform/vst/axPluginVst.h"
  //#define AX_WIN_DEFAULT AX_WIN_PLUGDEFAULT
#endif

#ifdef AX_FORMAT_EXE
  #include "platform/exe/axPluginExe.h"
  //#define AX_WIN_DEFAULT AX_WIN_APPDEFAULT
#endif

//----------------------------------------------------------------------

/*

this is inherited from Impl, because:
- axPluginBase defines the interface, the methods that each plugin format implements
- axPluginVst/Exe, implements these, and are the format spewcific layers
- axPluginExe/Vst is typedef'd into axPluginImpl
- axPlugin derives (inherits) from axPluginImpl, and yb that, drgging in the
  platform specific sub-layers...

- to make all this work, we need to do #define AX_FORMAT_EXE or #define AX_FORMAT_VST,
  either on the compiler command line (compiler scripts), or in an ide's
  build target setup

*/

//class axPlugin : public axPluginBase, public axParameterListener
class axPlugin : public axPluginImpl, public axParameterListener
{
  protected:
    axSystemInfo mSystemInfo;
  public:
    axPlugin(axContext* aContext,int aPluginFlags)
    //: axPluginBase(aContext,aPluginFlags)
    : axPluginImpl(aContext,aPluginFlags)
      {
        // auto open debug (win32)
        #ifdef AX_WIN32
          #ifdef AX_DEBUG_AUTO_STD
            axDstdCreate();
          #endif
          #ifdef AX_DEBUG_AUTO_WIN
            axDwinCreate();
          #endif
        #endif
        //updateSampleRate();
        //updateTimeInfo();
      }

    virtual ~axPlugin()
      {
        #ifndef AX_NOAUTODELETE
        deleteParameters();
        #endif

        // auto close debug (win32)
        #ifdef AX_WIN32
          #ifdef AX_DEBUG_AUTO_STD
            axDstdDestroy();
          #endif
          #ifdef AX_DEBUG_AUTO_WIN
            axDwinDestroy();
          #endif
        #endif
      }

    //--------------------------------------------------

    virtual axSystemInfo* getSystemInfo(void)
      {
        #ifdef AX_LINUX
          mSystemInfo.os = "linux";
        #endif
        #ifdef AX_WIN32
          mSystemInfo.os = "win32";
        #endif
        mSystemInfo.cpuid    = axCpuCaps();
        mSystemInfo.cpuidstr = axCpuCapsString();
        return &mSystemInfo;
      }

    //--------------------------------------------------

    inline void appendParameter(axParameter* aParameter)
      {
        int index = mParameters.size();
        aParameter->setIndex(index);
        mParameters.append(aParameter);
      }

    //----------

    inline void deleteParameters(void)
      {
        for (int i=0; i<mParameters.size(); i++) delete mParameters[i];
      }

    //inline axParameter* param(int aIndex)
    //  {
    //    return mParameters[aIndex];
    //  }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        doSetParameter(aParameter);
        if (mEditorOpen && mEditorWindow) mEditorWindow->paramChanged(aParameter);
      }

};

//----------------------------------------------------------------------
#endif
