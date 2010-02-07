// in-progress now.- in the middle between simple-gui and bitmap-gui!

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  8
#define AX_WIDTH      150
#define AX_HEIGHT     350
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
//#define AX_AUTOSYNC

#define AX_DEBUG

#include "axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"
//#include "axBitmap.h"
//#include "axSurface.h"

#include "parFloat.h"
#include "parInteger.h"

#include "wdgPanel.h"
#include "wdgLabel.h"
#include "wdgValue.h"
#include "wdgKnob.h"

//#include "wdgImgKnob.h"
//#include "wdgImage.h"

//#include "extern/lodepng.h"
//#include "extern/lodepng.cpp"

//----------------------------------------------------------------------

char* str_noyes[]  = { (char*)"no", (char*)"yes" };
char* str_updown[] = { (char*)"up", (char*)"down" };

//----------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  public:
    axEditor* mEditor;
    int sync;
    float speed;
    float beats;
    int dir;
    int n1;
    int n2;
    int nstart;
    int nnum;

    bool running;
    int offset;
    int count;
    int num1;
    int num2;
    int out;

  public:
//    axBitmap*       knob_bmp;
//    axSurface*      knob_srf;
//    axBitmap*       back_bmp;
//    axSurface*      back_srf;
    unsigned char*  buffer;
    unsigned int    width, height;
    bool            is_gui_initialized;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        is_gui_initialized = false;
        describe("midi_fibonacci0","ccernn","product_string",0,0xfaccface);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);

        appendParameter( new parInteger(this,0,"sync",            "", 1,    0, 1,   str_noyes ) );
        appendParameter( new parInteger(this,1,"speed (ms)",      "", 125,  1, 1000 ) );
        appendParameter( new parFloat(  this,2,"beats (is sync)", "", 0.25, 0, 4,   0.05   ) );
        appendParameter( new parInteger(this,3,"direction",       "", 0,    0, 1,   str_updown ) );
        appendParameter( new parInteger(this,4,"first num",       "", 0,   -10,10   ) );
        appendParameter( new parInteger(this,5,"second num",      "", 1,   -10,10   ) );
        appendParameter( new parInteger(this,6,"note start",      "", 60,   0, 127  ) );
        appendParameter( new parInteger(this,7,"num notes",       "", 24,   1, 127  ) );
        processParameters();
        count = 0;
        num1  = 0;
        num2  = 0;
      }

//    virtual ~myPlugin()
//      {
////        if(is_gui_initialized)
////        {
////          delete knob_bmp;
////          delete knob_srf;
////        }
//      }


    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

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

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: sync   = f;                       break;
          case 1: speed  = mSampleRate * (f/1000);  break;
          case 2: beats  = f;                       break;
          case 3: dir    = f;                       break;
          case 4: n1     = f;                       break;
          case 5: n2     = f;                       break;
          case 6: nstart = f;                       break;
          case 7: nnum   = f;                       break;
        }
        out   = 0;
        count = 0;
        num1 = n1;
        num2 = n2;
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        if( sync==1 ) speed = beats*mSampleRate*60/mTempo;
        speed = axMax(256,speed);
        //play_state&1 ? (
        if( mPlayState&2 )
        {
          if( !running )
          {
            running = true;
            num1 = n1;
            num2 = n2;
            count = 0;
          }
        } else running = false;
        offset = 0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        //------------------------------
        if( (sync==0) || (sync==1 && running))
        {
          offset += 1;
          count -= 1;
          if( count<=0 )
          {
            count = speed;
            out   = (num1 + num2) % nnum;
            num1  = num2;
            num2  = out;
            int note;
            if( dir==0 ) note = nstart + out;
            else note = nstart + nnum - 1 - out;
            sendMidi(offset,0x90+0, note, 127 );
          }
        }
        //------------------------------
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------

    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        axEditor* EDIT = new axEditor( "midi_fibonacci_editor",this, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), AX_FLAGS );
        wdgPanel* panel;
        EDIT->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client) );
        // E->appendWidget( new wdgImage( E, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), wal_None, NULL, back_srf ) );
        for( int i=0; i<mParameters.size(); i++ )
        {
          axParameter* par = mParameters[i]; // skip background
          //axWidget* wdg = new wdgImgKnob(E,i,axRect(10,10+i*40,32,32),wal_None,par,20,knob_srf);
          axWidget* wdg = new wdgKnob(this,i,axRect(10,10+i*40,128,32),wal_None/*,par*/);
          panel->appendWidget(wdg);
          EDIT->connect(wdg,par);
        }
        EDIT->doRealign();
        mEditor = EDIT;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        //TODO: lock?
        axEditor* EDIT = mEditor;
        mEditor = NULL;
        delete EDIT;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        //TRACE("pre: onChange par\n");
        if(mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
