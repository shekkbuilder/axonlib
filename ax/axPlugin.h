#ifndef axPlugin_included
#define axPlugin_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "base/axPluginBase.h"
#include "platform/axContext.h"

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

class axPlugin : public axPluginImpl,
                 public axParameterListener
{
  public:
    axPlugin(axContext* aContext,int aPluginFlags)
    : axPluginImpl(aContext,aPluginFlags)
      {
      }

    virtual ~axPlugin()
      {
        deleteParameters();
      }

    //----------

    inline void appendParameter(axParameter* aParameter)
      {
        int index = mParameters.size();
        aParameter->setIndex(index);
        mParameters.append(aParameter);
      }

    inline void deleteParameters(void)
      {
        for (int i=0; i<mParameters.size(); i++) delete mParameters[i];
      }

    //inline axParameter* param(int aIndex)
    //  {
    //    return mParameters[aIndex];
    //  }

    virtual void onChange(axParameter* aParameter)
      {
        doSetParameter(aParameter);
        if (mEditorOpen && mEditorWindow) mEditorWindow->paramChanged(aParameter);
      }

};

//----------------------------------------------------------------------
#endif
