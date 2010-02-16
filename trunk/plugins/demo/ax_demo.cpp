#define AX_DEBUG
#include "axDebug.h"

//----------

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  0


#include "axPlugin.h"
#include "parFloat.h"

//----------

#define AX_WIDTH      600
#define AX_HEIGHT     400
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#include "axEditor.h"
#include "wdgPanel.h"
#include "wdgResizer.h"
#include "wdgKnob.h"
#include "wdgButton.h"

#include "axBitmapLoader.h"
#include "images/fibonacci.h"

#include "axHost.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
    axEditor*   mEditor;
    wdgResizer* wSplit;
    bool        mGuiInitialized;
  //float       mValue;
  //parFloat*   pValue;
  //wdgKnob*    wValue;

    wdgPanel*   wTop;
    wdgPanel*   wBottom;
    wdgPanel*   wLeft;
    wdgPanel*   wMain;
    wdgResizer* wResizer;

    axBrush*    mLeftBrush;
    axBrush*    mSizerBrush;
    axSurface*  mBackSrf;

  public:

    //--------------------------------------------------
    // con/de-structor
    //--------------------------------------------------

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        //trace("- ax_demo.constructor");
        mEditor = NULL;
        mGuiInitialized = false;
        describe("ax_demo","ccernn","product_string",0,0);
        setupAudio(2,2,false);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        //appendParameter(pValue=new parFloat(this,0,"value","",0.5));
        //processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        if (mGuiInitialized)
        {
          delete mLeftBrush;
          delete mSizerBrush;
          delete mBackSrf;
        }
      }

    //--------------------------------------------------
    // events
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

    //virtual void doProcessParameter(axParameter* aParameter)
    //  {
    //    int  id = aParameter->mID;
    //    float f = aParameter->getValue();
    //    switch(id)
    //    {
    //      case 0: mValue = f;  break;
    //    }
    //  }

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


  public:

    virtual void* doCreateEditor(void)
      {
        //move to ax_demo_gui.h
        //  init_bitmaps
        //  init_left
        //  init_top
        //  init_bottom
        //  init_main

        //  init_tab_1
        //  init_tab_2
        //  init_tab_3
        //  init_tab_4
        //  init_tab_5

        if (!mGuiInitialized)
        {
          mLeftBrush = new axBrush(axColor(112,112,112));
          mSizerBrush = new axBrush(AX_GREY_DARK);
          axBitmap* bmp = decodePng((unsigned char*)fibonacci,fibonacci_size);
          bmp->convertRgbaBgra(); // swap rgb <-> bgr
          bmp->setBackground(128,128,128); // replace background (w/alpha)
          bmp->prepare(); // prepare for blitting
          mBackSrf = uploadBitmap(bmp); // blit bitmap to surface
          delete bmp;
          mGuiInitialized = true;
        }

        axEditor* EDIT = new axEditor("ax_demo_editor",this,0,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);

        EDIT->appendWidget( wBottom = new wdgPanel(  this,-1,axRect(0,0,0,20),wal_Bottom) );
          wBottom->setDrawMode(pdm_Background);

        EDIT->appendWidget( wLeft   = new wdgPanel(  this,-2,axRect(0,0,150,0),wal_Left) );
          wLeft->setDrawMode(  pdm_Border|pdm_Background);
          wLeft->setBackgroundBrush( mLeftBrush );
          wLeft->setMinWidth(100);
          wLeft->setMaxWidth(500);

        EDIT->appendWidget( wSplit  = new wdgResizer(this,100,axRect(0,0,5,0),wal_Left) );
          wSplit->setDrawMode(pdm_Background);
          wSplit->setBackgroundBrush(mSizerBrush);

        EDIT->appendWidget( wTop    = new wdgPanel(  this,-3,axRect(0,0,0,50),wal_Top) );
          wTop->setDrawMode(pdm_Border|pdm_Background);
          //wTop->setBackgroundBrush( mBackBrush );
          wTop->setAlign(10,10,5,5);
          wTop->appendWidget( new wdgButton(this,101,axRect(0,0,30,30),wal_LeftTop, "1" ) );
          wTop->appendWidget( new wdgButton(this,102,axRect(0,0,30,30),wal_LeftTop, "2" ) );
          wTop->appendWidget( new wdgButton(this,103,axRect(0,0,30,30),wal_LeftTop, "3" ) );
          wTop->appendWidget( new wdgButton(this,104,axRect(0,0,30,30),wal_LeftTop, "4" ) );
          wTop->appendWidget( new wdgButton(this,105,axRect(0,0,30,30),wal_LeftTop, "5" ) );

        EDIT->appendWidget( wMain   = new wdgPanel(  this,-4,axRect(0,0,0,0),wal_Client) );
          wMain->setDrawMode( pdm_Image );
          wMain->setImage( mBackSrf );
          //wMain->setBackgroundBrush( mMainBrush );

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
        if (mEditor) mEditor->redrawDirty();
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
        //trace("id: " << aWidget->mID);
        //int id = aWidget->mID;
        //if (id>=101 && id<=105)
        //{
        //  trace("button " << id-100);
        //}
        if (mEditor) mEditor->onChange(aWidget);
      }

    //----------

    virtual void onResize(axWidget* aWidget,int dX, int dY)   // delta x,y
      {
//        if (aWidget->mID == 1) // wdgSplitter, window resizer
//        {
//          int w = mEditor->getRect().w + dX;
//          int h = mEditor->getRect().h + dY;
//          mEditor->resizeWindow(w,h);
//          //recalc_scroller();
//          //mEditor->onChange(scr);
//        }
        if (aWidget->mID==100) // wdgSplitter, left splitter
        {
          int w = wLeft->getRect().w + dX;
          int h = wLeft->getRect().h;// + dY;
          wLeft->doResize(w,h);
          mEditor->doRealign();
          //recalc_scroller();
          mEditor->redraw();
        }
        //if (aWidget->mID == 1000) // group boxes
        //{
        //  wLeft->doRealign();
        //  //recalc_scroller();
        //  mEditor->onChange(wLeft);
        //}
      }



};

//----------------------------------------------------------------------
#include "axMain.h"

