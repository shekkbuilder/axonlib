#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  5

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

char* str_onoff[] =
{
  (char*)"off",
  (char*)"on"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{

  private:
    int     pos;
    float*  BUF;
    float*  BUF_t;
    float   bufsize, bufsize_t;
    float   decay, decay_t;
    float   decay2;
    float   vol;
    float   bs,d,d2,v,fr;
  public:
    parFloat    *pSize;
    parFloat    *pDecay;
    parFloat    *pXfade;
    parFloat    *pVolume;
    parInteger  *pFreeze;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_blur0","ccernn","product_string",0,1);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        mParameters.append( pSize   = new parFloat(  this,0,"size",  "", 0.5,  0,1, NULL)  );
        mParameters.append( pDecay  = new parFloat(  this,1,"decay", "", 0.2,  0,1, NULL)  );
        mParameters.append( pXfade  = new parFloat(  this,2,"xfade", "", 0.5,  0,1, NULL)  );
        mParameters.append( pVolume = new parFloat(  this,3,"volume","", 0.7,  0,1, NULL)  );
        mParameters.append( pFreeze = new parInteger(this,4,"freeze","", 0,    0,1, str_onoff)  );
        processParameters();
        //

        BUF     = new float[0x40000];
        memset(BUF,0,0x30000*4);
        BUF_t   = BUF + 0x10000;
        pos     = 0;
        bufsize = 0;
        decay   = 0;
        //
        float srate = updateSampleRate();
        bufsize_t = srate * (bs*bs);
        decay_t   = d*d;
        decay2    = d2*d2;
        vol       = v*v;

      }

    //parameters is automatically deleted
    virtual ~myPlugin()
      {
        delete BUF;
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

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id  = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            bs = f / 2;
            bufsize_t = mSampleRate * (bs*bs);
            break;
          case 1:
            d = f;
            decay_t = d*d;
            break;
          case 2:
            d2 = f / 10;
            decay2 = d2*d2;
            break;
          case 3:
            v = f * 2;
            vol = v*v;
            break;
          case 4:
            fr = f;
            break;
        }
      }

    //----------

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    TRACE("DoProcessMidi: %i - %i,%i,%i\n",osg,msg1,msg2,msg3);
    //  }

    //----------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        bufsize += ( bufsize_t - bufsize ) * decay2;
        decay   += ( decay_t   - decay   ) * decay2;

        if( fr<0.5 ) BUF_t[pos] = spl0;
        BUF[pos] += ( BUF_t[pos] - BUF[pos] ) * decay;
        spl0 = BUF[pos] * vol;

        int po2 = pos + 0x20000;

        if( fr<0.5 ) BUF_t[po2] = spl1;
        BUF[po2] += ( BUF_t[po2] - BUF[po2] ) * decay;
        spl1 = BUF[po2] * vol;

        pos += 1;
        if( pos >= (int)bufsize ) pos=0;

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"



