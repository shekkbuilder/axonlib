/*
  ******************************************************************************
  buquad filter band (rbj)
  (c) 2010, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://axonlib.googlecode.com/>
  ******************************************************************************
*/
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  5

#define AX_DEBUG      // add debug
#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"
#include "parFloat.h"

#include "dspRBJ.h"   // add filter

// string arrays
char* str_processing[] =
{
  (char*)"stereo",
  (char*)"mono"
};
char* ftype_ar[] =
{
  (char*)"off",
  (char*)"LP",
  (char*)"HP",
  (char*)"BP",
  (char*)"BR",
  (char*)"PK",
  (char*)"LS",
  (char*)"HS"
};

// class
class myPlugin : public axPlugin
{

private:
  unsigned int mono, ftype;
  float srate, q, freq, gain;
  // init 2 fiters (left / right)
  dspRBJ f0,f1;

public:
  // constructor
  myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
  {
    describe("biquad_rbj", "liteon", "biquad_rbj", 0, 0x6c74);
    // add 2 ins 2 outs
    setupAudio(2, 2);
    // set init values
    mono = 0;
    freq = q = gain = 0.f;
    srate = updateSampleRate();
    // add params
    appendParameter( new parInteger( this, 0, "mode", "",  0, 0, 1, str_processing ) );
    appendParameter( new parInteger( this, 1, "filter", "", 1, 0, 7, ftype_ar ) );
    appendParameter( new parFloat( this, 2, "freq", "scale", 0.5f, 0, 1.0135, 0.0001) );
    appendParameter( new parFloat( this, 3, "BW", "octaves", 2.543f, 0.01, 4, 0.001) );
    appendParameter( new parFloat( this, 4, "gain", "dB", 0.f, -24, 24, 0.001) );
    // first process params
    processParameters();
  }

  // process state
  virtual void doProcessState(int aState)
  {
    // add debug console (win32)
    #if defined(AX_DEBUG) && defined(WIN32)
    switch (aState)
    {
      case pst_Open:      // open
        axDstdCreate();
        break;
      case pst_Close:
        axDstdDestroy();  // close
        break;
    }
    #endif
  }

  // process parameters
  virtual void doProcessParameter(axParameter* aParameter)
  {
    // param id, value
    int  id = aParameter->mID;
    float f = aParameter->getValue();

    switch (id)
    {
      // mono / stereo
    case 0:
      mono = (int)f;
      break;
      // filter type
    case 1:
      ftype = (int)f;
      break;
      // frequency
    case 2:
      freq = axExpf(f*6.90775f + 2.99573f); // [0 - 1.0135] to [20hz - ~22000hz]
      trace("freq = " << freq);
      break;
      // q factor
    case 3:
      q = axOctaves2Q(f);
      break;
      // gain
    case 4:
      gain = f;
      break;
    }
    // setup filters f0, f1
    f0.setup(ftype, srate, freq, q, gain, true);
    if (!mono) f1.setup(ftype, srate, freq, q, gain, true);
  }
    
  virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
  {
    f0.interpolate(sampleFrames);
    f1.interpolate(sampleFrames);    
    return false;
  }

  // per sample
  virtual void doProcessSample(float** ins, float** outs)
  {
    float spl0 = *ins[0];
    float spl1 = *ins[1];
    // mono
    if (mono)
    {
      spl0 = spl1 = f0.process(spl0);
    }
    // stereo
    else
    {
      spl0 = f0.process(spl0);
      spl1 = f1.process(spl1);
    }
    *outs[0] = spl0;
    *outs[1] = spl1;
  }
};
// -----------------
#include "axMain.h"
