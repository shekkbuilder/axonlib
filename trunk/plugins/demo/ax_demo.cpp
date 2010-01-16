#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
//#define AX_AUTOSYNC
#define AX_WIDTH      640
#define AX_HEIGHT     480
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
//---parameters---
#include "parFloat.h"
//#include "parInteger.h"
//---gui---
#include "axEditor.h"

#include "wdgPanel.h"
#include "wdgScrollBox.h"
#include "wdgGroupBox.h"
#include "wdgTabs.h"

#include "wdgLabel.h"
#include "wdgKnob.h"
#include "wdgResizer.h"

#include "wdgImgKnob.h"
#include "wdgImgSwitch.h"

#include "axBitmapLoader.h"
#include "../ccernn/images/vslider1_20x100_65h.h" // 46002
#include "../ccernn/images/testbutton.h"          // 10935
#include "../ccernn/images/testbutton2.h"         // 10935
#include "../ccernn/images/testbutton3.h"         // 10935

//#include "axMutex.h"
//#include "axThread.h"

//----------------------------------------------------------------------

//class myThread1 : public axThread
//{
//  public:
//    myThread1() : axThread() {}
//    virtual void doThreadFunc(void) { printf("tick1..\n"); }
//};
//
//class myThread2 : public axThread
//{
//  public:
//    myThread2() : axThread() {}
//    virtual void doThreadFunc(void) { printf("tick2..\n"); }
//};

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener//,
                 //public myThread
{
  private:

    //myThread1 thread1;
    //myThread2 thread2;

    // ---parameters-----
    float         mValue;
    // ---gui---
    bool          mGuiPrepared;
    axEditor      *mEditor;
    axContainer   *wLeft;
    wdgPanel      *wRight, *wTop, *wBottom, *wClient;
    wdgScrollBox  *wScrollBox1,*wScrollBox2;
    wdgGroupBox   *gr1,*gr2,*gr3,*gr4,*gr5;

    wdgLabel      *wStatus;
    wdgKnob       *wKnob1,*wKnob2,*wKnob3;
    wdgResizer    *wSplit1,*wSplit2,*wSplit3,*wWinSizer;

    wdgTabs       *wTabs;
    axContainer   *wPage1,*wPage2,*wPage3;

    wdgImgKnob    *slid;
    wdgImgSwitch  *wBut1,*wBut2,*wBut3,*wBut4;
    axSurface     *mSrfSlider;
    axSurface     *mSrfBut1, *mSrfBut2, *mSrfBut3;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {

        mGuiPrepared = false;
        mEditor = NULL;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("ax_demo","ccernn","axonlib demo",0,0);
        //isSynth();
        appendParameter(new parFloat(this,0,"value","",0));
        processParameters();
        mWidth = AX_WIDTH;
        mHeight = AX_HEIGHT;


      }

    //----------

    virtual ~myPlugin()
      {
        if (mGuiPrepared)
        {
          delete mSrfSlider;
          delete mSrfBut1;
          delete mSrfBut2;
          delete mSrfBut3;
        }
      }

    //--------------------------------------------------
    //
    // editor
    //
    //--------------------------------------------------

    void append_lots_of_knobs(axContainer* con, axWidgetListener* lis, int num)
      {
        for (int i=0; i<num; i++)
        {
          con->appendWidget( new wdgKnob(lis,-1,axRect(0,i*16,64,16),wal_Stacked,0) );
        }
      }

    /*

    100
    200   top
    300   bottom
    400   left
    500   right
    600   client

    1000  sizers

    */

    virtual axWindow* doCreateEditor(void)
      {

        //thread1.startThread(100);
        //thread2.startThread(-1);

        if(!mGuiPrepared)
        {
          mSrfSlider = loadPng( vslider1,    46002 );
          mSrfBut1   = loadPng( testbutton,  10935 );
          mSrfBut2   = loadPng( testbutton2, 10935 );
          mSrfBut3   = loadPng( testbutton3, 10935 );
          mGuiPrepared = true;
        }
        axEditor* ed = new axEditor("demo_window",this,-1,axRect(0,0,mWidth,mHeight/*AX_WIDTH,AX_HEIGHT*/),AX_FLAGS);
          ed->setBackground(false);

          // main panels & splitters

          ed->appendWidget( wLeft     = new axContainer(this, 400,axRect(0,0,200,  0),wal_Left  ) );
          ed->appendWidget( wSplit1   = new wdgResizer( this,1000,axRect(0,0,  5,  0),wal_Left  ) );
          ed->appendWidget( wTop      = new wdgPanel(   this, 200,axRect(0,0,  0, 40),wal_Top   ) );
          ed->appendWidget( wSplit2   = new wdgResizer( this,1001,axRect(0,0,  0,  5),wal_Top   ) );
          ed->appendWidget( wBottom   = new wdgPanel(   this, 300,axRect(0,0,  0, 20),wal_Bottom) );
          ed->appendWidget( wRight    = new wdgPanel(   this, 500,axRect(0,0,100,  0),wal_Right ) );
          ed->appendWidget( wSplit3   = new wdgResizer( this,1002,axRect(0,0,  5,  0),wal_Right ) );
          ed->appendWidget( wClient   = new wdgPanel(   this, 600,NULL_RECT,          wal_Client) );

          wTop->setFlag(wfl_Clip);
          wBottom->setFlag(wfl_Clip);
          wLeft->setFlag(wfl_Clip);
          wRight->setFlag(wfl_Clip);
          wClient->setFlag(wfl_Clip);

          // top

          wTop->setAlign(5,5);
          wTop->appendWidget(  wBut1  = new wdgImgSwitch(this, 101, axRect( 0,0,30,30 ),wal_LeftTop,1, mSrfBut1 ) );
          wTop->appendWidget(  wBut2  = new wdgImgSwitch(this, 102, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut2 ) );
          wTop->appendWidget(  wBut3  = new wdgImgSwitch(this, 103, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut3 ) );
          //wTop->appendWidget(  wBut4  = new wdgImgSwitch(this, 199, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut3 ) );

          // bottom

          wBottom->setAlign(3,3,10,0);
          wBottom->appendWidget( wWinSizer  = new wdgResizer(this, 1003,axRect(0,0,16,16),wal_Right  ));
          wBottom->appendWidget( wStatus    = new wdgLabel(  this, 301, axRect( 0,0,16,16 ),wal_Client, "[status]", AX_GREY_DARK, tal_Left ) );

          // left

          wLeft->appendWidget( wScrollBox1 = new wdgScrollBox(this,401,NULL_RECT,wal_Client) );
          append_lots_of_knobs(wScrollBox1,this,50);
          wScrollBox1->wContainer->setAlign(5,5,16,8);

          // right

          wRight->setAlign(5,5);
          wRight->appendWidget( wKnob1  = new wdgKnob(    this,-99,  axRect( 10, 10, 80,16),wal_None,0 ) );
          wRight->appendWidget( wKnob2  = new wdgKnob(    this,-199, axRect( 10, 30, 80,16),wal_None,0 ) );
          wRight->appendWidget( wKnob3  = new wdgKnob(    this,-299, axRect( 10, 50, 80,16),wal_None,0 ) );
          wRight->appendWidget( slid    = new wdgImgKnob( this,-1,   axRect( 10,100,20,100),wal_None,65,mSrfSlider ));
          slid->mSens1 = 0.01;

          // client

          wClient->setAlign(5,5,0,3);
          wClient->appendWidget( wTabs  = new wdgTabs(this, -1,axRect(10,10,256,128),wal_Client));
          wTabs->appendPage( wPage1 = new axContainer(this, -1,NULL_RECT,wal_Client) );
          wTabs->appendPage( wPage2 = new axContainer(this, -1,NULL_RECT,wal_Client) );
          wTabs->appendPage( wPage3 = new axContainer(this, -1,NULL_RECT,wal_Client) );

          // -- page 1

          wPage1->setBackground(true);
          wPage1->setAlign(16,16,8,8);
          wPage1->setFlag(wfl_Vertical);
          for (int i=0; i<11; i++)
          {
            wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 128,32),wal_Stacked,1 ) );
            wPage1->mWidgets[i]->doSetValue((float)i*0.1);
          }

          // -- page 2

          wPage2->setBackground(true);
          wPage2->setAlign(0,0,10,10);

          wPage2->appendWidget( wScrollBox2 = new wdgScrollBox(this,501,NULL_RECT,wal_Client) );
          wScrollBox2->appendWidget( gr1 = new wdgGroupBox( wPage2,502, axRect( 0,0,0,120),wal_Top ));
          wScrollBox2->appendWidget( gr2 = new wdgGroupBox( wPage2,502, axRect( 0,0,0, 80),wal_Top ));
          wScrollBox2->appendWidget( gr3 = new wdgGroupBox( wPage2,502, axRect( 0,0,0,100),wal_Top ));
          wScrollBox2->appendWidget( gr4 = new wdgGroupBox( wPage2,502, axRect( 0,0,0,300),wal_Top ));
          wScrollBox2->appendWidget( gr5 = new wdgGroupBox( wPage2,502, axRect( 0,0,0,100),wal_Top ));
          gr1->setup("groupbox1",  false,true);
          gr2->setup("group 2",    false,true);
          gr3->setup("box 3",      false,true);
          gr4->setup("...4",       false,true);
          gr5->setup("and five..", false,true);
          gr3->wContainer->setAlign(0,0,3,3);
          gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
          gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
          gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
          gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
          gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );

          gr1->wContainer->setBackground(true, axColor(128,112,112) );
          gr2->wContainer->setBackground(true, axColor(128,128,112) );
          gr3->wContainer->setBackground(true, axColor(112,128,112) );
          gr4->wContainer->setBackground(true, axColor(112,128,128) );
          gr5->wContainer->setBackground(true, axColor(112,112,128) );

          // -- page 3

          wPage3->setBackground(true);
          wPage3->setAlign(16,16,8,8);
          wPage3->appendWidget( new wdgKnob( this,0,axRect( 50, 50,128,32),wal_None,1 ) );
          wPage3->appendWidget( new wdgKnob( this,0,axRect( 50, 90, 64,32),wal_None,0 ) );
          wPage3->appendWidget( new wdgKnob( this,0,axRect( 130,90, 64,32),wal_None,0 ) );

          wTabs->setPage(0);
          // ---
          ed->doRealign();
          ed->connect(wKnob1,mParameters[0]);
        mEditor = ed;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {

        //thread1.stopThread();
        //thread2.stopThread();

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
    //
    // vst
    //
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

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mValue = f;  break;
        }
      }

    //--------------------------------------------------
    //
    // audio
    //
    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------
    //
    // widget listener
    //
    //--------------------------------------------------

    // if you have an editor, to pass parameters changes to widgets
    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        int id = aWidget->mID;
        float val = aWidget->doGetValue();
        switch(id)
        {
          //case -99:
          //  wTabs->setPage( (int)axMin(2,floorf(val*3)) );
          //  mEditor->onChange(wTabs);
          //  break;
          case 101:
            if (val>0.5)
            {
              wTabs->setPage(0);
              wBut2->doSetValue(0);
              wBut3->doSetValue(0);
              mEditor->onChange(wTabs);
              mEditor->onChange(wBut2);
              mEditor->onChange(wBut3);
            }
            else wBut1->doSetValue(1);
            break;
          case 102:
            if (val>0.5)
            {
              wTabs->setPage(1);
              wBut1->doSetValue(0);
              wBut3->doSetValue(0);
              mEditor->onChange(wTabs);
              mEditor->onChange(wBut1);
              mEditor->onChange(wBut3);
            }
            else wBut2->doSetValue(1);
            break;
          case 103:
            if (val>0.5)
            {
              wTabs->setPage(2);
              wBut1->doSetValue(0);
              wBut2->doSetValue(0);
              mEditor->onChange(wTabs);
              mEditor->onChange(wBut1);
              mEditor->onChange(wBut2);
            }
            else wBut3->doSetValue(1);
            break;
//          case 199:
//            if (val>0.5)
//            {
//              testwin = new axWindow( "win_2", this, 12345, axRect(0,0,300,200), AX_WINDOWED|AX_BUFFERED);
//              wdgPanel* pnl;
//              testwin->appendWidget( new wdgPanel(    testwin,-1, axRect( 0,0,100,0  ),wal_Left    ) );
//              testwin->appendWidget( new wdgPanel(    testwin,-1, axRect( 0,0,0,  50 ),wal_Top     ) );
//              testwin->appendWidget( new wdgPanel(    testwin,-1, axRect( 0,0,0,  20 ),wal_Bottom  ) );
//              testwin->appendWidget( new wdgPanel(    testwin,-1, axRect( 0,0,40, 0  ),wal_Right   ) );
//              testwin->appendWidget( pnl=new wdgPanel(testwin,-1, NULL_RECT, wal_Client  ) );
//              pnl->setAlign(5,5,5,5);
//              pnl->appendWidget( new wdgKnob( testwin,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
//              pnl->appendWidget( new wdgKnob( testwin,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
//              pnl->appendWidget( new wdgKnob( testwin,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
//              pnl->appendWidget( new wdgKnob( testwin,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
//              pnl->appendWidget( new wdgKnob( testwin,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
//              testwin->doRealign();
//              testwin->show();
//              testwin->redraw();
//              testwin->eventLoop(); // blocks!!
//              testwin->hide();
//              delete testwin;
//              testwin = NULL;
//            }
//            break;
        }
        // then back to the default handler
        if (mEditor) mEditor->onChange(aWidget);
      }

    //----------

    virtual void onResize(axWidget* aWidget,int dX, int dY)   // delta x,y
      {
        int id = aWidget->mID;
        switch(id)
        {
          case 1000: // left
            {
              int w = wLeft->mRect.w + dX;
              int h = wLeft->mRect.h;// + dY;
              wLeft->doResize(w,h);
              mEditor->doRealign();
              mEditor->redraw();
            }
            break;
          case 1001: // top
            {
              int w = wTop->mRect.w;// + dY;
              int h = wTop->mRect.h + dY;
              wTop->doResize(w,h);
              mEditor->doRealign();
              mEditor->redraw();
            }
            break;
          case 1002: // right
            {
              int w = wRight->mRect.w - dX;
              int h = wRight->mRect.h;// + dY;
              wRight->doResize(w,h);
              mEditor->doRealign();
              mEditor->redraw();
            }
            break;
          case 1003: // window resizer
            {
              int w = mEditor->mRect.w + dX;
              int h = mEditor->mRect.h + dY;
              mEditor->resizeWindow(w,h);
            }
            break;
          case 502: // group boxes
            {
              wPage2->doRealign();
              mEditor->onChange(wPage2);
            }
            break;

        }
      }

    //----------

    virtual void onCursor(int aCursor)
      {
        mEditor->onCursor(aCursor);
      }

    //----------

    virtual void onHint(axString aHint)
      {
        wStatus->setText(aHint);
        mEditor->onChange(wStatus);
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"

