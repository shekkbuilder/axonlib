#include "axPlugin.h"
#include "par/parInteger.h"
#include "par/parFloat.h"

//----------------------------------------------------------------------

char* str_type[] =
{
  (char*)"bypass",
  (char*)"clip"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
  // process
  //'internal' parameters
    int         m_Type;
    float       m_Thr;
    float       m_Pre;
    float       m_Post;
    float       m_Vol;
  //vst parameters
    parInteger* p_Type;
    parFloat*   p_Thr;
    parFloat*   p_Pre;
    parFloat*   p_Post;
    parFloat*   p_Vol;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {
        describe("fx_distortion","ccernn","axonlib example",2,AX_MAGIC+0x1003);
        setupAudio(2,2,false);
        appendParameter( p_Type = new parInteger(this,"type",      "", 0, 0,1, str_type) );
        appendParameter( p_Thr  = new parFloat(  this,"threshold", "", 1       ) );
        appendParameter( p_Pre  = new parFloat(  this,"pre gain",  "", 1, 1,2  ) );
        appendParameter( p_Post = new parFloat(  this,"post gain", "", 1, 0,2  ) );
        appendParameter( p_Vol  = new parFloat(  this,"volume",    "", 1, 0,1  ) );
        setupParameters();
      }

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        switch ( aParameter->getIndex() )
        {
          case 0: m_Type  = (int)aParameter->getValue();  break;
          case 1: m_Thr   =      aParameter->getValue3(); break;
          case 2: m_Pre   =      aParameter->getValue2();  break;
          case 3: m_Post  =      aParameter->getValue2();  break;
          case 4: m_Vol   =      aParameter->getValue2();  break;
        }
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float in0 = *aInputs[0];
        float in1 = *aInputs[1];
        float out0 = in0 * m_Pre;
        float out1 = in1 * m_Pre;
        switch (m_Type)
        {
          case 1: // clip
            out0 = axMin(axMax(out0,-m_Thr),m_Thr);
            out1 = axMin(axMax(out1,-m_Thr),m_Thr);
            break;
        }
        out0 *= m_Post;
        out1 *= m_Post;
        // filter?
        *aOutputs[0] = out0*m_Vol;
        *aOutputs[1] = out1*m_Vol;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

