#ifndef fx_cutter_included
#define fx_cutter_included
//----------------------------------------------------------------------

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_WIDTH      320
#define AX_HEIGHT     240
#define AX_FLAGS      (AX_BUFFERED|AX_EMBEDDED)
//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//#include "axWidget.h"
#include "axEditor.h"
#include "wdgPanel.h"
#include "wdgKnob.h"

//----------------------------------------------------------------------

#define MIN_BPM       30
#define MAX_SRATE     192000
#define MAX_BEATS     16
#define MAX_BEATSIZE  (MAX_SRATE * 60 / MIN_BPM)
#define MAX_BUFSIZE   (MAX_BEATS * MAX_BEATSIZE)

//----------------------------------------------------------------------
//
// painter
//
//----------------------------------------------------------------------

class myPainter : public axWidget
{
  private:
    axBrush*  mBackBrush;
    axBrush*  mSelBrush;
    axBrush*  mRepBrush;
    axPen*    mWavePen;
    axPen*    mRecPen;
    axPen*    mPlayPen;
    axPen*    mGridPen;
    float*    mBuffer;
    int       mSize;
    int       mMode;
    int       mRecPos;
    int       mPlayPos;
    int       mNumGrid;
    int mSelSt;
    int mSelLen;
    int mRepLen;

  public:
    myPainter(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        mBuffer     = NULL;
        mSize       = 0;
        mMode       = 0;
        mRecPos     = 0;
        mPlayPos    = 0;
        mNumGrid    = 0;
        mBackBrush  = new axBrush(AX_GREY_DARK );
        mSelBrush   = new axBrush(AX_GREEN_DARK );
        mRepBrush   = new axBrush(AX_YELLOW_DARK );
        mWavePen    = new axPen(  AX_GREY_LIGHT );
        mRecPen     = new axPen(  AX_RED );
        mPlayPen    = new axPen(  AX_GREEN );
        mGridPen    = new axPen(  AX_GREY );
      }
    virtual ~myPainter()
      {
        delete mBackBrush;
        delete mSelBrush;
        delete mRepBrush;
        delete mWavePen;
        delete mRecPen;
        delete mPlayPen;
        delete mGridPen;
      }
    //this is ugly... but works..
    void setup(float* aBuffer, int aSize, int aMode, int aRecPos, int aPlayPos, int aNumGrid, int selst, int sellen, int replen)
      {
        mBuffer = aBuffer;
        mSize = aSize;
        mMode = aMode;
        mRecPos = aRecPos;
        mPlayPos = aPlayPos;
        mNumGrid = aNumGrid;
        mSelSt = selst;
        mSelLen = sellen;
        mRepLen = replen;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mRect.w>0)
        {
          // draw background
          aCanvas->selectBrush( mBackBrush );
          aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );

          if (mSize>0)
          {
            // draw selection
            aCanvas->selectBrush(mSelBrush);
            float x1 = mRect.x + mRect.w * ((float)mSelSt/(float)mSize);
            float x2 = mRect.x + mRect.w * ((float)(mSelSt+mSelLen)/(float)mSize);
            aCanvas->fillRect( (int)x1, mRect.y, (int)x2, mRect.y2() );

            // draw repeat
            aCanvas->selectBrush(mRepBrush);
            x1 = mRect.x + mRect.w * ((float)mSelSt/(float)mSize);
            x2 = mRect.x + mRect.w * ((float)(mSelSt+mRepLen)/(float)mSize);
            aCanvas->fillRect( (int)x1, mRect.y+2, (int)x2, mRect.y2()-2 );

            // draw wave
            int hh = mRect.h/2;
            int yy = mRect.y + hh;
            float pos = 0;
            float step = mSize / mRect.w;
            aCanvas->selectPen(mWavePen);
            int x = mRect.x;
            int y = yy;
            for (int i=1; i<mRect.w; i++)
            {
              float v = mBuffer[((int)pos)*2];
              int y2 = yy - (hh*v);
              aCanvas->drawLine(x-1,y,x,y2);
              x++;
              y = y2;
              pos += step;
            } //for width

            // draw grid
            aCanvas->selectPen(mGridPen);
            if (mNumGrid>1)
            {
              float step = (float)mRect.w / (float)mNumGrid;
              float pos = mRect.x + step;
              for( int i=1; i<mNumGrid; i++ )
              {
                aCanvas->drawLine( (int)pos, mRect.y, (int)pos, mRect.y2() );
                pos += step;
              } //for numgrid
            } //numgrid>1

            // draw rec pen
            aCanvas->selectPen(mRecPen);
            pos = mRect.x + mRect.w * ((float)mRecPos/(float)mSize);
            aCanvas->drawLine( (int)pos, mRect.y, (int)pos, mRect.y2() );

            // draw play pen
            aCanvas->selectPen(mPlayPen);
            pos = mRect.x + mRect.w * ((float)mPlayPos/(float)mSize);
            aCanvas->drawLine( (int)pos, mRect.y, (int)pos, mRect.y2() );

          } //size>0
        } //w>0
      }
};

//----------------------------------------------------------------------
//
// fx_cutter
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:

  // plugin internal

    float   mBuffer[MAX_BUFSIZE*2]; // *2 stereo
    int     mBufferSize;
    int     mRecPos;
    int     mPlayPos;
    int     mBeatSize;
    int     mSampleCounter;
    int     mPrevState;
    int     mBeatCounter;
    int     mSelection;
    int     mRepeats;
    int     mSelectionStart;
    int     mSelectionLen;
    int     mRepeatLen;
    int     mBeatsLeft;

  // parameters

    int         mNumBeats;
    int         mMaxSelect;
    int         mMaxRepeat;
    float       mProb;

    parInteger* pNumBeats;
    parInteger* pMaxSelect;
    parInteger* pMaxRepeat;
    parFloat*   pProb;

  //gui

    bool        mGuiInitialized;
    axEditor*   mEditor;
    wdgPanel*   wMain;
    wdgKnob*    wNumBeats;
    wdgKnob*    wMaxSelect;
    wdgKnob*    wMaxRepeat;
    wdgKnob*    wProb;
    myPainter*  wPainter;

  public:

    //----------------------------------------
    // constructor & destructor
    //----------------------------------------

    //myPlugin(audioMasterCallback audioMaster,long numProgs,long numParams, int aPlugFlags=0)
    //: axPlugin(audioMaster,numProgs,numParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //TRACE("MAX_BEATSIZE %i\n",MAX_BEATSIZE);
        //TRACE("MAX_BUFSIZE %i\n",MAX_BUFSIZE);
        mEditor = NULL;
        mGuiInitialized = false;
        mPrevState = -1;
        mRecPos = 0;
        mPlayPos = 0;
        mSampleCounter = 0;
        mBeatCounter = 0;
        mBeatsLeft = 0;
        mProb = 1;
        describe("fx_cutter0","ccernn","axonlib example plugin",0,AX_MAGIC+0x0001);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        appendParameter( pNumBeats  = new parInteger(this,0,"num beats", "",4, 1,16 ) );
        appendParameter( pMaxSelect = new parInteger(this,1,"max select","",4, 1,8  ) );
        appendParameter( pMaxRepeat = new parInteger(this,2,"max repeat","",4, 1,16 ) );
        appendParameter( pProb      = new parFloat(  this,3,"prob",      "",0.25    ) );
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //----------------------------------------
    //
    // plugin
    //
    //----------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("state %i\n",aState);
    //  }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float val = aParameter->getValue();
        switch (id)
        {
          case 0: mNumBeats  = (int)val; break;
          case 1: mMaxSelect = (int)val; break;
          case 2: mMaxRepeat = (int)val; break;
          case 3: mProb      =      val; break;
        }
      }

    //----------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        mBeatSize = mSampleRate * 60 / mTempo;
        mBufferSize  = mBeatSize * mNumBeats;
        while (mRecPos>=mBufferSize) mRecPos-=mBufferSize;
        if (mPlayState&ptr_changed)
        {
          if (mPlayState&ptr_playing)
          {
            mRecPos = 0;
            mPlayPos = 0;
            mSampleCounter = 0;
            mBeatCounter = 0;
            mBeatsLeft = mNumBeats;
          }
        }
        return false;
      }

    //----------------------------------------
    //
    // process sample
    //
    //----------------------------------------

    virtual void doProcessSample(float** ins, float** outs)
      {
        // buffering
        mBuffer[mRecPos*2  ] = *ins[0];
        mBuffer[mRecPos*2+1] = *ins[1];

        // increase & wrap
        mRecPos++;
        if (mRecPos>=mBufferSize)
        {
          mRecPos = 0;
          mPlayPos = 0;
          mSampleCounter = 0;
          mBeatCounter = 0;
          mBeatsLeft = mNumBeats;
        }

        mSampleCounter--;
        if (mSampleCounter<=0)
        {
          mSampleCounter = (int)mBeatSize;
          mBeatCounter--;
          if (mBeatCounter<=0)
          {
            mSelection  = 1 + axRandomInt(mMaxSelect-1);
            if (mSelection>mBeatsLeft) mSelection = mBeatsLeft;
            mBeatsLeft -= mSelection;
            mBeatCounter    = mSelection;
            mSelectionStart = mRecPos;
            mSelectionLen   = mSelection * mBeatSize;
            mPlayPos = mRecPos;
            if (axRandom()<mProb)
            {
              mRepeats = 1 + axRandomInt(mMaxRepeat-1);
              mRepeatLen = mSelectionLen / mRepeats;
            }
            else mRepeatLen = mSelectionLen;
          } //beatcounter
        } //samplecounter

        // output
        mPlayPos++;
        if (mPlayPos>=(mSelectionStart+mRepeatLen))
        {
          mPlayPos = mSelectionStart;
          //TODO: speed halve/double
          //TODO: halve/double selection
        }
        float spl0 = mBuffer[mPlayPos*2  ];
        float spl1 = mBuffer[mPlayPos*2+1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------------------------------------
    //
    // editor
    //
    //----------------------------------------

    virtual void* doCreateEditor(int aParent)
      {
        axEditor* EDIT = new axEditor("with_gui_win",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS,aParent);
        EDIT->appendWidget( wMain = new wdgPanel(this,-1,NULL_RECT,wal_Client));
        if (!mGuiInitialized) { mGuiInitialized = true; }
        wMain->appendWidget( wNumBeats  = new wdgKnob(   this, 0,axRect(10, 10, 128,32 ),wal_None) );
        wMain->appendWidget( wMaxSelect = new wdgKnob(   this, 1,axRect(10, 50, 128,32 ),wal_None) );
        wMain->appendWidget( wMaxRepeat = new wdgKnob(   this, 2,axRect(150,50, 128,32 ),wal_None) );
        wMain->appendWidget( wPainter   = new myPainter( this,-1,axRect(10, 100,300,130),wal_None) );
        wMain->appendWidget( wProb      = new wdgKnob(   this, 3,axRect(150,10, 128,32 ),wal_None) );
        EDIT->connect(wNumBeats, pNumBeats );
        EDIT->connect(wMaxSelect,pMaxSelect);
        EDIT->connect(wMaxRepeat,pMaxRepeat);
        EDIT->connect(wProb,pProb);
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
        if (mEditor)
        {
          wPainter->setup(mBuffer,mBufferSize,0,mRecPos,mPlayPos,mNumBeats,mSelectionStart,mSelectionLen,mRepeatLen);
          mEditor->appendDirty( wPainter );
          mEditor->redrawDirty();
        }
      }

    //----------------------------------------
    //
    // listeners
    //
    //----------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if (mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);
      }

};

#undef MIN_BPM
#undef MAX_SRATE
#undef MAX_BEATS
#undef MAX_BEATSIZE
#undef MAX_BUFSIZE

#include "axMain.h"
//----------------------------------------------------------------------
#endif
