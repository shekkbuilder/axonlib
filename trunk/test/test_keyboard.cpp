
#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgOctave.h"


//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*     main_panel;
    wdgOctave*  octave1;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      axCanvas* aCanvas = getCanvas();
      appendWidget( main_panel = new wdgPanel(this,NULL_RECT,wa_Client) );
        main_panel->appendWidget( octave1 = new wdgOctave(this,axRect(0,0,200,60),wa_Client) );
          octave1->setDist(2);
          //octave1->activate(0);
          //octave1->activate(3);
          //octave1->activate(7);
          octave1->setColors( aCanvas->getColor(255,255,255),
                              aCanvas->getColor(  0,  0,  0),
                              aCanvas->getColor(192,192,192),
                              aCanvas->getColor( 96, 96, 96)
          );
      setSkin(mDefaultSkin,true);
      doRealign();
      octave1->recalc();
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
        setupEditor(640,480);
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
