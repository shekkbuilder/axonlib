#define AX_PLUGIN     mySynth
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  21
#define AX_WIDTH      500
#define AX_HEIGHT     330
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

#include "axModule.h"
#include "axGraph.h"

//----------------------------------------------------------------------

#define MAX_VOICES 16
#include "axSynth1_voice.h"

class mySynth : public axSynth
{
  private:
    bool        is_gui_initialized;
    axSurface   *srfKnob;
    // --- parameters ---

    parFloat    *pO1Gain;
    parFloat    *pO1VolAtt, *pO1VolDec, *pO1VolSus, *pO1VolRel;
    parInteger  *pO1TuneOct, *pO1TuneSemi, *pO1TuneCent;

    parFloat    *pO2Gain;
    parFloat    *pO2VolAtt, *pO2VolDec, *pO2VolSus, *pO2VolRel;
    parInteger  *pO2TuneOct, *pO2TuneSemi, *pO2TuneCent;

    parFloat    *pMasGain;
    parFloat    *pMasVolAtt, *pMasVolDec, *pMasVolSus, *pMasVolRel;

    // --- widgets ---

    wdgImgKnob2 *wO1Gain;
    wdgImgKnob2 *wO1VolAtt, *wO1VolDec, *wO1VolSus, *wO1VolRel;
    wdgImgKnob2 *wO1TuneOct, *wO1TuneSemi, *wO1TuneCent;

    wdgImgKnob2 *wO2Gain;
    wdgImgKnob2 *wO2VolAtt, *wO2VolDec, *wO2VolSus, *wO2VolRel;
    wdgImgKnob2 *wO2TuneOct, *wO2TuneSemi, *wO2TuneCent;

    wdgImgKnob2 *wMasGain;
    wdgImgKnob2 *wMasVolAtt, *wMasVolDec, *wMasVolSus, *wMasVolRel;

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

        appendParameter( pO1Gain     = new parFloat(  this,v_o1_gain,     "gain","",  0.5 ));
        appendParameter( pO1VolAtt   = new parFloat(  this,v_o1_vol_att,  "att", "",  0 ));
        appendParameter( pO1VolDec   = new parFloat(  this,v_o1_vol_dec,  "dec", "",  0 ));
        appendParameter( pO1VolSus   = new parFloat(  this,v_o1_vol_sus,  "sus", "",  1 ));
        appendParameter( pO1VolRel   = new parFloat(  this,v_o1_vol_rel,  "rel", "",  0 ));

        appendParameter( pO1TuneOct   = new parInteger(this,v_o1_tune_oct,  "oct", "",  0, -4, 4  ));
        appendParameter( pO1TuneSemi  = new parInteger(this,v_o1_tune_semi, "semi","",  0, -12,12 ));
        appendParameter( pO1TuneCent  = new parInteger(this,v_o1_tune_cent, "cent","",  0, -50,50 ));

        appendParameter( pO2Gain     = new parFloat(  this,v_o2_gain,     "gain","",  0.5 ));
        appendParameter( pO2VolAtt   = new parFloat(  this,v_o2_vol_att,  "att", "",  0 ));
        appendParameter( pO2VolDec   = new parFloat(  this,v_o2_vol_dec,  "dec", "",  0 ));
        appendParameter( pO2VolSus   = new parFloat(  this,v_o2_vol_sus,  "sus", "",  1 ));
        appendParameter( pO2VolRel   = new parFloat(  this,v_o2_vol_rel,  "rel", "",  0 ));

        appendParameter( pO2TuneOct   = new parInteger(this,v_o2_tune_oct,  "oct", "",  0, -4, 4  ));
        appendParameter( pO2TuneSemi  = new parInteger(this,v_o2_tune_semi, "semi","",  0, -12,12 ));
        appendParameter( pO2TuneCent  = new parInteger(this,v_o2_tune_cent, "cent","",  0, -50,50 ));

        appendParameter( pMasGain     = new parFloat(  this,v_mas_gain,     "gain","",  0.5 ));
        appendParameter( pMasVolAtt   = new parFloat(  this,v_mas_vol_att,  "att", "",  0 ));
        appendParameter( pMasVolDec   = new parFloat(  this,v_mas_vol_dec,  "dec", "",  0 ));
        appendParameter( pMasVolSus   = new parFloat(  this,v_mas_vol_sus,  "sus", "",  1 ));
        appendParameter( pMasVolRel   = new parFloat(  this,v_mas_vol_rel,  "rel", "",  0 ));
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

        panel->appendWidget(wO1Gain     = new wdgImgKnob2(this, v_o1_gain,      axRect( 10, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1VolAtt   = new wdgImgKnob2(this, v_o1_vol_att,   axRect( 90, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1VolDec   = new wdgImgKnob2(this, v_o1_vol_dec,   axRect(130, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1VolSus   = new wdgImgKnob2(this, v_o1_vol_sus,   axRect(170, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1VolRel   = new wdgImgKnob2(this, v_o1_vol_rel,   axRect(210, 10,32,64),wal_None,65,srfKnob));

        panel->appendWidget(wO1TuneOct  = new wdgImgKnob2(this, v_o1_tune_oct,  axRect(250, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1TuneSemi = new wdgImgKnob2(this, v_o1_tune_semi, axRect(290, 10,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO1TuneCent = new wdgImgKnob2(this, v_o1_tune_cent, axRect(330, 10,32,64),wal_None,65,srfKnob));

        panel->appendWidget(wO2Gain     = new wdgImgKnob2(this, v_o2_gain,      axRect( 10, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2VolAtt   = new wdgImgKnob2(this, v_o2_vol_att,   axRect( 90, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2VolDec   = new wdgImgKnob2(this, v_o2_vol_dec,   axRect(130, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2VolSus   = new wdgImgKnob2(this, v_o2_vol_sus,   axRect(170, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2VolRel   = new wdgImgKnob2(this, v_o2_vol_rel,   axRect(210, 90,32,64),wal_None,65,srfKnob));

        panel->appendWidget(wO2TuneOct  = new wdgImgKnob2(this, v_o2_tune_oct,  axRect(250, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2TuneSemi = new wdgImgKnob2(this, v_o2_tune_semi, axRect(290, 90,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wO2TuneCent = new wdgImgKnob2(this, v_o2_tune_cent, axRect(330, 90,32,64),wal_None,65,srfKnob));

        panel->appendWidget(wMasGain    = new wdgImgKnob2(this, v_mas_gain,     axRect( 10,170,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wMasVolAtt  = new wdgImgKnob2(this, v_mas_vol_att,  axRect( 90,170,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wMasVolDec  = new wdgImgKnob2(this, v_mas_vol_dec,  axRect(130,170,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wMasVolSus  = new wdgImgKnob2(this, v_mas_vol_sus,  axRect(170,170,32,64),wal_None,65,srfKnob));
        panel->appendWidget(wMasVolRel  = new wdgImgKnob2(this, v_mas_vol_rel,  axRect(210,170,32,64),wal_None,65,srfKnob));

        aEditor->connect( wO1Gain,     pO1Gain    );
        aEditor->connect( wO1VolAtt,   pO1VolAtt  );
        aEditor->connect( wO1VolDec,   pO1VolDec  );
        aEditor->connect( wO1VolSus,   pO1VolSus  );
        aEditor->connect( wO1VolRel,   pO1VolRel  );

        aEditor->connect( wO1TuneOct,   pO1TuneOct  );
        aEditor->connect( wO1TuneSemi,  pO1TuneSemi );
        aEditor->connect( wO1TuneCent,  pO1TuneCent );

        aEditor->connect( wO2Gain,     pO2Gain    );
        aEditor->connect( wO2VolAtt,   pO2VolAtt  );
        aEditor->connect( wO2VolDec,   pO2VolDec  );
        aEditor->connect( wO2VolSus,   pO2VolSus  );
        aEditor->connect( wO2VolRel,   pO2VolRel  );

        aEditor->connect( wO2TuneOct,   pO2TuneOct  );
        aEditor->connect( wO2TuneSemi,  pO2TuneSemi );
        aEditor->connect( wO2TuneCent,  pO2TuneCent );

        aEditor->connect( wMasGain,     pMasGain    );
        aEditor->connect( wMasVolAtt,   pMasVolAtt  );
        aEditor->connect( wMasVolDec,   pMasVolDec  );
        aEditor->connect( wMasVolSus,   pMasVolSus  );
        aEditor->connect( wMasVolRel,   pMasVolRel  );
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
          case v_o1_gain:      VM->control( v_o1_gain,      v3 );     break;
          case v_o1_vol_att:   VM->control( v_o1_vol_att,   v  );     break;
          case v_o1_vol_dec:   VM->control( v_o1_vol_dec,   v  );     break;
          case v_o1_vol_sus:   VM->control( v_o1_vol_sus,   v2 );     break;
          case v_o1_vol_rel:   VM->control( v_o1_vol_rel,   v  );     break;

          case v_o1_tune_oct:   VM->control( v_o1_tune_oct,   v  );     break;
          case v_o1_tune_semi:  VM->control( v_o1_tune_semi,  v  );     break;
          case v_o1_tune_cent:  VM->control( v_o1_tune_cent,  v/100 );  break;

          case v_o2_gain:      VM->control( v_o2_gain,      v3 );     break;
          case v_o2_vol_att:   VM->control( v_o2_vol_att,   v  );     break;
          case v_o2_vol_dec:   VM->control( v_o2_vol_dec,   v  );     break;
          case v_o2_vol_sus:   VM->control( v_o2_vol_sus,   v2 );     break;
          case v_o2_vol_rel:   VM->control( v_o2_vol_rel,   v  );     break;

          case v_o2_tune_oct:   VM->control( v_o2_tune_oct,   v  );     break;
          case v_o2_tune_semi:  VM->control( v_o2_tune_semi,  v  );     break;
          case v_o2_tune_cent:  VM->control( v_o2_tune_cent,  v/100 );  break;

          case v_mas_gain:      VM->control( v_mas_gain,      v3 );     break;
          case v_mas_vol_att:   VM->control( v_mas_vol_att,   v  );     break;
          case v_mas_vol_dec:   VM->control( v_mas_vol_dec,   v  );     break;
          case v_mas_vol_sus:   VM->control( v_mas_vol_sus,   v2 );     break;
          case v_mas_vol_rel:   VM->control( v_mas_vol_rel,   v  );     break;
        }
        #undef v2
        #undef v3
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
