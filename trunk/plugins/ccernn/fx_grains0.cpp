#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  14
//#define AX_WIDTH      320
//#define AX_HEIGHT     200
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//#include "axEditor.h"
//#include "wdgKnob.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE 4000000
#define MAX_GRAINS  256

//----------

char* str_onoff[] = { (char*)"off", (char*)"on" };

//----------

struct GRAIN
{
  int   active;
  float pos;
  float start;
  float end;
  float duration;
  float speed;
  float phase;
  float phase_add;
  float phase2;
  float phase2_add;
};

//----------------------------------------

class myPlugin : public axPlugin
{
  private:
    float*      BUFFER;
    GRAIN       GRAINS[MAX_GRAINS];
    int         index;
    int         countdown;
  protected:
    // -------- effect
    //float     mValue;
    float       mMaster;
  //float       mNumGrains;
    int         mNumGrains;
    float       mBufferSize;
    float       mFreeze;
    float       mGrainDist;
    float       mGrainSize;
    float       mGrainDur;
    float       mGrainPitch;
    float       mEnv;
    float       mGrainEnv;
    float       mStartJit;
    float       mPitchJit;
    float       mSizeJit;
    float       mDurJit;
//    // -------- editor
//    axEditor    *mEditor;
//    //wdgKnob   *mKnob1;
//    wdgBuffer   *wBuffer;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_grains0","ccernn","axonlib example plugin",0,1);
        setupAudio(2,2);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //appendParameter( mParam1 = new parFloat(this,0,"mParam1","",0,-1,1,.1) );
        //processParameters();
        //mValue = 0;
        axRandomize(418);
        BUFFER = new float[BUFFER_SIZE];
        memset(BUFFER,0,BUFFER_SIZE*sizeof(float));
        memset(GRAINS,0,MAX_GRAINS*sizeof(GRAIN));
        index       = 0;
        countdown   = 0;
        appendParameter( new parFloat(  this,0, "master volume",     "",   1,    0, 2   ) );
        appendParameter( new parInteger(this,1, "number of grains",  "",   10,   1, 255 ) );
        appendParameter( new parFloat(  this,2, "buffer size",       "ms", 1000, 1, 5000) );
        appendParameter( new parInteger(this,3, "freeze",            "",   0,    0, 1, str_onoff ) );
        appendParameter( new parFloat(  this,4, "grain distance",    "ms", 20,   1, 100 ) );
        appendParameter( new parFloat(  this,5, "grain size",        "ms", 30,   1, 100 ) );
        appendParameter( new parFloat(  this,6, "grain duration",    "ms", 300,  1, 2000) );
        appendParameter( new parFloat(  this,7, "grain pitch",       "",   1,    0, 4   ) );
        appendParameter( new parFloat(  this,8, "envelope",          "",   1   ) );
        appendParameter( new parFloat(  this,9, "grain envelope",    "",   1   ) );
        appendParameter( new parFloat(  this,10,"distance jitter",   "",   0.2 ) );
        appendParameter( new parFloat(  this,11,"pitch jitter",      "",   0.2 ) );
        appendParameter( new parFloat(  this,12,"size jitter",       "",   0.2 ) );
        appendParameter( new parFloat(  this,13,"duration jitter",   "",   0.2 ) );
        processParameters();
      }

    virtual ~myPlugin()
      {
        delete BUFFER;
      }

    //----------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id   = aParameter->mID;
        float val = aParameter->getValue();
        float ms  = getSampleRate() * 0.001;
        switch(id)
        {
          case 0:   mMaster      = val;      break;
          case 1:   mNumGrains   = (int)val; break;
          case 2:   mBufferSize  = val * ms;
                    while (index>=(int)mBufferSize) index -= (int)mBufferSize;
                    break;
          case 3:   mFreeze      = val;      break;
          case 4:   mGrainDist   = val * ms; break;
          case 5:   mGrainSize   = val * ms; break;
          case 6:   mGrainDur    = val * ms; break;
          case 7:   mGrainPitch  = val;      break;
          case 8:   mEnv         = val;      break;
          case 9:   mGrainEnv    = val;      break;
          case 10:  mStartJit    = val;      break;
          case 11:  mPitchJit    = val;      break;
          case 12:  mSizeJit     = val;      break;
          case 13:  mDurJit      = val;      break;
        }

      }

    //----------------------------------------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        float in0 =  spl0;
        float in1 =  spl1;
        float out0 = 0;
        float out1 = 0;

        GRAIN* newgrain = NULL;
        GRAIN* G = GRAINS;

        //-----

        for( int i=0; i<mNumGrains; i++ )
        {
          if( G->active==1 )
          {
            G->pos += G->speed;
            if (G->pos >= G->end) G->pos = G->start;
            if (G->pos >= mBufferSize) G->pos -= mBufferSize;
            //if( G->pos < 0 ) G->pos += mBufferSize; // if we allow negative speeds

            G->phase += ( G->phase_add * 2 );
            if (G->phase >= 2) G->phase -= 2;

            G->phase2 += ( G->phase2_add * 2 );
            if (G->phase2 >= 2) G->phase2 -= 2;

            G->duration -= 1;
            if (G->duration <= 0) G->active = 0;

            float gvol = G->phase  * ( 2 - axAbs(G->phase ) );
            float dvol = G->phase2 * ( 2 - axAbs(G->phase2) );

            gvol = gvol*mGrainEnv + (1-mGrainEnv);
            dvol = dvol*mEnv      + (1-mEnv);

            int gpos = (int)G->pos * 2;
            out0 += BUFFER[gpos  ] * dvol * gvol;
            out1 += BUFFER[gpos+1] * dvol * gvol;
          } //on
          else if (!newgrain) newgrain = G;
          G += 1;//sizeof(GRAIN);
        } //for, numgrains

        //-----

        if( countdown <= 0 )
        {
          countdown = mGrainDist;
          if( newgrain )
          {
            float startrnd = 1 + (mStartJit * axRandomSigned() ); // 0..2
            float pitchrnd = 1 + (mPitchJit * axRandomSigned() );
            float sizernd  = 1 + (mSizeJit  * axRandomSigned() );
            float durrnd   = 1 + (mDurJit   * axRandomSigned() );
            float siz = mGrainSize * sizernd;

            //float st  = index * startrnd;
            float st =  index + startrnd*mBufferSize;
            if( st >= mBufferSize ) st -= mBufferSize;
            if( st <  0           ) st += mBufferSize;

            float en  = st + siz;
            if( en >= mBufferSize ) en = mBufferSize; // clamp
            if( en <  0           ) en = 0;


            float du = mGrainDur * durrnd;
            newgrain->active      = 1;
            newgrain->pos         = st;
            newgrain->start       = st;
            newgrain->end         = en;
            newgrain->duration    = du;
            newgrain->speed       = mGrainPitch * pitchrnd;
            newgrain->phase       = 0;
            newgrain->phase_add   = 1 / siz;
            newgrain->phase2      = 0;
            newgrain->phase2_add  = 1 / du;

          } //newgrain
        } //countdown
        countdown -= 1;

        //-----

        int index2 = index*2;
        if( mFreeze < 0.5 ) BUFFER[index2  ] = in0;
        if( mFreeze < 0.5 ) BUFFER[index2+1] = in1;
        index += 1;
        if( index >= mBufferSize ) index -= mBufferSize;

        float mmm = mMaster*mMaster*mMaster;

        *outs[0] = out0 * mmm;
        *outs[1] = out1 * mmm;

      }

};

//----------------------------------------------------------------------
#include "axMain.h"
