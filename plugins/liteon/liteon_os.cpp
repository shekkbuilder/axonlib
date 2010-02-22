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

#include "dspInterpolate.h"   // add interpolation container
#include "dspOnePole.h"       // add one pole filter to test processing

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
  
  // some processing with a filter to test os
  dspOnePole f0, f1;
  
public:
  // constructor
  myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
      : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
  {
    describe("oversample_test", "liteon", "oversample_test", 0, 0x6c74);
    // add 2 ins 2 outs
    setupAudio(2, 2);    
    // get sample rate
    srate = updateSampleRate();
    // example lp at 11k
    f0.setup(1, srate, 11000.f);
    f1.setup(1, srate, 11000.f);
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
  
  // declare a static processing functions here
  // the dspInterpolate::process() method will call them back
  // - f0
  template <class T>
  static inline float callback_f0(const T pThis, float sample)
  {
    // cast instance pointer
    myPlugin* _pThis = (myPlugin*) pThis;
    // call local method f0.process()
    return _pThis->f0.process(sample);
  }
  // - f1
  template <class T>
  static inline float callback_f1(const T pThis, float sample)
  {
    myPlugin* _pThis = (myPlugin*) pThis;
    return _pThis->f1.process(sample);
  }
  
  // per sample
  virtual void doProcessSample(float** ins, float** outs)
  {
    float spl0 = *ins[0];
    float spl1 = *ins[1];
    
    // pass: class instance, input sample (splX) and callback function
    spl0 = i0.process(this, callback_f0, spl0);
    spl1 = i1.process(this, callback_f1, spl1);
    
    *outs[0] = spl0;
    *outs[1] = spl1;
  }
};

// -----------------
#include "axMain.h"
