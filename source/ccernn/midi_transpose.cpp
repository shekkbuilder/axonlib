#define NO_GUI
#include "format/axFormat.h"
#include "par/parInteger.h"

/*
  todo: add a list of keys that should NOT have their note offs transposed.
        when we change the transpose parameter, mark all currently active
        voices as 'don't transpose note off'
*/

//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
    int         m_Transpose;
    parInteger* p_Transpose;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {
        describe("midi_transpose","ccernn","axonlib example",2,AX_MAGIC+0x1002);
        setupAudio(2,2,false);
        appendParameter( p_Transpose = new parInteger(this,"transpose","semi",0, -12,12 ) );
        setupParameters();
      }

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Transpose) m_Transpose = (int)aParameter->getValue();
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
        axMemcpy(aOutputs[0],aInputs[0],aSize*sizeof(SPL));
        axMemcpy(aOutputs[1],aInputs[1],aSize*sizeof(SPL));
        return true; // we don't want doProcessSample
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
