#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axContainer.h"

//----------------------------------------------------------------------

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
        aCanvas->selectBrush(0);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        aCanvas->drawImage(mSurface, 10,10, 0,0,100,100);
        //axContainer::doPaint(aCanvas,aRect);
      }
};

//----------------------------------------------------------------------
/*

todo/ideas/thoughts/whatever:

- streamline the #define AX_DEBUG + #include "core/axDebug.h" things?
  just defining AX_DEBUG could be enough?
  or we could find a platform-independent way to see if we're in a debug build,
  to get rid of the needed AX_DEBUG altogether...
  #ifdef __DEBUG__ or something?

- window size is set in two places...
  setupEditor vs doOpenEditor/axEditor constructor

- brush, pen etc attached to canvas, lost when resizing editor/window
  because the surface (and thus the canvas) have to be reconstructor

- change color of pen/brush without creating a new one?
  linux: easy, no real pen, just saving the color, and use it in selectPen() etc
  win32: can we re-color the pen/brush in some way?

- drawing pixels with rgb values directly (create temporary pen, draw, delete,
  or re-color an existing stock one, if possible)

*/
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
        mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT/*AX_WIN_MSGDELETE*/);
        mEditor->appendWidget( wdg = new myWidget(mEditor,axRect(0,0,640,480)) );

        srf = mEditor->createSurface(100,100);
        wdg->mSurface = srf;

        axCanvas* can = srf->getCanvas();
        can->clearPen();
        can->createBrush(AX_YELLOW);
        can->createBrush(AX_RED_DARK);
        can->selectBrush(2);
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

//----------------------------------------------------------------------
AX_ENTRYPOINT(test1)


