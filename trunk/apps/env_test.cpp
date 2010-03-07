#define AX_NODIRTYWIDGETS

#define AX_DEBUG
#include "axDebug.h"

#include "axApplication.h"
#include "axWindow.h"
#include "wdgPanel.h"
#include "wdgKnob.h"

#include "dspEnvelope.h"

//#include "wdgImage.h"
//#include "wdgImgSwitch.h"
//#include "wdgTextBox.h"

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class myWin : public axWindow
{
  private:
    wdgPanel  *panel;
    axPen     *wpen;//,*rpen,*gpen;
    wdgKnob   *k1,*k2,*k3,*k4;
  private:
    float a,d,s,r;
    dspEnvExpADSR env;
  public:
    myWin(int aID, axRect aRect, int aFlags=0)
    : axWindow("test1_win",this,aID,aRect,aFlags)
      {
        a = 0.5;
        d = 0.5;
        s = 0.5;
        r = 0.5;
        env.setADSR(a,d,s,r);
        panel = new wdgPanel(this,-1,NULL_RECT,wal_Client);
        appendWidget(panel);
        k1 = new wdgKnob( this,0,axRect(  0,0,128,32),wal_None );
        k2 = new wdgKnob( this,1,axRect(128,0,128,32),wal_None );
        k3 = new wdgKnob( this,2,axRect(256,0,128,32),wal_None );
        k4 = new wdgKnob( this,3,axRect(384,0,128,32),wal_None );
        k1->doSetValue(a);
        k2->doSetValue(d);
        k3->doSetValue(s);
        k4->doSetValue(r);
        panel->appendWidget( k1 );
        panel->appendWidget( k2 );
        panel->appendWidget( k3 );
        panel->appendWidget( k4 );
        doRealign();
        wpen = new axPen(AX_WHITE);
        //rpen = new axPen(AX_RED);
        //gpen = new axPen(AX_GREEN);
      }
    //virtual ~myWin()
    //  {
    //  }
    virtual void onChange(axWidget* aWidget)
      {
        int id = aWidget->mID;
        float v = aWidget->doGetValue();
        switch(id)
        {
          case 0 : a=v; /*env.setStage(env_Attack, 1,a);*/ break;
          case 1 : d=v; /*env.setStage(env_Decay,  s,d);*/ break;
          case 2 : s=v; /*env.setStage(env_Sustain,s,1);*/ break;
          case 3 : r=v; /*env.setStage(env_Release,0,r);*/ break;
        }
        env.setADSR(a,d,s,r);
        //axWindow::onChange(aWidget);  // redraw knob
        //axWindow::onChange(panel);    // redraw envelope
        axWindow::onRedraw(panel);
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int x;
        float v;
        axWindow::doPaint(aCanvas,aRect);
        aCanvas->selectPen(wpen);
        env.noteOn();
        for (x=0; x<480; x++)
        {
          //v = env.getValue();
          v = env.process();
          aCanvas->drawPoint(x,480-v*400);
        }
        env.noteOff();
        for (x=480; x<640; x++)
        {
          //s += (0-s)*rel;
          //aCanvas->drawPoint(x,480-s*400);
          v = env.process();
          aCanvas->drawPoint(x,480-v*400);
        }
      }

};

//----------------------------------------------------------------------
//
// application
//
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
        win->setTitle("env_test");
        win->show();
        win->eventLoop();
        win->hide();
        delete win;
      }
};

//----------------------------------------------------------------------
#include "axMain.h"
