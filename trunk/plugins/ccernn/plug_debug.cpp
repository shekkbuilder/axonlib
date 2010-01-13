#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  0
//#define AX_AUTOSYNC
//#define AX_PAINTERS
#define AX_WIDTH      512
#define AX_HEIGHT     384
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"

//#include "parFloat.h"
//#include "parInteger.h"

#include "wdgLabel.h"
#include "wdgKnob.h"
#include "wdgTabs.h"
#include "wdgPanel.h"
#include "wdgResizer.h"
#include "wdgGroupBox.h"
//#include "wdgScroller.h"
#include "wdgScrollBox.h"

#include "wdgImgKnob.h"
#include "wdgImgSwitch.h"
#include "axBitmapLoader.h"
#include "images/vslider1_20x100_65h.h" // 46002
#include "images/testbutton.h"          // 10935
#include "images/testbutton2.h"         // 10935
#include "images/testbutton3.h"         // 10935

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  public:
    bool        is_gui_initialized;
    axEditor    *mEditor;
    //axWindow    *testwin;

    axSurface   *mSrfSlider;
    axSurface   *mSrfBut1, *mSrfBut2, *mSrfBut3;

    wdgKnob       *wKnob,*wKnob2,*wKnob3;
    wdgTabs       *wTabs;
    axContainer   *wPage1,*wPage2,*wPage3;
    wdgResizer    *wSizer;
    wdgResizer    *wSplitter;
    wdgPanel      *wLeft, *wRight, *wTop, *wBottom, *wClient;
    wdgImgKnob    *slid;
    wdgImgSwitch  *wBut1,*wBut2,*wBut3,*wBut4;
    wdgLabel      *wStatus;
    wdgGroupBox   *gr1,*gr2,*gr3,*gr4,*gr5;

    //wdgScroller   *scr;
    wdgScrollBox   *scr;
    //float prevscroll;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        //prevscroll = 0;
        //testwin = NULL;
        mEditor = NULL;
        is_gui_initialized = false;
        mSrfSlider = NULL;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("plug_debug","ccernn","product_string",0,0);
      }

    //----------

    virtual ~myPlugin()
      {
        if (is_gui_initialized)
        {
          delete mSrfSlider;
          delete mSrfBut1;
          delete mSrfBut2;
          delete mSrfBut3;
        }
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //    if(mEditor) mEditor->onChange(aParameter);
    //    doProcessParameter(aParameter);
    //  }

    virtual void onChange(axWidget* aWidget)
      {
        // if changed widget = knob, switch tab/page
        int id = aWidget->mID;
        float val = aWidget->doGetValue();
        switch(id)
        {
          case -99:
            wTabs->setPage( (int)axMin(2,floorf(val*3)) );
            mEditor->onChange(wTabs);
            break;
//          case -199:
//          {
//            int hh = wLeft->mRect.h - wLeft->mMarginY;
//            int i = (float)(wLeft->mMaxY - hh) * val;
//            int dy = i - prevscroll;
//            wLeft->doScroll(0,-dy);
//            mEditor->onChange(wLeft);
//            //TRACE("%i\n",y2);
//            //wLeft->doScroll(dx,dy);
//            prevscroll = i;
//          }
//            break;
//          //case -299:
//          //  break;

          case 100:
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
          case 101:
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
          case 102:
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
        mEditor->onChange(aWidget);
      }

    //----------

//    void recalc_scroller(void)
//      {
//        //HACK
//        int total = gr1->mRect.h
//                  + gr2->mRect.h
//                  + gr3->mRect.h
//                  + gr4->mRect.h
//                  + gr5->mRect.h
//                  + (5*4);
//        //TRACE("total: %i\n",total);
//        float n = (float)scr->mRect.h / (float)total;
//        n = axMax(0,axMin(1,n));
//        scr->setThumbSize(n);
//        //mEditor->onChange(scr);
//      }

    virtual void onResize(axWidget* aWidget,int dX, int dY)   // delta x,y
      {
        if (aWidget->mID == 314) // wdgSplitter, window resizer
        {
          int w = mEditor->mRect.w + dX;
          int h = mEditor->mRect.h + dY;
          mEditor->resizeWindow(w,h);
          //recalc_scroller();
          //mEditor->onChange(scr);
        }
        if (aWidget->mID == 315) // wdgSplitter, left splitter
        {
          int w = wLeft->mRect.w + dX;
          int h = wLeft->mRect.h;// + dY;
          wLeft->doResize(w,h);
          mEditor->doRealign();
          //recalc_scroller();
          mEditor->redraw();
        }
//        if (aWidget->mID == 1000) // group boxes
//        {
//          wLeft->doRealign();
//          //recalc_scroller();
//          mEditor->onChange(wLeft);
//        }
      }

    virtual void onCursor(int aCursor) { mEditor->onCursor(aCursor); }

    //

    virtual void onHint(axString aHint)
      {
        wStatus->setText(aHint);
        mEditor->onChange(wStatus);
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    // if AX_AUTOSYNC
    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //--------------------------------------------------
    // midi / params
    //--------------------------------------------------

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

//    virtual void doProcessParameter(axParameter* aParameter)
//      {
//        int  id = aParameter->mID;
//        float f = aParameter->getValue();
//        switch(id)
//        {
//          case 0: mValue = f;  break;
//        }
//      }

    //--------------------------------------------------
    // audio
    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
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

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* ed = new axEditor("plug_debug_win",this,-1,axRect(0,0,mWidth,mHeight),AX_FLAGS);
        if(!is_gui_initialized)
        {
          mSrfSlider = loadPng( vslider1,    46002 );
          mSrfBut1   = loadPng( testbutton,  10935 );
          mSrfBut2   = loadPng( testbutton2, 10935 );
          mSrfBut3   = loadPng( testbutton3, 10935 );
          is_gui_initialized=true;
        }

        // the 5 main panels & splitter

        ed->setBackground(false);
        ed->appendWidget(    wLeft      = new wdgPanel(   this,-1,   axRect( 0,0,200,0  ),wal_Left    ) );
        ed->appendWidget( wSplitter     = new wdgResizer( this, 315, axRect( 0,0,5,5    ),wal_Left ) );
        ed->appendWidget(     wTop      = new wdgPanel(   this,-1,   axRect( 0,0,0,  40 ),wal_Top     ) );
        ed->appendWidget(     wBottom   = new wdgPanel(   this,-1,   axRect( 0,0,0,  20 ),wal_Bottom  ) );
        ed->appendWidget(     wRight    = new wdgPanel(   this,-1,   axRect( 0,0,100,0  ),wal_Right   ) );
        ed->appendWidget(     wClient   = new wdgPanel(   this,-1,   NULL_RECT,          wal_Client  ) );

        // top : buttons for tabs

        wTop->setAlign(5,5);
        wTop->appendWidget(  wBut1   = new wdgImgSwitch(this, 100, axRect( 0,0,30,30 ),wal_LeftTop,1, mSrfBut1 ) );
        wTop->appendWidget(  wBut2   = new wdgImgSwitch(this, 101, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut2 ) );
        wTop->appendWidget(  wBut3   = new wdgImgSwitch(this, 102, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut3 ) );
        wTop->appendWidget(  wBut4   = new wdgImgSwitch(this, 199, axRect( 0,0,30,30 ),wal_LeftTop,0, mSrfBut3 ) );

        // left: group boxes

        //wLeft->setAlign(5,5,5,5);
        //wLeft->appendWidget( scr = new wdgScroller( this,1001, axRect(0,0,8,0), wal_Left ));
        wLeft->appendWidget( scr = new wdgScrollBox( this,1001, NULL_RECT, wal_Client ));

        /*wLeft*/scr->appendWidget( gr1 = new wdgGroupBox( wLeft,1000, axRect( 0,0,0, 75),wal_Top ));
        /*wLeft*/scr->appendWidget( gr2 = new wdgGroupBox( wLeft,1000, axRect( 0,0,0, 50),wal_Top ));
        /*wLeft*/scr->appendWidget( gr3 = new wdgGroupBox( wLeft,1000, axRect( 0,0,0,100),wal_Top ));
        /*wLeft*/scr->appendWidget( gr4 = new wdgGroupBox( wLeft,1000, axRect( 0,0,0,200),wal_Top ));
        /*wLeft*/scr->appendWidget( gr5 = new wdgGroupBox( wLeft,1000, axRect( 0,0,0, 50),wal_Top ));

        gr3->wContainer->setAlign(0,0,3,3);
        gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        gr3->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );

        //scr->setFlag(wfl_Vertical);
        //scr->setThumb(0, 0.25);

//        gr1->setup("groupbox1",  false,true);
//        gr2->setup("group 2",    false,true);
//        gr3->setup("box 3",      false,true);
//        gr4->setup("...4",       false,true);
//        gr5->setup("and five..", false,true);

//        gr1->setBackground(false,axColor(96,96,96));
//        gr2->setBackground(false,axColor(96,96,96));
//        gr3->setBackground(false,axColor(96,96,96));
//        gr4->setBackground(false,axColor(144,144,144));
//        gr5->setBackground(false,axColor(96,96,96));

        // bottom: status bar & resize widget

        wBottom->setAlign(3,3,10,0);
        wBottom->appendWidget( wSizer = new wdgResizer( this, 314,  axRect( 0,0,16,16 ),wal_Right ) );
        wBottom->appendWidget( wStatus = new wdgLabel(  this, -1,   axRect( 0,0,16,16 ),wal_Client, "[status]", AX_GREY_DARK, tal_Left ) );

        // right: some xtra widgets for testing

        wRight->setAlign(5,5);
        wRight->appendWidget( wKnob   = new wdgKnob(    this,-99,  axRect( 10, 10, 80,16),wal_None,0 ) );
        wRight->appendWidget( wKnob2  = new wdgKnob(    this,-199, axRect( 10, 30, 80,16),wal_None,0 ) );
        wRight->appendWidget( wKnob3  = new wdgKnob(    this,-299, axRect( 10, 50, 80,16),wal_None,0 ) );
        wRight->appendWidget( slid    = new wdgImgKnob( this,-1,   axRect( 10,100,20,100),wal_None,65,mSrfSlider ));
        slid->mSens1 = 0.01;

        // client area: tabs

        wClient->setAlign(5,5);
        wClient->appendWidget( wTabs  = new wdgTabs(this, -1,axRect(10,10,256,128),wal_Client));

        wTabs->appendPage( wPage1 = new axContainer(this, -1,NULL_RECT,wal_Client) );
        wTabs->appendPage( wPage2 = new axContainer(this, -1,NULL_RECT,wal_Client) );
        wTabs->appendPage( wPage3 = new axContainer(this, -1,NULL_RECT,wal_Client) );

        wPage1->setBackground(true);
        wPage2->setBackground(true);
        wPage3->setBackground(true);

        wPage1->setAlign(10,10,5, 15);
        wPage2->setAlign(10,10,20,20);
        wPage3->setAlign(10,10,5, 5);

        wPage2->setFlag(wfl_Vertical);

        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );
        wPage1->appendWidget( new wdgKnob( this,0,axRect(0,0, 80,16),wal_Stacked,0 ) );

        for (int i=0; i<11; i++) wPage1->mWidgets[i]->doSetValue((float)i*0.1);

        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,1 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,1 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,1 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,1 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,1 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );
        wPage2->appendWidget( new wdgKnob( this,0,axRect( 0, 0,128,32),wal_Stacked,0 ) );

        wPage3->appendWidget( new wdgKnob( this,0,axRect( 50, 50,128,32),wal_None,1 ) );
        wPage3->appendWidget( new wdgKnob( this,0,axRect( 50, 90, 64,32),wal_None,0 ) );
        wPage3->appendWidget( new wdgKnob( this,0,axRect( 130,90, 64,32),wal_None,0 ) );

        wTabs->setPage(0);

        ed->doRealign();

        //recalc_scroller();

        mEditor = ed;
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
        if (mEditor) mEditor->redrawDirty();
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
