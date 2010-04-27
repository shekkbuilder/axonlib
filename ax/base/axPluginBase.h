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

#define AX_PLUG_DEFAULT ( pf_HasEditor )

//----------

// plugin states (doStateChange)
#define ps_Open     1
#define ps_Close    2
#define ps_Suspend  3
#define ps_Resume   4

// plugin transport (doTransportChange)
#define pt_Changed      1   // indicates that play, cycle or record state has changed
#define pt_Playing      2   // set if Host sequencer is currently playing
#define pt_CycleActive  4   // set if Host sequencer is in cycle mode
#define pt_Recording    8   // set if Host sequencer is in record mode
#define pt_AutoWriting  64  // set if automation write mode active (record parameter changes)
#define pt_AutoReading  128 // set if automation read mode active (play parameter changes)

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
        mPlugFlags    = aPluginFlags;//0;
        mEditorOpen   = false;
        mEditorRect   = axRect(0,0,256,256);
        mEditorWindow = NULL;
        }

    virtual ~axPluginBase()
      {
        //wtrace("axPluginBase.destructor");
        //deleteParameters();
      }

  public:

    //inline int getFlags(void)         { return mPlugFlags; }
    inline axRect getEditorRect(void) { return mEditorRect; }
    inline bool isEditorOpen(void)    { return mEditorOpen; }

    virtual int    getPlayState(void)  { return 0; }
    virtual double getSamplePos(void)  { return 0; }
    virtual double getSampleRate(void) { return 0; }
    virtual double getBeatPos(void)    { return 0; }
    virtual double getTempo(void)      { return 0; }

    //

    // call this to descript your plugin.
    // most of these are directly related to similar vst features
    virtual void  describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID) {}

    // setup audio. an effect would normally have 2 inputs and 2 outputs,
    // while a synth often has 0 inputs and 2 outputs (and the 'isSynth' flag set
    virtual void  setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false) {}

    // this both tells the framework that you have an editor, and its (initial) size
    // (doOpenEditor,doCloseEditor,coIdleEditor will be called)
    virtual void  setupEditor(int aWidth, int aHeight) {}

    // after appending your parameters (appendParameter), you should call
    // this, to tell the framework how many parameters you have,
    // and to distribute the parameter values.
    // (doSetParameter will be called for each parameter)
    virtual void  setupParameters(void) {}

    // when tweaking a widget, we need to tell the host that the value has changed,
    // so that it can automate it, or redraw something...
    virtual void  notifyParamChanged(axParameter* aParameter) {}

    // (not used yet, not finished..)
    // call this to tell the host you want to resize the editor
    // note that not all hosts support resizeable editors
    virtual void  notifyResizeEditor(int aWidth, int aHeight) {}

    // call this to get updated info about project tempo,
    // current time (in beats), samplerate, etc...
    virtual void  updateTimeInfo(void) {}

    //virtual void resizeEditor(int aWidth, int aHeight) { mEditorRect.w=aWidth; mEditorRect.h=aHeight; }


  protected:

    //----------

    // this will be called when the plugins is suspend/resume, and open/close
    virtual void      doStateChange(int aState) {}

    // transport state has changed
    virtual void      doTransportChange(int aState) {}

    // set new program
    virtual void      doSetProgram(int aProgram) {}

    // parameter calue has changed, grab new value here
    virtual void      doSetParameter(axParameter* aParameter) {}

    // not used
    //virtual bool      doProcessEvents(void) {return false;}

    // this will be called for each incoming midi message, before doProcessBlock
    virtual void      doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

    // process a block of samples. return true if you did all needed work,
    // or false if doProcessSample should be called for each sample
    virtual bool      doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize) {return false;}

    // process one sample.
    // only called if you returned true from doProcessBlock
    virtual void      doProcessSample(SPL** aInputs, SPL** aOutputs) {}

    // called after current block is finished, do any eventual post-processing here
    virtual void      doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize) {}

    // create and open your editor
    virtual axWindow* doOpenEditor(axContext* aContext) { return NULL; }

    // close and destroy the editor
    virtual void      doCloseEditor(void) {}

    // called regularly when the editor is open
    // host decides rate (some hosts is configure-able)
    virtual void      doIdleEditor(void) {}

    //----------

    //virtual void onChange(axParameter* aParameter) { trace("axPluginBase.onChange(par)"); }


};

//----------------------------------------------------------------------
#endif
