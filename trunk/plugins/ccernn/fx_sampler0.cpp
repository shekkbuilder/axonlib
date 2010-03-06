#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  7
#define AX_WIDTH      320
#define AX_HEIGHT     270
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

//#define AX_AUTOSYNC
//#define AX_DIRTYWIDGETS
//#define AX_PAINTERS

//#define AX_DEBUG
//#include "axDebug.h"

#include "axPlugin.h"
//#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
#include "wdgPanel.h"
#include "wdgKnob.h"

//----------------------------------------------------------------------
//
// painter
//
//----------------------------------------------------------------------

#define MIN_BPM       30
#define MAX_SRATE     192000
#define MAX_BEATS     16
#define MAX_BEATSIZE  (MAX_SRATE * 60 / MIN_BPM)
#define MAX_BUFSIZE   (MAX_BEATS * MAX_BEATSIZE)

#define MAX_EVENTS_PER_BLOCK 65536

/*
  this one is almost identical to the one in fx_cutter
  TODO: create new wdgScope
*/

class myPainter : public axWidget
{
  private:

    axBrush*  mBackBrush;
    //axBrush*  mSelBrush;
    //axBrush*  mRepBrush;
    axPen*    mWavePen;
    axPen*    mRecPen;
    axPen*    mPlayPen;
    axPen*    mErasePen;
    axPen*    mBypassPen;
    axPen*    mGridPen;
    float*    mBuffer;
    int       mSize;
    int       mMode;
    int       mPos;
    //int       mPlayPos;
    int       mNumGrid;
    //int       mSelSt;
    //int       mSelLen;
    //int       mRepLen;

  public:
    myPainter(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        mBuffer     = NULL;
        mSize       = 0;
        mMode       = 0;
        mPos        = 0;
        mNumGrid    = 0;
        mBackBrush  = new axBrush(AX_GREY_DARK );
        //mSelBrush   = new axBrush(AX_GREEN_DARK );
        //mRepBrush   = new axBrush(AX_YELLOW_DARK );
        mWavePen    = new axPen( AX_GREY_LIGHT );
        mGridPen    = new axPen( AX_GREY );
        mRecPen     = new axPen( AX_RED );
        mPlayPen    = new axPen( AX_GREEN );
        mErasePen   = new axPen( AX_BLUE );
        mBypassPen  = new axPen( AX_WHITE );
      }
    virtual ~myPainter()
      {
        delete mBackBrush;
        //delete mSelBrush;
        //delete mRepBrush;
        delete mWavePen;
        delete mGridPen;
        delete mRecPen;
        delete mPlayPen;
        delete mErasePen;
        delete mBypassPen;
      }
    //this is ugly... but works..
    void setup(float* aBuffer, int aSize, int aMode, int aPos, int aNumGrid)
      {
        mBuffer = aBuffer;
        mSize = aSize;
        mMode = aMode;
        mPos = aPos;
        mNumGrid = aNumGrid;
        //mSelSt = selst;
        //mSelLen = sellen;
        //mRepLen = replen;
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
            //// draw selection
            //aCanvas->selectBrush(mSelBrush);
            //float x1 = mRect.x + mRect.w * ((float)mSelSt/(float)mSize);
            //float x2 = mRect.x + mRect.w * ((float)(mSelSt+mSelLen)/(float)mSize);
            //aCanvas->fillRect( (int)x1, mRect.y, (int)x2, mRect.y2() );

            //// draw repeat
            //aCanvas->selectBrush(mRepBrush);
            //x1 = mRect.x + mRect.w * ((float)mSelSt/(float)mSize);
            //x2 = mRect.x + mRect.w * ((float)(mSelSt+mRepLen)/(float)mSize);
            //aCanvas->fillRect( (int)x1, mRect.y+2, (int)x2, mRect.y2()-2 );

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

//        if (evt&2 ) mMode=1;  // rec
//        if (evt&4 ) mMode=2;  // play
//        if (evt&8 ) mMode=3;  // erase
//        if (evt&16) mMode=4; // bypass

            // draw rec pen
            if (mMode==1) aCanvas->selectPen(mRecPen);
            if (mMode==2) aCanvas->selectPen(mPlayPen);
            if (mMode==3) aCanvas->selectPen(mErasePen);
            if (mMode==4) aCanvas->selectPen(mBypassPen);
            pos = mRect.x + mRect.w * ((float)mPos/(float)mSize);
            aCanvas->drawLine( (int)pos, mRect.y, (int)pos, mRect.y2() );

//            // draw play pen
//            aCanvas->selectPen(mPlayPen);
//            pos = mRect.x + mRect.w * ((float)mPos/(float)mSize);
//            aCanvas->drawLine( (int)pos, mRect.y, (int)pos, mRect.y2() );

          } //size>0
        } //w>0
      }
};

//----------------------------------------------------------------------
//
// fx_sample
//
//----------------------------------------------------------------------

char* str_sync[] = { (char*)"manual", (char*)"host" };

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
    int   mEvents[MAX_EVENTS_PER_BLOCK];
    float mBuffer[MAX_BUFSIZE*2]; // *2 stereo
    int   mOffset;
    int   mBufferSize;
    int   mBeatSize;
    int   mPos;
    //int   mPlayPos;

    int mMode;
    int playmode;
    //int pos;

  // editor
    axEditor*   mEditor;
    bool        mGuiInitialized;
    wdgPanel*   wPanel;
    myPainter*  wScope;
  // plugin variables
    int         mNumBeats;
    int         mSyncMode;
    int         mTransportNote;
    int         mRecordNote;
    int         mPlayNote;
    int         mEraseNote;
    int         mBypassNote;
  // parameters
    parInteger* pBeats;
    parInteger* pSyncMode;
    parInteger* pTransportNote;
    parInteger* pRecordNote;
    parInteger* pPlayNote;
    parInteger* pEraseNote;
    parInteger* pBypassNote;
  // widgets
    wdgKnob*    wBeats;
    wdgKnob*    wSyncMode;
    wdgKnob*    wTransportNote;
    wdgKnob*    wRecordNote;
    wdgKnob*    wPlayNote;
    wdgKnob*    wEraseNote;
    wdgKnob*    wBypassNote;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        mGuiInitialized = false;

        mBufferSize = 0;
        mPos = 0;
        mNumBeats = 0;

        //@init
        //  ext_noinit=1;
        //  EVENTS = 0;
        //  BUFFER = 65536;

        describe("fx_sampler0","ccernn","axonlib example plugin",0,AX_MAGIC+0x0000);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);

        //slider1: 4  < 1,16   > beats (buffersize)
        //slider2: 2  < 0,2,  1{off,dots,lines}> draw mode // !!
        //slider3: 0  < 0,1,  1{manual,seq}> sync mode
        //slider4: 48 < 0,127,1> transport note
        //slider5: 49 < 0,127,1> record note
        //slider6: 50 < 0,127,1> play note
        //slider7: 51 < 0,127,1> erase note
        //slider8: 52 < 0,127,1> bypass note

        appendParameter( pBeats = new parInteger( this, 0, "beats (buffersize)","", 4,  1,16  ));
        appendParameter( pBeats = new parInteger( this, 1, "sync mode)",        "", 0,  0,1,  str_sync ));
        appendParameter( pBeats = new parInteger( this, 2, "transport note",    "", 48, 0,127 ));
        appendParameter( pBeats = new parInteger( this, 3, "record note",       "", 49, 0,127 ));
        appendParameter( pBeats = new parInteger( this, 4, "play note",         "", 50, 0,127 ));
        appendParameter( pBeats = new parInteger( this, 5, "erase note",        "", 51, 0,127 ));
        appendParameter( pBeats = new parInteger( this, 6, "bypass note",       "", 52, 0,127 ));

        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
          if (mGuiInitialized) {}
      }

    //--------------------------------------------------
    // midi, events, ..
    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    // if AX_AUTOSYNC defined
    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    // called for every midi event, just before doProcessBlock/Sample
    // call sendMidi(ofs,msg2,msg2,msg3) to send midi out

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {

        //while(
        //midirecv(ofs,msg1,msg23) ? (
        //  msg == (9*16) ? ( // note on
        //          note = msg23 & 127;
        //  note==transnote  ? EVENTS[ofs] |= 1;
        //      note==recnote    ? EVENTS[ofs] |= 2;
        //      note==playnote   ? EVENTS[ofs] |= 4;
        //      note==erasenote  ? EVENTS[ofs] |= 8;
        //      note==bypassnote ? EVENTS[ofs] |= 16;
        //  );
        //midisend(ofs,msg1,msg23);

        int msg = ((msg1 & 0xf0) >> 4);
        if (msg==9)
        {
          int note = msg2;
          if (note==mTransportNote) mEvents[ofs] |= 1;
          if (note==mRecordNote)    mEvents[ofs] |= 2;
          if (note==mPlayNote)      mEvents[ofs] |= 4;
          if (note==mEraseNote)     mEvents[ofs] |= 8;
          if (note==mBypassNote)    mEvents[ofs] |= 16;
        }
        sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    //@slider
    //  numbeats   = slider1;
    //  drawmode   = slider2;
    //  syncmode   = slider3;
    //  transnote  = slider4;
    //  recnote    = slider5;
    //  playnote   = slider6;
    //  erasenote  = slider7;
    //  bypassnote = slider8;

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mNumBeats       = (int)f;  break;
          case 1: mSyncMode       = (int)f;  break;
          case 2: mTransportNote  = (int)f;  break;
          case 3: mRecordNote     = (int)f;  break;
          case 4: mPlayNote       = (int)f;  break;
          case 5: mEraseNote      = (int)f;  break;
          case 6: mBypassNote     = (int)f;  break;
        }
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    // return trur to indicate that you have processed the samples,
    // if you return false, doProcessSample will be called for each sample
    // in the block

//@block
//  trigger & 1 ? pos = 0;
//  bufsize = numbeats * srate * 60 / tempo;
//  while(
//    midirecv(ofs,msg1,msg23) ? (
//      msg = msg1 & 240;
//      msg == (9*16) ? ( // note on
//        note = msg23 & 127;
//        note==transnote  ? EVENTS[ofs] |= 1;
//        note==recnote    ? EVENTS[ofs] |= 2;
//        note==playnote   ? EVENTS[ofs] |= 4;
//        note==erasenote  ? EVENTS[ofs] |= 8;
//        note==bypassnote ? EVENTS[ofs] |= 16;
//      );
//      midisend(ofs,msg1,msg23);
//    );
//  );
//  offset = 0;

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        mBeatSize = mSampleRate * 60 / mTempo;
        mBufferSize  = mBeatSize * mNumBeats;
        while (mPos>=mBufferSize) mPos-=mBufferSize;
        //if (mPlayState&ptr_changed)
        //{
        //  if (mPlayState&ptr_playing)
        //  {
        //    mPos = 0;
        //    mPlayPos = 0;
        //    mSampleCounter = 0;
        //    mBeatCounter = 0;
        //    mBeatsLeft = mNumBeats;
        //  }
        //}
        mOffset = 0;
        return false;
      }

    //----------

    //@sample
    //  evt = EVENTS[offset];
    //  EVENTS[offset] = 0;
    //  offset += 1;
    //  evt & 2 ? mode=1;  // rec
    //  evt & 4 ? mode=2;  // play
    //  evt & 8 ? mode=3;  // erase
    //  evt & 16 ? mode=4; // bypass
    //  evt & 1 ? playmode = 1-playmode;
    //  pos2 = pos*2;
    //  mode==1 ? ( // record
    //    BUFFER[pos2] = spl0;
    //    BUFFER[pos2+1] = spl1;
    //  );
    //  mode==2 ? ( // play
    //    spl0 = BUFFER[pos2];
    //    spl1 = BUFFER[pos2+1];
    //  ) :
    //  mode==3 ? ( // erase
    //    BUFFER[pos2] = 0;
    //    BUFFER[pos2+1] = 0;
    //  );
    //
    //  (syncmode && (play_state&1))
    //    || playmode ?  pos+=1;
    //  pos>=bufsize ? pos=0;

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        int evt = mEvents[mOffset];
        mEvents[mOffset] = 0;
        mOffset += 1;
        if (evt&1 ) playmode = 1-playmode;
        if (evt&2 ) mMode=1;  // rec
        if (evt&4 ) mMode=2;  // play
        if (evt&8 ) mMode=3;  // erase
        if (evt&16) mMode=4; // bypass


        int pos2 = mPos*2;

        //mode==1 ? ( // record
        //  BUFFER[pos2] = spl0;
        //  BUFFER[pos2+1] = spl1;
        //);
        if (mMode==1)
        {
          mBuffer[pos2  ] = spl0;
          mBuffer[pos2+1] = spl1;
        }
        //mode==2 ? ( // play
        //  spl0 = BUFFER[pos2];
        //  spl1 = BUFFER[pos2+1];
        //) :
        if (mMode==2)
        {
          spl0 = mBuffer[pos2  ];
          spl1 = mBuffer[pos2+1];
        }
        //mode==3 ? ( // erase
        //  BUFFER[pos2] = 0;
        //  BUFFER[pos2+1] = 0;
        //);
        if (mMode==3)
        {
          mBuffer[pos2  ] = 0;
          mBuffer[pos2+1] = 0;
        }

        //(syncmode && (play_state&1))
        //  || playmode ?  pos+=1;
        //pos>=bufsize ? pos=0;
        if (
              ( (mSyncMode==1) && (mPlayState&ptr_playing) )
             || (playmode==1)
           ) mPos++;
        if (mPos>=mBufferSize) mPos=0;

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        if (!mGuiInitialized) { mGuiInitialized = true; }
        axEditor* EDIT = new axEditor("with_gui_win",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        EDIT->appendWidget( wPanel = new wdgPanel(this,-1,NULL_RECT,wal_Client) );
        wPanel->appendWidget( wBeats          = new wdgKnob(  this, 0,axRect( 10, 10, 128, 32),wal_None) );
        wPanel->appendWidget( wSyncMode       = new wdgKnob(  this, 1,axRect( 10, 50, 128, 32),wal_None) );
        wPanel->appendWidget( wTransportNote  = new wdgKnob(  this, 2,axRect(160, 10, 128, 32),wal_None) );
        wPanel->appendWidget( wRecordNote     = new wdgKnob(  this, 3,axRect(160, 50, 128, 32),wal_None) );
        wPanel->appendWidget( wPlayNote       = new wdgKnob(  this, 4,axRect(160, 90, 128, 32),wal_None) );
        wPanel->appendWidget( wEraseNote      = new wdgKnob(  this, 5,axRect(160,130, 128, 32),wal_None) );
        wPanel->appendWidget( wBypassNote     = new wdgKnob(  this, 6,axRect(160,170, 128, 32),wal_None) );

        wPanel->appendWidget( wScope          = new myPainter(this,-1,axRect( 10,210, 300, 50),wal_None) );

        for( int i=0; i<AX_NUMPARAMS; i++) EDIT->connect( wPanel->getWidget(i), mParameters[i] ); // skip wPanel

//        EDIT->connect(wBeats,pBeats);
//        EDIT->connect(wValue,pValue);
//        EDIT->connect(wValue,pValue);
//        EDIT->connect(wValue,pValue);
//        EDIT->connect(wValue,pValue);
//        EDIT->connect(wValue,pValue);
//        EDIT->connect(wValue,pValue);

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

//@gfx
//  gfx_clear=0;
//  gfx_a=1;
//  h2 = gfx_h * 0.5;
//  gfx_x=0;
//  gfx_y = h2;
//  gfx_r=1; gfx_g=1; gfx_b=1;
//  bx = 0;
//  bxadd = bufsize / gfx_w;
//  x = 0;
//  loop( gfx_w,
//    xx = x;
//    n = ( BUFFER[bx*2] + BUFFER[(bx*2)+1] ) * 0.5;
//    yy = h2 + ( n * h2 );
//    drawmode==1 ? ( gfx_x=xx; gfx_y=yy; gfx_setpixel(1,1,1); ) :
//    drawmode==2 ? gfx_lineto(xx,yy,0);
//    x += 1;
//    bx += bxadd;
//  );
//  //mode==0 ? ( gfx_r=0.5; gfx_g=0.5; gfx_b=0.5; ) :
//  mode==1 ? ( gfx_r=1; gfx_g=0; gfx_b=0; ) :
//  mode==2 ? ( gfx_r=0; gfx_g=1; gfx_b=0; ) :
//  mode==3 ? ( gfx_r=0; gfx_g=0; gfx_b=1; ) :
//  mode==4 ? ( gfx_r=1; gfx_g=1; gfx_b=1; );
//  x = pos / bufsize * gfx_w;
//  gfx_x = x;
//  gfx_y = 0;
//  gfx_lineto(x,gfx_h,0);

    virtual void doIdleEditor(void)
      {
        if (mEditor)
        {
          wScope->setup(mBuffer,mBufferSize,mMode,mPos,mNumBeats);
          mEditor->appendDirty( wScope );
          mEditor->redrawDirty();
        }
      }

    //--------------------------------------------------
    // parameter listener
    //--------------------------------------------------

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

#undef MAX_EVENTS_PER_BLOCK

//----------------------------------------------------------------------
#include "axMain.h"
