#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axWidget.h"

#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------
//
// container
//
//----------------------------------------------------------------------

class myWidget : public axWidget
{
  public:

    myWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //setBorders(10,10,5,5);
        appendWidget( new wdgPanel(this,axRect(0,0,32,32),wa_Top) );
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawPanel(aCanvas,mRect);
        axWidget::doPaint(aCanvas,aRect);
      }

//    virtual int appendWidget(axWidget* aWidget)
//      {
//        wtrace("appendWidget");
//        aWidget->doSetSkin(mSkin,false); // inherit skin
//        int num = wContainer->appendWidget(aWidget);
//        //int num = axContainer::appendWidget(aWidget);
//        return num;
//      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*     mainpanel;
    axWidget*     wdg;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      appendWidget( mainpanel = new wdgPanel(this,NULL_RECT,wa_Client) );
        mainpanel->appendWidget(wdg = new wdgPanel(this,axRect(10,10,320,200), wa_None) );
      //  wdg->appendWidget(new myWidget(this,axRect(10,10,100,100), wa_None) );
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
