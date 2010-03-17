#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
//#define AX_AUTOSYNC
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

//----------------------------------------------------------------------

#define MAXEVENTS 1024

class myPlugin : public axPlugin
{
  public:
    //axEditor* mEditor;
    unsigned char EVENTS[MAXEVENTS*4];
    int numevents;
    int prevstate;
    int isplaying;
    int countdown;

    float mBeats;

    float speed;
    int offset;
    int block;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //mEditor = NULL;
        describe("midi_syncdelay0","ccernn","axonlib example plugin",0,0);
        setupAudio(2,2);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter(new parFloat(this,0,"beats","",4, 0.25, 16, 0.25 ));
        processParameters();
        numevents = 0;
        prevstate = 0;
        isplaying = 0;
        countdown = 0;

      }

    //----------

    //virtual ~myPlugin()
    //  {
    //  }

    //----------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //    if(mEditor) mEditor->onChange(aParameter);
    //    doProcessParameter(aParameter);
    //  }

    //--------------------------------------------------

    //virtual axWindow* doCreateEditor(void)
    //  {
    //    axEditor* E = new axEditor(this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
    //    E->setLayout(10,10,5,5);
    //    //E->setFlag(wfl_Vertical);
    //    E->appendWidget(new wdgKnobPar(E,0,axRect(0,0,100,32),wal_Stacked,mParameters[0]));
    //    E->updateWidgetValues();
    //    E->doRealign();
    //    mEditor = E;
    //    return mEditor;
    //  }

    //----------

    //virtual void doDestroyEditor(void)
    //  {
    //    axEditor* tempeditor = mEditor;
    //    mEditor = NULL;
    //    delete tempeditor;
    //  }

    //----------

    //virtual void doIdleEditor(void)
    //  {
    //    mEditor->redrawDirty();
    //  }

    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    // if has editor
    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        // buffer incoming midi events
        if (numevents<MAXEVENTS)
        {
          EVENTS[numevents*4  ] = msg1;
          EVENTS[numevents*4+1] = msg2;
          EVENTS[numevents*4+2] = msg3;
          numevents+=1;
        }
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mBeats = f;  break;
        }
      }

    //----------


    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo(); // AX_AUTOSYNC
        speed = mBeats * mSampleRate / (mTempo/60);
        //ms =  beats * (60/tempo) * 1000;
        if (speed<64) speed=64;

        mPlayState &= 0xfe;
        if (mPlayState!=prevstate)
        {
          if (mPlayState&2) // 2=play?
          {
            isplaying = 1;
            countdown = 0; // speed;
            //numevents = 0;
            //cycle = 0;//cyclecount;
          } else isplaying=0;
          prevstate = mPlayState;
        }

        if (isplaying)
        {
          offset = 0;
          block = mBlockSize;
          while (block>0)
          {
            if (countdown>=block)
            {
              countdown -= block;
              block = 0;
            }
            else
            {
              offset += countdown;
              block -= countdown;
              //E = EVENTS;
              //loop( numevents,
              //  midisend(offset,E[0],E[1]);
              //  E += 2;
              //);
              for( int i=0;i<numevents;i++ ) sendMidi(offset,EVENTS[i*4],EVENTS[i*4+1],EVENTS[i*4+2]);
              numevents = 0;
              countdown = speed;
            }
          }

        }

        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
