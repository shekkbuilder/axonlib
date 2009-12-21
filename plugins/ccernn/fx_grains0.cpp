#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  13
//#define AX_WIDTH      320
//#define AX_HEIGHT     200
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//#include "axEditor.h"
//#include "wdgKnob.h"

//----------------------------------------------------------------------

// 16 * 64k = 1m samples
#define BUFFER_SIZE 1000000
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
    float       mNumGrains;
    float       mBufferSize;
    float       mFreeze;
    float       mGrainDist;
    float       mGrainSize;
    float       mGrainDur;
    float       mGrainPitch;
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

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_grains0","ccernn","product_string",0,1);
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
        appendParameter( new parInteger(this,2, "buffer size",       "ms", 1000, 1, 1000) );
        appendParameter( new parInteger(this,3, "freeze",            "",   0,    0, 1, str_onoff ) );
        appendParameter( new parInteger(this,4, "grain distance",    "ms", 20,   1, 100 ) );
        appendParameter( new parInteger(this,5, "grain size",        "ms", 30,   1, 100 ) );
        appendParameter( new parInteger(this,6, "grain duration",    "ms", 300,  1, 1000) );
        appendParameter( new parFloat(  this,7, "grain pitch",       "",   1,    0, 4   ) );
        appendParameter( new parFloat(  this,8, "grain envelope",    "",   0   ) );
        appendParameter( new parFloat(  this,9, "distance jitter",   "",   0.2 ) );
        appendParameter( new parFloat(  this,10,"pitch jitter",      "",   0.2 ) );
        appendParameter( new parFloat(  this,11,"size jitter",       "",   0.2 ) );
        appendParameter( new parFloat(  this,12,"duration jitter",   "",   0.2 ) );
        processParameters();
      }

    virtual ~myPlugin()
      {
        delete BUFFER;
      }

//    //----------------------------------------
//
//    virtual void onChange(axParameter* aParameter)
//      {
//        if(mEditor) mEditor->onChange(aParameter);
//        doProcessParameter(aParameter);
//      }
//
//    //----------------------------------------
//
//    // can this one die because of some threading issues? (ala close editor) think,think..
//    virtual axWindow* doCreateEditor(void)
//      {
//        axEditor* E = new axEditor(this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
//        E->appendWidget( mKnob1 = new wdgKnob(E,0,axRect(10,10,128,32),wal_None,mParam1) );
//        E->connect( mKnob1, mParam1 );
//        // could mEditor be in use from another thread? gui? audio? setParameter?
//        // it shouldn't (we are creating the editor), but, ...
//        mEditor = E;
//        return mEditor;
//      }
//
//    //----------------------------------------
//
//    // see below (idle)
//    virtual void doDestroyEditor(void)
//      {
//        axEditor* E = mEditor;
//        mEditor = NULL;
//        delete E;
//      }
//
//    //----------------------------------------
//
//    // let's say this has just been called from the gui thread to update the interface
//    // immediately after this, you close the editor (doDestroyEditor above)
//    // it would be no good to delete the mEditor class while we're still drawing
//    // so, some locking is probablynecessary
//    virtual void doIdleEditor(void)
//      {
//        if(mEditor) mEditor->redrawDirty();
//      }
//
//    //----------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    //TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------------------------------------

    //virtual void doProcessTransport(int aState)
    //  {
    //    //TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------------------------------------

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    //TRACE("doProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float val = aParameter->getValue();
//        //lock
//        switch(id)
//        {
//          //case 0: mValue = val;  break;
//        }
//        //unlock
        float ms = updateSampleRate() * 0.001; // mSampleRate * 0.001;
        switch(id)
        {
          case 0:  mMaster      = val;      /*TRACE("mMaster: %f\n",    mMaster);     */  break;
          case 1:  mNumGrains   = val;      /*TRACE("mNumGrains: %f\n", mNumGrains);  */  break;
          case 2:  mBufferSize  = val * ms; /*TRACE("mBufferSize: %f\n",mBufferSize); */  break;
          case 3:  mFreeze      = val;      /*TRACE("mFreeze: %f\n",    mFreeze);     */  break;
          case 4:  mGrainDist   = val * ms; /*TRACE("mGrainDist: %f\n", mGrainDist);  */  break;
          case 5:  mGrainSize   = val * ms; /*TRACE("mGrainSize: %f\n", mGrainSize);  */  break;
          case 6:  mGrainDur    = val * ms; /*TRACE("mGrainDur: %f\n",  mGrainDur);   */  break;
          case 7:  mGrainPitch  = val;      /*TRACE("mGrainPitch: %f\n",mGrainPitch); */  break;
          case 8:  mGrainEnv    = val;      /*TRACE("mGrainEnv: %f\n",  mGrainEnv);   */  break;
          case 9:  mStartJit    = val;      /*TRACE("mStartJit: %f\n",  mStartJit);   */  break;
          case 10: mPitchJit    = val;      /*TRACE("mPitchJit: %f\n",  mPitchJit);   */  break;
          case 11: mSizeJit     = val;      /*TRACE("mSizeJit: %f\n",   mSizeJit);    */  break;
          case 12: mDurJit      = val;      /*TRACE("mDurJit: %f\n",    mDurJit);     */  break;
        }

      }

    //----------------------------------------

//    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
//      {
//        updateTimeInfo(); // AX_AUTOSYNC
//        //float beatsize = (60/mTempo) * mSampleRate;
//        if( mPlayState&1 )
//        {
//          index = 0;
//          countdown = 0;
//        }
//        return false; // call process_sample
//      }

    //----------------------------------------

    virtual void doProcessSample(float** ins, float** outs)
      {
        //float spl0 = *ins[0];
        //float spl1 = *ins[1];
        //*outs[0] = spl0;
        //*outs[1] = spl1;
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        float in = (spl0+spl1)*0.5;
        float out = 0;

        GRAIN* newgrain = NULL;
        GRAIN* G = GRAINS;

        //-----

        for( int i=0; i<mNumGrains; i++ )
        {
          if( G->active==1 )
          {
            G->pos += G->speed;
            if( G->pos >= G->end ) G->pos = G->start;
            if( G->pos >= mBufferSize ) G->pos -= mBufferSize;

            G->phase += ( G->phase_add * 2 );
            if( G->phase >= 2 ) G->phase -= 2;

            G->phase2 += ( G->phase2_add * 2 );
            if( G->phase2 >= 2 ) G->phase2 -= 2;

            G->duration -= 1;
            if( G->duration <= 0 ) G->active = 0;

            float gvol = G->phase  * ( 2 - abs(G->phase ) );
            float dvol = G->phase2 * ( 2 - abs(G->phase2) );

            out += BUFFER[ (int)G->pos ] * dvol * gvol;
          } //on
          else if(!newgrain) newgrain = G;
          G += 1;//sizeof(GRAIN);
        } //for, numgrains

        //-----

        if( countdown <= 0 )
        {
          countdown = mGrainDist;
          if( newgrain )
          {
            float startrnd = 1 + (mStartJit * axRandomSigned() );
            float pitchrnd = 1 + (mPitchJit * axRandomSigned() );
            float sizernd  = 1 + (mSizeJit  * axRandomSigned() );
            float durrnd   = 1 + (mDurJit   * axRandomSigned() );
            float siz = mGrainSize * sizernd;
            float st  = index * startrnd;
            if( st >= mBufferSize ) st -= mBufferSize;
            if( st <  0           ) st += mBufferSize;
            //while( st >= mBufferSize ) st -= mBufferSize;
            //while( st <  0           ) st += mBufferSize;
            float en  = st + siz;
            if( en >= mBufferSize ) en = mBufferSize; // clamp
            if( en <  0           ) en = 0;
            //while( en >= mBufferSize ) en -= mBufferSize;
            //while( en <  0           ) en += mBufferSize;
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

        if( mFreeze < 0.5 ) BUFFER[index] = in;
        index += 1;
        if( index >= mBufferSize ) index -= mBufferSize;

        float mmm = mMaster*mMaster*mMaster;

        *outs[0] = out * mmm;
        *outs[1] = out * mmm;

      }

};

//----------------------------------------------------------------------
#include "axMain.h"
