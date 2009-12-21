#define AX_PLUGIN     myPlugin
#define AX_PLUGIN_AUTOSYNC
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  34
#define AX_WIDTH      640
#define AX_HEIGHT     480
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )

#define MAXEVENTS 65536

#define AX_DEBUG

#include "axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "parInteger.h"
#include "wdgLabel.h"
#include "wdgKnobPar.h"
#include "wdgKnobSmallPar.h"

char* str_osc[] = { (char*)"none",(char*)"const",(char*)"audio in",(char*)"noise",(char*)"sine",(char*)"saw",(char*)"inv saw",(char*)"square" };
char* str_flt[] = { (char*)"none",(char*)"low-pass",(char*)"high-pass",(char*)"band-pass",(char*)"notch" };
char* str_com[] = { (char*)"osc1",(char*)"osc2",(char*)"mix (1+2)",(char*)"AM (1*2)",(char*)"PD / FM" };

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:

    float EVENTS[MAXEVENTS];
    int   offset;
    float irate, irat2;
    int   midinote;
    float midivel, midifreq;

    float ph1, ph2, pp2;
    float _L,_H,_B,_N;
    float o1_z1,o1_z2,o2_z1,o2_z2, out_z1,out_z2;
    float out,o0,o1,o2;

    int   Osc1_type;  // osc 1 type
    float Osc1_vs;    // osc 1 vol start
    float Osc1_ve;    // osc 1 vol end
    float Osc1_vd;    // osc 1 vol decay
    float Osc1_ps;    // osc 1 pitch start
    float Osc1_pe;    // osc 1 pitch end
    float Osc1_pd;    // osc 1 pitch decay

    int   Flt1_type;  // flt 1 type
    float Flt1_fs;    // flt 1 freq start
    float Flt1_fe;    // flt 1 freq end
    float Flt1_fd;    // flt 1 freq decay
    float Flt1_bw;    // flt 1 bw

    int   Osc2_type;  // osc 1 type
    float Osc2_vs;    // osc 1 vol start
    float Osc2_ve;    // osc 1 vol end
    float Osc2_vd;    // osc 1 vol decay
    float Osc2_ps;    // osc 1 pitch start
    float Osc2_pe;    // osc 1 pitch end
    float Osc2_pd;    // osc 1 pitch decay

    int   Flt2_type;  // flt 1 type
    float Flt2_fs;    // flt 1 freq start
    float Flt2_fe;    // flt 1 freq end
    float Flt2_fd;    // flt 1 freq decay
    float Flt2_bw;    // flt 1 bw

    int   Comb_mode;   // combine mode
    float Xfade;       // crossfade
    float Boost;       // boost
    float Clip;        // clip
    int   Flt3_type;   // flt type
    float Flt3_fr;     // flt freq
    float Flt3_bw;     // flt bw
    float Master;      // master
    float Master_d;    // master decay
    int   Midi;        // midi

    float Osc1_vt, Osc1_pt, Flt1_ft, Osc2_vt, Osc2_pt, Flt2_ft, Master_t;
    float Osc1_vc, Osc1_pc, Flt1_fc, Osc2_vc, Osc2_pc, Flt2_fc, Master_c;

  public:

    parInteger  *pOsc1_type;  // osc 1 type
    parFloat    *pOsc1_vs;    // osc 1 vol start
    parFloat    *pOsc1_ve;    // osc 1 vol end
    parFloat    *pOsc1_vd;    // osc 1 vol decay
    parFloat    *pOsc1_ps;    // osc 1 pitch start
    parFloat    *pOsc1_pe;    // osc 1 pitch end
    parFloat    *pOsc1_pd;    // osc 1 pitch decay

    parInteger  *pFlt1_type;  // flt 1 type
    parFloat    *pFlt1_fs;    // flt 1 freq start
    parFloat    *pFlt1_fe;    // flt 1 freq end
    parFloat    *pFlt1_fd;    // flt 1 freq decay
    parFloat    *pFlt1_bw;    // flt 1 bw

    parInteger  *pOsc2_type;  // osc 1 type
    parFloat    *pOsc2_vs;    // osc 1 vol start
    parFloat    *pOsc2_ve;    // osc 1 vol end
    parFloat    *pOsc2_vd;    // osc 1 vol decay
    parFloat    *pOsc2_ps;    // osc 1 pitch start
    parFloat    *pOsc2_pe;    // osc 1 pitch end
    parFloat    *pOsc2_pd;    // osc 1 pitch decay

    parInteger  *pFlt2_type;  // flt 1 type
    parFloat    *pFlt2_fs;    // flt 1 freq start
    parFloat    *pFlt2_fe;    // flt 1 freq end
    parFloat    *pFlt2_fd;    // flt 1 freq decay
    parFloat    *pFlt2_bw;    // flt 1 bw

    parInteger  *pComb_mode;   // combine mode
    parFloat    *pXfade;       // crossfade
    parFloat    *pBoost;       // boost
    parFloat    *pClip;        // clip
    parInteger  *pFlt3_type;   // flt type
    parFloat    *pFlt3_fr;     // flt freq
    parFloat    *pFlt3_bw;     // flt bw
    parFloat    *pMaster;      // master
    parFloat    *pMaster_d;    // master decay
    parInteger  *pMidi;        // midi

  public:
    axEditor    *mEditor;

    wdgKnobPar      *wOsc1_type;  // osc 1 type
    wdgKnobSmallPar *wOsc1_vs;    // osc 1 vol start
    wdgKnobSmallPar *wOsc1_ve;    // osc 1 vol end
    wdgKnobSmallPar *wOsc1_vd;    // osc 1 vol decay
    wdgKnobSmallPar *wOsc1_ps;    // osc 1 pitch start
    wdgKnobSmallPar *wOsc1_pe;    // osc 1 pitch end
    wdgKnobSmallPar *wOsc1_pd;    // osc 1 pitch decay

    wdgKnobPar      *wFlt1_type;  // flt 1 type
    wdgKnobSmallPar *wFlt1_fs;    // flt 1 freq start
    wdgKnobSmallPar *wFlt1_fe;    // flt 1 freq end
    wdgKnobSmallPar *wFlt1_fd;    // flt 1 freq decay
    wdgKnobSmallPar *wFlt1_bw;    // flt 1 bw

    wdgKnobPar      *wOsc2_type;  // osc 1 type
    wdgKnobSmallPar *wOsc2_vs;    // osc 1 vol start
    wdgKnobSmallPar *wOsc2_ve;    // osc 1 vol end
    wdgKnobSmallPar *wOsc2_vd;    // osc 1 vol decay
    wdgKnobSmallPar *wOsc2_ps;    // osc 1 pitch start
    wdgKnobSmallPar *wOsc2_pe;    // osc 1 pitch end
    wdgKnobSmallPar *wOsc2_pd;    // osc 1 pitch decay

    wdgKnobPar      *wFlt2_type;  // flt 1 type
    wdgKnobSmallPar *wFlt2_fs;    // flt 1 freq start
    wdgKnobSmallPar *wFlt2_fe;    // flt 1 freq end
    wdgKnobSmallPar *wFlt2_fd;    // flt 1 freq decay
    wdgKnobSmallPar *wFlt2_bw;    // flt 1 bw

    wdgKnobPar      *wComb_mode;   // combine mode
    wdgKnobSmallPar *wXfade;       // crossfade
    wdgKnobSmallPar *wBoost;       // boost
    wdgKnobSmallPar *wClip;        // clip
    wdgKnobPar      *wFlt3_type;   // flt type
    wdgKnobSmallPar *wFlt3_fr;     // flt freq
    wdgKnobSmallPar *wFlt3_bw;     // flt bw
    wdgKnobPar      *wMaster;      // master
    wdgKnobSmallPar *wMaster_d;    // master decay
    wdgKnobSmallPar *wMidi;        // midi

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        axRandomize(418);
        describe("syn_perc0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        isSynth();

        //memset(EVENTS,0,sizeof(float)*65536);
        for( int i=0; i<MAXEVENTS; i++ ) EVENTS[i] = -1;
        irate  = 1/mSampleRate;
        irat2  = 44100 / mSampleRate;

        Osc1_type = 4;
        Osc1_vs   = 1;
        Osc1_ve   = 0;
        Osc1_vd   = 20;
        Osc1_ps   = 0.5;
        Osc1_pe   = 0;
        Osc1_pd   = 20;

        Flt1_type = 0;
        Flt1_fs   = 1;
        Flt1_fe   = 0;
        Flt1_fd   = 100;
        Flt1_bw   = 1;

        Osc2_type = 4;
        Osc2_vs   = 1;
        Osc2_ve   = 0;
        Osc2_vd   = 20;
        Osc2_ps   = 0.5;
        Osc2_pe   = 0;
        Osc2_pd   = 20;

        Flt2_type = 0;
        Flt2_fs   = 1;
        Flt2_fe   = 0;
        Flt2_fd   = 100;
        Flt2_bw   = 1;

        Comb_mode = 2;
        Xfade     = 4;
        Boost     = 1;
        Clip      = 1;
        Flt3_type = 0;
        Flt3_fr   = 1;
        Flt3_bw   = 1;
        Master    = 1;
        Master_d  = 50;
        Midi      = -1;

        appendParameter( pOsc1_type = new parInteger( this,  0, "oscillator 1",     "", Osc1_type, 0,7, str_osc ) );
        appendParameter( pOsc1_vs   = new parFloat(   this,  1, "osc1 vol start",   "", Osc1_vs ) );
        appendParameter( pOsc1_ve   = new parFloat(   this,  2, "osc1 vol end",     "", Osc1_ve ) );
        appendParameter( pOsc1_vd   = new parFloat(   this,  3, "osc1 vol decay",   "", Osc1_vd,0,100 ) );
        appendParameter( pOsc1_ps   = new parFloat(   this,  4, "osc1 pitch start", "", Osc1_vs ) );
        appendParameter( pOsc1_pe   = new parFloat(   this,  5, "osc1 pitch end",   "", Osc1_ve ) );
        appendParameter( pOsc1_pd   = new parFloat(   this,  6, "osc1 pitch decay", "", Osc1_vd,0,100 ) );

        appendParameter( pFlt1_type = new parInteger( this,  7, "filter 1",         "", Flt1_type, 0,4, str_flt ) );
        appendParameter( pFlt1_fs   = new parFloat(   this,  8, "flt1 freq start",  "", Flt1_fs ) );
        appendParameter( pFlt1_fe   = new parFloat(   this,  9, "flt1 freq end",    "", Flt1_fe ) );
        appendParameter( pFlt1_fd   = new parFloat(   this, 10, "flt1 freq decay",  "", Flt1_fd,0,100 ) );
        appendParameter( pFlt1_bw   = new parFloat(   this, 11, "flt1 bw",          "", Flt1_bw ) );

        appendParameter( pOsc2_type = new parInteger( this, 12, "oscillator 2",     "", Osc2_type, 0,7, str_osc ) );
        appendParameter( pOsc2_vs   = new parFloat(   this, 13, "osc2 vol start",   "", Osc2_vs ) );
        appendParameter( pOsc2_ve   = new parFloat(   this, 14, "osc2 vol end",     "", Osc2_ve ) );
        appendParameter( pOsc2_vd   = new parFloat(   this, 15, "osc2 vol decay",   "", Osc2_vd,0,100 ) );
        appendParameter( pOsc2_ps   = new parFloat(   this, 16, "osc2 pitch start", "", Osc2_ps ) );
        appendParameter( pOsc2_pe   = new parFloat(   this, 17, "osc2 pitch end",   "", Osc2_pe ) );
        appendParameter( pOsc2_pd   = new parFloat(   this, 18, "osc2 pitch decay", "", Osc2_pd,0,100 ) );

        appendParameter( pFlt2_type = new parInteger( this, 19, "filter 2",         "", Flt2_type, 0,4, str_flt ) );
        appendParameter( pFlt2_fs   = new parFloat(   this, 20, "flt2 freq start",  "", Flt2_fs ) );
        appendParameter( pFlt2_fe   = new parFloat(   this, 21, "flt2 freq end",    "", Flt2_fe ) );
        appendParameter( pFlt2_fd   = new parFloat(   this, 22, "flt2 freq end",    "", Flt2_fd,0,100 ) );
        appendParameter( pFlt2_bw   = new parFloat(   this, 23, "flt2 bw",          "", Flt2_bw ) );

        appendParameter( pComb_mode = new parInteger( this, 24, "combine",          "", Comb_mode, 0,4, str_com ) );
        appendParameter( pXfade     = new parFloat(   this, 25, "xfade",            "", Xfade, 0,10 ) );
        appendParameter( pBoost     = new parFloat(   this, 26, "boost",            "", Boost, 1,10 ) );
        appendParameter( pClip      = new parFloat(   this, 27, "clip",             "", Clip ) );
        appendParameter( pFlt3_type = new parInteger( this, 28, "filter 3",         "", Flt3_type, 0,4, str_flt ) );
        appendParameter( pFlt3_fr   = new parFloat(   this, 29, "flt3 freq",        "", Flt3_fr ) );
        appendParameter( pFlt3_bw   = new parFloat(   this, 30, "flt3 bw",          "", Flt3_bw ) );
        appendParameter( pMaster    = new parFloat(   this, 31, "master",           "", Master ) );
        appendParameter( pMaster_d  = new parFloat(   this, 32, "master decay",     "", Master_d, 0,100 ) );
        appendParameter( pMidi      = new parInteger( this, 33, "midi",             "", Midi, -1,127 ) );

        Osc1_vt = Osc1_pt = Flt1_ft = Osc2_vt = Osc2_pt = Flt2_ft = Master_t = 0;
        Osc1_vc = Osc1_pc = Flt1_fc = Osc2_vc = Osc2_pc = Flt2_fc = Master_c = 0;

        midinote  = 0;
        midivel   = 0;
        midifreq  = 0;
        _L = _H = _B = _N = 0;
        ph1 = ph2 = pp2 = 0;
        o0 = o1_z1 = o1_z2 = o2_z1 = o2_z2 = out_z1 = out_z2 = 0;

      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor( this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );

        //E->setLayout(10,10,10,10);
        //E->setFlag(wfl_Vertical);

        E->appendWidget( wOsc1_type = new wdgKnobPar(      E,  0, axRect( 10, 10, 128, 32), wal_None, pOsc1_type ) );
        E->appendWidget( wOsc1_vs   = new wdgKnobSmallPar( E,  1, axRect( 10, 50,  64, 16), wal_None, pOsc1_vs ) );
        E->appendWidget( wOsc1_ve   = new wdgKnobSmallPar( E,  2, axRect( 10, 70,  64, 16), wal_None, pOsc1_ve ) );
        E->appendWidget( wOsc1_vd   = new wdgKnobSmallPar( E,  3, axRect( 10, 90,  64, 16), wal_None, pOsc1_vd ) );
        E->appendWidget( wOsc1_ps   = new wdgKnobSmallPar( E,  4, axRect( 80, 50,  64, 16), wal_None, pOsc1_ps ) );
        E->appendWidget( wOsc1_pe   = new wdgKnobSmallPar( E,  5, axRect( 80, 70,  64, 16), wal_None, pOsc1_pe ) );
        E->appendWidget( wOsc1_pd   = new wdgKnobSmallPar( E,  6, axRect( 80, 90,  64, 16), wal_None, pOsc1_pd ) );

        E->appendWidget( wFlt1_type = new wdgKnobPar(      E,  7, axRect( 10,110, 128, 32), wal_None, pFlt1_type ) );
        E->appendWidget( wFlt1_fs   = new wdgKnobSmallPar( E,  8, axRect( 10,150,  64, 16), wal_None, pFlt1_fs ) );
        E->appendWidget( wFlt1_fe   = new wdgKnobSmallPar( E,  9, axRect( 10,170,  64, 16), wal_None, pFlt1_fe ) );
        E->appendWidget( wFlt1_fd   = new wdgKnobSmallPar( E, 10, axRect( 10,190,  64, 16), wal_None, pFlt1_fd ) );
        E->appendWidget( wFlt1_bw   = new wdgKnobSmallPar( E, 11, axRect( 80,150,  64, 16), wal_None, pFlt1_bw ) );

        E->appendWidget( wOsc2_type = new wdgKnobPar(      E, 12, axRect(210, 10, 128, 32), wal_None, pOsc2_type ) );
        E->appendWidget( wOsc2_vs   = new wdgKnobSmallPar( E, 13, axRect(210, 50,  64, 16), wal_None, pOsc2_vs ) );
        E->appendWidget( wOsc2_ve   = new wdgKnobSmallPar( E, 14, axRect(210, 70,  64, 16), wal_None, pOsc2_ve ) );
        E->appendWidget( wOsc2_vd   = new wdgKnobSmallPar( E, 15, axRect(210, 90,  64, 16), wal_None, pOsc2_vd ) );
        E->appendWidget( wOsc2_ps   = new wdgKnobSmallPar( E, 16, axRect(280, 50,  64, 16), wal_None, pOsc2_ps ) );
        E->appendWidget( wOsc2_pe   = new wdgKnobSmallPar( E, 17, axRect(280, 70,  64, 16), wal_None, pOsc2_pe ) );
        E->appendWidget( wOsc2_pd   = new wdgKnobSmallPar( E, 18, axRect(280, 90,  64, 16), wal_None, pOsc2_pd ) );

        E->appendWidget( wFlt2_type = new wdgKnobPar(      E, 19, axRect(210,110, 128, 64), wal_None, pFlt2_type ) );
        E->appendWidget( wFlt2_fs   = new wdgKnobSmallPar( E, 20, axRect(210,150,  64, 16), wal_None, pFlt2_fs ) );
        E->appendWidget( wFlt2_fe   = new wdgKnobSmallPar( E, 21, axRect(210,170,  64, 16), wal_None, pFlt2_fe ) );
        E->appendWidget( wFlt2_fd   = new wdgKnobSmallPar( E, 22, axRect(210,190,  64, 16), wal_None, pFlt2_fd ) );
        E->appendWidget( wFlt2_bw   = new wdgKnobSmallPar( E, 23, axRect(280,150,  64, 16), wal_None, pFlt2_bw ) );

        E->appendWidget( wComb_mode = new wdgKnobPar(      E, 24, axRect(410, 10, 128, 32), wal_None, pComb_mode ) );
        E->appendWidget( wXfade     = new wdgKnobSmallPar( E, 25, axRect(410, 50,  64, 16), wal_None, pXfade ) );
        E->appendWidget( wBoost     = new wdgKnobSmallPar( E, 26, axRect(410, 70,  64, 16), wal_None, pBoost ) );
        E->appendWidget( wClip      = new wdgKnobSmallPar( E, 27, axRect(480, 50,  64, 16), wal_None, pClip ) );
        E->appendWidget( wFlt3_type = new wdgKnobPar(      E, 28, axRect(410, 90, 128, 32), wal_None, pFlt3_type ) );
        E->appendWidget( wFlt3_fr   = new wdgKnobSmallPar( E, 29, axRect(410,130,  64, 16), wal_None, pFlt3_fr ) );
        E->appendWidget( wFlt3_bw   = new wdgKnobSmallPar( E, 30, axRect(480,130,  64, 16), wal_None, pFlt3_bw ) );
        E->appendWidget( wMaster    = new wdgKnobPar(      E, 31, axRect(410,150, 128, 32), wal_None, pMaster ) );
        E->appendWidget( wMaster_d  = new wdgKnobSmallPar( E, 32, axRect(410,190,  64, 16), wal_None, pMaster_d ) );
        E->appendWidget( wMidi      = new wdgKnobSmallPar( E, 33, axRect(480,190,  64, 16), wal_None, pMidi ) );

        //E->doRealign();

        wOsc1_type->doSetValue(pOsc1_type->doGetValue() );
        wOsc1_vs->doSetValue(  pOsc1_vs->doGetValue() );
        wOsc1_ve->doSetValue(  pOsc1_ve->doGetValue() );
        wOsc1_vd->doSetValue(  pOsc1_vd->doGetValue() );
        wOsc1_ps->doSetValue(  pOsc1_ps->doGetValue() );
        wOsc1_pe->doSetValue(  pOsc1_pe->doGetValue() );
        wOsc1_pd->doSetValue(  pOsc1_pd->doGetValue() );

        wFlt1_type->doSetValue(pFlt1_type->doGetValue() );
        wFlt1_fs->doSetValue(  pFlt1_fs->doGetValue() );
        wFlt1_fe->doSetValue(  pFlt1_fe->doGetValue() );
        wFlt1_fd->doSetValue(  pFlt1_fd->doGetValue() );
        wFlt1_bw->doSetValue(  pFlt1_bw->doGetValue() );

        wOsc2_type->doSetValue(pOsc2_type->doGetValue() );
        wOsc2_vs->doSetValue(  pOsc2_vs->doGetValue() );
        wOsc2_ve->doSetValue(  pOsc2_ve->doGetValue() );
        wOsc2_vd->doSetValue(  pOsc2_vd->doGetValue() );
        wOsc2_ps->doSetValue(  pOsc2_ps->doGetValue() );
        wOsc2_pe->doSetValue(  pOsc2_pe->doGetValue() );
        wOsc2_pd->doSetValue(  pOsc2_pd->doGetValue() );

        wFlt2_type->doSetValue(pFlt2_type->doGetValue() );
        wFlt2_fs->doSetValue(  pFlt2_fs->doGetValue() );
        wFlt2_fe->doSetValue(  pFlt2_fe->doGetValue() );
        wFlt2_fd->doSetValue(  pFlt2_fd->doGetValue() );
        wFlt2_bw->doSetValue(  pFlt2_bw->doGetValue() );

        wComb_mode->doSetValue(pComb_mode->doGetValue() );
        wXfade->doSetValue(    pXfade->doGetValue() );
        wBoost->doSetValue(    pBoost->doGetValue() );
        wClip->doSetValue(     pClip->doGetValue() );
        wFlt3_type->doSetValue(pFlt3_type->doGetValue() );
        wFlt3_fr->doSetValue(  pFlt3_fr->doGetValue() );
        wFlt3_bw->doSetValue(  pFlt3_bw->doGetValue() );
        wMaster->doSetValue(   pMaster->doGetValue() );
        wMaster_d->doSetValue( pMaster_d->doGetValue() );
        wMidi->doSetValue(     pMidi->doGetValue() );

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

    //----------

    virtual void doProcessState(int aState)
      {
        TRACE("DoProcessState: %i\n",aState);
      }

    //----------

    virtual void doProcessTransport(int aState)
      {
        TRACE("doProcessTransport: %i\n",aState);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case  0: Osc1_type  = f;                        break;
          case  1: Osc1_vs    = f*f;                      break;
          case  2: Osc1_ve    = f*f;                      break;
          case  3: Osc1_vd    = irat2/axMax(1,f*f*f);     break;
          case  4: Osc1_ps    = mSampleRate*(f*f*f)*0.1;  break;
          case  5: Osc1_pe    = mSampleRate*(f*f*f)*0.1;  break;
          case  6: Osc1_pd    = irat2 / axMax(1,f*f*f);   break;

          case  7: Flt1_type  = f;                        break;
          case  8: Flt1_fs    = f*f*f;                    break;
          case  9: Flt1_fe    = f*f*f;                    break;
          case 10: Flt1_fd    = irat2/axMax(1,f*f*f);     break;
          case 11: Flt1_bw    = f*f;                      break;

          case 12: Osc2_type  = f;                        break;
          case 13: Osc2_vs    = f*f;                      break;
          case 14: Osc2_ve    = f*f;                      break;
          case 15: Osc2_vd    = irat2/axMax(1,f*f*f);     break;
          case 16: Osc2_ps    = mSampleRate*(f*f*f)*0.1;  break;
          case 17: Osc2_pe    = mSampleRate*(f*f*f)*0.1;  break;
          case 18: Osc2_pd    = irat2/axMax(1,f*f*f);     break;

          case 19: Flt2_type  = f;                        break;
          case 20: Flt2_fs    = f*f*f;                    break;
          case 21: Flt2_fe    = f*f*f;                    break;
          case 22: Flt2_fd    = irat2/axMax(1,f*f*f);     break;
          case 23: Flt2_bw    = f*f;                      break;

          case 24: Comb_mode  = f;                        break;
          case 25: Xfade      = irat2/axMax(1,f*f*f);     break;
          case 26: Boost      = f;                        break;
          case 27: Clip       = f*f;                      break;
          case 28: Flt3_type  = f;                        break;
          case 29: Flt3_fr    = f*f*f;                    break;
          case 30: Flt3_bw    = f*f;                      break;
          case 31: Master     = f*f;                      break;
          case 32: Master_d   = irat2/axMax(1,f*f*f);     break;
          case 33: Midi       = f;                        break;

       }
      }

    //----------

    virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
      {
        //TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
        if( msg1==9*16 )
        {
          int _note = msg2;
          if( midinote<0 || midinote==_note )
          {
            float _vel = (float)msg3 * inv127;/// 127;
            //_freq   = 220 * pow(2.0,(_note-36.0) / 12);
            float _freq   = 440 * pow(2.0,(_note-69.0) / 12);
            EVENTS[ofs*2]     = _vel;
            EVENTS[(ofs*2)+1] = _freq;
            //TRACE("vel:%f, freq:%f\n",_vel,_freq);
          }
        }
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //updateTimeInfo();
        //TODO: if playstate changed, reset buffer, counters
        //for( int i=0; i<MAX_EVENTS; i++ ) EVENTS[i] = -1;
        offset = 0;
        return false; // we want doProcessSample calls
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

//---------- midi

        if( EVENTS[offset*2] > 0 )
        {
          //TRACE("event!\n");
          midivel   = EVENTS[offset*2];
          midifreq  = EVENTS[(offset*2)+1];
          Osc1_vc   = Osc1_vs;
          Osc1_pc   = Osc1_ps;
          Flt1_fc   = Flt1_fs;
          Osc2_vc   = Osc2_vs;
          Osc2_pc   = Osc2_ps;
          Flt2_fc   = Flt2_fs;
          //Flt3_fc   = Flt3_fr;
          Master_c  = Master;
          if( midinote<0 )
          {
            if( Osc2_type==2 )
            { // noise
              Flt2_fc = midifreq/mSampleRate * 8;
              //o2_frqt = midifreq/mSampleRate * 8;
            }
            else
            { // osc
              Osc2_pc = midifreq;
              //o2_pitt = midifreq/mSampleRate * 8;
            }
          }
        }
        EVENTS[offset*2] = -1;
        offset += 1;

//---------- osc 1

        o1 = 0;                          // none
        switch(Osc1_type)
        {
          case 1: o1 = 1;                 break;  // const
          case 2: o1 = axRandomSigned();  break;  // noise
          case 3: o1 = spl0;              break;  // audio
          case 4: o1 = (ph1*2)-1;         break;  // saw
          case 5: o1 = 1-(ph1*2);         break;  // inv.saw
          case 6: o1 = (ph1<0.5)?1:-1;    break;  // squ
          case 7: o1 = sin(ph1*PI2);      break;  // sin (todo:sinusoid)
        }
        o1 *= Osc1_vc;
        // update
        if( Osc1_type>0 )
        {
          ph1 += Osc1_pc*irate;
          if( ph1 >= 1 ) ph1 -= 1;
          Osc1_vc += (Osc1_vt-Osc1_vc) * Osc1_vd;
          Osc1_pc += (Osc1_pt-Osc1_pc) * Osc1_pd;
        }

//---------- filter 1

        if( Flt1_type > 0 )
        {
          _L = o1_z2 + Flt1_fc * o1_z1;
          _H = o1 - _L - Flt1_bw * o1_z1;
          _B = Flt1_fc * _H + o1_z1;
          _N = _H + _L;
          o1_z1 = _B;
          o1_z2 = _L;
          switch(Flt1_type)
          {
            case 1: o1 = _L; break;
            case 2: o1 = _H; break;
            case 3: o1 = _B; break;
            case 4: o1 = _N; break;
          }
          // update
          Flt1_fc += (Flt1_ft-Flt1_fc) * Flt1_fd;
        }

//---------- phase distort

        pp2 = ph2;
        if( Comb_mode==4 )
        {
          pp2 += o1;
          if( pp2 >= 1 ) pp2-=1;
        }

//---------- osc 2

        o2 = 0;                          // none
        switch(Osc2_type)
        {
          case 1: o2 = 1;                   break; // const
          case 2: o2 = axRandomSigned();    break; // noise
          case 3: o2 = spl1;                break; // audio
          case 4: o2 = (pp2*2)-1;           break; // saw
          case 5: o2 = 1-(pp2*2);           break; // inv.saw
          case 6: o2 = (pp2<0.5) ? 1 : -1;  break; // squ
          case 7: o2 = sin(pp2*PI2);        break; // sin (todo:sinusoid)
        }
        o2 *= Osc2_vc;
        // update
        if( Osc2_type>0 )
        {
          ph2 += Osc2_pc*irate;
          if( ph2 >= 1 ) ph2-=1;
          Osc2_vc += (Osc2_vt-Osc2_vc) * Osc2_vd;
          Osc2_pc += (Osc2_pt-Osc2_pc) * Osc2_pd;
        }

//---------- filter 2

        if( Flt2_type > 0 )
        {
          _L = o2_z2 + Flt2_fc * o2_z1;
          _H = o2 - _L - Flt2_bw * o2_z1;
          _B = Flt2_fc * _H + o2_z1;
          _N = _H + _L;
          o2_z1 = _B;
          o2_z2 = _L;
          switch(Flt2_type)
          {
            case 1: o2 = _L; break;
            case 2: o2 = _H; break;
            case 3: o2 = _B; break;
            case 4: o2 = _N; break;
          }
          // update
          Flt2_fc += (Flt2_ft-Flt2_fc) * Flt2_fd;
        }

//---------- combine

        out = 0;
        switch( Comb_mode )
        {
          case 0: out = o1;     break;
          case 1: out = o2;     break;
          case 2: out = o1+o2;  break;
          case 3: out = o1*o2;  break;
          case 4: out = o2;     break;
        }
        out *= midivel;
        out *= Master_c;
        out = ( o0 += (out-o0) * Xfade );
        out *= Boost;
        // update
        Master_c += (Master_t-Master_c) * Master_d;

//---------- filter

        if( Flt3_type > 0 )
        {
          _L = out_z2 + Flt3_fr * out_z1;
          _H = out - _L - Flt3_bw * out_z1;
          _B = Flt3_fr * _H + out_z1;
          _N = _H + _L;
          out_z1 = _B;
          out_z2 = _L;
          switch(Flt3_type)
          {
            case 1: out = _L; break;
            case 2: out = _H; break;
            case 3: out = _B; break;
            case 4: out = _N; break;
          }
        }
        // clip
        out = axMin(Clip,axMax(-Clip,out));

//----------

        *outs[0] = out;
        *outs[1] = out;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"










/*



        midinote  =
        midivel   =
        midifreq  =
        o1_volc   =
        o1_pitc   =
        o1_frqc   =
        o2_volc   =
        o2_pitc   =
        o2_frqc   =
        out_frqc  =
        mas_c     =
        mas_t     =
        _L = _H = _B = _N =
        ph1 = ph2 = pp2 =
        o0 = o1_z1 = o1_z2 = o2_z1 = o2_z2 = out_z1 = out_z2 = 0;

      }

    // --- process ---

    virtual void process_parameter(axParameter* aParameter)
      {
        switch(id)
        {
          case  0: o1_wav  = val; break;
          case  1: o1_vol  = val*val; break;
          case  2: o1_volt = val*val; break;
          case  3: o1_vold = irat2 / axMax(1,val*val*val); break;
          case  4: o1_pit  = mSampleRate * ( val*val*val)*0.1; break;
          case  5: o1_pitt = mSampleRate * ( val*val*val)*0.1; break;
          case  6: o1_pitd = irat2 / axMax(1,val*val*val); break;
          case  7: o1_flt  = val; break;
          case  8: o1_frq  = val*val*val; break;
          case  9: o1_frqt = val*val*val; break;
          case 10: o1_frqd = irat2 / axMax(1,val*val*val); break;
          case 11: o1_bw   = val*val; break;
          case 12: o2_wav  = val; break;
          case 13: o2_vol  = val*val; break;
          case 14: o2_volt = val*val; break;
          case 15: o2_vold = irat2 / axMax(1,val*val*val); break;
          case 16: o2_pit  = mSampleRate * ( val*val*val)*0.1; break;
          case 17: o2_pitt = mSampleRate * ( val*val*val)*0.1; break;
          case 18: o2_pitd = irat2 / axMax(1,val*val*val); break;
          case 19: o2_flt  = val; break;
          case 20: o2_frq  = val*val*val; break;
          case 21: o2_frqt = val*val*val; break;
          case 22: o2_frqd = irat2 / axMax(1,val*val*val); break;
          case 23: o2_bw   = val*val; break;
          //case 24: mixmode = val; break;
          case 24: mode    = val; break;
          case 25: fade    = irat2 / axMax(1,val*val*val); break;
          case 26: boost   = val; break;
          case 27: clip    = val*val; break;
          case 28: out_flt = val; break;
          case 29: out_frq = val*val*val; break;
          case 30: out_bw  = val*val; break;
          case 31: master  = val*val; break;
          case 32: mas_d   = irat2 / axMax(1,val*val*val); break;
          case 33: midinote= val; break;
        }
      }


    virtual bool process_block(float** inputs, float** outputs, long sampleFrames)
      {
        //TRACE("process_block\n");
        updateTimeInfo();
        //TODO: if playstate changed, reset buffer, counters
        //for( int i=0; i<MAX_EVENTS; i++ ) EVENTS[i] = -1;
        offset = 0;
        return false;
      }

    virtual void process_sample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

//---------- midi

        if( EVENTS[offset*2] > 0 )
        {
          //TRACE("event!\n");
          midivel   = EVENTS[offset*2];
          midifreq  = EVENTS[(offset*2)+1];
          o1_volc   = o1_vol;
          o1_pitc   = o1_pit;
          o1_frqc   = o1_frq;
          o2_volc   = o2_vol;
          o2_pitc   = o2_pit;
          o2_frqc   = o2_frq;
          out_frqc  = out_frq;
          mas_c     = master;
          if( midinote<0 )
          {
            if( o2_wav==2 )
            { // noise
              o2_frqc = midifreq/mSampleRate * 8;
              //o2_frqt = midifreq/mSampleRate * 8;
            }
            else
            { // osc
              o2_pitc = midifreq;
              //o2_pitt = midifreq/mSampleRate * 8;
            }
          }
        }
        EVENTS[offset*2] = -1;
        offset += 1;

//---------- osc 1

        o1 = 0;                          // none
        switch(o1_wav)
        {
          case 1: o1 = 1;                 break;  // const
          case 2: o1 = axRandomSigned();  break;  // noise
          case 3: o1 = spl0;              break;  // audio
          case 4: o1 = (ph1*2)-1;         break;  // saw
          case 5: o1 = 1-(ph1*2);         break;  // inv.saw
          case 6: o1 = (ph1<0.5)?1:-1;    break;  // squ
          case 7: o1 = sin(ph1*PI2);      break;  // sin (todo:sinusoid)
        }
        o1 *= o1_volc;
        // update
        if( o1_wav>0 )
        {
          ph1 += o1_pitc*irate;
          if( ph1 >= 1 ) ph1 -= 1;
          o1_volc += (o1_volt-o1_volc) * o1_vold;
          o1_pitc += (o1_pitt-o1_pitc) * o1_pitd;
        }

//---------- filter 1

        if( o1_flt > 0 )
        {
          _L = o1_z2 + o1_frqc * o1_z1;
          _H = o1 - _L - o1_bw * o1_z1;
          _B = o1_frqc * _H + o1_z1;
          _N = _H + _L;
          o1_z1 = _B;
          o1_z2 = _L;
          switch(o1_flt)
          {
            case 1: o1 = _L; break;
            case 2: o1 = _H; break;
            case 3: o1 = _B; break;
            case 4: o1 = _N; break;
          }
          // update
          o1_frqc += (o1_frqt-o1_frqc) * o1_frqd;
        }

//---------- phase distort

        pp2 = ph2;
        if( mode==4 )
        {
          pp2 += o1;
          if( pp2 >= 1 ) pp2-=1;
        }

//---------- osc 2

        o2 = 0;                          // none
        switch(o2_wav)
        {
          case 1: o2 = 1;                   break; // const
          case 2: o2 = axRandomSigned();    break; // noise
          case 3: o2 = spl1;                break; // audio
          case 4: o2 = (pp2*2)-1;           break; // saw
          case 5: o2 = 1-(pp2*2);           break; // inv.saw
          case 6: o2 = (pp2<0.5) ? 1 : -1;  break; // squ
          case 7: o2 = sin(pp2*PI2);        break; // sin (todo:sinusoid)
        }
        o2 *= o2_volc;
        // update
        if( o2_wav>0 )
        {
          ph2 += o2_pitc*irate;
          if( ph2 >= 1 ) ph2-=1;
          o2_volc += (o2_volt-o2_volc) * o2_vold;
          o2_pitc += (o2_pitt-o2_pitc) * o2_pitd;
        }

//---------- filter 2

        if( o2_flt > 0 )
        {
          _L = o2_z2 + o2_frqc * o2_z1;
          _H = o2 - _L - o2_bw * o2_z1;
          _B = o2_frqc * _H + o2_z1;
          _N = _H + _L;
          o2_z1 = _B;
          o2_z2 = _L;
          switch(o2_flt)
          {
            case 1: o2 = _L; break;
            case 2: o2 = _H; break;
            case 3: o2 = _B; break;
            case 4: o2 = _N; break;
          }
          // update
          o2_frqc += (o2_frqt-o2_frqc) * o2_frqd;
        }

//---------- combine

        out = 0;
        switch( mode )
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

        if( out_flt > 0 )
        {
          _L = out_z2 + out_frqc * out_z1;
          _H = out - _L - out_bw * out_z1;
          _B = out_frqc * _H + out_z1;
          _N = _H + _L;
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
        // clip
        out = axMin(clip,axMax(-clip,out));

//----------

        *outs[0] = out;
        *outs[1] = out;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"


*/
