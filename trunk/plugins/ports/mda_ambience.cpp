#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   8
#define AX_NUMPARAMS  5
//#define AX_AUTOSYNC
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    float buf1[1024];
    float buf2[1024];
    float buf3[1024];
    float buf4[1024];
    float slider1,slider2,slider3,slider4,slider5;
  public:
    //axEditor* mEditor;
    float f, fb;
    int p;
    float size,damp,/*mix,*/dry,wet,out;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        memset(buf1,0,1024*sizeof(float));
        memset(buf2,0,1024*sizeof(float));
        memset(buf3,0,1024*sizeof(float));
        memset(buf4,0,1024*sizeof(float));
        f   = 0;
        p   = 0;
        fb  = 0.8;
        //mEditor = NULL;
        describe("mda_ambience","ccernn","product_string",0,0);
        setupAudio(2,2);

        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter(new parFloat(this,0,"size",   "",0.7));
        appendParameter(new parFloat(this,1,"hf damp","",0.7));
        appendParameter(new parFloat(this,2,"mix",    "",0.9));
        appendParameter(new parFloat(this,3,"output", "",0.5));
        appendParameter(new parFloat(this,4,"[fb]",   "",0.8));
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

//@slider
//  fb   = 0.8;
//  damp = 0.05+(0.9*slider2);
//  tmp  = pow(10,(2*slider4)-1);
//  dry  = tmp - (slider3*slider3*tmp);
//  wet  = (0.4+0.4)*slider3*tmp;
//  tmp  = 0.025 + (2.665*slider1);
//  size = tmp;

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        float tmp;
        switch(id)
        {
          case 0: slider1=f; break; // size
          case 1: slider2=f; break; // damp
          case 2: slider3=f; break; // mix
          case 3: slider4=f; break; // output
          case 4: slider5=f; break; // [fb]
        }
        fb   = slider5;//0.8;
        damp = 0.05+(0.9*slider2);
        tmp  = pow(10,(2*slider4)-1);
        dry  = tmp - (slider3*slider3*tmp);
        wet  = (0.4+0.4)*slider3*tmp;
        tmp  = 0.025 + (2.665*slider1);
        size = tmp;

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
        int d1 = (p+(int)(107*size)) & 1023;
        int d2 = (p+(int)(142*size)) & 1023;
        int d3 = (p+(int)(277*size)) & 1023;
        int d4 = (p+(int)(379*size)) & 1023;
        float a = *ins[0];;
        float b = *ins[1];
        f += (damp*((wet*(a+b))-f)); //hf-damping
        float r = f;

        float t = buf1[p];
        r -= (fb*t);
        buf1[d1] = r; //allpass
        r += t;

        t = buf2[p];
        r -= (fb*t);
        buf2[d2] = r; //allpass
        r += t;

        t = buf3[p];
        r -= (fb*t);
        buf3[d3] = r; //allpass
        r += t;
        float c = ((dry*a)+r-f); //left output

        t = buf4[p];
        r -= (fb*t);
        buf4[d4] = r; //allpass
        r += t;
        float d = ((dry*b)+r-f); //right output

        p +=1;  p &=1023;
        d1+=1;  d1&=1023;
        d2+=1;  d2&=1023;
        d3+=1;  d3&=1023;
        d4+=1;  d4&=1023;

        *outs[0] = c;
        *outs[1] = d;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
