#define AX_DEBUG
#include "axDebug.h"
#include "axApplication.h"
#include "axWindow.h"

//----------------------------------------------------------------------

class myWin : public axWindow
{
  public:

    myWin(/*myApp* aListener, */int aID, axRect aRect, int aFlags=0)
    : axWindow("test1_win",this,aID,aRect,aFlags)
      {
      }

    //----------

    virtual ~myWin()
      {
      }

    //--------------------------------------------------

    //virtual void onChange(axWidget* aWidget)
    //  {
    //  }

    //--------------------------------------------------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //  }

    //--------------------------------------------------

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    axWindow::doPaint(aCanvas,aRect);
    //  }

};

//----------------------------------------------------------------------

#define AX_APPLICATION myApp
class myApp : public axApplication
{
  private:
    myWin* win;
  public:
    myApp() : axApplication() {}
    virtual ~myApp() {}
    virtual void main(void)
      {
        axInitialize(AX_WINDOWED);
        win = new myWin(/*this,*/0,axRect(0,0,640,480),(AX_WINDOWED | AX_BUFFERED ));
        win->setTitle("test [linux]");
        win->show();
        win->eventLoop();
        win->hide();
        delete win;
      }
};

//----------------------------------------------------------------------
#include "axMain.h"

