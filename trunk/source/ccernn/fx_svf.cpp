#define AX_NOGUI

//#include "format/axFormat.h"
#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"
#include "dsp/dspSVF.h"

//----------------------------------------------------------------------

char* str_params[] =
{
  (char*)"mode",
  (char*)"freq",
  (char*)"bw"
};

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_svf"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 3; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

//----------------------------------------------------------------------

char* str_filter[] =
{
  (char*)"off",
  (char*)"lowpass",
  (char*)"highpass",
  (char*)"bandpass",
  (char*)"notch"
};

//----------

//class myPlugin : public axFormat
class myInstance : public AX_INSTANCE
{
  private:
    dspSVF svf1,svf2;

  public:

    //myPlugin(axContext* aContext, int aFlags)
    //: axFormat(aContext)
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //describe("fx_svf","ccernn","axonlib example",0,AX_MAGIC+0x1006);
        //setupAudio(2,2);
        appendParameter( new parInteger(  this,"_mode_", "", 0, 0,4, str_filter ) );
        appendParameter( new parFloatPow( this,"_freq_", "", 1, 0,1,0, 2 ) );
        appendParameter( new parFloatPow( this,"_bw_",   "", 1, 0,1,0, 2 ) );
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

//AX_ENTRYPOINT(myPlugin)

//AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
AX_MAIN(myDescriptor,myInstance)
