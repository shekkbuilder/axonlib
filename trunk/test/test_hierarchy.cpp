#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axWidget.h"

#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*     panel;
    axWidget*     wdg;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      appendWidget( panel = new wdgPanel(this,NULL_RECT,wa_Client) );

        panel->setBorders(30,30,10,10);


        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,64,64), wa_LeftTop) );
        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,64,64), wa_TopLeft) );

        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,10,10), wa_Top) );
        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,10,10), wa_Left) );
        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,10,10), wa_Right) );
        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,10,10), wa_Bottom) );

        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,64,64), wa_LeftTop) );
        panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,64,64), wa_RightTop) );

        //panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,80,80), wa_RightBottom) );
        //panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,20,20), wa_LeftTop) );
        //panel->appendWidget(wdg = new wdgPanel(this,axRect(0,0,40,40), wa_RightTop) );

        //

        for (int i=0;i<32; i++)
          panel->appendWidget(wdg = new wdgPanel(this,axRect(10,10,32,32+i*2), wa_StackedVert) );

      setSkin(mDefaultSkin,true);
      doRealign();
      //startTimer(250);
    }

    //----------

    //virtual ~myEditor()
    //  {
    //    //stopTimer();
    //  }

    //----------

    //virtual void doTimer()
    //  {
    //    //invalidate(mRect.x,mRect.y,mRect.w,mRect.h);
    //  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    myEditor* mEditor;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, AX_PLUG_DEFAULT)
      {
        describe("test_","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(640,480);
        //setupParameters();
      }

    //----------

    //virtual ~myPlugin() {}

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //----------

    //virtual void doIdleEditor()
    //  {
    //    wtrace("...doIdleEditor");
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
