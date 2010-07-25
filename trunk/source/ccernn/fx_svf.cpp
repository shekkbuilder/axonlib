#define AX_NOGUI

#include "base/axBase.h"
#include "dsp/dspSVF.h"

//----------------------------------------------------------------------

char* str_flt[] =
{
  (char*)"off",
  (char*)"lowpass",
  (char*)"highpass",
  (char*)"bandpass",
  (char*)"notch"
};

//----------

axParamInfo param_infos[] =
{
//  type           name    def,min,max,step,str,     aux
  { pa_Int, (char*)"mode", 0,  0,  4,  1,   str_flt    },
  { pa_Pow, (char*)"freq", 1,  0,  1,  0,   NULL,    3 },
  { pa_Pow, (char*)"bw",   1,  0,  1,  0,   NULL,    2 }
};

//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    DESCRIPTOR_CONSTRUCT(myDescriptor);
    DESCRIPTOR_NAME(fx_svf)
    DESCRIPTOR_AUTHOR(ccernn)
    DESCRIPTOR_PRODUCT(axonlib example plugin)
    DESCRIPTOR_VERSION(0)
    DESCRIPTOR_UNIQUEID(AX_MAGIC + 0x0000)
    DESCRIPTOR_NUMPARAMS(3)
    DESCRIPTOR_PARAMINFOS(param_infos)
};

//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
    dspSVF svf1,svf2;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        svf1.setup(0,1,1);
        svf2.setup(0,1,1);
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0: // mode
            svf1.mMode = val;
            svf2.mMode = val;
            break;
          case 1: // freq
            svf1.mFreq = val;
            svf2.mFreq = val;
            break;
          case 2: // bw
            svf1.mBW = val;
            svf2.mBW = val;
            break;
        }
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = svf1.process(aInputs,aOutputs);
        *aOutputs[1] = svf2.process(aInputs,aOutputs);
      }

};

//----------------------------------------------------------------------
AX_MAIN(myDescriptor,myInstance)
