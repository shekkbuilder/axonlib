#define AX_PLUGIN     myPlugin
#define AX_PLUGIN_AUTOSYNC
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  5
#define AX_WIDTH      ( 10+ 100           +(20*2) )
#define AX_HEIGHT     ( 10+ (32*5)+(10*4) +(20*2) )
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "parInteger.h"
//#include "wdgLabel.h"
//#include "wdgValuePar.h"
#include "wdgKnobPar.h"

//----------------------------------------------------------------------

#define MAX_SRATE           192000
#define MAX_SamplesPerSec   ( MAX_SRATE * 2 )
#define MAX_BUFFER_SIZE     (int)( MAX_SamplesPerSec )

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    float BUFFER[MAX_BUFFER_SIZE];
    int   bufsize;
    int   pos;
    int mindelay,maxdelay,speed,num,offset;
  public:
    axEditor    *mEditor;
    //parFloat    *pBeats,*pFB,*pDry,*pWet;
    //wdgKnobPar  *wBeats,*wFB,*wDry,*wWet;
    parFloat    *pMinDelay; // min delay
    parFloat    *pMaxDelay; // max delay
    parFloat    *pSpeed;    // modulation speed
    parInteger  *pNum;      // num stages/taps
    parFloat    *pOffset;   // phase offset
    wdgKnobPar  *wMinDelay,*wMaxDelay,*wSpeed,*wNum,*wOffset;
  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_moddelay0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        mindelay  = 0;
        maxdelay  = 0;
        speed     = 0;
        num       = 0;
        offset    = 0;
        appendParameter( pMinDelay = new parFloat(  this,0,"min delay","", mindelay  ) );
        appendParameter( pMaxDelay = new parFloat(  this,1,"max delay","", maxdelay  ) );
        appendParameter( pSpeed    = new parFloat(  this,2,"speed",    "", speed     ) );
        appendParameter( pNum      = new parInteger(this,3,"num",      "", num       ) );
        appendParameter( pOffset   = new parFloat(  this,4,"offset",   "", offset    ) );
        bufsize = 0;
        pos     = 0;
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor( this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
        E->setLayout(10,10,10,10);
        E->setFlag(wfl_Vertical);
        E->appendWidget( wMinDelay = new wdgKnobPar(  E, 0, axRect(0,0,100,32), wal_Stacked, pMinDelay ) );
        E->appendWidget( wMaxDelay = new wdgKnobPar(  E, 1, axRect(0,0,100,32), wal_Stacked, pMaxDelay ) );
        E->appendWidget( wSpeed    = new wdgKnobPar(  E, 2, axRect(0,0,100,32), wal_Stacked, pSpeed ) );
        E->appendWidget( wNum      = new wdgKnobPar(  E, 3, axRect(0,0,100,32), wal_Stacked, pNum ) );
        E->appendWidget( wOffset   = new wdgKnobPar(  E, 4, axRect(0,0,100,32), wal_Stacked, pOffset ) );
        E->doRealign();
        wMinDelay->doSetValue( pMinDelay->doGetValue() );
        wMaxDelay->doSetValue( pMaxDelay->doGetValue() );
           wSpeed->doSetValue( pSpeed->doGetValue() );
             wNum->doSetValue( pNum->doGetValue() );
          wOffset->doSetValue( pOffset->doGetValue() );
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
        mEditor->redrawDirty();
      }

    //----------

    virtual void doProcessState(int aState)
      {
        TRACE("DoProcessState: %i\n",aState);
      }

    //----------

    virtual void doProcessTransport(int aState)
      {
        TRACE("doProcessTransport: %i\n",aState);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mindelay  = f;  break;
          case 1: maxdelay  = f;  break;
          case 2: speed     = f;  break;
          case 3: num       = f;  break;
          case 4: offset    = f;  break;
        }
      }

    //----------

    virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
      {
        TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        ////updateTimeInfo(); // AX_PLUGIN_AUTOSYNC
        //float beatsize = (60/mTempo) * mSampleRate;
        //int size = beatsize * beats;
        //bufsize = size;
        //if( pos>=bufsize ) pos = 0;
        return false; // we want doProcessSample calls
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float in0 = *ins[0];
        float in1 = *ins[1];
        //------------------------------
        //int pos2 = pos*2;
        //float spl0 = BUFFER[pos2  ];
        //float spl1 = BUFFER[pos2+1];
        //BUFFER[pos2  ] = in0 + (spl0*fb);
        //BUFFER[pos2+1] = in1 + (spl1*fb);
        //pos+=1;
        //if( pos>=bufsize ) pos=0;
        //------------------------------
        *outs[0] = in0;//*dry + spl0*wet;
        *outs[1] = in1;//*dry + spl1*wet;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
