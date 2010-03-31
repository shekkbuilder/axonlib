// in project/build options, cor compiler command line, like -DAX_FORMAT_VST)
//#define AX_FORMAT_EXE
//#define AX_FORMAT_VST

#define AX_DEBUG
#include "core/axDebug.h"

#include "axConfig.h"
#include "core/axMath.h"

#include "axPlugin.h"
#include "axEditor.h"

#include "gui/axWidget.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgTest.h"

#include "audio/axGraph.h"
#include "audio/axModule.h"

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    //myEditor*     mEditor;
    axEditor*     mEditor;
    axParameter*  pGain;
    float         mGain;

    myWidget*   mWidget;
    myWidget*   mWidget2;
    myWidget*   mWidget3;
//    axDefSkin*  mSkin;
//    axSkin*     mSkin2;

    axGraph*    mGraph;
    axModule*   mModule;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext,AX_PLUG_DEFAULT)
      {
        wtrace("myPlugin.constructor");

        describe("plug_debug","ccernn","axonlib build.cpp",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        setupEditor(512,384);
        appendParameter( pGain = new axParameter(this,"gain","db") );
        pGain->setValue(mGain); // move to parameter constructor argument?
        setupParameters();

        // test
        mGraph  = new axGraph("graph");
        mModule = new axModule(mGraph,"Module");
          mModule->appendPin( new axPin() );
          mModule->appendPin( new axPin() );
        mGraph->appendModule(mModule);
      }

    //----------

    virtual ~myPlugin()
      {
        //parameters are auto-deleted in axPluginBase destructor
        delete mGraph; // modules auto-deleted in destructor
      }

    //----------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        wtrace("myPlugin.doOpenEditor");
        //mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        wtrace("mEditor = " << mEditor);
        wdgPanel* panel = new wdgPanel(mEditor,axRect(0,0,512,384),wa_None,0);
        mEditor->appendWidget(panel);
        //mEditor->reparent((Window)aContxt->mWindow);
        panel->appendWidget(  mWidget   = new myWidget(mEditor,axRect( 0,  0,200,100),wa_None) );
        mWidget->appendWidget(mWidget2  = new myWidget(mEditor,axRect( 0,  0,100, 30),wa_None) );
        panel->appendWidget(  mWidget3  = new myWidget(mEditor,axRect(250,20,100, 30),wa_None) );

        mWidget->mName  = "red";
        mWidget2->mName = "grey";
        mWidget3->mName = "third";

        mEditor->connect(mWidget, pGain );

          trace("myEditor=" << this);
          trace("mWidget=" << mWidget);
          trace("mWidget2=" << mWidget2);
          trace("mWidget3=" << mWidget3);

        axCanvas* can = mEditor->getCanvas();

        can->createPen(64,64,64, 0);
        can->createPen(128,128,128, 0);
        can->createPen(192,192,192, 0);
        can->createBrush(64,64,64, 0);
        can->createBrush(128,128,128, 0);
        can->createBrush(192,192,192, 0);
        can->createFont("default", 255,255,255, 0, 0);

        wtrace("can = " << can);

//        mSkin  = new axDefSkin(can);
//        mSkin2 = new axSkin(can);
//        mSkin2->appendPen(can->createPen(AX_RED));
//        mSkin2->appendPen(can->createPen(AX_YELLOW));
//        mSkin2->appendPen(can->createPen(AX_GREEN));
//        mSkin2->appendBrush(can->createBrush(AX_RED_DARK));
//        mSkin2->appendBrush(can->createBrush(AX_GREEN_DARK));
//        mSkin2->appendBrush(can->createBrush(AX_YELLOW_DARK));
//        mSkin2->appendFont(can->createFont("default",AX_WHITE));
//        mEditor->setSkin(mSkin,true);
//        mWidget->setSkin(mSkin2);
//        //mWidget2->setSkin(mSkin2);
//        mWidget3->setSkin(mSkin2);

        mEditor->show();

        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
//        delete mSkin;
      }

    //----------

    //virtual void doIdleEditor(void) {}

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        //trace("myPlugin.doSetParameter " << val);
        if (aParameter==pGain) mGain = val;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
//----------------------------------------------------------------------
