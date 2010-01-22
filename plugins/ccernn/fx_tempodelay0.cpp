#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  4
#define AX_WIDTH      (10+100+(20*2))
#define AX_HEIGHT     (10+(32*4)+(10*3)+(20*2))
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_AUTOSYNC

#define AX_DEBUG

#include "axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"

#include "wdgLabel.h"
#include "wdgValue.h"
#include "wdgKnob.h"

//----------------------------------------------------------------------

#define MIN_BPM             30.0f
#define MAX_SRATE           192000
#define MAX_NUMBEATS        4
#define MAX_SecondsPerBeat  ( 60.0f / MIN_BPM )
#define MAX_SamplesPerSec   ( MAX_SRATE * 2 )
#define MAX_BUFFER_SIZE     (int)( MAX_NUMBEATS * MAX_SecondsPerBeat * MAX_SamplesPerSec  )

class myPlugin : public axPlugin
{
  public:
    float     BUFFER[MAX_BUFFER_SIZE];
    int       bufsize;
    int       pos;
    float     beats,fb,dry,wet;
    axEditor* mEditor;

    wdgKnob *k1,*k2,*k3,*k4;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("fx_tempodelay0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter( new parFloat( this,0,"beats",   "", 0.75, 0.25, MAX_NUMBEATS, 0.25  ) );
        appendParameter( new parFloat( this,1,"feedback","", 0.75  ) );
        appendParameter( new parFloat( this,2,"dry",     "", 1.00 ) );
        appendParameter( new parFloat( this,3,"wet",     "", 0.75 ) );
        processParameters();
        bufsize = 0;
        pos     = 0;
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor( "fx_tempodelay_editor", this, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), AX_FLAGS );
        //E->setLayout(0,0,10,10);
        //E->setFlag(wfl_Vertical);
        E->appendWidget( k1 = new wdgKnob( E, 0, axRect( 10, 10, 128,32 ), wal_None/*,mParameters[0]*/ ) );
        E->appendWidget( k2 = new wdgKnob( E, 1, axRect( 10, 50, 128,32 ), wal_None/*,mParameters[1]*/ ) );
        E->appendWidget( k3 = new wdgKnob( E, 2, axRect( 10, 90, 128,32 ), wal_None/*,mParameters[2]*/ ) );
        E->appendWidget( k4 = new wdgKnob( E, 3, axRect( 10,130, 128,32 ), wal_None/*,mParameters[3]*/ ) );
        //E->updateWidgetValues(); // connect by wdg/par.mID !!
        E->connect( k1, mParameters[0] );
        E->connect( k2, mParameters[1] );
        E->connect( k3, mParameters[2] );
        E->connect( k4, mParameters[3] );
        mEditor = E;
        //TRACE("fx_tempodelay.mEditor = %x\n",(int)mEditor);
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if(mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    //TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessTransport(int aState)
    //  {
    //    //TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    //TRACE("doProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
//lock
        switch(id)
        {
          case 0: beats = f;  break;
          case 1: fb    = f;  break;
          case 2: dry   = f;  break;
          case 3: wet   = f;  break;
        }
//unlock
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //updateTimeInfo(); // AX_AUTOSYNC
        float beatsize = (60/mTempo) * mSampleRate;
        int size = beatsize * beats;
        bufsize = size;
        if( pos>=bufsize ) pos=0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float in0 = *ins[0];
        float in1 = *ins[1];
        //------------------------------
        int pos2 = pos*2;
        float spl0 = BUFFER[pos2  ];
        float spl1 = BUFFER[pos2+1];
        BUFFER[pos2  ] = in0 + (spl0*fb);
        BUFFER[pos2+1] = in1 + (spl1*fb);
        pos+=1;
        if( pos>=bufsize ) pos=0;
        //------------------------------
        *outs[0] = in0*dry + spl0*wet;
        *outs[1] = in1*dry + spl1*wet;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"