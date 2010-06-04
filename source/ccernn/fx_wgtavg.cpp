
#include "axFormat.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

char* str_mode[] =
{
  (char*)"lp", (char*)"hp"
};

class myPlugin : public axFormat
{
  private:

    int   m_Mode;
    float m_Weight;
    float z0,z1;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext)
      {
        describe("fx_wgtavg","ccernn","axonlib example",0,AX_MAGIC+0x1005);
        setupAudio(2,2);
        appendParameter( new parInteger(this,"mode",  "", 0, 0,1, str_mode) );
        appendParameter( new parFloat3( this,"weight","", 0.5) );
        setupParameters();
        z0=z1=0;
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0: m_Mode   = (int)val; break;
          case 1: m_Weight = val;      break;
        }
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        SPL spl0 = *aInputs[0];
        SPL spl1 = *aInputs[1];
        z0 += ((spl0-z0)*m_Weight);
        z1 += ((spl1-z1)*m_Weight);
        switch (m_Mode)
        {
          case 0: // lowpass
            spl0 = z0;
            spl1 = z1;
            break;
          case 1: // highpass
            spl0 -= z0;
            spl1 -= z1;
            break;
        }
        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_ENTRYPOINT(myPlugin)
