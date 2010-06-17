
#include "format/axFormat.h"

float prog0 = { 0.0 };
float prog1 = { 0.5 };
float prog2 = { 1.0 };

class myPlugin : public axFormat
{
  private:

    float m_Gain;

  public:

    myPlugin(axContext* aContext, int aFormatFlags)
    : axFormat(aContext)
      {
        describe("test_gain_nogui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        appendParameter( new axParameter(this,"gain","",0) );
        setupParameters();
        appendProgram( new axProgram("first", 1,&prog0) );
        appendProgram( new axProgram("second",1,&prog1) );
        appendProgram( new axProgram("third", 1,&prog2) );
        setupPrograms();
      }

    virtual void doPreProgram(int aProgram)
      {
        trace("doPreProgram "<<aProgram);
        saveProgram( getCurrentProgram() );
      }

    virtual void doSetProgram(int aProgram)
      {
        trace("doSetProgram "<<aProgram);
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
