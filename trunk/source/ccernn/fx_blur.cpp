#include "format/axFormat.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

char* str_onoff[] =
{
  (char*)"off",
  (char*)"on"
};

//----------

class myPlugin : public axFormat
{
  private:
    int         pos;
    float*      BUF;
    float*      BUF_t;
    float       bufsize, bufsize_t;
    float       decay, decay_t;
    float       decay2;
    float       vol;
    float       bs,d,d2,v,fr;
  public:
    parFloat*   p_Size;
    parFloat*   p_Decay;
    parFloat*   p_Xfade;
    parFloat*   p_Volume;
    parInteger* p_Freeze;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {

        BUF     = new float[0x40000];
        axMemset(BUF,0,0x30000*4);
        BUF_t   = BUF + 0x10000;
        pos     = 0;
        bufsize = 0;
        decay   = 0;
        //
        float srate = getSampleRate();
        bufsize_t = srate * (bs*bs);
        decay_t   = d*d;
        decay2    = d2*d2;
        vol       = v*v;

        describe("fx_blur","ccernn","axonlib example",0,AX_MAGIC+0x1008);
        setupAudio(2,2);
        //appendParameter( new axParameter(this,"gain","",0) );
        mParameters.append( p_Size   = new parFloat(  this,"size",  "", 0.5,  0,1 ) );
        mParameters.append( p_Decay  = new parFloat(  this,"decay", "", 0.2,  0,1 ) );
        mParameters.append( p_Xfade  = new parFloat(  this,"xfade", "", 0.5,  0,1 ) );
        mParameters.append( p_Volume = new parFloat(  this,"volume","", 0.7,  0,1 ) );
        mParameters.append( p_Freeze = new parInteger(this,"freeze","", 0,    0,1, str_onoff)  );
        setupParameters();
      }

    virtual ~myPlugin()
      {
        delete BUF;
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        //if (aParameter->getIndex()==0) m_Gain = aParameter->getValue();
        float srate = getSampleRate();
        int id  = aParameter->getIndex();
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            bs = f / 2;
            bufsize_t = srate * (bs*bs);
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

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];

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

        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_ENTRYPOINT(myPlugin)
