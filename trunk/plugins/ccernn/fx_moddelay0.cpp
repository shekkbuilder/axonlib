#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  8
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

//----------------------------------------------------------------------

// 50 ms = 1/20 sec

#define MAX_MS      50
#define MAX_SRATE   192000
#define MAX_DELAY   1/MAX_MS
//#define MAX_BUFSIZE 192000
#define MAX_BUFSIZE ((int)(MAX_SRATE*MAX_DELAY))

char* str_com[] = { (char*)"add", (char*)"sub", (char*)"mul" };

class myPlugin : public axPlugin
{
  public:
    //axEditor* mEditor;
    //float mValue;
    float BUFFER[MAX_BUFSIZE*2];
    int   index;

    float ph;
    //float srate;
    //
    float delay;
    float depth;
    float rate;
    float dry;
    float wet;
    int   comb;
    int   taps;
    float phdif;
    float phadd;
    //wet /= taps;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //mEditor = NULL;
        describe("fx_moddelay0","ccernn","axonlib example plugin",0,0);
        setupAudio(2,2);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();

        //  MAXDELAY = srate;
        //  BUFFER = 1000;
        //  PI2 = $pi*2;
        //  ph = 0;
        //for (int i=0; i<MAX_BUFSIZE*2; i++) BUFFER[i]=0;
        ph = 0;
        index = 0;
        //srate = updateSampleRate();

        //        slider1: 60  <0, 500, 0.001 > delay (ms)
        //        slider2: 15  <0, 100, 0.001 > mod depth (ms)
        //        slider3: .5  <0, 4,   0.001 > mod rate (hz)
        //        slider4: .75 <0, 1,   0.001 > dry
        //        slider5: .75 <0, 1,   0.001 > wet
        //        slider6: 0   <0, 2,   1     {add,sub,mul}> combine
        //        slider7: 2   <1, 10,  1     > num taps
        //        slider8: .2  <0, 1,   0.001 > tap phase diff

        trace("MAX_BUFSIZE " << MAX_BUFSIZE);

        appendParameter(new parFloat(  this,0,"delay",      "ms",  20,   0, MAX_MS ));
        appendParameter(new parFloat(  this,1,"depth",      "ms",  5,    0, 25 ));
        appendParameter(new parFloat(  this,2,"rate",       "hz",  0.5,  0, 4 ));
        appendParameter(new parFloat(  this,3,"dry",        "",    0.75  ));
        appendParameter(new parFloat(  this,4,"wet",        "",    0.75  ));
        appendParameter(new parInteger(this,5,"[combine]",  "",    0,    0, 2, str_com ));
        appendParameter(new parInteger(this,6,"taps",       "",    2,    1, 10 ));
        appendParameter(new parFloat(  this,7,"phase diff", "",    0.5   ));
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

    //@gfx
    //  gfx_clear = 0;
    //  x = ph * gfx_w;
    //  gfx_x=x; gfx_y=0;
    //  gfx_r=1; gfx_g=1; gfx_b=1; gfx_a=1;
    //  gfx_lineto(x,gfx_h,0);
    //  p = ph;
    //  out0 = out1 = 0;
    //  loop( taps,
    //    de = index - delay + (depth*sin(p*PI2));
    //    de<0 ? de+=MAXDELAY;
    //    de>=MAXDELAY ? de-=MAXDELAY;
    //    //comb==0 ? (
    //    //  out0 += BUFFER[de];
    //    //  out1 += BUFFER[de+MAXDELAY];
    //    //);
    //    p += phdif;
    //    p>=1 ? p-=1;
    //  );
    //  ph += phadd;
    //  ph>=1 ? ph-=1;

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

    //  delay = (slider1/1000) * srate;
    //  depth = (slider2/1000) * srate;
    //  rate  =  slider3;
    //  dry   =  slider4*slider4;
    //  wet   =  slider5*slider5;
    //  comb  =  slider6;
    //  taps  =  slider7*slider7;
    //  phdif =  slider8;
    //  rate>0 ? phadd=rate/srate : phadd=0;
    //  //wet /= taps;

    virtual void doProcessParameter(axParameter* aParameter)
      {
        float srate = updateSampleRate();
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: delay = (f*0.001)*srate; break;
          case 1: depth = (f*0.001)*srate; break;
          case 2: rate  =  f;   if (rate>0) phadd=rate/srate; else phadd=0; break;
          case 3: dry   =  f*f; break;
          case 4: wet   =  f*f; break;
          case 5: comb  =  f;   break;
          case 6: taps  =  f;   break;
          case 7: phdif =  f;   break;
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
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        int pos2 = index*2;
        BUFFER[pos2] = spl0;
        BUFFER[pos2+1] = spl1;
        index += 1;
        if (index>=MAX_BUFSIZE) index=0;
        float out0 = 0;
        float out1 = 0;
        float p = ph;
        //loop( taps,
        for (int i=0; i<taps; i++ )
        {
          float de = (float)index - delay + (depth*sin(p*PI2));
          while (de<0) de+=MAX_BUFSIZE;
          while (de>=MAX_BUFSIZE) de-=MAX_BUFSIZE;
          int p2 = (int)floor(de) * 2;
          //TRACE("pos2:%i\n",pos2); // !!! danger real-time thread, audio!!!!
          if (comb==0)
          {
            out0 += BUFFER[p2];
            out1 += BUFFER[p2+1];
          }
          else if (comb==1)
          {
            out0 -= BUFFER[p2];
            out1 -= BUFFER[p2+1];
          }
          else if (comb==2)
          {
            out0 *= BUFFER[p2];
            out1 *= BUFFER[p2+1];
          }
          p += phdif;
          if (p>=1) p-=1;
        }
        ph += phadd;
        if (ph>=1) ph-=1;
        spl0 = (spl0*dry) + (out0*wet);
        spl1 = (spl1*dry) + (out1*wet);

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
