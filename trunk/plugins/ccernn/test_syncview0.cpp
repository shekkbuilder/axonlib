#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   16
#define AX_NUMPARAMS  3

//#define AX_AUTOSYNC
#define AX_WIDTH      220
#define AX_HEIGHT     200
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"

#include "parFloat.h"
#include "parInteger.h"

#include "dspBuffer.h"
#include "wdgScope.h"

#include "wdgImgKnob.h"
//#include "wdgVuMeter.h"
//#include "wdgLabel.h"

#include "axBitmapLoader.h"
//#include "images/knob1.h"   // 20*20, 65, 6668
#include "images/knob2.h"   // 32*32, 65, 15255
//#include "images/button1.h" // 12*12, 3,  647

//----------------------------------------------------------------------

#include "wdgImgKnob2.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:

    axEditor    *mEditor;
    bool        is_gui_initialized;

    axSurface*  srfKnob;
    wdgImgKnob* wBeats;
    wdgImgKnob* wSlices;

    wdgImgKnob2* k2;

    wdgScope*   scope;
    dspBuffer*  buffer;

    int numbeats;
    int numslices;

    int beatsize;
    int buffersize;
    int slicesize;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        srfKnob = NULL;
        is_gui_initialized = false;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("test_syncview0","ccernn","product_string",0,0);
        //isSynth();
        appendParameter(new parInteger( this, 0, "beats", "", 4, 1,16 ));
        appendParameter(new parInteger( this, 1, "slices","", 1, 1,16 ));
        appendParameter(new parFloat(   this, 2, "test",  "", 0 ));
        processParameters();

        buffer = new dspBuffer();
        buffer->create(1024);
      }

    virtual ~myPlugin()
      {
        delete buffer;
        if (srfKnob) delete srfKnob;
      }

    //--------------------------------------------------

    //TODO: save programs with host project, chunks
    //TODO2: -> axPlugin
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
          srfKnob = loadPng( knob2, 15255 );
          is_gui_initialized=true;
        }
        E->appendWidget(new wdgImgKnob( E,0,axRect(  30, 70, 32,32),wal_None,/*mParameters[ 0],*/ 65, srfKnob ));
        E->appendWidget(new wdgImgKnob( E,1,axRect(  70, 70, 32,32),wal_None,/*mParameters[ 1],*/ 65, srfKnob ));
        E->appendWidget(k2 = new wdgImgKnob2(E,2,axRect(  70,110, 32,64),wal_None,/*mParameters[ 1],*/ 65, srfKnob ));
        //for (int i=0;i<AX_NUMPARAMS; i++) E->connect( E->mWidgets[i], mParameters[i] );
        E->connect( E->mWidgets[0], mParameters[0] );
        E->connect( E->mWidgets[1], mParameters[1] );
        E->connect( k2->knob, mParameters[2] );

        E->appendWidget( scope = new wdgScope(E,-1,axRect(  10, 10, 200,50),wal_None ));
        scope->setup( buffer->getBuffer(), buffer->getLength(), 0);
        scope->mDrawFlags |= (wbf_Slices|wbf_Cursors);

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
        scope->mNumSlices = numbeats*numslices;
        scope->mNumCursors = 1;
        scope->mCursors[0].pos = (float)buffer->getIndex() / (float)buffer->getLength();
        scope->mCursors[0].color = AX_RED;
        //meter->mLevel = gainred;
        mEditor->onChange(scope);
        mEditor->redrawDirty();
      }

    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: numbeats  = (int)f; break;
          case 1: numslices = (int)f; break;
        }
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        beatsize = (60/mTempo) * mSampleRate;
        buffersize = beatsize * numbeats;
        slicesize = beatsize * numslices;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        buffer->doProcessSample(ins,outs);
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
