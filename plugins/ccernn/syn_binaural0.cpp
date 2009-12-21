#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2

#define AX_WIDTH      286
#define AX_HEIGHT     410
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_AUTOSYNC

#define AX_DEBUG

#include "axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "wdgLabel.h"
#include "wdgValue.h"
#include "wdgKnob.h"

#include "axBitmap.h"
#include "axSurface.h"
#include "wdgImage.h"

#include "../extern/lodepng.h"
#include "../extern/lodepng.cpp"
#include "images/binaural_back.h"

//----------------------------------------------------------------------

#define MAXEVT 65536

class myPlugin : public axPlugin
{
  private:
    bool is_gui_initialized;
    axBitmap*       back_bmp;
    axSurface*      back_srf;
  public:
    axEditor* mEditor;
    SPL       EVENTS[MAXEVT*2];
    int       offset;
    float     ph1,  ph2;
    float     diff, decay;
    float     vol,  frq;
    float     vol_t,frq_t;
    float     irate;
    int       playing_note;
    float     playing_freq;
    wdgKnob   *k1,*k2;



  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        is_gui_initialized = false;
        describe("syn_binaural0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        isSynth();
        //setNumInputs(2);
        //axParameter* par;
        appendParameter( /*par =*/ new parFloat( this,0,"hz diff","", 4,  -25, 25, 0.005 ) );
        //TRACE("par1 = %x\n",(int)par);
        appendParameter( /*par2 =*/ new parFloat( this,1,"xfade",  "", 25,  1, 100 ) );
        //TRACE("par2 = %x\n",(int)par);
        processParameters();
        for( int i=0; i<MAXEVT; i++ ) EVENTS[i*2]=-1;
        ph1 = ph2 = 0;
        playing_note = -1;
      }

    virtual ~myPlugin()
      {
        if(is_gui_initialized)
        {
          delete back_bmp;
          delete back_srf;
        }
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        //lock();
        if( mEditor ) mEditor->onChange( aParameter );
        //unlock();
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor( "syn_binaural_gui",this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );

        if(!is_gui_initialized)
        {
          unsigned char*  buffer;
          unsigned char*  image;
          unsigned int    buffersize;
          unsigned int    imagesize;
          LodePNG_Decoder decoder;
          //LodePNG_loadFile(&buffer, &buffersize, filename);
          buffer     =  binaural_back;
          buffersize =  41373;
          LodePNG_Decoder_init(&decoder);
          LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
          if (!decoder.error)
          {
            int width = decoder.infoPng.width;
            int height = decoder.infoPng.height;
            //int depth = decoder.infoPng.color.bitDepth;
            //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
            back_bmp = new axBitmap(width,height,image);
            back_bmp->convertRgbaBgra();
            back_bmp->setBackground(128,128,128);
            back_srf = new axSurface(width,height,0);
            back_srf->mCanvas->drawBitmap(back_bmp, 0,0, 0,0,width,height);
            free(image);
          }
          //free(buffer);
          LodePNG_Decoder_cleanup(&decoder);
          is_gui_initialized=true;
        }

        //E->setLayout(0,0,10,10);
        //E->setFlag(wfl_Vertical);

        E->appendWidget( new wdgImage(     E, 2, axRect(  0,  0, 286,410), wal_None/*, NULL,*/, back_srf ) );
        E->appendWidget( k1 = new wdgKnob( E, 0, axRect( 10,365, 128, 32), wal_None/*, mParameters[0]*/ ) );
        E->appendWidget( k2 = new wdgKnob( E, 1, axRect(148,365, 128, 32), wal_None/*, mParameters[1]*/ ) );
        //E->updateWidgetValues(); // connect by wdg/par.mID !!
        E->connect( k1, mParameters[0] );
        E->connect( k2, mParameters[1] );
        mEditor = E;
        //TRACE("syn_binaural = %x\n",(int)mEditor);
        //TRACE("  k1 = %x\n",(int)k1);
        //TRACE("  k1 = %x\n",(int)k2);
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if( mEditor ) mEditor->redrawDirty();
      }

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

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
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
        //TRACE("mSampleRate %f\n",mSampleRate);
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

};

//----------------------------------------------------------------------
#include "axMain.h"
