#define AX_DEBUG
#include "axDebug.h"

#include "axApplication.h"
#include "axWindow.h"
#include "wdgPanel.h"
#include "wdgImage.h"
#include "wdgImgSwitch.h"
#include "wdgTextBox.h"

#include "axBitmap.h"
#include "axSurface.h"

#include "axBitmapLoader.h"

//----------------------------------------------------------------------

class myWin : public axWindow
{
  private:
    axBitmap      *bmp, *bmp2;
    axSurface     *srf, *srf2;
    wdgPanel      *panel;
    wdgImgSwitch  *sw1;
    wdgImage      *im1;
    wdgTextBox    *txt;

    int counter;

  public:

    myWin(int aID, axRect aRect, int aFlags=0)
    : axWindow("test1_win",this,aID,aRect,aFlags)
      {
        counter = 0;

        panel = new wdgPanel(this,-1,NULL_RECT,wal_Client);
        appendWidget(panel);

        unsigned char* buffer = NULL;
        int size = loadFile((char*)"/home/cern/Pictures/cern256.png",&buffer);
        axBitmap* bmp = decodePng(buffer,size);
        free(buffer); buffer=NULL;

        size = loadFile((char*)"/home/cern/Pictures/testbutton.png",&buffer);
        axBitmap* bmp2 = decodePng(buffer,size);
        free(buffer); buffer=NULL;

        bmp->convertRgbaBgra();
        bmp->setBackground(128,128,128);
        bmp->prepare();
        srf = uploadBitmap(bmp);

        bmp2->convertRgbaBgra();
        //bmp2->setBackground(128,128,128);
        bmp2->setBackground(bmp,axRect(30,30,30,30));
        bmp2->prepare();
        srf2 = uploadBitmap(bmp2);

        delete bmp;
        delete bmp2;

        //----------

        panel->appendWidget( im1 = new wdgImage(    this,-1,axRect(10, 10,256,256),wal_None,  srf) );
        panel->appendWidget( sw1 = new wdgImgSwitch(this, 0,axRect(40, 40,30, 30 ),wal_None,  0, srf2) );
        panel->appendWidget( txt = new wdgTextBox(  this, 1,axRect(300,10,150,256),wal_None   ) );
        doRealign();
      }

    //----------

    virtual ~myWin()
      {
        delete srf;
        delete srf2;
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget->mID==0) // button
        {
          char* buf = new char[32];
          sprintf(buf,"click #%i",counter);
          counter++;
          txt->appendText(buf);

//          txt->unscroll();
//          txt->appendText(buf);
//          txt->doRealign();
//          redrawWidget(txt);
        }
        redrawWidget(aWidget);
      }

    //--------------------------------------------------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //  }

    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->blit( srf->getCanvas(),  100, 50, 0,0, 256,256);
        axWindow::doPaint(aCanvas,aRect);
        //aCanvas->blit( srf2->getCanvas(), 130, 80, 0,0, 90,30 );
      }

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
        win->setTitle("test4");
        win->show();
        win->eventLoop();
        win->hide();
        delete win;
      }
};

//----------------------------------------------------------------------
#include "axMain.h"
