#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  2
//#define AX_AUTOSYNC

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"

//----------------------------------------------------------------------

#define MAX_EVENTS 65536

class myPlugin : public axPlugin
{
  public:
    float EVENTS[MAX_EVENTS];
    int   offset;
    int   midicc, aout;
    float out;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("midi_ccaudio0","ccernn","axonlib example plugin",0,0);
        setupAudio(2,2);
        for (int i=0; i<MAX_EVENTS; i++) EVENTS[i] = -1;
        out = 0;
        appendParameter(new parInteger(this,0,"midi cc",    "", 60, 0,127 ));
        appendParameter(new parInteger(this,1,"audio chan", "", 0,  0,1   ));
        processParameters();
      }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        int msg = msg1 & 240;
        if (msg==11*16)
        {
          int cc = msg2 & 127;
          if (cc==midicc)
          {
            float val = msg3;// & 127;
            EVENTS[ofs] = val * inv127;
          }
        }
        sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: midicc = f;  break;
          case 1: aout   = f;  break;
        }
      }

    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        offset = 0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        if (EVENTS[offset]>=0) out=EVENTS[offset];
        EVENTS[offset] = -1;
        offset += 1;
        *outs[0] = 0;
        *outs[1] = 0;
        *outs[aout] = out;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"



