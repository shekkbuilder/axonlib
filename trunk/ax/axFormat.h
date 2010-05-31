#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

//TODO: updateList (dirtyParameters)

#include "axConfig.h"
#include "axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axUtils.h"
#include "core/axAssert.h"
#include "platform/axContext.h"
#include "base/axFormatBase.h"

#include "axParameter.h"
//#include "gui/axWidget.h"
#include "gui/axWindow.h"

//----------

#ifdef AX_FORMAT_VST
  //#include "platform/vst/axPluginVst.h"
  #include "format/axFormatVst.h"
  //#define AX_WIN_DEFAULT AX_WIN_PLUGDEFAULT
#endif

#ifdef AX_FORMAT_EXE
  //#include "platform/exe/axPluginExe.h"
  #include "format/axFormatExe.h"
  //#define AX_WIN_DEFAULT AX_WIN_APPDEFAULT
#endif

//----------------------------------------------------------------------

/*

this is inherited from Impl, because:
- axFormatBase defines the interface, the methods that each plugin format implements
- axFormatVst/Exe, implements these, and are the format spewcific layers
- axFormatExe/Vst is typedef'd into axFormatImpl
- axFormat derives (inherits) from axFormatImpl, and yb that, drgging in the
  platform specific sub-layers...

- to make all this work, we need to do #define AX_FORMAT_EXE or #define AX_FORMAT_VST,
  either on the compiler command line (compiler scripts), or in an ide's
  build target setup

*/

//class axFormat : public axFormatBase, public axParameterListener
class axFormat : public axFormatImpl, public axParameterListener
{
  protected:
    axSystemInfo mSystemInfo;
  public:
    axFormat(axContext* aContext,int aFormatFlags)
    //: axFormatBase(aContext,aFormatFlags)
    : axFormatImpl(aContext,aFormatFlags)
      {
        // auto open debug (win32)
        #if defined AX_DEBUG && defined AX_WIN32
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

    virtual ~axFormat()
      {
        #ifndef AX_NOAUTODELETE
          deleteParameters();
        #endif

        // auto close debug (win32)
        #if defined AX_DEBUG && defined AX_WIN32
          #ifdef AX_DEBUG_AUTO_WIN
            axDwinDestroy();
          #endif
          #ifdef AX_DEBUG_AUTO_STD
            axDstdDestroy();
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
        mSystemInfo.bigend   = axBigEndian();
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

    virtual void doIdleEditor()
      {
        #ifndef AX_WIDGET_NOUPDATELIST
        if (mEditorWindow) mEditorWindow->redrawUpdates();
        #endif
      }

    // from plugin

    virtual void onChange(axParameter* aParameter)
      {
        trace("onChange");
        doSetParameter(aParameter);
        if (mEditorOpen && mEditorWindow) mEditorWindow->paramChanged(aParameter);
      }

};

//----------------------------------------------------------------------
#endif
