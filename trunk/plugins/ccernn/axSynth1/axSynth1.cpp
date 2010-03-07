#define AX_PLUGIN     mySynth
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  8
#define AX_WIDTH      260
#define AX_HEIGHT     200
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axSynth.h"
#include "parFloat.h"
#include "parInteger.h"
#include "wdgPanel.h"
#include "wdgLabel.h"
#include "wdgImgKnob2.h"
#include "axBitmapLoader.h"
#include "images/knob.h"

//----------------------------------------------------------------------

#define MAX_VOICES 16
#include "axSynth1_voice.h"

class mySynth : public axSynth
{
  private:
    bool        is_gui_initialized;
    axSurface   *srfKnob;
    // --- parameters ---
    parFloat    *pGain;
    parInteger  *pTuneOct, *pTuneSemi, *pTuneCent;
    parFloat    *pAttack, *pDecay, *pSustain, *pRelease;
    // --- widgets ---
    wdgImgKnob2 *wGain;
    wdgImgKnob2 *wTuneOct, *wTuneSemi, *wTuneCent;
    wdgImgKnob2 *wAttack, *wDecay, *wSustain, *wRelease;
  public:

    //--------------------------------------------------

    mySynth(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axSynth(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        wtrace("axSynth1 constructor");
        is_gui_initialized = false;
        axRandomize(19);
        describe("axSynth1","ccernn","ccernn.audio",0001,AX_MAGIC+0x1001);
        setupAudio(0,2,true);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        for (int i=0; i<MAX_VOICES; i++) VM->appendVoice( new myVoice() );
        appendParameter( pGain     = new parFloat(  this,v_gain,    "gain","",  0.5 ));
        appendParameter( pTuneOct  = new parInteger(this,v_tuneoct, "oct", "",  0, -4, 4  ));
        appendParameter( pTuneSemi = new parInteger(this,v_tunesemi,"semi","",  0, -12,12 ));
        appendParameter( pTuneCent = new parInteger(this,v_tunecent,"cent","",  0, -50,50 ));
        appendParameter( pAttack   = new parFloat(  this,v_attack,  "att", "",  0 ));
        appendParameter( pDecay    = new parFloat(  this,v_decay,   "dec", "",  0 ));
        appendParameter( pSustain  = new parFloat(  this,v_sustain, "sus", "",  1 ));
        appendParameter( pRelease  = new parFloat(  this,v_release, "rel", "",  0 ));
        processParameters();
      }

    //--------------------------------------------------

    virtual ~mySynth()
      {
        if (is_gui_initialized) delete srfKnob;
      }

    //--------------------------------------------------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* panel;
        aEditor->appendWidget( panel = new wdgPanel(this,-100,NULL_RECT,wal_Client) );
        if(!is_gui_initialized)
        {
          srfKnob = loadPng( (unsigned char*)knob, knob_size );
          is_gui_initialized=true;
        }
        panel->appendWidget(wGain     = new wdgImgKnob2(this, v_gain,     axRect( 10,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneOct  = new wdgImgKnob2(this, v_tuneoct,  axRect( 50,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneSemi = new wdgImgKnob2(this, v_tunesemi, axRect( 90,30,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wTuneCent = new wdgImgKnob2(this, v_tunecent, axRect(130,30,32,64),wal_None,65,srfKnob));

        panel->appendWidget(wAttack   = new wdgImgKnob2(this, v_attack,   axRect( 10,90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wDecay    = new wdgImgKnob2(this, v_decay,    axRect( 50,90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wSustain  = new wdgImgKnob2(this, v_sustain,  axRect( 90,90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wRelease  = new wdgImgKnob2(this, v_release,  axRect(130,90,32,64),wal_None,65,srfKnob));


        aEditor->connect(wGain,     pGain);
        aEditor->connect(wTuneOct,  pTuneOct);
        aEditor->connect(wTuneSemi, pTuneSemi);
        aEditor->connect(wTuneCent, pTuneCent);
        aEditor->connect(wAttack,   pAttack);
        aEditor->connect(wDecay,    pDecay);
        aEditor->connect(wSustain,  pSustain);
        aEditor->connect(wRelease,  pRelease);
        aEditor->doRealign();
      }

    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        float v = aParameter->getValue();
        //#define vvv  (val*val*val)
        #define v2  (v*v)
        #define v3  (v*v*v)
        switch(aParameter->mID)
        {
          case v_gain:      VM->control( v_gain,      v3 );     break;
          case v_tuneoct:   VM->control( v_tuneoct,   v );      break;
          case v_tunesemi:  VM->control( v_tunesemi,  v );      break;
          case v_tunecent:  VM->control( v_tunecent,  v/100 );  break;
          case v_attack:    VM->control( v_attack,    v );      break;
          case v_decay:     VM->control( v_decay,     v );      break;
          case v_sustain:   VM->control( v_sustain,   v );      break;
          case v_release:   VM->control( v_release,   v );      break;
        }
        #undef v2
        #undef v3
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
