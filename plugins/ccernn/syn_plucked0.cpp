#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2
//#define AX_AUTOSYNC
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"

//----------------------------------------------------------------------

#define MAX_BUFSIZE 192000

//----------

class myPlugin : public axPlugin
{
  public:
    float *buf;
    float frac, z0;
    int   noi, pos, size;
    float damp1, damp2;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("syn_plucked0","ccernn","product_string",0,0);
        setNumInputs(0);
        isSynth();
        axRandomize(418);
        buf  = new float[MAX_BUFSIZE];
        noi  = 0;
        pos  = 0;
        size = 0;
        //damp1 = 0.5;
        //damp2 = 0.99;
        frac = 0;
        z0   = 0;
        appendParameter(new parFloat(this,0,"damp1 freq","",0.5));
        appendParameter(new parFloat(this,1,"damp2 ampl","",0.99));
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        delete[] buf;
      }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //TODO: EVENTS list for sample offsets
        float srate = updateSampleRate();
        int m = msg1 & 240;
        if (m==9*16)
        {
          int   note = msg2;// & 127;
          //int   vel  = msg3;// & 127);
          //float vol  = (float)vel * inv127;
          float freq = 440 * pow(2.0,(note-69.0)/12.0);
          float dly  = srate/freq;
          size = floor(dly);
          frac = dly-size;
          pos  = 0;
          noi = size;
        } // noteon?
        //sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: damp1 = f*f;  break;
          case 1: damp2 = f;    break;
        }
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float d0;
        noi -= 1;
        if (noi>0) d0 = axRandomSigned(); else d0 = buf[pos];
        float i0 =  (d0*(1-frac )) + (z0*frac );
        buf[pos] = ((d0*(1-damp1)) + (z0*damp1)*damp2);
        z0 = d0;
        pos += 1;
        if (pos>=size) pos=0;
        *outs[0] = i0;
        *outs[1] = i0;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
