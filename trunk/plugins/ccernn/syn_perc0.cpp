#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   32
#define AX_NUMPARAMS  34
//#define AX_AUTOSYNC
#define AX_WIDTH      445
#define AX_HEIGHT     275
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"
#include "wdgImgKnob.h"
#include "wdgImage.h"
#include "wdgLabel.h"

#include "axBitmapLoader.h"
#include "images/syn_perc_back.h"
#include "images/syn_perc_knob_20x20_65.h"
#include "images/syn_perc_osc_32x32_8.h"
#include "images/syn_perc_flt_32x32_5.h"
#include "images/syn_perc_com_32x32_5.h"

//----------------------------------------------------------------------

#define MAX_EVENTS 65536
char* str_osc[] = { (char*)"none",(char*)"const",(char*)"audio",(char*)"noise",(char*)"ramp",(char*)"saw",(char*)"squ",(char*)"sin" };
char* str_flt[] = { (char*)"none",(char*)"lowpass",(char*)"highpass",(char*)"bandpass",(char*)"notch" };
char* str_com[] = { (char*)"osc.1",(char*)"osc.2",(char*)"mix",(char*)"AM",(char*)"PD" };

//----------

class myPlugin : public axPlugin,
                 public axWidgetListener
{



  private:
    float EVENTS[MAX_EVENTS];
    float srate,irate,irat2;
    int   offset;
    // parameters
    int   midinote;
    int   o1_wav;
    float o1_vol;
    float o1_volt;
    float o1_vold;
    float o1_pit;
    float o1_pitt;
    float o1_pitd;
    int   o1_flt;
    float o1_frq;
    float o1_frqt;
    float o1_frqd;
    float o1_bw;
    int   o2_wav;
    float o2_vol;
    float o2_volt;
    float o2_vold;
    float o2_pit;
    float o2_pitt;
    float o2_pitd;
    int   o2_flt;
    float o2_frq;
    float o2_frqt;
    float o2_frqd;
    float o2_bw;
    int   mode;
    float fade;
    float boost;
    float clip;
    int   out_flt;
    float out_frq;
    float out_bw;
    float master;
    float mas_d;
    // synth
    float midivel;
    float midifreq;
    float o1_volc;
    float o1_pitc;
    float o1_frqc;
    float o2_volc;
    float o2_pitc;
    float o2_frqc;
    float out_frqc;
    float mas_c;
    float mas_t;
    float ph1,ph2;
    float o1_z1, o1_z2;
    float o2_z1, o2_z2;
    float o0;
    float out_z1, out_z2;

  private:
    bool        is_gui_initialized;
    axEditor    *mEditor;
    axSurface   *mSrfBack,*mSrfKnob,*mSrfKnob_osc,*mSrfKnob_flt,*mSrfKnob_com;
    wdgLabel    *midilabel;
    parInteger  *pMidi;
    char        buf[256];

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {

        mEditor = NULL;
        is_gui_initialized = false;
        describe("syn_perc0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        isSynth();

        midivel = 0;
        midifreq = 0;

        o1_volc = 0;
        o1_pitc = 0;
        o1_frqc = 0;
        o2_volc = 0;
        o2_pitc = 0;
        o2_frqc = 0;
        out_frqc = 0;
        mas_c= 0;
        mas_t = 0;
        ph1 = ph2 = 0;
        o1_z1 = o1_z2 = 0;
        o2_z1 = o2_z2 = 0;
        o0 = 0;
        out_z1 = out_z2 = 0,

        srate = updateSampleRate();
        irate  = 1/srate;
        irat2  = 44100/srate;

        appendParameter( pMidi = new parInteger(this, 0,"midi note",    "", -1,    -1, 127  ));

        appendParameter( new parInteger(this, 1,"osc.1 -----",  "",  0,     0, 7,   str_osc ));
        appendParameter( new parFloat(  this, 2,"volume",       "",  0.75           ));
        appendParameter( new parFloat(  this, 3,"end",          "",  0              ));
        appendParameter( new parFloat(  this, 4,"decay",        "",  20,    0, 100  ));
        appendParameter( new parFloat(  this, 5,"pitch",        "",  0.5            ));
        appendParameter( new parFloat(  this, 6,"end",          "",  0              ));
        appendParameter( new parFloat(  this, 7,"decay",        "",  20,    0, 100  ));
        appendParameter( new parInteger(this, 8,"flt.1 -----",  "",  0,     0, 4,   str_flt ));
        appendParameter( new parFloat(  this, 9,"freq",         "",  0.5            ));
        appendParameter( new parFloat(  this,10,"end",          "",  0              ));
        appendParameter( new parFloat(  this,11,"decay",        "",  100,   0, 100  ));
        appendParameter( new parFloat(  this,12,"bw",           "",  1              ));

        appendParameter( new parInteger(this,13,"osc.2 -----",  "",  7,     0, 7,   str_osc ));
        appendParameter( new parFloat(  this,14,"volume",       "",  0.75           ));
        appendParameter( new parFloat(  this,15,"end",          "",  0              ));
        appendParameter( new parFloat(  this,16,"decay",        "",  20,    0, 100  ));
        appendParameter( new parFloat(  this,17,"pitch",        "",  0.246          ));
        appendParameter( new parFloat(  this,18,"end",          "",  0.246          ));
        appendParameter( new parFloat(  this,19,"decay",        "",  20,    0, 100  ));
        appendParameter( new parInteger(this,20,"flt.2 -----",  "",  0,     0, 4,   str_flt ));
        appendParameter( new parFloat(  this,21,"freq",         "",  0.5            ));
        appendParameter( new parFloat(  this,22,"end",          "",  0              ));
        appendParameter( new parFloat(  this,23,"decay",        "",  100,   0, 100  ));
        appendParameter( new parFloat(  this,24,"bw",           "",  1              ));

        appendParameter( new parInteger(this,25,"combine -----","",  1,     0, 4,   str_com ));
        appendParameter( new parFloat(  this,26,"x-click",      "",  0,     0, 10   ));
        appendParameter( new parFloat(  this,27,"boost",        "",  1,     1, 10   ));
        appendParameter( new parFloat(  this,28,"clip",         "",  0.99           ));
        appendParameter( new parInteger(this,29,"flt  ",        "",  0,     0, 4,   str_flt ));
        appendParameter( new parFloat(  this,30,"freq",         "",  1              ));
        appendParameter( new parFloat(  this,31,"bw",           "",  1              ));
        appendParameter( new parFloat(  this,32,"master",       "",  0.98           ));
        appendParameter( new parFloat(  this,33,"decay",        "",  100,   0, 100  ));
        processParameters();
        //saveProgram();
        //duplicatePrograms();
        initPrograms();
      }

    //----------

    virtual ~myPlugin()
      {
        //if (is_gui_initialized)
        //{
          if (mSrfBack)     delete mSrfBack;
          if (mSrfKnob)     delete mSrfKnob;
          if (mSrfKnob_osc) delete mSrfKnob_osc;
          if (mSrfKnob_flt) delete mSrfKnob_flt;
          if (mSrfKnob_com) delete mSrfKnob_com;
        //}
      }

    //----------

    void initPrograms(void)
      {
        saveProgram();
        duplicatePrograms();
      }

    //----------

    // we need to intercept this, because if the 'midi parameter
    // changes (from the host, automation, ..)
    // we want to update the text display too..
    // we pass on the message to their normal recipients,
    // but do our thing before returning

    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
        if (mEditor && aParameter->mID==0)
        {
          pMidi->doGetDisplay(buf);
          midilabel->setText(buf);
          mEditor->redrawWidget(midilabel);
        }
      }

    //----------

    // and the same thing if we yweak the 'midi' knob
    // we want to do the normal things,
    // then draw the new text

    virtual void onChange(axWidget* aWidget)
      {
        mEditor->onChange(aWidget);
        if (aWidget->mID==0)
        {
          pMidi->doGetDisplay(buf);
          midilabel->setText(buf);
          mEditor->redrawWidget(midilabel);
        }
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("syn_perc_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        if(!is_gui_initialized)
        {
          mSrfBack     = loadPng( syn_perc_back, 13978 );
          mSrfKnob     = loadPng( syn_perc_knob, 7923  );
          mSrfKnob_osc = loadPng( syn_perc_osc,  3767  );
          mSrfKnob_flt = loadPng( syn_perc_flt,  2625  );
          mSrfKnob_com = loadPng( syn_perc_com,  2483  );
          is_gui_initialized=true;
        }

        pMidi->doGetDisplay(buf);

        E->appendWidget(new wdgImage(E,-1,axRect(0,0,445,275),wal_None,/*NULL,*/ mSrfBack));
        E->appendWidget(midilabel = new wdgLabel(E,33,axRect(366,205,51,14),wal_None,/*NULL,*/buf,AX_GREY_LIGHT,tal_Right)); // midi-label

        E->appendWidget(new wdgImgKnob(this,0,axRect( 366,183,  20, 20 ),wal_None,/*mParameters[ 0],*/ 65, mSrfKnob )); // midi
        E->appendWidget(new wdgImgKnob(E,   1,axRect(   9, 45,  32, 32 ),wal_None,/*mParameters[ 1],*/  8, mSrfKnob_osc ));
        E->appendWidget(new wdgImgKnob(E,   2,axRect(  38,102,  20, 20 ),wal_None,/*mParameters[ 2],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,   3,axRect(  62,102,  20, 20 ),wal_None,/*mParameters[ 3],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,   4,axRect(  86,102,  20, 20 ),wal_None,/*mParameters[ 4],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,   5,axRect(  38,126,  20, 20 ),wal_None,/*mParameters[ 5],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,   6,axRect(  62,126,  20, 20 ),wal_None,/*mParameters[ 6],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,   7,axRect(  86,126,  20, 20 ),wal_None,/*mParameters[ 7],*/ 65, mSrfKnob ));

        E->appendWidget(new wdgImgKnob(E,   8,axRect(   9,157,  32, 32 ),wal_None,/*mParameters[ 8],*/  5, mSrfKnob_flt ));
        E->appendWidget(new wdgImgKnob(E,   9,axRect(  38,214,  20, 20 ),wal_None,/*mParameters[ 9],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  10,axRect(  62,214,  20, 20 ),wal_None,/*mParameters[10],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  11,axRect(  86,214,  20, 20 ),wal_None,/*mParameters[11],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  12,axRect(  38,238,  20, 20 ),wal_None,/*mParameters[12],*/ 65, mSrfKnob ));

        E->appendWidget(new wdgImgKnob(E,  13,axRect( 137, 45,  32, 32 ),wal_None,/*mParameters[13],*/  8, mSrfKnob_osc ));
        E->appendWidget(new wdgImgKnob(E,  14,axRect( 166,102,  20, 20 ),wal_None,/*mParameters[14],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  15,axRect( 190,102,  20, 20 ),wal_None,/*mParameters[15],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  16,axRect( 214,102,  20, 20 ),wal_None,/*mParameters[16],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  17,axRect( 166,126,  20, 20 ),wal_None,/*mParameters[17],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  18,axRect( 190,126,  20, 20 ),wal_None,/*mParameters[18],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  19,axRect( 214,126,  20, 20 ),wal_None,/*mParameters[19],*/ 65, mSrfKnob ));

        E->appendWidget(new wdgImgKnob(E,  20,axRect( 137,157,  32, 32 ),wal_None,/*mParameters[20],*/  5, mSrfKnob_flt ));
        E->appendWidget(new wdgImgKnob(E,  21,axRect( 166,214,  20, 20 ),wal_None,/*mParameters[21],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  22,axRect( 190,214,  20, 20 ),wal_None,/*mParameters[22],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  23,axRect( 214,214,  20, 20 ),wal_None,/*mParameters[23],*/ 65, mSrfKnob ));
        E->appendWidget(new wdgImgKnob(E,  24,axRect( 166,238,  20, 20 ),wal_None,/*mParameters[24],*/ 65, mSrfKnob ));

        E->appendWidget(new wdgImgKnob(E,  25,axRect( 265, 45,  32, 32 ),wal_None,/*mParameters[25],*/  5, mSrfKnob_com )); // combine
        E->appendWidget(new wdgImgKnob(E,  26,axRect( 366,158,  20, 20 ),wal_None,/*mParameters[26],*/ 65, mSrfKnob )); // x-click
        E->appendWidget(new wdgImgKnob(E,  27,axRect( 278, 94,  20, 20 ),wal_None,/*mParameters[27],*/ 65, mSrfKnob )); // boost
        E->appendWidget(new wdgImgKnob(E,  28,axRect( 278,118,  20, 20 ),wal_None,/*mParameters[28],*/ 65, mSrfKnob )); // clip
        E->appendWidget(new wdgImgKnob(E,  29,axRect( 265,157,  32, 32 ),wal_None,/*mParameters[29],*/  5, mSrfKnob_flt )); // flt
        E->appendWidget(new wdgImgKnob(E,  30,axRect( 278,198,  20, 20 ),wal_None,/*mParameters[30],*/ 65, mSrfKnob )); // freq
        E->appendWidget(new wdgImgKnob(E,  31,axRect( 278,222,  20, 20 ),wal_None,/*mParameters[31],*/ 65, mSrfKnob )); // bw
        E->appendWidget(new wdgImgKnob(E,  32,axRect( 366, 94,  20, 20 ),wal_None,/*mParameters[32],*/ 65, mSrfKnob )); // master
        E->appendWidget(new wdgImgKnob(E,  33,axRect( 366,118,  20, 20 ),wal_None,/*mParameters[33],*/ 65, mSrfKnob )); // decay

        for (int i=0;i<AX_NUMPARAMS; i++) E->connect( E->mWidgets[i+2], mParameters[i] );
        //midilabel->setBackground(true, axColor(120,120,120));
        mEditor = E;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* tempeditor = mEditor;
        mEditor = NULL;
        delete tempeditor;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        mEditor->redrawDirty();
      }

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

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
        int msg = msg1 & 240;
        if (msg == 9*16)
        {
          int _note = msg2;// & 127;
          if (midinote<0 || midinote==_note)
          {
            float _vel = msg3 * inv127;
          //float _freq   = 220 * pow(2.0,(_note-36.0) / 12);
            float _freq   = 440 * pow(2.0,(_note-69.0) / 12);
            EVENTS[ofs*2  ] = _vel;
            EVENTS[ofs*2+1] = _freq;
          }
        }
        sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case  0:
            midinote  = f;
//            if (mEditor)
//            {
//              pMidi->doGetDisplay(buf);
//              midilabel->setText(buf);
//              mEditor->redrawWidget(midilabel);
//            }
            break;
          case  1: o1_wav    = f; break;
          case  2: o1_vol    = f*f; break;
          case  3: o1_volt   = f*f; break;
          case  4: o1_vold   = irat2 / axMax(1,f*f*f); break;
          case  5: o1_pit    = srate * f*f*f*0.1; break;
          case  6: o1_pitt   = srate * f*f*f*0.1; break;
          case  7: o1_pitd   = irat2 / axMax(1,f*f*f); break;
          case  8: o1_flt    = f; break;
          case  9: o1_frq    = f*f*f; break;
          case 10: o1_frqt   = f*f*f; break;
          case 11: o1_frqd   = irat2 / axMax(1,f*f*f); break;
          case 12: o1_bw     = f*f; break;
          case 13: o2_wav    = f; break;
          case 14: o2_vol    = f*f; break;
          case 15: o2_volt   = f*f; break;
          case 16: o2_vold   = irat2 / axMax(1,f*f*f); break;
          case 17: o2_pit    = srate * f*f*f*0.1; break;
          case 18: o2_pitt   = srate * f*f*f*0.1; break;
          case 19: o2_pitd   = irat2 / axMax(1,f*f*f); break;
          case 20: o2_flt    = f; break;
          case 21: o2_frq    = f*f*f; break;
          case 22: o2_frqt   = f*f*f; break;
          case 23: o2_frqd   = irat2 / axMax(1,f*f*f); break;
          case 24: o2_bw     = f*f; break;
          case 25: mode      = f; break;
          case 26: fade      = irat2 / axMax(1,f*f); break;
          case 27: boost     = f; break;
          case 28: clip      = f*f; break;
          case 29: out_flt   = f; break;
          case 30: out_frq   = f*f*f; break;
          case 31: out_bw    = f*f; break;
          case 32: master    = f*f; break;
          case 33: mas_d     = irat2 / axMax(1,f*f*f); break;
        }
      }

    //--------------------------------------------------


    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //srate = updateSampleRate();
        offset = 0;
        return false;
      }

    //----------


    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        //---------- midi

        if (EVENTS[offset*2]>0)
        {
          midivel  = EVENTS[offset*2];
          midifreq = EVENTS[offset*2+1];
          o1_volc = o1_vol;
          o1_pitc = o1_pit;
          o1_frqc = o1_frq;
          o2_volc = o2_vol;
          o2_pitc = o2_pit;
          o2_frqc = o2_frq;
          out_frqc = out_frq;
          mas_c = master;
          if (midinote<0)
          {
            if (o2_wav==3) // noise
            {
              o2_frqc = midifreq/srate * 8;
              //o2_frqt = midifreq/srate * 8;
            } else { // osc
              o2_pitc = midifreq;
              //o2_pitt = midifreq;
            }
          }
        }
        EVENTS[offset*2] = -1;
        offset += 1;

        //---------- osc 1

        float o1 = 0; // none
        switch(o1_wav)
        {
          case 1: o1 = 1;                         break; // const
          case 2: o1 = spl0;                      break; // audio
          case 3: o1 = axRandomSigned();          break; // noise
          case 4: o1 = 1-(ph1*2);                 break; // saw
          case 5: o1 = (ph1*2)-1;                 break; // ramp
          case 6: if (ph1<0.5) o1=1; else o1=-1;  break; // squ
          case 7: o1 = sin(ph1*PI2); break;       // sin (todo:sinusoid)
        }
        o1 *= o1_volc;
        // update
        if( o1_wav>0)
        {
          ph1 += o1_pitc*irate;
          if (ph1>=1) ph1-=1;
          o1_volc += (o1_volt-o1_volc) * o1_vold;
          o1_pitc += (o1_pitt-o1_pitc) * o1_pitd;
        }

        //---------- filter 1

        if (o1_flt>0)
        {
          float _L = o1_z2 + o1_frqc * o1_z1;
          float _H = o1 - _L - o1_bw * o1_z1;
          float _B = o1_frqc * _H + o1_z1;
          float _N = _H + _L;
          o1_z1 = _B;
          o1_z2 = _L;
          switch(o1_flt)
          {
            case 1: o1=_L; break;
            case 2: o1=_H; break;
            case 3: o1=_B; break;
            case 4: o1=_N; break;
          }
          // update
          o1_frqc += (o1_frqt-o1_frqc) * o1_frqd;
        }
        //---------- phase distort
        float pp2 = ph2;
        if (mode==4)
        {
          pp2 += o1;
          if (pp2>=1) pp2-=1;
        }

        //---------- osc 2

        float o2 = 0; // none
        switch(o2_wav)
        {
          case 1: o2 = 1;                         break; // const
          case 2: o2 = spl1;                      break; // audio
          case 3: o2 = axRandomSigned();          break; // noise
          case 4: o2 = 1-(ph2*2);                 break; // saw
          case 5: o2 = (ph2*2)-1;                 break; // ramp
          case 6: if (pp2<0.5) o2=1; else o2=-1;  break; // squ
          case 7: o2 = sin(pp2*PI2);              break; // sin (todo:sinusoid)
        }
        o2 *= o2_volc;
        // update
        if( o2_wav>0)
        {
          ph2 += o2_pitc*irate;
          if (ph2>=1) ph2-=1;
          o2_volc += (o2_volt-o2_volc) * o2_vold;
          o2_pitc += (o2_pitt-o2_pitc) * o2_pitd;
        }

        //  //---------- filter 2

        if (o2_flt>0)
        {
          float _L = o2_z2 + o2_frqc * o2_z1;
          float _H = o2 - _L - o2_bw * o2_z1;
          float _B = o2_frqc * _H + o2_z1;
          float _N = _H + _L;
          o2_z1 = _B;
          o2_z2 = _L;
          switch(o2_flt)
          {
            case 1: o2=_L; break;
            case 2: o2=_H; break;
            case 3: o2=_B; break;
            case 4: o2=_N; break;
          }
          // update
          o2_frqc += (o2_frqt-o2_frqc) * o2_frqd;
        }

        //---------- combine

        float out = 0;
        switch(mode)
        {
          case 0: out = o1;     break;
          case 1: out = o2;     break;
          case 2: out = o1+o2;  break;
          case 3: out = o1*o2;  break;
          case 4: out = o2;     break;
        }
        out *= midivel;
        out *= mas_c;// master;
        out = ( o0 += (out-o0) * fade );
        out *= boost;
        // update
        mas_c += (mas_t-mas_c) * mas_d;

        //---------- filter

        if (out_flt>0)
        {
          float _L = out_z2 + out_frqc * out_z1;
          float _H = out - _L - out_bw * out_z1;
          float _B = out_frqc * _H + out_z1;
          float _N = _H + _L;
          out_z1 = _B;
          out_z2 = _L;
          switch(out_flt)
          {
            case 1: out = _L; break;
            case 2: out = _H; break;
            case 3: out = _B; break;
            case 4: out = _N; break;
          }
        }

        out = axMin(clip,axMax(-clip,out));
        *outs[0] = out;
        *outs[1] = out;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
