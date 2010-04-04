#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axPlugin.h"
#include "axEditor.h"
//#include "gui/axBitmap.h"

//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    axColor mBackColor;
    axColor mPenColor;

  public:
    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      axCanvas* can = getCanvas();
      mBackColor = can->getColor(AX_GREY_LIGHT);
      mPenColor  = can->getColor(AX_GREY_DARK);
      startTimer(40);
    }
    virtual ~myEditor()
      {
        stopTimer();
      }
    virtual void onTimer()
      {
        //wtrace("onTimer");
        invalidate(mRect.x,mRect.y,mRect.w,mRect.h);
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //wtrace("doPaint");
        aCanvas->setBrushColor(mBackColor);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        //
        aCanvas->setPenColor(mPenColor);
        int num1 = mRect.w * mRect.h; // total number of pixels
        int num2 = num1 / 256;
        int x1 = mRect.w / 2;
        int y1 = mRect.h / 2;
        for (int i=0; i<num2; i++)
        {
          int x2 = axRandomInt(mRect.x2());
          int y2 = axRandomInt(mRect.y2());
          //aCanvas->drawPoint(x,y);
          aCanvas->drawLine(x1,y1,x2,y2);
          x1 = x2;
          y1 = y2;
        }
      }

};

//----------------------------------------------------------------------

class test : public axPlugin
{
  private:
    myEditor* mEditor;

  public:

    test(axContext* aContext)
    : axPlugin(aContext, AX_PLUG_DEFAULT)
      {
        describe("test_timer","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(256,256);
        //setupParameters();
      }

    virtual ~test()
      {
      }

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT/*AX_WIN_MSGDELETE*/);
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
AX_ENTRYPOINT(test)
