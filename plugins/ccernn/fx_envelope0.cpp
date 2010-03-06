#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2
#define AX_FLAGS    (AX_EMBEDDED|AX_BUFFERED)

//#define AX_DEBUG
//#include "axDebug.h"
#include "axMath.h"

#include "axPlugin.h"
#include "parFloat.h"

//----------------------------------------------------------------------

#define MAX_EVENTS 65536

class myPlugin : public axPlugin
{
  public:
    parFloat* pAttack;
    parFloat* pRelease;
    float*    EVENTS;
    int       offset;
    float     mAttack;
    float     mRelease;
    float     att_c, att_t;
    float     rel_c, rel_t;
    float     out;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_envelope0","ccernn","product_string",0,0);
        setupAudio(2,2);
        EVENTS = new float[MAX_EVENTS];
        for (int i=0; i<MAX_EVENTS; i++) EVENTS[i] = -1;
        //i16 = 1/16;
        //i127 = 1/127;
        //i256 = 1/256;
        //slider1: 5  <1,20,1> attack
        //slider2: 15 <1,25,1> release
        appendParameter( pAttack  = new parFloat(this,0,"attack", "", 5,  1,20 ) );
        appendParameter( pRelease = new parFloat(this,1,"release","", 15, 1,25 ) );
        processParameters();
        att_c = 0;
        rel_c = 1;
        att_t = 0;
        rel_t = 1;
        out   = 0;
      }

    //----------

    virtual ~myPlugin()
      {
        delete[] EVENTS;
      }

    //----------

    //attack  = 1 / max(1,slider1*slider1*slider1);
    //release = 1 / max(1,slider2*slider2*slider2);

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mAttack  = 1/axMax(1,f*f*f); break;
          case 1: mRelease = 1/axMax(1,f*f*f); break;
        }
      }

    //----------

    //@block
    //  while(
    //    midirecv(ofs,msg1,msg23) ? (
    //      msg = (msg1&240) * i16;
    //      msg==9 ? (
    //      //note =  msg23       & 127;
    //      //chn  = (msg1&15);
    //      //vel  = (msg23*i256) & 127;
    //        EVENTS[ofs] = 1; //vel*i127;
    //      ) :
    //      msg==8 ? (
    //        EVENTS[ofs] = 0;
    //      );
    //      midisend(ofs,msg1,msg23);
    //    );
    //  );
    //  offset = 0;

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        int msg = (msg1 & 240) * inv16;
        //TRACE("midi msg: %i\n",msg);
        if (msg==9) EVENTS[ofs] = 1;
        else if (msg==8) EVENTS[ofs] = 0;
        sendMidi(ofs,msg1,msg2,msg3);
      }

    virtual bool doProcessBlock(float** ins, float** outs, long samplesFrames)
      {
        offset = 0;
        return false;
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        int e = EVENTS[offset];
        if( e==1)
        {
          att_c = 0;
          att_t = 1;
          rel_c = 1;
          rel_t = 1;
        }
        else if (e==0)
        {
          rel_c = 1;
          rel_t = 0;
        }
        EVENTS[offset] = -1;
        offset += 1;

        out = att_c * rel_c;
        att_c += (att_t-att_c) * mAttack;
        rel_c += (rel_t-rel_c) * mRelease;
        *outs[0] = *ins[0] * out;
        *outs[1] = *ins[1] * out;
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
