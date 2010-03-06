#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  6
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC

//#define AX_DEBUG
//#include "axDebug.h"
#include "axPlugin.h"

#include "parFloat.h"
#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

//----------------------------------------------------------------------

#define MAX_SRATE   192000
#define MAX_SECONDS 1
#define MAX_BUFSIZE (MAX_SECONDS*MAX_SRATE*2)

char* str_freeze[] = { (char*)"off", (char*)"on" };
char* str_loopmode[] = { (char*)"wrap", (char*)"bidi" };

//----------

class myPlugin : public axPlugin
{
  public:
    //float mValue;
    float *BUFFER;
    int   index;
    float pos;

    int   bufsize;
    float size;
    float speed;
    float start;
    int   freeze;
    int   mode;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //mEditor = NULL;
        describe("fx_freeze0","ccernn","product_string",0,0);
        setupAudio(2,2);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
//@init
//  BUFFER = 1000;
//  index = 0;
//  pos = 0;
        BUFFER = new float[MAX_BUFSIZE];
        index  = 0;
        pos    = 0;

//slider1: 1000 <1, 1000       > buffersize (ms)
//slider2: 1    <0, 1,   0.001 > size
//slider3: 1    <0, 4,   0.001 > speed
//slider4: 0    <0, 1,   0.001 > start
//slider5: 0    <0, 1,   1     > freeze
//slider6: 1    <0, 1,   1{wrap,bidi}> loop mode
        //appendParameter(new parFloat(this,0,"value","",0));
        appendParameter( new parFloat(  this,0,"buffersize","ms", 1000, 1, 1000 ));
        appendParameter( new parFloat(  this,1,"size",      "",   1,    0, 1 ));
        appendParameter( new parFloat(  this,2,"speed",     "",   1,    0, 2 ));
        appendParameter( new parFloat(  this,3,"start",     "",   0,    0, 1 ));
        appendParameter( new parInteger(this,4,"freeze",    "",   0,    0, 1, str_freeze ));
        appendParameter( new parInteger(this,5,"loop mode", "",   1,    0, 1, str_loopmode ));
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        delete[] BUFFER;
      }

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

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

//@slider
//  bufsize = (slider1/1000)*srate;
//  size    = bufsize * slider2;
//  speed   = slider3;
//  start   = bufsize * slider4;
//  freeze  = slider5;
//  mode    = slider6;
//  size = max(size,1);

    virtual void doProcessParameter(axParameter* aParameter)
      {
        float srate = updateSampleRate();
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: bufsize = (f*0.001)*srate;      break;
          case 1: size    = axMax(1,(f*bufsize)); break;
          case 2: speed   = f;                    break;
          case 3: start   = f*bufsize;            break;
          case 4: freeze  = f;                    break;
          case 5: mode    = f;                    break;
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
        int p2;

        if (freeze==0)
        {
          p2 = index*2;
          BUFFER[p2  ] = spl0;
          BUFFER[p2+1] = spl1;
        }
        index += 1;
        if (index>=bufsize) index = 0;
        //out = BUFFER[pos];
        pos += speed;
        if( mode==0) // wraparound
        {
          if (pos>=size) pos -= size;
          if (pos<0) pos += size;
        }
        else if (mode==1) // bidi looping
        {
          if (pos>=size)
          {
            pos = size - (pos-size);
            speed = -speed;
          }
          else if (pos<0)
          {
            pos = -pos;
            speed = -speed;
          }
        }
        p2 = start + pos;
        /*if (p>=bufsize)*/ while (p2>=bufsize) p2-=bufsize;
        /*if (p<0)*/ while (p2<0) p2+=bufsize;
        p2 *= 2;
        spl0 = BUFFER[p2];
        spl1 = BUFFER[p2+1];


        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"



/*
desc:fx_freeze :: ccernn.2009 :: v0.0.1

slider1: 1000 <1, 1000       > buffersize (ms)
slider2: 1    <0, 1,   0.001 > size
slider3: 1    <0, 4,   0.001 > speed
slider4: 0    <0, 1,   0.001 > start
slider5: 0    <0, 1,   1     > freeze
slider6: 1    <0, 1,   1{wrap,bidi}> loop mode

@init
  BUFFER = 1000;
  index = 0;
  pos = 0;

@slider
  bufsize = (slider1/1000)*srate;
  size    = bufsize * slider2;
  speed   = slider3;
  start   = bufsize * slider4;
  freeze  = slider5;
  mode    = slider6;
  size = max(size,1);

@sample
  !freeze ? (
    BUFFER[index] = spl0;
    BUFFER[index+65536] = spl1;
  );
  index += 1;
  index>=bufsize ? index=0;
  //out = BUFFER[pos];
  pos += speed;
  mode==0 ? ( // wraparound
    pos >= size ? pos-=size;
    pos <  0    ? pos+=size;
  ) :
  mode==1 ? ( // bidi looping
    pos >= size ? (
      pos = size - (pos-size);
      speed = -speed;
    ) :
    pos < 0 ? (
      pos = -pos;
      speed = -speed;
    );
  );
  p = start + pos;
  p>=bufsize ? while( p-=bufsize; p>=bufsize; );
  p<0 ? while( p+=bufsize; p<0; );
  //out = BUFFER[p];
  spl0 = BUFFER[p];
  spl1 = BUFFER[p+65536];
*/
