#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2

#define AX_WIDTH      286
#define AX_HEIGHT     410
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_AUTOSYNC

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "wdgLabel.h"
#include "wdgValue.h"
#include "wdgKnob.h"

#include "axBitmap.h"
#include "axSurface.h"
#include "wdgImage.h"

//#include "../extern/lodepng.h"
//#include "../extern/lodepng.cpp"
#include "axBitmapLoader.h"
#include "images/binaural_back.h"

//----------------------------------------------------------------------

#define MAXEVT 65536

class myPlugin : public axPlugin,
                 public axWidgetListener

{
  private:
    bool        is_gui_initialized;
    axSurface*  back_srf;
  public:
    axEditor*   mEditor;
    SPL         EVENTS[MAXEVT*2];
    int         offset;
    float       ph1,  ph2;
    float       diff, decay;
    float       vol,  frq;
    float       vol_t,frq_t;
    float       irate;
    int         playing_note;
    float       playing_freq;
    wdgKnob     *k1,*k2;



  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        is_gui_initialized = false;
        describe("syn_binaural0","ccernn","product_string",0,0);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        setupAudio(2,2,true);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        appendParameter( new parFloat( this,0,"hz diff","", 4,  -25, 25, 0.005 ) );
        appendParameter( new parFloat( this,1,"xfade",  "", 25,  1, 100 ) );
        processParameters();
        for( int i=0; i<MAXEVT; i++ ) EVENTS[i*2]=-1;
        ph1 = ph2 = 0;
        playing_note = -1;
      }

    virtual ~myPlugin()
      {
        if(is_gui_initialized)
        {
          delete back_srf;
        }
      }

    //--------------------------------------------------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        if( msg1==9*16)
        {
          int   _note = msg2;
          float _freq = 440 * pow(2.0,(_note-69.0) / 12);
          float _vel = (float)msg3 * inv127;
          EVENTS[ofs*2]   = _vel;
          EVENTS[ofs*2+1] = _freq;
          playing_note = _note;
          playing_freq = _freq;
        }
        else if( msg1==8*16 )
        {
          int _note = msg2;
          if( _note==playing_note )
          {
            EVENTS[ofs*2]   = 0;
            EVENTS[ofs*2+1] = playing_freq;
            //playing_note = -1;
          }
        }
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        //TODO: do something better...
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: diff  = f;  break;
          case 1: decay = 1/(f*f);  break;
        }
      }

    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //updateTimeInfo(); // AX_AUTOSYNC;
        irate = 1 / mSampleRate;
        offset = 0;
        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        //------------------------------
        int ofs2 = offset*2;
        if( EVENTS[ofs2]>=0 )
        {
          vol_t = EVENTS[ofs2];
          frq_t = EVENTS[ofs2+1];
          EVENTS[ofs2] = -1;
        }
        offset++;
        frq += ( frq_t - frq ) * decay;
        vol += ( vol_t - vol ) * decay;
        ph1 += (frq*irate);
        if( ph1>=1 ) ph1-=1;
        ph2 += ((frq+diff)*irate);
        if( ph2>=1 ) ph2-=1;
        spl0 = sin(ph1*PI2) * vol;
        spl1 = sin(ph2*PI2) * vol;
        //------------------------------
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------

    //--------------------------------------------------

    virtual void* doCreateEditor(int aParent)
      {
        axEditor* EDIT = new axEditor( "syn_binaural_gui",this, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), AX_FLAGS, aParent );

        if(!is_gui_initialized)
        {
          back_srf = loadPng( binaural_back, 41373 );
          is_gui_initialized=true;
        }
        EDIT->appendWidget( new wdgImage(     this, 2, axRect(  0,  0, 286,410), wal_None, back_srf ) );
        EDIT->appendWidget( k1 = new wdgKnob( this, 0, axRect( 10,365, 128, 32), wal_None ) );
        EDIT->appendWidget( k2 = new wdgKnob( this, 1, axRect(148,365, 128, 32), wal_None ) );
        EDIT->connect( k1, mParameters[0] );
        EDIT->connect( k2, mParameters[1] );
        EDIT->doRealign();
        mEditor = EDIT;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* EDIT = mEditor;
        mEditor = NULL;
        delete EDIT;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if (mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        //lock();
        if (mEditor) mEditor->onChange(aParameter);
        //unlock();
        doProcessParameter(aParameter);
      }

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
