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

class myPlugin : public axPlugin
{
  public:
    axEditor* mEditor;
    //float     BUFFER[MAX_BUFFER_SIZE];
    //int       bufsize;
    //int       pos;
    //float     beats,fb,dry,wet;
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
//    bool            is_gui_initialized;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("midi_fibonacci0","ccernn","product_string",0,0xfaccface);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        mEditor = NULL;
//        is_gui_initialized = false;
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

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        //TRACE("creating editor (midi_fibonacci)\n");
        //TRACE("- is_gui_initialized = %i\n",(int)is_gui_initialized);
//        if(!is_gui_initialized)
//        {
//          is_gui_initialized=true;
//          char path_img[1024];
//          char* path_vst = (char*)AudioEffectX::getDirectory();
//          TRACE("- path_vst = %s\n",path_vst);
//          //#ifdef WIN32
//          //TRACE("- axBasePath() = %s\n",axBasePath());
//          //#endif
//          //GetModuleFileName( GetModuleHandle("yourdllname.dll" ), buffer, buffer_size );
//          //----------
//          //TODO: srf = axBitmapLoader.loadPng(filename,&w,&h,&buf);
//          path_img[0] = '\0';
//          strncat(path_img,path_vst,256);
//          strncat(path_img,(char*)"cernknob01.png",256);
//          TRACE("- loading:%s\n",path_img);
//          LodePNG_decode32f(&buffer,&width,&height,path_img);
//          knob_bmp = new axBitmap(width,height,buffer);
//          knob_bmp->convertRgbaBgra();
//          knob_bmp->setBackground(128,128,128);
//          knob_srf = new axSurface(width,height,0);
//          knob_srf->mCanvas->drawBitmap(knob_bmp, 0,0, 0,0,width,height);
//          free(buffer);
//          //----------
//          path_img[0] = '\0';
//          strncat(path_img,path_vst,256);
//          strncat(path_img,(char*)"cernback01.png",256);
//          TRACE("- loading:%s\n",path_img);
//          LodePNG_decode32f(&buffer,&width,&height,path_img);
//          back_bmp = new axBitmap(width,height,buffer);
//          back_bmp->convertRgbaBgra();
//          back_bmp->setBackground(128,128,128);
//          back_srf = new axSurface(width,height,0);
//          back_srf->mCanvas->drawBitmap(back_bmp, 0,0, 0,0,width,height);
//          free(buffer);
//          //----------
//        }
        axEditor* EDIT = new axEditor( "midi_fibonacci_editor",this, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), AX_FLAGS );
//        E->appendWidget( new wdgImage( E, -1, axRect(0,0,AX_WIDTH,AX_HEIGHT), wal_None, NULL, back_srf ) );
        for( int i=0; i<mParameters.size(); i++ )
        {
          axParameter* par = mParameters[i]; // skip background
//          axWidget* wdg = new wdgImgKnob(E,i,axRect(10,10+i*40,32,32),wal_None,par,20,knob_srf);
          axWidget* wdg = new wdgKnob(EDIT,i,axRect(10,10+i*40,128,32),wal_None/*,par*/);
          EDIT->appendWidget(wdg);
          EDIT->connect(wdg,par);
        }
        //E->updateWidgetValues();
        //E->doRealign();
        //TRACE("....ok (midi_fibonacci)\n");
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
        //TRACE("process param %i = %f\n",id,f);
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

};

//----------------------------------------------------------------------
#include "axMain.h"
