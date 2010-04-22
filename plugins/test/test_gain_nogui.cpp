//#define AX_DEBUG
#include "axPlugin.h"

class myPlugin : public axPlugin
{
  private:
    axParameter*  p_Gain;
    float         m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {
        m_Gain = 0;
        describe("test_gain_nogui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        appendParameter( p_Gain = new axParameter(this, "gain", "") );
        setupParameters();
        
        axDwinCreate();
        wdebug("hello dbg");        
        
        axDstdCreate();
        trace("hello dbg");        
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Gain) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
