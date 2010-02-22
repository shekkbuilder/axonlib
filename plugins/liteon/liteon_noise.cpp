/*
  ******************************************************************************
  one pole filters
  (c) 2010, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://axonlib.googlecode.com/>
  ******************************************************************************
*/
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1

#define AX_DEBUG      // add debug
#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"
#include "parFloat.h"

#include "dspPRNG0.h"   // add noise gen

// class
class myPlugin : public axPlugin
{

private:  
  float gain, n_out;
  // init 2 fiters (left / right)
  dspPRNG0 ng0;

public:
  // constructor
  myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
  {
    describe("noise_gen", "liteon", "noise_gen", 0, 0x6c74);
    // add 0 ins 2 outs
    setupAudio(0, 2);
    // set init values
    gain = 0.25f;
    // add params
    appendParameter( new parFloat( this, 0, "gain", "dB", -12.f, -24.f, 0, 0.001f) );
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
      // gain
      case 0:
        gain = axDB2Lin(f);   // convert db to linear
        break;
    }
  }
  
  // per block
  virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
  {
    // debug noise output sample
    trace(n_out);
    return false;
  }

  // per sample
  virtual void doProcessSample(float** ins, float** outs)
  {
    // get random value from ng0;
    n_out = ng0.process()*gain;
    // output
    *outs[0] = *outs[1] = n_out;
  }
};
// -----------------
#include "axMain.h"
