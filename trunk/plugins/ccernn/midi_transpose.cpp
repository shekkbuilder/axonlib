//#define AX_DEBUG
//#include "core/axDebug.h"

#include "axPlugin.h"
#include "par/parInteger.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    int         m_Transpose;
    parInteger* p_Transpose;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {
        describe("midi_transpose","ccernn","axonlib example",2,AX_MAGIC+0x1002);
        setupAudio(2,2,false);
        appendParameter( p_Transpose = new parInteger(this,"transpose","semi",0, -12,12 ) );
        setupParameters();
      }

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Transpose) m_Transpose = aParameter->getValue();
      }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        if (msg1==144 || msg1==128) { msg2 += m_Transpose; }
        sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        // pass audio through, untouched
        memcpy(aOutputs[0],aInputs[0],aSize*sizeof(SPL));
        memcpy(aOutputs[1],aInputs[1],aSize*sizeof(SPL));
        return true; // we don't want doProcessSample
      }

};

//----------------------------------------------------------------------

#undef MAX_BEATS
#undef MIN_BPM
#undef MAX_SRATE
#undef MAX_SECONDS_PER_BEAT
#undef MAX_SAMPLES_PER_BEAT
#undef MAX_DELAY_LENGTH

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
