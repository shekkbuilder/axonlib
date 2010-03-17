#define AX_PLUGIN     myEffect
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  4
//#define AX_WIDTH      150
//#define AX_HEIGHT     180
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

//#define AX_DEBUG
//#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"
#include "wdgPanel.h"
#include "wdgKnob.h"

//----------------------------------------------------------------------

#define MIN_BPM             30.0f
#define MAX_SRATE           192000
#define MAX_NUMBEATS        4
#define MAX_BeatsPerSecond  ( MIN_BPM / 60.0f )
#define MAX_SecondsPerBeat  ( 60.0f / MIN_BPM )
#define MAX_SamplesPerSec   ( MAX_SRATE * 2 )
#define MAX_BeatSize        ( MAX_SamplesPerSec / MAX_BeatsPerSecond )
#define MAX_BUFFER_SIZE     ( MAX_NUMBEATS * MAX_BeatSize )

class myEffect : public axEffect
{
  public:
    float     BUFFER[1000000];//MAX_BUFFER_SIZE];
    int       bufsize,pos;
    float     beats,fb,dry,wet;
    wdgKnob   *k1,*k2,*k3,*k4;
    parFloat  *p1,*p2,*p3,*p4;
  public:

    myEffect(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_tempodelay0","ccernn","axonlib example plugin",0,0);
        setupAudio(2,2);
        setupEditor(150,180);//(AX_WIDTH,AX_HEIGHT);
        appendParameter( p1 = new parFloat( this,0,"beats",   "", 0.75, 0.25, MAX_NUMBEATS, 0.25  ) );
        appendParameter( p2 = new parFloat( this,1,"feedback","", 0.75  ) );
        appendParameter( p3 = new parFloat( this,2,"dry",     "", 1.00 ) );
        appendParameter( p4 = new parFloat( this,3,"wet",     "", 0.75 ) );
        processParameters();
        bufsize = 0;
        pos = 0;
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: beats = f;  break;
          case 1: fb    = f;  break;
          case 2: dry   = f;  break;
          case 3: wet   = f;  break;
        }
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        float beatsize = (60/mTempo) * mSampleRate;
        int size = beatsize * beats;
        bufsize = size;
        if (pos>=bufsize) pos=0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float in0 = *ins[0];
        float in1 = *ins[1];
        int pos2 = pos*2;
        float spl0 = BUFFER[pos2  ];
        float spl1 = BUFFER[pos2+1];
        BUFFER[pos2  ] = in0 + (spl0*fb);
        BUFFER[pos2+1] = in1 + (spl1*fb);
        pos+=1;
        if( pos>=bufsize ) pos=0;
        *outs[0] = in0*dry + spl0*wet;
        *outs[1] = in1*dry + spl1*wet;
      }

    //----------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* P;
        aEditor->appendWidget( P = new wdgPanel(this, -1, NULL_RECT,wal_Client) );
          P->appendWidget( k1 = new wdgKnob( this, -1, axRect( 10, 10, 128,32 ), wal_None ) );
          P->appendWidget( k2 = new wdgKnob( this, -1, axRect( 10, 50, 128,32 ), wal_None ) );
          P->appendWidget( k3 = new wdgKnob( this, -1, axRect( 10, 90, 128,32 ), wal_None ) );
          P->appendWidget( k4 = new wdgKnob( this, -1, axRect( 10,130, 128,32 ), wal_None ) );
        aEditor->doRealign();
        aEditor->connect( k1, p1 );
        aEditor->connect( k2, p2 );
        aEditor->connect( k3, p3 );
        aEditor->connect( k4, p4 );
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
