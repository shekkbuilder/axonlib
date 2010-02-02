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

//----------------------------------------------------------------------
//
// grid
//
//----------------------------------------------------------------------

class myGrid: public wdgGrid
{
  public:
    myGrid(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : wdgGrid(aListener,aID,aRect,aAlignment)
      {
      }
    virtual void on_ClickCell(int aX, int aY, int aB)
      {
        TRACE("x:%i, y:%i, b:%i\n",aX,aY,aB);
      }
};

//----------------------------------------------------------------------
//
// midi_cellular
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
  // gui
    bool      mGuiInitialized;
    axEditor* mEditor;
    wdgPanel* wPanel;
    wdgGrid*  wGrid;

    wdgKnob     *wWidth, *wHeight;
    parInteger  *pWidth, *pHeight;
    int          mWidth,  mHeight;

    bool      redrawgrid;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        mGuiInitialized = false;
        describe("midi_cellular0","ccernn","axonlib example project",0,AX_MAGIC+0x0000);
        hasEditor(AX_WIDTH,AX_HEIGHT);

        appendParameter( pWidth   = new parInteger(this,0,"width",  "", 16, 4,64 ));
        appendParameter( pHeight  = new parInteger(this,1,"height", "", 16, 4,64 ));

        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (mGuiInitialized)
        {
        }
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
          case 0: mWidth  = i;  redrawgrid=true; break;
          case 1: mHeight = i;  redrawgrid=true; break;
        }
      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    // return trur to indicate that you have processed the samples,
    // if you return false, doProcessSample will be called for each sample
    // in the block

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // or AX_AUTOSYNC
    //    return false;
    //  }

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

        wPanel->appendWidget( wGrid   = new myGrid( this,1,axRect(10,10,320,320),wal_None) );

//slider1:  16   < 4,    64,   1   > width
//slider2:  16   < 4,    64,   1   > height
//slider3:  0    < 0,1,1 {off,on,sync} > play mode
//slider4:  0    < 0,1,1 {ms,beats}    > speed mode
//slider5:  500  < 10,   2000, .01 > ms
//slider6:  0.25 < .02,  4,    .01 > beats
//slider7:  1    < 0,    4,    1{cellular automata - 23/3 (life),scroll - left,scroll - right,scroll - up,scroll - down}> update mode
//slider8:  1    < 0,1,1 {off,on} > wrap
//slider9:  50   < 0,    100   .01 > random
//slider10: 2    < 0,2,1 {none,reset,random} > cycle mode
//slider11: 4    < 0,    64,   1  > cycle count
//slider12: 36   < 0,    127,  1  > midi note start (red)
//slider13: 60   < 0,    127,  1  > midi cc (green)
//slider14: 61   < 0,    127,  1  > midi cc (blue)


        wPanel->appendWidget( wWidth    = new wdgKnob(this, 0,axRect(10,340,155,32),wal_None) );
        wPanel->appendWidget( wHeight   = new wdgKnob(this, 1,axRect(10,380,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(10,420,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(10,460,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(10,500,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(10,540,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(10,580,155,32),wal_None) );

        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,340,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,380,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,420,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,460,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,500,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,540,155,32),wal_None) );
        wPanel->appendWidget(             new wdgKnob(this,-1,axRect(175,580,155,32),wal_None) );


        wGrid->setup(mWidth,mHeight);
        redrawgrid = true;

        EDIT->connect(wWidth,pWidth);
        EDIT->connect(wHeight,pHeight);

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

//----------------------------------------------------------------------
#include "axMain.h"





/*




desc:midi_automata :: ccernn.2009 :: v0.0.4

//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
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

//----------------------------------------------------------------------
@serialize

  file_mem(0,CELLS,SIZE2);
  file_mem(0,RTRIG,SIZE2);
  file_mem(0,GTRIG,SIZE2);
  file_mem(0,BTRIG,SIZE2);
  file_mem(0,SAVED,SIZE2);
  redraw=1;

//----------------------------------------------------------------------
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

//----------------------------------------------------------------------
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

//        doupdate ? (

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

          //---

//        ); // doupdate

        redraw = 1;

        //---

        countdown=speed;

      ); // count<block
      block>0;
    ); // while

  ); // isplaying

//----------------------------------------------------------------------
//@sample

//----------------------------------------------------------------------
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
