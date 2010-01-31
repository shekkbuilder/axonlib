#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  1
#define AX_WIDTH      52
#define AX_HEIGHT     72
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC
//#efint AX_PAINTERS

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"

#include "axEditor.h"
#include "wdgPanel.h"
#include "wdgLabel.h"
#include "wdgImgKnob.h"

#include "axBitmapLoader.h"
#include "images/knob2.h"

#include "axVoice.h"
#include "dspEnvelope.h"

/*

- #include "axVoice.h"
- class myVoice::axVoice { process() }
- constructor: for (int i=0; i<MAX_VOICES; i++) VM.appendVoice(new myVoice());
- processMidi: VM.noteOn, noteOff
- processSample: float out = VM.process(0);

*/

//----------------------------------------------------------------------
// voice
//----------------------------------------------------------------------

class myVoice : public axVoice
{
  public:
    virtual float process(void)
      {
        //axVoice::process();
        if (mEnvStage==env_offset)  mEnvStage = env_attack;
        if (mEnvStage==env_release) mEnvStage = env_finished;
        float out = sin(mPhase*PI2);
        mPhase += mPhaseAdd;
        if (mPhase>=1) mPhase-=1;
        return out * mVelocity;
      }
};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

#define MAX_VOICES 8

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
    bool        is_gui_initialized;
    axEditor    *mEditor;
    axSurface   *srfKnob;
    float       mGain;
    parFloat*   pGain;
    wdgImgKnob* wGain;
    axVoiceManager VM;
  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        is_gui_initialized = false;
        mEditor = NULL;
        //srfKnob = NULL;
        describe("syn_poly0","ccernnb","axonlib example plugin",0002,AX_MAGIC+0x0000);
        hasEditor(AX_WIDTH,AX_HEIGHT);
// TODO: axPlugin functiuons for these (no calling directly into vst)
        setNumInputs(0);
        isSynth();
//
        for (int i=0; i<MAX_VOICES; i++) VM.appendVoice(new myVoice());
        appendParameter( pGain = new parFloat(this,0,"gain","",1,0,2,0.1));
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (is_gui_initialized) delete srfKnob;
      }

    //--------------------------------------------------
    // signals
    //--------------------------------------------------

    //virtual void doProcessState(int aState) {}
    //virtual void doProcessTransport(int aState) {}

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        int msg = (msg1&0xf0) >> 4;
        //TRACE("msg:%i\n",msg);
        if (msg==9)
        {
          if (msg3==0) VM.noteOff(ofs,msg2,0);
          else VM.noteOn( ofs,msg2,msg3*inv127);
        }
        if (msg==8) VM.noteOff(ofs,msg2,msg3*inv127);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        if (aParameter->mID==0) mGain = aParameter->getValue();
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    {
      VM.setSampleRate( updateSampleRate() );
      return false;
    }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float out = VM.process(0);
        out *= mGain;
        *outs[0] = out;
        *outs[1] = out;
      }

    //----------

    virtual void doPostProcess(float** inputs, float** outputs, long sampleFrames)
      {
        VM.cleanup();
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        axEditor* ED = new axEditor("again_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        wdgPanel* panel;
        ED->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client) );
        if(!is_gui_initialized)
        {
          srfKnob = loadPng( knob2, 15255 );
          is_gui_initialized=true;
        }
        panel->appendWidget(wGain = new wdgImgKnob(this, 0,axRect(10,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(        new wdgLabel(  this,-1,axRect(10,10,32,16),wal_None,"gain",AX_GREY_LIGHT,tal_Center));
        ED->connect(wGain,pGain);
        ED->doRealign();
        mEditor = ED;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* ED = mEditor;
        mEditor = NULL;
        delete ED;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if (mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------
    // listeners
    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);

      }

};

//----------------------------------------------------------------------
#include "axMain.h"

















