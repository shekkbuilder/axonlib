//#define AX_DEBUG
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_AUTO_WIN

#include "axPlugin.h"
#include "core/axRand.h"
#include "../extern/mtrand.h"

class myPlugin : public axPlugin
{
  private:
    axParameter*  p_Gain;
    float         m_Gain;
    axRandSinf    axsin;
    MTRand        mtrnd;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {
        m_Gain = 0;
        describe("test_gain_nogui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2, 2, false);
        appendParameter( p_Gain = new axParameter(this, "gain", "") );
        setupParameters();
        
        wdebug("hello dbg");
        trace("hello dbg");
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Gain) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        SPL spl0 = *aInputs[0];
        SPL spl1 = *aInputs[1];
        //*aOutputs[0] = ( spl0 + axsin.randSigned() ) * m_Gain;
        *aOutputs[0] = ( spl0 + axRand() ) * m_Gain;
        *aOutputs[1] = ( spl1 + axRand() ) * m_Gain;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
