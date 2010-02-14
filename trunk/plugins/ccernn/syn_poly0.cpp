#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  3
#define AX_WIDTH      136
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

#define MAX_VOICES 16

//----------------------------------------------------------------------
// voice
//----------------------------------------------------------------------

class myVoice : public axVoice
{
  private:
    float ph;
    float phadd;
    float vel;
    float attack,release;
    float att, att_s;
    float rel, rel_s;
  public:
    myVoice()
    : axVoice()
      {
      }
    virtual ~myVoice()
      {
      }
    //void setAttack(float a) { attack=a; }
    //void setRelease(float r) { release=r; }
    virtual void noteOn(int aNote, int aVel)
      {
        float freq = 440 * powf(2.0,(aNote-69.0) / 12);
        ph    = 0;
        phadd = freq * iRate;
        vel   = (float)aVel * inv127;
        att   = 0;
        att_s = attack;
        rel   = 1;
        rel_s = 0;
      }
    virtual void noteOff(int aNote, int aVel)
      {
        //mState = vst_Off;
        rel = att;
        rel_s = release;
      }
    virtual void control(int aIndex, float aValue)
      {
        switch(aIndex)
        {
          case 0: attack  = aValue; break;
          case 1: release = aValue; break;
        }
      }
    virtual float process(void)
      {
        //float out = sinf(PI2*ph);
        float out = ph*2-1;
        ph += phadd;
        if (ph>=1) ph-=1;
        att += (1-att)*att_s;
        rel += (0-rel)*rel_s;
        if (rel<EPSILON) mState=vst_Off;   // !!!
        return out*vel*att*rel;
      }
};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:

    float           mGain;
    float           mAttack;
    float           mRelease;
    float           mAttack_d,  mAttack_c;
    float           mRelease_d, mRelease_c;
    axVoiceManager* VM;

    parFloat*       pGain;
    parFloat*       pAttack;
    parFloat*       pRelease;

    bool            is_gui_initialized;
    axEditor*       mEditor;
    axSurface*      srfKnob;
    wdgImgKnob*     wGain;
    wdgImgKnob*     wAttack;
    wdgImgKnob*     wRelease;

  public:

    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        axRandomize(19);
        VM = new axVoiceManager();//(MAX_VOICES);
        for (int i=0; i<MAX_VOICES; i++) VM->appendVoice( new myVoice() );
        is_gui_initialized = false;
        mEditor = NULL;
        describe("syn_poly0","ccernnb","axonlib example plugin",0002,AX_MAGIC+0x0000);
        setupAudio(0,2,true);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        appendParameter( pGain    = new parFloat(this,0,"gain",   "",1, 0,2));
        appendParameter( pAttack  = new parFloat(this,1,"attack", "",3, 1,50 ));
        appendParameter( pRelease = new parFloat(this,2,"release","",20,1,50 ));
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (is_gui_initialized) delete srfKnob;
        delete VM;
      }

    //--------------------------------------------------
    // signals
    //--------------------------------------------------

    virtual void doProcessState(int aState)
      {
        switch(aState)
        {
          case pst_Resume:
            VM->postProcess(); // to be sure there's no unprocessed events in the buffers
            break;
        }
      }

    //virtual void doProcessTransport(int aState) {}

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        VM->midi(ofs,msg1,msg2,msg3);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float val = aParameter->getValue();
        switch(id)
        {
          case 0:
            mGain = val;
            break;
          case 1:
            VM->control(0, 1/(val*val*val) );
            break;
          case 2:
            VM->control(1, 1/(val*val*val) );
            break;
        }
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    {
      VM->setSampleRate( updateSampleRate() );
      VM->preProcess();
      return false;
    }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float out = VM->process();
        *outs[0] = out * mGain;
        *outs[1] = out * mGain;
      }

    //----------

    virtual void doPostProcess(float** inputs, float** outputs, long sampleFrames)
      {
        VM->postProcess();
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
        panel->appendWidget(wGain    = new wdgImgKnob(this, 0,axRect(10,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wAttack  = new wdgImgKnob(this, 0,axRect(52,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wRelease = new wdgImgKnob(this, 0,axRect(94,30,32,32),wal_None,65,srfKnob));

        panel->appendWidget(           new wdgLabel(  this,-1,axRect(10,10,32,16),wal_None,"gain",   AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(           new wdgLabel(  this,-1,axRect(52,10,32,16),wal_None,"attack", AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(           new wdgLabel(  this,-1,axRect(94,10,32,16),wal_None,"release",AX_GREY_LIGHT,tal_Center));

        ED->connect(wGain,pGain);
        ED->connect(wAttack,pAttack);
        ED->connect(wRelease,pRelease);
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

















