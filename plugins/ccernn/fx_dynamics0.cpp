#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   16
#define AX_NUMPARAMS  6

//#define AX_AUTOSYNC
#define AX_WIDTH      230
#define AX_HEIGHT     170
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"

#include "axEditor.h"
#include "wdgImgKnob.h"
#include "wdgVuMeter.h"
#include "wdgLabel.h"

#include "axBitmapLoader.h"
#include "images/knob1.h"   // 20*20, 65, 6668
#include "images/knob2.h"   // 32*32, 65, 15255
#include "images/button1.h" // 12*12, 3,  647

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    // --- parameters
    float threshold;
    float ratio;
    float attack;
    float release;
    float pregain;
    float postgain;
    // --- synth/fx variables
    float value;
    float gainred;
    // --- gui
    axEditor    *mEditor;
    bool        is_gui_initialized;
    axSurface   *mSrfKnob;
    wdgVuMeter  *meter;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        mSrfKnob = NULL;
        is_gui_initialized = false;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("fx_dynamics0","ccernn","product_string",0,0);
        //isSynth();
        appendParameter(new parFloat(   this, 0, "threshold",   "", 1         ));
        appendParameter(new parFloat(   this, 1, "ratio",       "", 1,   1,20 ));
        appendParameter(new parFloat(   this, 2, "attack",      "", 7,   1,20 ));
        appendParameter(new parFloat(   this, 3, "release",     "", 10,  1,20 ));
        appendParameter(new parFloat(   this, 4, "pregain",     "", 1,   0,2  ));
        appendParameter(new parFloat(   this, 5, "postgain",    "", 1,   0,2  ));
        processParameters();
        value = 0;
        gainred = 0;
        initPrograms();
      }

    virtual ~myPlugin()
      {
        if (mSrfKnob) delete mSrfKnob;
      }

    //--------------------------------------------------

    //TODO: save programs with host project
    // chunks

    void initPrograms(void)
      {
        saveProgram();
        duplicatePrograms();
      }

    //----------

    // we must intercept this, because we need to tell the widgets to redraw themselves
    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("fx_dynamics0_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        if(!is_gui_initialized)
        {
          mSrfKnob = loadPng( knob2, 15255 );
          is_gui_initialized=true;
        }
        // knobs
        E->appendWidget(new wdgImgKnob(E,0,axRect(  30, 70, 32, 32),wal_None,/*mParameters[ 0],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,1,axRect(  30,125, 32, 32),wal_None,/*mParameters[ 1],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,2,axRect( 100, 70, 32, 32),wal_None,/*mParameters[ 2],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,3,axRect( 100,125, 32, 32),wal_None,/*mParameters[ 3],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,4,axRect( 170, 70, 32, 32),wal_None,/*mParameters[ 4],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,5,axRect( 170,125, 32, 32),wal_None,/*mParameters[ 5],*/ 65, mSrfKnob ));
        for (int i=0;i<AX_NUMPARAMS; i++) E->connect( E->mWidgets[i], mParameters[i] );
        // gain reduction meter
        E->appendWidget(meter = new wdgVuMeter(E,-1,axRect(10,10,210,8),wal_None/*,NULL*/ ));
        meter->clearFlag(wfl_Vertical);
        // labels
        E->appendWidget(new wdgLabel(E,-1,axRect(   30, 55, 32,15 ),wal_None,/*NULL,*/"thresh",  AX_GREY_LIGHT, tal_Center|tal_Top ));
        E->appendWidget(new wdgLabel(E,-1,axRect(   30,110, 32,15 ),wal_None,/*NULL,*/"ratio",   AX_GREY_LIGHT, tal_Center|tal_Top  ));
        E->appendWidget(new wdgLabel(E,-1,axRect(  100, 55, 32,15 ),wal_None,/*NULL,*/"attack",  AX_GREY_LIGHT, tal_Center|tal_Top  ));
        E->appendWidget(new wdgLabel(E,-1,axRect(  100,110, 32,15 ),wal_None,/*NULL,*/"release", AX_GREY_LIGHT, tal_Center|tal_Top ));
        E->appendWidget(new wdgLabel(E,-1,axRect(  170, 55, 32,15 ),wal_None,/*NULL,*/"pre",     AX_GREY_LIGHT, tal_Center|tal_Top  ));
        E->appendWidget(new wdgLabel(E,-1,axRect(  170,110, 32,15 ),wal_None,/*NULL,*/"post",    AX_GREY_LIGHT, tal_Center|tal_Top  ));
        mEditor = E;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* tempeditor = mEditor;
        mEditor = NULL;
        delete tempeditor;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        meter->mLevel = gainred;
        mEditor->onChange(meter);
        mEditor->redrawDirty();
      }

    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: threshold = f*f*f;      break;
          case 1: ratio     = 1/f;        break;
          case 2: attack    = 1/(f*f*f);  break;
          case 3: release   = 1/(f*f*f);  break;
          case 4: pregain   = f*f*f;      break;
          case 5: postgain  = f*f*f;      break;
        }
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //vu = 0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        float target;
        //float input = axMax(fabs(spl0),fabs(spl1));
        float input = fabs( (spl0+spl1)*0.5 );
        input *= pregain;
        float diff = input - threshold;
        if (diff>0) target = threshold + (diff*ratio);
        else target = 1;
        if (value>target) value += (target-value)*attack;
                     else value += (target-value)*release;
        spl0 *= value;
        spl1 *= value;
        *outs[0] = spl0 * postgain;
        *outs[1] = spl1 * postgain;
        gainred = 1-value;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
