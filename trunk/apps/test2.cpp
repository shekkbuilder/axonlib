#define AX_APPLICATION  myApp
#define AX_FLAGS (AX_WINDOWED|AX_BUFFERED)
#define AX_DEBUG

#include "axDebug.h"
#include "axApplication.h"

#include "axWindow.h"
#include "axPen.h"

//----------------------------------------------------------------------

class myWin : public axWindow
{
  private:
    axPen* pen_white;
    axPen* pen_yellow;
  public:
    myWin(axString aWinName, axWidgetListener* aListener, int aID, axRect aRect, int aWinFlags=0)
    : axWindow(aWinName, aListener,aID,aRect,aWinFlags)
      {
        pen_white  = new axPen(AX_WHITE,1,0);
        pen_yellow = new axPen(AX_YELLOW,5,0);
      }
    virtual ~myWin()
      {
        delete pen_white;
        delete pen_yellow;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWindow::doPaint(aCanvas,aRect);
        aCanvas->selectPen(pen_white);
        aCanvas->drawLine(10,10,100,100);
        aCanvas->selectPen(pen_yellow);
        aCanvas->drawLine(100,30,20,150);
      }
};

//----------------------------------------------------------------------

class myApp : public axApplication,
              public axWidgetListener
{
  private:
    myWin* win;
  public:

    myApp()
    : axApplication()
      {
        axInitialize(AX_FLAGS);
      }
    virtual ~myApp()
      {
        axInitialize(AX_FLAGS);
      }
    virtual void main(void)
      {
        win = new myWin("my_window",this,0,axRect(0,0,640,480),AX_FLAGS);
        win->show();
        win->eventLoop();
        win->hide();
        delete win;
      }
};

//----------------------------------------------------------------------
#include "axMain.h"
