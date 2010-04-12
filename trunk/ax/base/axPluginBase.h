#ifndef axPluginBase_included
#define axPluginBase_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "core/axRect.h"
#include "axParameter.h"
#include "gui/axWindow.h"

//----------------------------------------------------------------------

// plugin flags
#define pf_None       0
#define pf_HasEditor  1

//----------

#define AX_PLUG_DEFAULT ( pf_HasEditor )

//----------------------------------------------------------------------

class axPluginBase// : public axParameterListener
{
  friend class axEditor;
  protected:
    int           mPlugFlags;
    axParameters  mParameters;
    bool          mEditorOpen;
    axRect        mEditorRect;
    axWindow*     mEditorWindow;

  protected:

    axPluginBase(axContext* aContext, int aPluginFlags)
      {
        //wtrace("axPluginBase.constructor");
        mPlugFlags = aPluginFlags;//0;
        mEditorOpen = false;
        mEditorRect = axRect(0,0,256,256);
        mEditorWindow = NULL;
        }

    virtual ~axPluginBase()
      {
        //wtrace("axPluginBase.destructor");
        //deleteParameters();
      }

  public:

    virtual void  describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID) {}
    virtual void  setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false) {}
    virtual void  setupEditor(int aWidth, int aHeight) {}
    virtual void  setupParameters(void) {}
    virtual void  notifyParamChanged(axParameter* aParameter) {}
    virtual void  notifyResizeEditor(int aWidth, int aHeight) {}

    virtual void  updateTimeInfo(void) {}


  protected:

    //----------

    virtual void  doStateChange(int aState) {}
    virtual void  doTransportChange(int aState) {}

    virtual void  doSetProgram(int aProgram) {}
    virtual void  doSetParameter(axParameter* aParameter) {}

    virtual bool  doProcessEvents(void) {return false;}
    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

    virtual bool  doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize) {return false;}
    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs) {}
    virtual void  doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize) {}

    virtual axWindow* doOpenEditor(axContext* aContext) { return NULL; }
    //virtual void* doOpenEditor(axContext* aContext) { return NULL; }
    virtual void  doCloseEditor(void) {}
    virtual void  doIdleEditor(void) {}

    //virtual void onChange(axParameter* aParameter) { trace("axPluginBase.onChange(par)"); }


};

//----------------------------------------------------------------------
#endif
