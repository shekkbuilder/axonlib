#include "axFormat.h"
class myPlugin : public axFormat
{
  private:

    float m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_None)
      {
        describe("test_gain_nogui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        appendParameter( new axParameter(this,"gain","",0) );
        setupParameters();
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        if (aParameter->getIndex()==0) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

};

AX_ENTRYPOINT(myPlugin)
