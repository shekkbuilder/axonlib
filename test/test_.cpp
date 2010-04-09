#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axContainer.h"
#include "gui/axSymbol.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    axContainer* wClient;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      //axCanvas* canvas = getCanvas();
      appendWidget( wClient = new axContainer(this,NULL_RECT,wa_Client) );
      doSetSkin(mDefaultSkin/*,true*/);
      doRealign();
      //startTimer(250);
    }

    //----------

    virtual ~myEditor()
      {
        //stopTimer();
      }

    //----------

    //virtual void doTimer()
    //  {
    //    wtrace("doTimer...");
    //    //invalidate(mRect.x,mRect.y,mRect.w,mRect.h); // redraw everything
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
        setupEditor(500,400);
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
    //    wtrace("...doIdleEditor");
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
