#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  0
//#define AX_AUTOSYNC
#define AX_PAINTERS
#define AX_WIDTH      512
#define AX_HEIGHT     384
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"

//#include "parFloat.h"
//#include "parInteger.h"

//#include "wdgLabel.h"
#include "wdgKnob.h"
#include "wdgTabs.h"
#include "wdgPanel.h"
#include "wdgResizer.h"

#include "wdgImgKnob.h"
#include "axBitmapLoader.h"
#include "images/vslider1_20x100_65h.h" // 46002

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  public:
    bool        is_gui_initialized;
    axEditor    *mEditor;
    wdgKnob     *wKnob;
    wdgTabs     *wTabs;
    axContainer *wPage1,*wPage2,*wPage3;
    wdgResizer  *wSizer;
    wdgPanel    *wLeft, *wRight, *wTop, *wBottom, *wClient;

    axSurface   *mSrfSlider;
    wdgImgKnob  *slid;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        is_gui_initialized = false;
        mSrfSlider = NULL;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("plug_debug","ccernn","product_string",0,0);
      }

    //----------

    virtual ~myPlugin()
      {
        if (mSrfSlider) delete mSrfSlider;
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
        if (id == -99)
        {
          int page = (int) axMin(2,floorf(val*3));
          wTabs->setPage(page);
          mEditor->onChange(wTabs);
        }
        // then back to the default handler
        mEditor->onChange(aWidget);
      }

    //----------

    virtual void onResize(int dX, int dY)   // delta x,y
      {
        //int w = wScope->mRect.w + dX;
        //int h = wScope->mRect.h + dY;
        //wScope->doResize( w,h );
        mEditor->onResize(dX,dY);
      }

    //--------------------------------------------------
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

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("plug_debug_win",this,-1,axRect(0,0,mWidth,mHeight),AX_FLAGS);
        //E->setFlag(wfl_Align);
        if(!is_gui_initialized)
        {
          mSrfSlider = loadPng( vslider1, 46002 );
          is_gui_initialized=true;
        }

        E->setBackground(false);
        E->setFlag(wfl_Align);
        E->setAlign(0,0,0,0);
        E->appendWidget( wLeft   = new wdgPanel( this,-1,axRect(0,0,100,0 ),wal_Left    ) );
        E->appendWidget( wTop    = new wdgPanel( this,-1,axRect(0,0,0,  30),wal_Top     ) );
        E->appendWidget( wBottom = new wdgPanel( this,-1,axRect(0,0,0,  20),wal_Bottom  ) );
        E->appendWidget( wRight  = new wdgPanel( this,-1,axRect(0,0,100,0 ),wal_Right   ) );
        E->appendWidget( wClient = new wdgPanel( this,-1,NULL_RECT,         wal_Client  ) );

        wLeft->appendWidget( slid = new wdgImgKnob(this, -1, axRect(5,5,20,100), wal_None,65,mSrfSlider ));
        slid->mSens1 = 0.01;

        wBottom->setFlag(wfl_Align);
        wBottom->setAlign(5,5,0,0);
        wBottom->appendWidget( wSizer = new wdgResizer( this,-1,axRect(0,0,16,16),wal_Right ) );

        wRight->setFlag(wfl_Align);
        wRight->setAlign(5,5,0,0);
        wRight->appendWidget( wKnob  = new wdgKnob(this,-99,axRect(10,148,80,16),wal_Bottom,0 ) );

        wClient->setFlag(wfl_Align);
        wClient->setAlign(5,5,0,0);
        wClient->appendWidget( wTabs  = new wdgTabs(this, -1,axRect(10,10,256,128),wal_Client));

        wTabs->setFlag(wfl_Align);
        wTabs->setAlign(5,5,0,0);
        wTabs->appendPage( wPage1 = new axContainer(this, -1,NULL_RECT,wal_Client) );
        wTabs->appendPage( wPage2 = new axContainer(this, -1,NULL_RECT,wal_Client) );
        wTabs->appendPage( wPage3 = new axContainer(this, -1,NULL_RECT,wal_Client) );

        wPage1->setBackground(true,axColor(128,120,120));
        wPage2->setBackground(true,axColor(120,128,120));
        wPage3->setBackground(true,axColor(120,120,128));

        wPage1->setFlag(wfl_Vertical);
        wPage1->setFlag(wfl_Align);
        wPage1->setAlign(10,10,20,20);

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

        wPage2->appendWidget( new wdgKnob( this,0,axRect(10, 10,128,32),wal_Stacked,1 ) );
        wPage3->appendWidget( new wdgKnob( this,0,axRect(100,50,128,32),wal_Stacked,1 ) );
        wTabs->setPage(0);

        E->doRealign();
        mEditor = E;
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
        if (mEditor)
          mEditor->redrawDirty();
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
