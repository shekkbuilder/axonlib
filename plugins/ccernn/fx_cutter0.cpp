#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  4

#define AX_WIDTH      300
#define AX_HEIGHT     400
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
#include "wdgKnob.h"

//----------

#define MAX_BEATS 8
#define MAX_SRATE 192000
#define MIN_TEMPO 30

#define MAX_BEATSIZE ((60/MIN_TEMPO)*MAX_SRATE)
#define MAX_BUFSIZE  (MAX_BEATSIZE * MAX_BEATS)

//----------------------------------------------------------------------
//
// scope
//
//----------------------------------------------------------------------

class myScope : public axWidget
{
  public:
    int   mLength;
    int   mMode;
    float *mBuffer; // mLength*2 samples
    int mNumBeats;
    int mNumSlices;
    int mCursor1;
    int mCursor2;
    int mSection1;
    int mSection1Length;
    int mSection2;
    int mSection2Length;
  public:
    myScope(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        clearFlag(wfl_Active);
        setBackground(true,AX_GREY_DARK);
        mLength = 0;
        mBuffer = NULL;
        mMode   = 0;
        mNumBeats = 4;
        mNumSlices = 4;
      }
    void setup(float* aBuffer, int aLength)
      {
        mBuffer = aBuffer;
        mLength = aLength;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas, aRect);
        int w  = mRect.w;
        int h2 = (mRect.h/2);
        int px,py;
        float x,xx,xadd;

        if (mBuffer && w>0 && mLength>0)
        {
          // --- sections
          x  = mRect.x + w * mSection1 / mLength;
          xx =       x + w * mSection1Length / mLength;
          aCanvas->setPenColor( AX_GREEN_DARK );
          aCanvas->setBrushColor( AX_GREEN_DARK );
          aCanvas->fillRect(x,mRect.y,xx,mRect.y2());
          // --- grid
          float beatsize  = w / mNumBeats;
          float slicesize = beatsize / mNumSlices;
          x = mRect.x;
          for( int i=0; i<mNumBeats; i++ )
          {
            aCanvas->setPenColor( AX_WHITE );
            aCanvas->drawLine(x,mRect.y,x,mRect.y2());
            xx = x + slicesize;
            aCanvas->setPenColor( AX_GREY_LIGHT );
            for( int j=0; j<mNumSlices-1; j++ )
            {
              aCanvas->drawLine(xx,mRect.y,xx,mRect.y2());
              xx += slicesize;
            }
            x += beatsize;
          }
          // --- wave
          aCanvas->setPenColor( AX_YELLOW );
          x = 0;
          xadd = mLength / w;
          px = mRect.x;
          py = mRect.y +  h2;
          for (int i=0; i<w; i++)
          {
            float n;
            if (mMode==0) n=mBuffer[((int)x)*2];
            else if (mMode==1) n=mBuffer[((int)x)*2+1];
            int y = mRect.y +  h2 + (n*h2);
            aCanvas->drawLine( px,py,mRect.x+i, y );
            px = mRect.x+i;
            py = y;
            x+=xadd;
          }
          // --- cursor1 (rec)
          aCanvas->setPenColor( AX_RED );
          x = mRect.x + w * mCursor1 / mLength;
          aCanvas->drawLine(x,mRect.y,x,mRect.y2());
          // --- cursor2 (play)
          aCanvas->setPenColor( AX_GREEN );
          x = mRect.x + w * mCursor2 / mLength;
          aCanvas->drawLine(x,mRect.y,x,mRect.y2());
        }
      }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    float prev_frac;
    int   beats_count;
    int slices_left;
    int start_rnd;
    int beats_rnd,beats_len;
    int slices_rnd,slices_len;
  protected:
    float*      mBuffer;
    int         mBufferSize;
    int         mRecPos;
    int         mPlayPos;
  protected:
    int         mNumBeats;
    int         mNumSlices;
    int         mRndBeats;
    int         mRndSlices;
    parInteger* pParam1;
    parInteger* pParam2;
    parInteger* pParam3;
    parInteger* pParam4;
    int mBeatSize;
    int mSliceSize;

  public:
    axEditor*   mEditor;
    myScope*    wScope;
    wdgKnob*    wKnob1;
    wdgKnob*    wKnob2;
    wdgKnob*    wKnob3;
    wdgKnob*    wKnob4;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("fx_cutter0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        //setNumInputs(2);
        axRandomize(418);

        TRACE("MAX_BUFSIZE = %i\n",MAX_BUFSIZE);
        mBuffer = new float[MAX_BUFSIZE*2];
        mBufferSize = 1024; // default
        mRecPos = 0;
        mPlayPos = 0;
        prev_frac = 0;
        appendParameter( pParam1 = new parInteger(this,0,"num beats", "",4,1, 8) );
        appendParameter( pParam2 = new parInteger(this,1,"num slices","",2,1, 8) );
        appendParameter( pParam3 = new parInteger(this,2,"rnd beats", "",4,1, 8) );
        appendParameter( pParam4 = new parInteger(this,3,"rnd slices","",4,1,16) );
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (mBuffer) delete[] mBuffer;
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
        axEditor* E = new axEditor( "fx_cutter_gui",this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
        E->appendWidget( wScope = new myScope( E,-1, axRect(10,  10,266,40), wal_None ) );
        E->appendWidget( wKnob1 = new wdgKnob( E, 0, axRect(10,  60,128,32), wal_None/*, pParam1*/ ) );
        E->appendWidget( wKnob2 = new wdgKnob( E, 1, axRect(150, 60,128,32), wal_None/*, pParam2*/ ) );
        E->appendWidget( wKnob3 = new wdgKnob( E, 2, axRect(10, 100,128,32), wal_None/*, pParam3*/ ) );
        E->appendWidget( wKnob4 = new wdgKnob( E, 3, axRect(150,100,128,32), wal_None/*, pParam4*/ ) );
        E->connect( wKnob1, pParam1 );
        E->connect( wKnob2, pParam2 );
        E->connect( wKnob3, pParam3 );
        E->connect( wKnob4, pParam4 );
        wScope->setup(mBuffer,mBufferSize);
        mEditor = E;
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
        if(mEditor)
        {
          wScope->setup(mBuffer,mBufferSize);
          wScope->mNumBeats = mNumBeats;
          wScope->mNumSlices = mNumSlices;
          wScope->mCursor1 = mRecPos;
          wScope->mCursor2 = mPlayPos;

          wScope->mSection1 = start_rnd;
          wScope->mSection1Length = beats_len;

          //beats_rnd = 1 + axRandomInt(mRndBeats-1);
          //counter = mBeatSize * beats_rnd;

          mEditor->redrawWidget(wScope);
          mEditor->redrawDirty();
        }
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

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mNumBeats   = f;  break;
          case 1: mNumSlices  = f;  break;
          case 2: mRndBeats   = f;  break;
          case 3: mRndSlices  = f;  break;
        }
      }

    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        mBeatSize = ((60/mTempo)*mSampleRate);
        //mBufSize = mBeatSize * mNumBeats;
        mBufferSize = mBeatSize * mNumBeats;
        mSliceSize = mBeatSize / mNumSlices;
        while (mRecPos>=mBufferSize) mRecPos-=mBufferSize;
        //while (mPlayPos>=mBufferSize) mPlayPos-=mBufferSize;

//        float beat_frac = mBeatPos - floor(mBeatPos);
//        if (beat_frac < prev_frac)
//        {
//          TRACE("beat!\n");
//          beats_rnd = 1 + axRandomInt(mRndBeats-1);
//          counter = mBeatSize * beats_rnd;
//          //we are in a new beat
//        }
//        prev_frac = beat_frac;


        return false;
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        int pos = mRecPos*2;
        mBuffer[pos  ] = spl0;
        mBuffer[pos+1] = spl1;
        //------------------------------
        if (beats_count>0) beats_count--;
        else
        {
          start_rnd = mRecPos;
          beats_rnd = 1 + axRandomInt(mRndBeats-1);
          beats_len = mBeatSize * beats_rnd;
          beats_count = beats_len;
        }
        //------------------------------
        mRecPos++;
        if (mRecPos>=mBufferSize) mRecPos=0;
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
