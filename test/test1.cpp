#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"

#include "axPlugin.h"
#include "axEditor.h"

class myWidget : public /*axContainer*/axWidget
{
  public:
    axSurface* mSurface;
  public:
    myWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
    //: axContainer(aListener,aRect,aAlignment)
      {
        mSurface = NULL;
      }
    virtual ~myWidget() {}
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //trace("doPaint");
        aCanvas->selectBrush(0);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        //aCanvas->drawSurface(mSurface, 10,10, 0,0,100,100);
        aCanvas->drawImage(mSurface, 10,10, 0,0,100,100);
        //axContainer::doPaint(aCanvas,aRect);
      }
};

//----------------------------------------------------------------------

class test1 : public axPlugin
{
  private:
    axContext   ctx();
    axEditor*   mEditor;
    myWidget*   wdg;
    axSurface*  srf;

  public:
    test1(axContext* aContext)
    : axPlugin(aContext, AX_PLUG_DEFAULT)
      {
        describe("test1","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(640,480);
        //setupParameters();
      }
    virtual ~test1()
      {
      }

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        //mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_MSGDELETE);

        //axCanvas* canvas = mEditor->getCanvas();
        //canvas->createBrush(AX_GREY);

        mEditor->appendWidget( wdg = new myWidget(mEditor,axRect(0,0,640,480)) );

        srf = mEditor->createSurface(100,100);
        wdg->mSurface = srf;

        axCanvas* can = srf->getCanvas();
        trace("can="<<can);

        //can->createBrush(AX_BLACK);
        can->clearPen();
        can->createBrush(AX_YELLOW);
        can->selectBrush(0);
        can->fillRect(0,0,100,100);
        can->selectBrush(1);
        can->fillCircle(0,0,100,100);

        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {

        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
        delete srf;
      }

    //----------

};

AX_ENTRYPOINT(test1)


