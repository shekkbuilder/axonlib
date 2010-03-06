#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  1

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"

class myPlugin : public axPlugin
{
  public:

    int OFFSETS[128];
    int length;
    int ignore;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("midi_notelen0","ccernn","product_string",0,0);
        setupAudio(2,2);
        appendParameter( new parFloat(this,0,"note length", "ms", 100, 0,1000 ) );
        processParameters();
        for( int i=0; i<128; i++ ) OFFSETS[i]=-1;
      }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        if( id==0 ) length = mSampleRate * (f/1000);
      }

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        if( msg1==9*16 )
        {
          OFFSETS[(int)msg2] = length + ofs;
          sendMidi(ofs,msg1,msg2,msg3);
        }
        else if( msg1==8*16 ) { ignore=1; }
        else sendMidi(ofs,msg1,msg2,msg3);
      }

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        for( int n=0; n<128; n++ )
        {
          if( OFFSETS[n]>=0 )
          {
            if( OFFSETS[n]<sampleFrames )
            {
              sendMidi( OFFSETS[n], (8*16), n, 0 );
              OFFSETS[n] = -1;
            } else OFFSETS[n] -= sampleFrames;
          }
        }
        return false;
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }
};

#include "axMain.h"
