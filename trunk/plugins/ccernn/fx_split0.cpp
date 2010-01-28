#define AX_PLUGIN     myPlugin
#define AX_PLUGIN_VST
#define AX_PLUGIN_MULTI
#define AX_NUMINPUTS  2
#define AX_NUMOUTPUTS 6
#define AX_SYNTH      0
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  7

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

char* str_listen[] =
{
  (char*)"bypass",
  (char*)"mix",
  (char*)"low",
  (char*)"med",
  (char*)"hi"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    int  listen;
    float volume;
    float freqlo_0, freqlo_1, freqlo_d;
    float freqhi_0, freqhi_1, freqhi_d;
    float gainlo,   gainmid,  gainhi;

  public:
    parInteger *pListen;
    parFloat   *pFreqLo,*pFreqHi;
    parFloat   *pGainLo,*pGainMid,*pGainHi;
    parFloat   *pFilter,*pVolume;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_split0","ccernn","product_string",0,1);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        setNumInputs(2);
        setNumOutputs(6);
        appendParameter( pVolume  = new parFloat(  this, 0, "volume",   "", 1,    0, 2  ) );
        appendParameter( pListen  = new parInteger(this, 1, "listen",   "", 0,    0, 4, str_listen ) );
        appendParameter( pFreqLo  = new parFloat(  this, 2, "freq lo",  "", 0.30, 0, 1  ) );
        appendParameter( pFreqHi  = new parFloat(  this, 3, "freq hi",  "", 0.70, 0, 1  ) );
        appendParameter( pGainLo  = new parFloat(  this, 4, "gain lo",  "", 1,    0, 2  ) );
        appendParameter( pGainMid = new parFloat(  this, 5, "gain mid", "", 1,    0, 2  ) );
        appendParameter( pGainHi  = new parFloat(  this, 6, "gain hi",  "", 1,    0, 2  ) );

        volume    = 1;
        listen    = 0;
        freqlo_0  = 0;  freqlo_d  = (0.30*0.30*0.30);
        freqhi_0  = 0;  freqhi_d  = (0.70*0.70*0.70);
        freqlo_1  = 0;
        freqhi_1  = 0;
        gainlo    = 1;
        gainmid   = 1;
        gainhi    = 1;

      }

    //parameters is automatically deleted
    //virtual ~myPlugin() {}

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
        //TRACE("param %i = %f\n",id,f);
        switch(id)
        {
          case  0: volume   = f*f;    break;
          case  1: listen   = (int)f; break;
          case  2: freqlo_d = f*f*f;  break;
          case  3: freqhi_d = f*f*f;  break;
          case  4: gainlo   = f*f;    break;
          case  5: gainmid  = f*f;    break;
          case  6: gainhi   = f*f;    break;
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
// left
        float in  = *ins[0];
        freqlo_0 += (in-freqlo_0)*freqlo_d;
        freqhi_0 += (in-freqhi_0)*freqhi_d;
        float lo  = freqlo_0;
        float hi  = in - freqhi_0;
        float mid = in - lo - hi;
        lo        = (lo  * gainlo );
        mid       = (mid * gainmid);
        hi        = (hi  * gainhi );
        //float out;
        //switch(listen)
        //{
        //  case 0: out = in;           break;
        //  case 1: out = (lo+mid+hi);  break;
        //  case 2: out = lo;           break;
        //  case 3: out = mid;          break;
        //  case 4: out = hi;           break;
        //}
        //*outs[0] = out*volume;
        *outs[0] = lo*volume;
        *outs[2] = mid*volume;
        *outs[4] = hi*volume;
// right
        in        = *ins[1];
        freqlo_1 += (in-freqlo_1)*freqlo_d;
        freqhi_1 += (in-freqhi_1)*freqhi_d;
        lo        = freqlo_1;
        hi        = in - freqhi_1;
        mid       = in - lo - hi;
        lo        = (lo  * gainlo );
        mid       = (mid * gainmid);
        hi        = (hi  * gainhi );
        //switch(listen)
        //{
        //  case 0: out=in;           break;
        //  case 1: out=(lo+mid+hi);  break;
        //  case 2: out=lo;           break;
        //  case 3: out=mid;          break;
        //  case 4: out=hi;           break;
        //}
        //*outs[1] = out*volume;
        *outs[1] = lo*volume;
        *outs[3] = mid*volume;
        *outs[5] = hi*volume;
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
