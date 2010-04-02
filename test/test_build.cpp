
#define AX_DEBUG
#include "core/axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"

//#include "gui/axWidget.h"
//#include "wdg/wdgPanel.h"
//#include "wdg/wdgTest.h"
//#include "audio/axGraph.h"
//#include "audio/axModule.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {
        wtrace("myEditor.constructor");
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->selectBrush(2);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        aCanvas->selectPen(0);
        aCanvas->drawCircle(10,10,20,20);
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
    : axPlugin(aContext,AX_PLUG_DEFAULT)
    //: axPlugin(aContext,0/*pf_HasEditor*/)
      {
        wtrace("myPlugin.constructor");
        mEditor = NULL;
        //describe("test_win32","ccernn","axonlib test",0,AX_MAGIC+0x0000);
        //setupAudio(0,0);
        setupEditor(512,384);
      }

    //----------

    //virtual ~myPlugin()
    //  {
    //  }

    //----------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        wtrace("myPlugin.doOpenEditor");
        // #define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGDELETE)
        //mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_MSGDELETE);
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

    //virtual void doIdleEditor(void) {}

    //----------

    //virtual void doSetParameter(axParameter* aParameter)
    //  {
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
//----------------------------------------------------------------------
