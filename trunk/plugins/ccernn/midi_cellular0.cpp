// currently in a non-working state....
// delayed until a few other things are in place

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2
#define AX_WIDTH      340
#define AX_HEIGHT     630
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC
//#define AX_DIRTYWIDGETS
//#define AX_PAINTERS

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
#include "wdgPanel.h"
#include "wdgKnob.h"
#include "wdgGrid.h"


    // size of grid in one dimension (width)
    // +2 because of border for wrapping
    #define SIZE  (64+2)
    // full grid
    #define SIZE2 (SIZE*SIZE)

    // distance from CELLS
    //#define C2 SIZE2
    //#define CELLS  0
    #define SAVED  (SIZE2*2)
    #define RTRIG  (SIZE2*3)
    #define GTRIG  (SIZE2*4)
    #define BTRIG  (SIZE2*5)


//----------------------------------------------------------------------
//
// grid
//
//----------------------------------------------------------------------

class myGrid: public wdgGrid
{
  private:
    axBrush*  mBrushes[16];
    int*      CELLS; // dangerous
  public:
    myGrid(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, int* aCells)
    : wdgGrid(aListener,aID,aRect,aAlignment)
      {
        //CELLS = NULL;
        CELLS = aCells;
        mBrushes[ 0] = new axBrush( AX_GREY_DARK );     // ....
        mBrushes[ 1] = new axBrush( AX_RED_DARK );      // ...r
        mBrushes[ 2] = new axBrush( AX_GREEN_DARK );    // ..g.
        mBrushes[ 3] = new axBrush( AX_YELLOW_DARK );   // ..gr
        mBrushes[ 4] = new axBrush( AX_BLUE_DARK );     // .b..
        mBrushes[ 5] = new axBrush( AX_MAGENTA_DARK );  // .b.r
        mBrushes[ 6] = new axBrush( AX_CYAN_DARK );     // .bg.
        mBrushes[ 7] = new axBrush( AX_GREY_LIGHT );    // .bgr
        mBrushes[ 8] = new axBrush( AX_GREY_LIGHT );    // *...
        mBrushes[ 9] = new axBrush( AX_RED );           // *..r
        mBrushes[10] = new axBrush( AX_GREEN );         // *.g.
        mBrushes[11] = new axBrush( AX_YELLOW );        // *.gr
        mBrushes[12] = new axBrush( AX_BLUE );          // *b..
        mBrushes[13] = new axBrush( AX_MAGENTA );       // *b.r
        mBrushes[14] = new axBrush( AX_CYAN );          // *bg.
        mBrushes[15] = new axBrush( AX_WHITE );         // *bgr
      }
    virtual ~myGrid()
      {
        for (int i=0; i<16;i++) delete mBrushes[i];
      }
    //inline void setCells(int* aCells) { CELLS=aCells; }
    virtual void on_DrawCell(axCanvas* aCanvas, axRect aRect,int x, int y)
      {
        assert(CELLS);
        int index = (y*SIZE)+x;
        index += SIZE+1;
        int cel = CELLS[index];
        int brush = 0;
        if (cel==1) brush = 8;
        if (CELLS[RTRIG+index]==1) brush |= 1;
        if (CELLS[GTRIG+index]==1) brush |= 2;
        if (CELLS[BTRIG+index]==1) brush |= 4;
        aCanvas->selectBrush( mBrushes[brush] );
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }
    virtual void on_ClickCell(int x, int y, int b)
      {
        assert(CELLS);
        int index = (y*SIZE)+x;
        index += SIZE+1;
        trace( "x:" << x << " y:" << y << " b:" << b);
        if (b==1) CELLS[index] = 1;
        if (b==2) CELLS[index] = 0;
        if (b==but_Shift+1) CELLS[RTRIG+index] = 1;
        if (b==but_Shift+2) CELLS[RTRIG+index] = 0;
        if (b==but_Ctrl +1) CELLS[GTRIG+index] = 1;
        if (b==but_Ctrl +2) CELLS[GTRIG+index] = 0;
        if (b==but_Alt  +1) CELLS[BTRIG+index] = 1;
        if (b==but_Alt  +2) CELLS[BTRIG+index] = 0;
      }
};

//----------------------------------------------------------------------
//
// midi_cellular
//
//----------------------------------------------------------------------

//// size of grid in one dimension (width)
//// +2 because of border for wrapping
//#define GRIDSIZE  (64+2)
//// full grid
//#define GRIDSIZE2 (GRIDSIZE*GRIDSIZE)
//// distance from CELLS
//#define C2 GRIDSIZE2
////#define CELLS  0
//#define SAVED  (GRIDSIZE2*2)
//#define RTRIG  (GRIDSIZE2*3)
//#define GTRIG  (GRIDSIZE2*4)
//#define BTRIG  (GRIDSIZE2*5)
//
//#define NUMGRIDS 6
//
//// 0 =
//// 1 =
//// 2 = save
//// 3 = r
//// 4 = g
//// 5 = b

char* str_playmode[]    = { (char*)"off", (char*)"on", (char*)"sync" };
char* str_speedmode[]   = { (char*)"ms", (char*)"beats" };
char* str_updatemode[]  = { (char*)"life (2/3)", (char*)"scroll left", (char*)"scroll right", (char*)"scroll up", (char*)"scroll down" };
char* str_wrap[]        = { (char*)"off", (char*)"on" };
char* str_cyclemode[]   = { (char*)"none", (char*)"reset",(char*)"random" };

//----------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
  // gui
    bool      mGuiInitialized;
    axEditor* mEditor;
    wdgPanel* wPanel;
    myGrid*   wGrid;

    wdgKnob     *wWidth, *wHeight;
    wdgKnob     *wPlayMode, *wSpeedMode, *wUpdateMode, *wCycleMode;
    wdgKnob     *wCycleCount, *wWrap, *wMidiNote, *wMidiCC1, *wMidiCC2;
    wdgKnob     *wMS, *wBeats, *wRandom;

    parInteger  *pWidth, *pHeight;
    parInteger  *pPlayMode, *pSpeedMode, *pUpdateMode, *pCycleMode;
    parInteger  *pCycleCount, *pWrap, *pMidiNote, *pMidiCC1, *pMidiCC2;
    parFloat    *pMS, *pBeats, *pRandom;

    int         mWidth,  mHeight;
    int         mPlayMode, mSpeedMode, mUpdateMode, mCycleMode;
    int         mCycleCount, mWrap, mMidiNote, mMidiCC1, mMidiCC2;
    float       mMS, mBeats, mRandom;

    //int   width;
    //int   height;

    float speed;

    bool  redrawgrid;
    bool  redraw;
    int   prevstate;
    bool  isplaying;
    int   countdown;
    int   cycle;
    int   offset;
    int   block;
    int*  mCells;
    int   CELLS;
    int   SIZEh;
    int   C2;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //CELLS = new int[SIZE2 * 6];
        //memset(CELLS,0,SIZE2*6*sizeof(float));
        mCells = new int[SIZE2 * 6];
        memset(mCells,0,SIZE2*6*sizeof(float));
        CELLS = 0;
        C2 = SIZE2;
        isplaying = false;
        countdown = 0;
        cycle = 0;
        prevstate = 0;
        redraw = false;
        //width = 320;
        //height = 320;

        mEditor = NULL;
        mGuiInitialized = false;
        describe("midi_cellular0","ccernn","axonlib example project",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        //hasEditor(AX_WIDTH,AX_HEIGHT);

        appendParameter( pWidth       = new parInteger(this, 0,"width",       "", 16,   4,    64          ));
        appendParameter( pHeight      = new parInteger(this, 1,"height",      "", 16,   4,    64          ));
        appendParameter( pPlayMode    = new parInteger(this, 2,"play mode",   "", 0,    0,    2,          str_playmode ));
        appendParameter( pSpeedMode   = new parInteger(this, 3,"speed mode",  "", 0,    0,    1,          str_speedmode ));
        appendParameter( pMS          = new parFloat(  this, 4,"ms",          "", 500,  10,   2000, 0.01  ));
        appendParameter( pBeats       = new parFloat(  this, 5,"beats",       "", 0.25, 0.02, 4,    0.01  ));
        appendParameter( pUpdateMode  = new parInteger(this, 6,"update mode", "", 1,    0,    4,          str_updatemode ));

        appendParameter( pWrap        = new parInteger(this, 7,"wrap",        "", 1,    0,    1,          str_wrap ));
        appendParameter( pRandom      = new parFloat(  this, 8,"random",      "", 50,   0,    100,  0.01  ));
        appendParameter( pCycleMode   = new parInteger(this, 9,"cycle mode",  "", 2,    0,    2,          str_cyclemode ));
        appendParameter( pCycleCount  = new parInteger(this,10,"cycle count", "", 4,    0,    64 ));
        appendParameter( pMidiNote    = new parInteger(this,11,"midi note",   "", 36,   0,    127 ));
        appendParameter( pMidiCC1     = new parInteger(this,12,"midi cc",     "", 60,   0,    127 ));
        appendParameter( pMidiCC2     = new parInteger(this,13,"midi cc",     "", 61,   0,    127 ));

        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (mGuiInitialized)
        {
        }
      }

    #include "midi_cellular0_dsp.h"
    #include "midi_cellular0_gui.h"

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

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        int i = (int)f;
        switch(id)
        {
          case 0:  mWidth=i;      redrawgrid=true; break;
          case 1:  mHeight=i;     redrawgrid=true; SIZEh=SIZE*mHeight; break;
          case 2:  mPlayMode=i;   break;
          case 3:  mSpeedMode=i;  break;
          case 4:  mMS=f/1000;    break;
          case 5:  mBeats=f;      break;
          case 6:  mUpdateMode=i; break;
          case 7:  mWrap=i;       break;
          case 8:  mRandom=f/100; break;
          case 9:  mCycleMode=i;  break;
          case 10: mCycleCount=i; break;
          case 11: mMidiNote=i;   break;
          case 12: mMidiCC1=i;    break;
          case 13: mMidiCC2=i;    break;
        }
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        updateTimeInfo();
        do_Timing();
        do_Transport();
        do_Sync();
        do_Cycle();
        return false; //TODO: clear samples, return true
      }

    //----------

    //virtual void doProcessSample(float** ins, float** outs)
    //  {
    //    float spl0 = *ins[0];
    //    float spl1 = *ins[1];
    //    *outs[0] = spl0;
    //    *outs[1] = spl1;
    //  }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        axEditor* EDIT = new axEditor("with_gui_win",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        EDIT->appendWidget(wPanel = new wdgPanel(this,-1,NULL_RECT,wal_Client));
        if (!mGuiInitialized) { mGuiInitialized = true; }

        wPanel->appendWidget( wGrid       = new myGrid( this,-1,axRect( 10,10,320,320),wal_None,CELLS) );

        wPanel->appendWidget( wWidth      = new wdgKnob(this, 0,axRect( 10,340,155,32),wal_None) );
        wPanel->appendWidget( wHeight     = new wdgKnob(this, 1,axRect( 10,380,155,32),wal_None) );
        wPanel->appendWidget( wPlayMode   = new wdgKnob(this, 2,axRect( 10,420,155,32),wal_None) );
        wPanel->appendWidget( wSpeedMode  = new wdgKnob(this, 3,axRect( 10,460,155,32),wal_None) );
        wPanel->appendWidget( wMS         = new wdgKnob(this, 4,axRect( 10,500,155,32),wal_None) );
        wPanel->appendWidget( wBeats      = new wdgKnob(this, 5,axRect( 10,540,155,32),wal_None) );
        wPanel->appendWidget( wUpdateMode = new wdgKnob(this, 6,axRect( 10,580,155,32),wal_None) );

        wPanel->appendWidget( wWrap       = new wdgKnob(this, 7,axRect(175,340,155,32),wal_None) );
        wPanel->appendWidget( wRandom     = new wdgKnob(this, 8,axRect(175,380,155,32),wal_None) );
        wPanel->appendWidget( wCycleMode  = new wdgKnob(this, 9,axRect(175,420,155,32),wal_None) );
        wPanel->appendWidget( wCycleCount = new wdgKnob(this,10,axRect(175,460,155,32),wal_None) );
        wPanel->appendWidget( wMidiNote   = new wdgKnob(this,11,axRect(175,500,155,32),wal_None) );
        wPanel->appendWidget( wMidiCC1    = new wdgKnob(this,12,axRect(175,540,155,32),wal_None) );
        wPanel->appendWidget( wMidiCC2    = new wdgKnob(this,13,axRect(175,580,155,32),wal_None) );

        wGrid->setup(mWidth,mHeight);
        //wGrid->setCells(CELLS);
        redrawgrid = true;

        EDIT->connect(wWidth,pWidth);
        EDIT->connect(wHeight,pHeight);
        EDIT->connect(wPlayMode,pPlayMode);
        EDIT->connect(wSpeedMode,pSpeedMode);
        EDIT->connect(wMS,pMS);
        EDIT->connect(wBeats,pBeats);
        EDIT->connect(wUpdateMode,pUpdateMode);

        EDIT->connect(wWrap,pWrap);
        EDIT->connect(wRandom,pRandom);
        EDIT->connect(wCycleMode,pCycleMode);
        EDIT->connect(wCycleCount,pCycleCount);
        EDIT->connect(wMidiNote,pMidiNote);
        EDIT->connect(wMidiCC1,pMidiCC1);
        EDIT->connect(wMidiCC2,pMidiCC2);

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
          if (redrawgrid)
          {
            //trace("idle editor: redrawgrid");
            wGrid->setup(mWidth,mHeight);
            mEditor->appendDirty(wGrid);
            redrawgrid=false;
          }
          mEditor->redrawDirty();
        }
      }

    //--------------------------------------------------
    // parameter listener
    //--------------------------------------------------

    // needed if you have an editor, to notify it about
    // parameter changes. after we've told the editor, we
    // call doProcessEditor (the default, if you don't
    // override this)

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

    #undef SIZE
    #undef SIZE2
    //#undef C2
    #undef SAVED
    #undef RTRIG
    #undef GTRIG
    #undef BTRIG

//----------------------------------------------------------------------
#include "axMain.h"





/*




desc:midi_automata :: ccernn.2009 :: v0.0.4
  slider1:  16   < 4,    64,   1   > width
  slider2:  16   < 4,    64,   1   > height
  slider3:  0    < 0,1,1 {off,on,sync} > play mode
  slider4:  0    < 0,1,1 {ms,beats}    > speed mode
  slider5:  500  < 10,   2000, .01 > ms
  slider6:  0.25 < .02,  4,    .01 > beats
  slider7:  1    < 0,    4,    1{cellular automata - 23/3 (life),scroll - left,scroll - right,scroll - up,scroll - down}> update mode
  slider8:  1    < 0,1,1 {off,on} > wrap
  slider9:  50   < 0,    100   .01 > random
  slider10: 2    < 0,2,1 {none,reset,random} > cycle mode
  slider11: 4    < 0,    64,   1  > cycle count
  slider12: 36   < 0,    127,  1  > midi note start (red)
  slider13: 60   < 0,    127,  1  > midi cc (green)
  slider14: 61   < 0,    127,  1  > midi cc (blue)

@init
  //FR: ext_noinit=2
  //    clear variables, but not ram (only first time)
  //    or a separate @transport section
  //----------
  // to protect our grids/cells
  ext_noinit = 1;
  SIZE   = 64+2;
  SIZE2  = (SIZE*SIZE);
  CELLS  = 1000;  // ptr
  C2     = SIZE2; // distance from CELLS
  SAVED  = CELLS + SIZE2*2;
  RTRIG  = CELLS + SIZE2*3;
  GTRIG  = CELLS + SIZE2*4;
  BTRIG  = CELLS + SIZE2*5;
  isplaying  = 0;
  countdown  = 0;
  cycle      = 0;
  prevstate  = 0;
  redraw     = 0;
  width      = 1;
  height     = 1;

@serialize

  file_mem(0,CELLS,SIZE2);
  file_mem(0,RTRIG,SIZE2);
  file_mem(0,GTRIG,SIZE2);
  file_mem(0,BTRIG,SIZE2);
  file_mem(0,SAVED,SIZE2);
  redraw=1;

@slider
  ww          = slider1;
  hh          = slider2;
  playmode    = slider3;
  speedmode   = slider4;
  _speed      = slider5/1000;
  _beats      = slider6;
  updatemode  = slider7;
  wrapmode    = slider8;
  random      = slider9/100;
  cyclemode   = slider10;
  cyclecount  = slider11;
  notestart   = slider12+hh-1;
  ccnum1      = slider13;
  ccnum2      = slider14;
  ww!=width  ? ( width=ww;  redraw=1; );
  hh!=height ? ( height=hh; redraw=1; );
  SIZEh = SIZE*hh;

@block
  speedmode==0 ? ( // ms
    beats = _speed * (tempo/60);
    beats != slider6 ? ( slider6=beats; sliderchange(slider6); );
    speed = _speed*srate;
  ) :
  speedmode==1 ? ( // beats
    speed = _beats * srate / (tempo/60);
    ms    = _beats * (60/tempo) * 1000;
    ms != slider5 ? ( slider5=ms; sliderchange(slider5); );
  );
  // safeguarding
  speed < 256 ? speed=256;
  //----------
  play_state != prevstate ? (
    play_state & 1 ? (
      isplaying = 1;
      //playmode==2 ? countdown=0;
      countdown = 0;//speed;
      cycle = 0;//cyclecount;
    ) : (
      isplaying=0;
    );
    prevstate = play_state;
  );
  //----------
  trigger==1 ? redraw=1;
  trigger==2 ? (
    memset(CELLS,0,SIZE2);
    redraw=1;
  );
  trigger==4 ? (
  //memset(CELLS,0,SIZE2);
    memset(RTRIG,0,SIZE2);
    memset(GTRIG,0,SIZE2);
    memset(BTRIG,0,SIZE2);
    redraw=1;
  );
  trigger==8 ? (
    random > 0 ? (
      memset(CELLS,0,SIZE2);
      C = CELLS+SIZE+1;
      loop( width*height*random,
        x = rand(1)*width;
        y = rand(1)*height;
        C[(y*SIZE)+x] = 1;
      );
      redraw=1;
    );
  );
  trigger==16 ? (
    memcpy(SAVED,CELLS,SIZE2);
  );
  trigger==32 ? (
    memcpy(CELLS,SAVED,SIZE2);
    redraw=1;
  );
  //----------
  playmode==1 || (playmode==2 && isplaying) ? (
    offset = 0;
    block = samplesblock;
    while(
      countdown >= block ? (
        countdown-=block;
        block=0;
      ) : ( // count>=block
        offset += countdown;
        block-=countdown;
        //--- reset/randomize
        doupdate=1;

//----------

        cyclemode>0 ? (
          cycle-=1;
          cycle<=0 ? (
            cyclemode==1 ? memcpy(CELLS+C2,SAVED,SIZE2);
            cyclemode==2 ? (
              memset(CELLS+C2,0,SIZE2);
              random>0 ? (
                C = CELLS+C2+SIZE+1;
                loop( width*height*random,
                  x = rand(1)*width;
                  y = rand(1)*height;
                  C[(y*SIZE)+x] = 1;
                );
              ); // random>0
            ); // cyclemode=2
            cycle = cyclecount;
            doupdate=0;
          ); // cycle<=0
        ); // cyclemode>0

  //      doupdate ? (

          //--- wraparound
          cell = CELLS;
          wrapmode==1 ? (
            memcpy( cell, cell+SIZEh, width+2);
            memcpy( cell+SIZEh+SIZE, cell+SIZE, width+2);
          ) : (
            memset( cell, 0, width+2);
            memset( cell+SIZEh+SIZE, 0, width+2);
          );
          //temp = cell;
          loop( height+2,
            wrapmode==1 ? (
              cell[0] = cell[width];
              cell[width+1] = cell[1];
            ) : (
              cell[0] = 0;
              cell[width+1] = 0;
            );
            cell += SIZE;
          );
          //--- update grid
          cell = CELLS+SIZE+1;
          y = 0;
          loop( height,
            wrapmode==1 ? (
              cell[0] = cell[w];
              cell[w+1] = cell[1];
            ) : (
              cell[0] = 0;
              cell[w+1] = 0;
            );
            x = 0;
            loop( width,
              n = cell[];
              doupdate==1 ? (
              // TODO: updatemode 0,1,2,3 = scroll
              // (chance slider/dropbox...)
              // 4+, different cellular automata
              //
              // Life     = B3/S23
              // Gnarl    = B1/S1
              // Seeds    = B2/S
              // 34Life   = B34/S34
              // LongLife = B345/S5
              // HighLife = B36/S23
              updatemode==0 ? (
                neighbours = 0;
                neighbours += cell[-SIZE-1];
                neighbours += cell[-SIZE  ];
                neighbours += cell[-SIZE+1];
                neighbours += cell[     -1];
                neighbours += cell[     +1];
                neighbours += cell[+SIZE-1];
                neighbours += cell[+SIZE  ];
                neighbours += cell[+SIZE+1];
                // life: B3/S23
                n==0 ? ( // B3
                  neighbours==3 ? cell[C2]=1 : cell[C2]=0;
                );
                n==1 ? ( // S23
                  neighbours<2 ? cell[C2]=0 :
                  neighbours>3 ? cell[C2]=0 :
                  cell[C2]=1;
                );
              ); // life
              updatemode==1 ? cell[C2] = cell[+1];
              updatemode==2 ? cell[C2] = cell[-1];
              updatemode==3 ? cell[C2] = cell[+SIZE];
              updatemode==4 ? cell[C2] = cell[-SIZE];
              );
              //--- midi
              index = (y*SIZE)+x + SIZE+1;
              RTRIG[index] ? (
                note = notestart-y;
                cell[C2]==0 ? c==1 ?
                  midisend(offset-1,$x80, note | (0*256) );
                cell[C2]==1 ?
                  midisend(offset,$x90+0, note | (127*256) );
              );
              GTRIG[index] ? (
                value = (height-y-1) * (128/height); // (h-y-1) ?
                cell[C2]==1 ?
                  midisend(offset,$xb0+0, ccnum1  | (value*256) );
              );
              BTRIG[index] ? (
                value = (height-y-1) * (128/height); // (h-y-1) ?
                cell[C2]==1 ?
                  midisend(offset,$xb0+0, ccnum2  | (value*256) );
              );
              //---
              cell += 1;
              x += 1;
            ); // loop width
            cell += (SIZE-width);
            y += 1;
          ); // loop height
          CELLS += C2;
          C2 = -C2;

  //      ); // doupdate

//----------

        redraw = 1;
        //---
        countdown=speed;
      ); // count<block
      block>0;
    ); // while
  ); // isplaying

@gfx
  xcell = gfx_w / width;
  ycell = gfx_h / height;
  redraw ? (
    gfx_clear=0;
    gfx_a=1;
    redraw = 0;
    //---------- cells
    y = 0;
    loop( height,
      x = 0;
      loop( width,
        index = (y*SIZE)+x;
        index += SIZE+1;
        cel = CELLS[index];
        cel==1 ? (
          gfx_r=0.4; gfx_g=0.4; gfx_b=0.4;
          RTRIG[index] ? gfx_r=1;
          GTRIG[index] ? gfx_g=1;
          BTRIG[index] ? gfx_b=1;
        ) : ( // cel=1
          gfx_r=0; gfx_g=0; gfx_b=0;
          RTRIG[index] ? gfx_r=0.4;
          GTRIG[index] ? gfx_g=0.4;
          BTRIG[index] ? gfx_b=0.4;
        ); // cel!=1
        gfx_x=(x*xcell); gfx_y=(y*ycell);
        gfx_rectto(gfx_x+xcell,gfx_y+ycell);
        x += 1;//xcell;
      ); // loop width
      y += 1;//ycell;
    ); // loop height
    //---------- grid
    gfx_r=0.2; gfx_g=0.2; gfx_b=0.2;
    x = xcell;
    loop( width-1,
      gfx_x=x; gfx_y=0;
      gfx_lineto(x,gfx_h,0);
      x += xcell;
    );
    y = ycell;
    loop( height-1,
      gfx_x=0; gfx_y=y;
      gfx_lineto(gfx_w,y,0);
      y += ycell;
    );
  ); // redraw
  // ---------- mouse
  mouse_cap>0 ? (
    x = floor( mouse_x / xcell );
    y = floor( mouse_y / ycell );
    index = (y*SIZE)+x;
    index += SIZE+1;
    mouse_cap==1 ? CELLS[index]=1;
    mouse_cap==2 ? CELLS[index]=0;
    // shift
    mouse_cap==(8+1) ? RTRIG[index]=1;
    mouse_cap==(8+2) ? RTRIG[index]=0;
    // ctrl
    mouse_cap==(4+1) ? GTRIG[index]=1;
    mouse_cap==(4+2) ? GTRIG[index]=0;
    // alt
    mouse_cap==(16+1) ? BTRIG[index]=1;
    mouse_cap==(16+2) ? BTRIG[index]=0;
    redraw = 1;
  );

*/
