#define AX_PLUGIN     mySynth
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  6
#define AX_WIDTH      260
#define AX_HEIGHT     100
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axSynth.h"
#include "parFloat.h"
#include "parInteger.h"
#include "parDB.h"
#include "wdgPanel.h"
#include "wdgLabel.h"
#include "wdgImgKnob.h"
#include "axBitmapLoader.h"
#include "images/knob2.h"

//----------------------------------------------------------------------

class myVoice : public axVoice
{
  private:
    float gain, attack,release, tuneoct, tunesemi, tunecent, vel;
    float ph1, phadd1, ph2, phadd2, att1, att1_s, rel1, rel1_s;
  public:

    myVoice() : axVoice() {}

    virtual ~myVoice() {}

    virtual void noteOn(int aNote, int aVel)
      {
        vel = (float)aVel * inv127;
        ph1 = ph2 = att1 = rel1_s = 0;
        att1_s  = attack;
        rel1    = 1;
        float detune = (tuneoct*12) + tunesemi + tunecent;
        float fr1 = 440 * powf(2.0,(aNote-69.0) / 12);
        float fr2 = 440 * powf(2.0,(aNote-69.0+detune) / 12);
        phadd1  = fr1 * iRate;
        phadd2  = fr2 * iRate;
      }

    virtual void noteOff(int aNote, int aVel)
      {
        rel1   = att1;
        rel1_s = release;
      }

    //virtual void  ctrlChange(int aCtrl, int aVal) {}

    virtual void control(int aIndex, float aValue)
      {
        switch(aIndex)
        {
          case 0: gain     = aValue; break;
          case 1: attack   = aValue; break;
          case 2: release  = aValue; break;
          case 3: tuneoct  = aValue; break;
          case 4: tunesemi = aValue; break;
          case 5: tunecent = aValue; break;
        }
      }

    virtual void process(float* outs)
      {
        float out1 = ph1*2-1; // sinf(PI2*ph);
        float out2 = ph2*2-1; // sinf(PI2*ph);
        ph1 += phadd1;  if (ph1>=1) ph1-=1;
        ph2 += phadd2;  if (ph2>=1) ph2-=1;
        att1 += (1-att1)*att1_s;
        rel1 += (0-rel1)*rel1_s;
        if (rel1<EPSILON) mState=vst_Off;
        outs[0] = out1*vel*att1*rel1 * gain;
        outs[1] = out2*vel*att1*rel1 * gain;
      }

};

//----------------------------------------------------------------------

#define MAX_VOICES 16

class mySynth : public axSynth
{
  private:
    bool is_gui_initialized;
    axSurface *srfKnob;
    //float mGain, mAttack, mRelease, mTuneOct, mTuneSemi, mTuneCent;
    parFloat /**pGain,*/ *pAttack, *pRelease;
    parInteger *pTuneOct, *pTuneSemi, *pTuneCent;
    parDB *pGain;
    wdgImgKnob *wGain, *wAttack, *wRelease, *wTuneOct, *wTuneSemi, *wTuneCent;
  public:

    mySynth(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axSynth(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        is_gui_initialized = false;
        axRandomize(19);
        describe("syn_poly0","ccernnb","axonlib example plugin",0002,AX_MAGIC+0x0000);
        setupAudio(0,2,true);
        for (int i=0; i<MAX_VOICES; i++) VM->appendVoice( new myVoice() );
        //appendParameter( pGain     = new parFloat(  this,0,"gain",   "",0.5,0, 1  ));
        appendParameter( pGain     = new parDB(     this,0,"gain",   "db",0,-60, 12  ));
        appendParameter( pAttack   = new parFloat(  this,1,"attack", "",  3,  1, 50 ));
        appendParameter( pRelease  = new parFloat(  this,2,"release","",  20, 1, 50 ));
        appendParameter( pTuneOct  = new parInteger(this,3,"oct",    "",  0, -4, 4  ));
        appendParameter( pTuneSemi = new parInteger(this,4,"semi",   "",  0, -12,12 ));
        appendParameter( pTuneCent = new parInteger(this,5,"cent",   "",  0, -50,50 ));
        processParameters();
      }

    virtual ~mySynth() { if (is_gui_initialized) delete srfKnob; }

    virtual void doSetupEditor(axEditor* aEditor)
      {
        wdgPanel* panel;
        aEditor->appendWidget( panel = new wdgPanel(this,-100,NULL_RECT,wal_Client) );
        if(!is_gui_initialized)
        {
          srfKnob = loadPng( knob2, 15255 );
          is_gui_initialized=true;
        }
        panel->appendWidget(wGain     = new wdgImgKnob(this, 0,axRect( 10,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wAttack   = new wdgImgKnob(this, 1,axRect( 50,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wRelease  = new wdgImgKnob(this, 2,axRect( 90,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wTuneOct  = new wdgImgKnob(this, 3,axRect(130,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wTuneSemi = new wdgImgKnob(this, 4,axRect(170,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(wTuneCent = new wdgImgKnob(this, 5,axRect(210,30,32,32),wal_None,65,srfKnob));
        panel->appendWidget(            new wdgLabel(  this,-1,axRect( 10,10,32,16),wal_None,"gain",AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(            new wdgLabel(  this,-2,axRect( 50,10,32,16),wal_None,"att", AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(            new wdgLabel(  this,-3,axRect( 90,10,32,16),wal_None,"rel", AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(            new wdgLabel(  this,-4,axRect(130,10,32,16),wal_None,"oct", AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(            new wdgLabel(  this,-5,axRect(170,10,32,16),wal_None,"semi",AX_GREY_LIGHT,tal_Center));
        panel->appendWidget(            new wdgLabel(  this,-6,axRect(210,10,32,16),wal_None,"cent",AX_GREY_LIGHT,tal_Center));
        aEditor->connect(wGain,pGain);
        aEditor->connect(wAttack,pAttack);
        aEditor->connect(wRelease,pRelease);
        aEditor->connect(wTuneOct,pTuneOct);
        aEditor->connect(wTuneSemi,pTuneSemi);
        aEditor->connect(wTuneCent,pTuneCent);
        aEditor->doRealign();
      }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch(aParameter->mID)
        {
          //case 0: mGain = val; break;
          case 0: VM->control(0,val); break;
          case 1: VM->control(1, 1/(val*val*val) ); break;
          case 2: VM->control(2, 1/(val*val*val) ); break;
          case 3: VM->control(3,val); break;
          case 4: VM->control(4,val); break;
          case 5: VM->control(5,val/100); break;
        }
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
