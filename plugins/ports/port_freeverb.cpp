#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  6
//#define AX_AUTOSYNC
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_DEBUG
//#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

#include "../../extern/freeverb/allpass.cpp"
#include "../../extern/freeverb/comb.cpp"
#include "../../extern/freeverb/revmodel.cpp"

//----------------------------------------------------------------------

//revmodel();
//void	mute();
//void	processmix(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
//void	processreplace(float *inputL, float *inputR, float *outputL, float *outputR, long numsamples, int skip);
//void	setroomsize(float value);
//void	setdamp(float value);
//void	setwet(float value);
//void	setdry(float value);
//void	setwidth(float value);
//void	setmode(float value);
//float	getroomsize();
//float	getdamp();
//float	getwet();
//float	getdry();
//float	getwidth();
//float	getmode();

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    //axEditor* mEditor;
    revmodel freeverb;
    //float	roomsize;
    //float	damp;
    //float	wet;
    //float	dry;
    //float	width;
    //float	mode;     // 1 = freeze

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //mEditor = NULL;
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("port_freeverb","jezar","axonlib ported plugin",0,0);
        setupAudio(2,2);
        appendParameter(new parFloat(  this,0,"roomsize", "",0 ));
        appendParameter(new parFloat(  this,1,"damp",     "",0 ));
        appendParameter(new parFloat(  this,2,"wet",      "",0 ));
        appendParameter(new parFloat(  this,3,"dry",      "",0 ));
        appendParameter(new parFloat(  this,4,"width",    "",0 ));
        appendParameter(new parInteger(this,5,"mode",     "",0, 0,1 ));
        processParameters();
      }

    //----------

    //virtual ~myPlugin()
    //  {
    //  }

    //----------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //    if(mEditor) mEditor->onChange(aParameter);
    //    doProcessParameter(aParameter);
    //  }

    //--------------------------------------------------

    //virtual axWindow* doCreateEditor(void)
    //  {
    //    axEditor* E = new axEditor(this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
    //    E->setLayout(10,10,5,5);
    //    //E->setFlag(wfl_Vertical);
    //    E->appendWidget(new wdgKnobPar(E,0,axRect(0,0,100,32),wal_Stacked,mParameters[0]));
    //    E->updateWidgetValues();
    //    E->doRealign();
    //    mEditor = E;
    //    return mEditor;
    //  }

    //----------

    //virtual void doDestroyEditor(void)
    //  {
    //    axEditor* tempeditor = mEditor;
    //    mEditor = NULL;
    //    delete tempeditor;
    //  }

    //----------

    //virtual void doIdleEditor(void)
    //  {
    //    mEditor->redrawDirty();
    //  }

    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    // if has editor
    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: freeverb.setroomsize(f); break;
          case 1: freeverb.setdamp(f);     break;
          case 2: freeverb.setwet(f);      break;
          case 3: freeverb.setdry(f);      break;
          case 4: freeverb.setwidth(f);    break;
          case 5: freeverb.setmode(f);     break;
        }
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float in0 = *ins[0];
        float in1 = *ins[1];
        float out0,out1;

        freeverb.processreplace(&in0, &in1, &out0, &out1,1,0);

        *outs[0] = out0;
        *outs[1] = out1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
