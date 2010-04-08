#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axContainer.h"
#include "gui/axSymbol.h"

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class myWidget : public axContainer
{
  public:
    myWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axContainer(aListener,aRect,aAlignment)
      {
      }
    virtual void doEnter(axWidget* aWidget) { /*wtrace("doEnter");*/ onCursor(cu_Hand); }
    virtual void doLeave(axWidget* aWidget) { /*wtrace("doLeave");*/ onCursor(DEF_CURSOR); }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

// most of this will be moved to axEditor

char* symbol1 = (char*)".................OOOOOO..oOOOOo...OOOO....oOOo.....OO...........";
char* symbol2 = (char*)"o.o.o.o..o.o.o.oo.o.o.o..o.o.o.oo.o.o.o..o.o.o.oo.o.o.o..o.o.o.o";
char* symbol3 = (char*)".........OOOOOO..OOOOOO..OOOOOO..OOOOOO..OOOOOO..OOOOOO.........";

class myEditor : public axEditor
{
  private:
    axSkinDefault*  mSkin;
    axColor         mBackColor;
    axColor         mPenColor;
    axColor         mSym1Color;
    axColor         mSym2Color;
    axContainer*    wClient;
    axSurface*      mSymSurf;
    axSymbols*      mSymbols;
    int sym;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      axCanvas* can = getCanvas();
      mBackColor  = can->getColor(AX_GREY);
      mPenColor   = can->getColor(AX_GREY_DARK);
      mSym1Color  = can->getColor(AX_WHITE);
      mSym2Color  = can->getColor(AX_GREY_LIGHT);
      setAlignment(30,30,10,10);
      appendWidget(           new axContainer(this,axRect( 0, 0, 100,0  ), wa_Right) );
      appendWidget(           new axContainer(this,axRect( 0, 0, 0,  50 ), wa_Bottom) );
      appendWidget(           new axContainer(this,axRect( 0, 0, 50, 0  ), wa_Left) );
      appendWidget(           new axContainer(this,axRect( 0, 0, 0,  50 ), wa_Top) );
      appendWidget( wClient = new axContainer(this,NULL_RECT,              wa_Client) );
      wClient->setAlignment(20,20,10,10);
      wClient->setFlag(wf_Clip);
      for (int i=0; i<20; i++)
      {
        myWidget* w = new myWidget(this,axRect(0,0, 32,32), wa_Stacked);
        //w->setOption(wo_Bevel);
        wClient->appendWidget(w);
      }
      mSkin = new axSkinDefault(can);
      doSetSkin(mSkin,true);
      doRealign();
      //startTimer(40); // 25 fps

      mSymSurf = new axSurface(aContext,256,256);
      can = mSymSurf->getCanvas();
      can->setBrushColor(mBackColor);
      can->fillRect(0,0,255,255);
      mSymbols = new axSymbols(mSymSurf);
      sym = mSymbols->create(symbol1,axRect( 0,0,8,8),mSym1Color,mSym2Color);
      sym = mSymbols->create(symbol2,axRect( 8,0,8,8),mSym1Color,mSym2Color);
      sym = mSymbols->create(symbol3,axRect(16,0,8,8),mSym1Color,mSym2Color);

    }

    //----------

    virtual ~myEditor()
      {
        //stopTimer();
        delete mSymbols;
        delete mSymSurf;
      }

    //----------

    // be careful about things here. it's being called from a separate thread, so it can be called anytime!
    // here we just invalidate the entire window, to force a redraw
    // if you need more advanced things, or need to write to variables that might currently be in use,
    // you might need to use mutexes or other thread synchronization methods.

    //virtual void doTimer()
    //  {
    //    invalidate(mRect.x,mRect.y,mRect.w,mRect.h); // redraw everything
    //    axEditor::doTimer();
    //  }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mBackColor);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        axEditor::doPaint(aCanvas,aRect);
        mSymbols->draw(aCanvas,5,5, 0);
        mSymbols->draw(aCanvas,15,5,1);
        mSymbols->draw(aCanvas,25,5,2);
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
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
