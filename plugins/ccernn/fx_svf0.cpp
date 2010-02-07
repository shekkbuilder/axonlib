//#define AX_PLUGIN_AUTOSYNC
#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  3

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "dspSVF.h"

//----------------------------------------------------------------------

char* str_filter[] =
{
  (char*)"off",
  (char*)"lowpass",
  (char*)"highpass",
  (char*)"bandpass",
  (char*)"notch"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    dspSVF      svf1, svf2;
    parInteger  *pMode;
    parFloat    *pFreq, *pBW;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_svf0","ccernn","product_string",0,0);
        setupAudio(2,2);
        appendParameter( pMode = new parInteger(this,0,"mode", "", 0, 0,4, str_filter ) );
        appendParameter( pFreq = new parFloat(  this,1,"freq", "", 1 ) );
        appendParameter( pBW   = new parFloat(  this,2,"bw",   "", 1 ) );
        svf1.setup(0,1,1);
        svf2.setup(0,1,1);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: // mode
            svf1.mMode = f;
            svf2.mMode = f;
            break;
          case 1: // freq
            svf1.mFreq = f*f;
            svf2.mFreq = f*f;
            break;
          case 2: // bw
            svf1.mBW = f*f;
            svf2.mBW = f*f;
            break;
        }
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        *outs[0] = svf1.process( ins, outs );
        *outs[1] = svf2.process( ins, outs );
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
