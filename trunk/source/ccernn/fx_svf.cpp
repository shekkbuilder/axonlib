
#include "axFormat.h"
#include "par/parFloat.h"
#include "par/parInteger.h"
#include "dsp/dspSVF.h"

char* str_filter[] =
{
  (char*)"off",
  (char*)"lowpass",
  (char*)"highpass",
  (char*)"bandpass",
  (char*)"notch"
};

class myPlugin : public axFormat
{
  private:
    dspSVF svf1,svf2;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_None)
      {
        describe("fx_svf","ccernn","axonlib example",0,AX_MAGIC+0x1006);
        setupAudio(2,2);
        appendParameter( new parInteger(this,"mode", "", 0, 0,4, str_filter ) );
        appendParameter( new parFloat2( this,"freq", "", 1 ) );
        appendParameter( new parFloat2( this,"bw",   "", 1 ) );
        setupParameters();
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

AX_ENTRYPOINT(myPlugin)
