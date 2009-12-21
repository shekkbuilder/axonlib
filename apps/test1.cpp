#define AX_DEBUG
#include "axDebug.h"
#include "axApplication.h"
#include "axWindow.h"
#include "wdgKnob.h"
#include "wdgLabel.h"
#include "wdgImgKnob.h"
#include "wdgImgSwitch.h"

#include "axSurface.h"
#include "axBitmapLoader.h"
#include "images/testknob.h"
#include "images/testbutton.h"

#include "dspBuffer.h"


//
#define AX_APPLICATION myApp
class myApp : public axApplication,
              public axWidgetListener
{
  private:
    axWindow*     win;
    wdgKnob*      knob1;
    wdgImgKnob*   knob2;
    wdgImgSwitch* butt1;
    wdgImgSwitch* butt2;
    wdgLabel*     label;
    axSurface*    knobsrf;
    axSurface*    buttsrf;
    char          buf[16];

  public:
    dspBuffer*    buffer;

  public:
    myApp() : axApplication() {}
    virtual ~myApp() {}
    virtual void main(void)
      {
        axInitialize(AX_WINDOWED);
        //win = new myWindow("myApp_window",NULL,-1,axRect(0,0,256,256),(AX_WINDOWED | AX_BUFFERED ));
        win = new axWindow("myApp_window",NULL,-1,axRect(0,0,256,256),(AX_WINDOWED | AX_BUFFERED ));
        //
        //TODO: new wdgImgSwitch, etc, axBitmap as parameter, not axSurface
        // convert to surface in constructor
        // and. loadpng returns bitmap
        knobsrf = loadPng(testknob,  16329);
        buttsrf = loadPng(testbutton,10935);
        win->appendWidget( knob1  = new wdgKnob(     win, 0,axRect(10, 10,128,32),wal_None,NULL) );
        win->appendWidget( knob2  = new wdgImgKnob(  this,1,axRect(10, 50, 32,32),wal_None,NULL,65,knobsrf) );
        win->appendWidget( label  = new wdgLabel(    win, 2,axRect(10, 90, 64,16),wal_None,NULL, "", AX_YELLOW, tal_Left ) );
        win->appendWidget( butt1  = new wdgImgSwitch(this,3,axRect(50, 50, 30,30),wal_None,NULL,0,buttsrf ) );
        win->appendWidget( butt2  = new wdgImgSwitch(this,4,axRect(80, 50, 30,30),wal_None,NULL,1,buttsrf ) );
        //label->setBackground(true,AX_GREY_DARK);
        //
        win->setTitle("test [linux]");
        win->show();
        win->eventLoop();
        //
        win->hide();
        delete win;
        delete knobsrf;
        delete buttsrf;
      }
    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget->mID==1)
        {
          float n = aWidget->doGetValue();//mValue;
          sprintf(buf, "%.2f",n);
          //TRACE("buf:%s\n",buf);
          label->setText(buf);
          win->onRedraw(label);
          win->sync();//flush();  // ?????
        }
        win->onChange(aWidget);
      }
};

//----------------------------------------------------------------------
#include "axMain.h"

