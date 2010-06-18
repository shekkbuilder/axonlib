
#include "format/axFormat.h"
#include "par/parFloat.h"
#include "par/parInteger.h"
#include "dsp/dspRC.h"

char* str_mode[] = { (char*)"lowpass", (char*)"highpass" };

class myPlugin : public axFormat
{
  private:
    int     m_Mode;
    dspRC   m_RC0,m_RC1;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {
        describe("fx_wgtavg","ccernn","axonlib example",0,AX_MAGIC+0x1005);
        setupAudio(2,2);
        appendParameter( new parInteger( this,"mode",  "", 0,   0,1,   str_mode) );
        appendParameter( new parFloatPow(this,"weight","", 0.5, 0,1,0, 3) );
        setupParameters();
      }

    virtual ~myPlugin()
      {
        //delete rc1;
        //delete rc2;
      }

    virtual void doStateChange(int aState)
      {
        float s;
        switch (aState)
        {
          case fs_Resume:
            s = getSampleRate();
            //trace("samplerate = " << s);
            m_RC0.setSRate(s);
            m_RC1.setSRate(s);
            break;
        }
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0:
            m_Mode = (int)val;
            break;
          case 1:
            m_RC0.setWeight(val);
            m_RC1.setWeight(val);
            break;
        }
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        SPL spl0 = *aInputs[0];
        SPL spl1 = *aInputs[1];
        switch (m_Mode)
        {
          case 0: // lowpass
            spl0 = m_RC0.process(spl0);
            spl1 = m_RC1.process(spl1);
            break;
          case 1: // highpass
            spl0 -= m_RC0.process(spl0);
            spl1 -= m_RC1.process(spl1);
            break;
        }
        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_ENTRYPOINT(myPlugin)
