
#define AX_PLUGIN     myPlugin
#define AX_SYNTH
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  3
#define AX_WIDTH      320
#define AX_HEIGHT     240

#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "dspSVF.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

// should we protect this in some way?
// we only read from it (runtime)
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
    axEditor*   mEditor;
    dspSVF      svf1, svf2;
    parInteger  *pMode;
    parFloat    *pFreq, *pBW;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("test1","ccernn","product_string",0,0);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter( pMode = new parInteger(  this,0,"mode", "", 0, 0,4, str_filter ) );
        appendParameter( pFreq = new parFloat(    this,1,"freq", "", 1 ) );
        appendParameter( pBW   = new parFloat(    this,2,"bw",   "", 1 ) );
        svf1.setup(0,1,1);
        svf2.setup(0,1,1);
      }

    //parameters is automatically deleted
    //virtual ~myPlugin() {}

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //virtual axWindow* doCreateEditor(void)
    //  {
    //    mEditor = new axEditor( this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
    //    mEditor->setupParameters(mParameters);
    //    return mEditor;
    //  }

    //----------

    //virtual void doDestroyEditor(void)
    //  {
    //    delete mEditor;
    //  }

    //----------

    //virtual void doIdleEditor(void)
    //  {
    //  }

    //----------

    //virtual void doProcessState(int aState)
    //  {
    //  }

    //----------

    //virtual void doProcessTransport(int aState)
    //  {
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            svf1.mMode = f;
            svf2.mMode = f;
            break;
          case 1:
            f*=f;
            svf1.mFreq = f;
            svf2.mFreq = f;
            break;
          case 2:
            f*=f;
            svf1.mBW = f;
            svf2.mBW = f;
            break;
        }
      }

    //----------

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //  }

    //----------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        *outs[0] = svf1.process( *ins[0] );
        *outs[1] = svf2.process( *ins[1] );
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
