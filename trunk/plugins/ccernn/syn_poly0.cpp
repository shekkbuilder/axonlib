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
//#include "parDB.h"
#include "wdgPanel.h"
#include "wdgLabel.h"
//#include "wdgImgKnob.h"
#include "wdgImgKnob2.h"
#include "axBitmapLoader.h"
#include "images/knob2.h"

#include "dspOsc0.h"
#include "dspRC.h"

//----------------------------------------------------------------------

class myVoice : public axVoice
{
  private:
    float   gain, attack,release, tuneoct, tunesemi, tunecent, vel;
    dspRC   att1,rel1;
    dspOsc0 osc1,osc2;
  public:

    myVoice() : axVoice()
      {
        osc1.setup(osc_Ramp,0,0);
        osc2.setup(osc_Squ,0,0);
        att1.setup(0,1,0);
        rel1.setup(0,0,0);
      }

    virtual ~myVoice() {}

    virtual void  setSampleRate(float aRate)
      {
        osc1.setSampleRate(aRate);
        osc2.setSampleRate(aRate);
        axVoice::setSampleRate(aRate);
      }

    virtual void noteOn(int aNote, int aVel)
      {
        vel = (float)aVel * inv127;
        att1.setValue(0);         // start from 0
        att1.setWeight(attack);   // fade up
        rel1.setValue(1);         // initially, set to full on
        rel1.setWeight(0);        // fade speed = 0 (until note off)
        float detune = (tuneoct*12) + tunesemi + tunecent;
        float fr1 = 440 * powf(2.0,(aNote-69.0) / 12);
        float fr2 = 440 * powf(2.0,(aNote-69.0+detune) / 12);
        osc1.setFreq(fr1);
        osc2.setFreq(fr2);
      }

    virtual void noteOff(int aNote, int aVel)
      {
        rel1.setValue( att1.getValue() ); // start from current amplitude
        rel1.setWeight(release);          // and let it fade down
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
        float out1 = osc1.process();
        float out2 = osc2.process();
        float a = att1.process();
        float r = rel1.process();
        if (r<EPSILON) mState=vst_Off;
        outs[0] = out1*vel*a*r * gain;
        outs[1] = out2*vel*a*r * gain;
      }

};

//----------------------------------------------------------------------

#define MAX_VOICES 16

class mySynth : public axSynth
{
  private:
    bool        is_gui_initialized;
    axSurface   *srfKnob;
    //parDB       *pGain;
    parFloat    *pGain;
    parFloat    *pAttack, *pRelease;
    parInteger  *pTuneOct, *pTuneSemi, *pTuneCent;
    wdgImgKnob2 *wGain, *wAttack, *wRelease, *wTuneOct, *wTuneSemi, *wTuneCent;
  public:

    mySynth(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axSynth(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        is_gui_initialized = false;
        axRandomize(19);
        describe("syn_poly0","ccernnb","axonlib example plugin",0002,AX_MAGIC+0x0000);
        setupAudio(0,2,true);
        for (int i=0; i<MAX_VOICES; i++) VM->appendVoice( new myVoice() );
        //appendParameter( pGain     = new parDB(     this,0,"gain","db",0,-48, 0  ));
        appendParameter( pGain     = new parFloat(  this,0,"gain","db",1,  0, 1  ));
        appendParameter( pAttack   = new parFloat(  this,1,"att", "",  3,  1, 50 ));
        appendParameter( pRelease  = new parFloat(  this,2,"rel", "",  20, 1, 50 ));
        appendParameter( pTuneOct  = new parInteger(this,3,"oct", "",  0, -4, 4  ));
        appendParameter( pTuneSemi = new parInteger(this,4,"semi","",  0, -12,12 ));
        appendParameter( pTuneCent = new parInteger(this,5,"cent","",  0, -50,50 ));
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
        panel->appendWidget(wGain     = new wdgImgKnob2(this, 0,axRect( 10,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wAttack   = new wdgImgKnob2(this, 1,axRect( 50,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wRelease  = new wdgImgKnob2(this, 2,axRect( 90,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneOct  = new wdgImgKnob2(this, 3,axRect(130,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneSemi = new wdgImgKnob2(this, 4,axRect(170,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneCent = new wdgImgKnob2(this, 5,axRect(210,30,32,64),wal_None,65,srfKnob));
//        panel->appendWidget(            new wdgLabel(  this,-1,axRect( 10,10,32,16),wal_None,"gain",AX_GREY_LIGHT,tal_Center));
//        panel->appendWidget(            new wdgLabel(  this,-2,axRect( 50,10,32,16),wal_None,"att", AX_GREY_LIGHT,tal_Center));
//        panel->appendWidget(            new wdgLabel(  this,-3,axRect( 90,10,32,16),wal_None,"rel", AX_GREY_LIGHT,tal_Center));
//        panel->appendWidget(            new wdgLabel(  this,-4,axRect(130,10,32,16),wal_None,"oct", AX_GREY_LIGHT,tal_Center));
//        panel->appendWidget(            new wdgLabel(  this,-5,axRect(170,10,32,16),wal_None,"semi",AX_GREY_LIGHT,tal_Center));
//        panel->appendWidget(            new wdgLabel(  this,-6,axRect(210,10,32,16),wal_None,"cent",AX_GREY_LIGHT,tal_Center));

        aEditor->connect(wGain,pGain);
        aEditor->connect(wAttack,pAttack);
        aEditor->connect(wRelease,pRelease);
        aEditor->connect(wTuneOct,pTuneOct);
        aEditor->connect(wTuneSemi,pTuneSemi);
        aEditor->connect(wTuneCent,pTuneCent);

//        aEditor->connect(wGain->getKnob(),pGain);
//        aEditor->connect(wAttack->getKnob(),pAttack);
//        aEditor->connect(wRelease->getKnob(),pRelease);
//        aEditor->connect(wTuneOct->getKnob(),pTuneOct);
//        aEditor->connect(wTuneSemi->getKnob(),pTuneSemi);
//        aEditor->connect(wTuneCent->getKnob(),pTuneCent);

        aEditor->doRealign();
      }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch(aParameter->mID)
        {
          case 0: VM->control(0,val*val*val); break;
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
