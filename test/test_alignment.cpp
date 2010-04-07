#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

// most of this will be moved to axEditor

class myEditor : public axEditor
{
  private:
    axSkinDef*  mSkin;
    axColor     mBackColor;
    wdgPanel*   wPanel1;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      axCanvas* can = getCanvas();
      mBackColor  = can->getColor(AX_GREY);
      setAlignment(10,10,2,2);
      appendWidget( wPanel1 = new wdgPanel(this,axRect(10,10,100,100),wa_Right) );
      appendWidget(           new wdgPanel(this,axRect(10,10,32, 32), wa_Bottom) );
      appendWidget(           new wdgPanel(this,axRect(10,10,32, 32), wa_Left) );
      appendWidget(           new wdgPanel(this,axRect(10,10,32, 32), wa_Top) );
      appendWidget(           new wdgPanel(this,axRect(10,10,32, 32), wa_Client) );
      mSkin = new axSkinDef(can);
      setSkin(mSkin,true);
      doRealign();
      //startTimer(40); // 25 fps
    }

    //----------

    virtual ~myEditor()
      {
        //stopTimer();
      }

    //----------

    // be careful about things here.
    // it's being called from a separate thread,
    // so it can be called anytime!
    // here we just invalidate the entire window,
    // and fore a redraw (from the event handler)

    virtual void doTimer()
      {
        // redraw everything
        invalidate(mRect.x,mRect.y,mRect.w,mRect.h);
        axEditor::doTimer();
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mBackColor);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        axContainer::doPaint(aCanvas,aRect);
      }

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
        describe("test_alignment","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(320,240);
        //setupParameters();
      }

    virtual ~myPlugin()
      {
      }

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
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
