/*
  ******************************************************************************
  oversample test
  (c) 2010, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://axonlib.googlecode.com/>
  ******************************************************************************
*/
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  2

//#define AX_DEBUG      // add debug
//#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"
#include "parFloat.h"

#include "dspInterpolate.h"   // add filter

// string arrays
char* f_ar[] =
{
  (char*)"off",
  (char*)"on"
};

// class
class myPlugin : public axPlugin
{

private:
  float srate;
  unsigned int os_factor;  
  bool filter;
  
  // init 2 interpolators
  dspInterpolate i0, i1;

public:
  // constructor
  myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
  {
    describe("oversample_test", "liteon", "oversample_test", 0, 0x6c74);
    // add 2 ins 2 outs
    setupAudio(2, 2);
    // set init values
    srate = updateSampleRate();
    // add params
    appendParameter( new parInteger( this, 0, "aa filter", "",  1, 0, 1, f_ar ) );
    appendParameter( new parInteger( this, 1, "os factor", "x", 2, 1, 16 ) );    
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
    // ---
    switch (id)
    {
      // aa filter on/off
      case 0:
        filter = (int)f;
        break;
      // os factor
      case 1:
        os_factor = (int)f;
        break;         
    }
    
    // setup the two interpolators
    i0.setup(os_factor, srate, filter);
    i1.setup(os_factor, srate, filter);
  }
  
  // declare a static processing function here
  // the dspInterpolate::process() method will call it back
  static float callback_f(float sample)
  {
    // some processing
    sample = axRandomSigned()*0.25f; // white noise
    return sample;
  }  
  
  // per sample
  virtual void doProcessSample(float** ins, float** outs)
  {
    float spl0 = *ins[0];
    float spl1 = *ins[1];
    
    // pass input sample (splX) and pointer to function (callback_f)
    spl0 = i0.process(callback_f, spl0);
    spl1 = i1.process(callback_f, spl1);
    
    *outs[0] = spl0;
    *outs[1] = spl1;
  }
};

// -----------------
#include "axMain.h"
