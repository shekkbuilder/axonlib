#define AX_DEBUG
#include "axDebug.h"

#include "axApplication.h"
#include "axWindow.h"
#include "axBitmap.h"
#include "axSurface.h"

#include "axBitmapLoader.h"

#include "wdgPanel.h"
#include "wdgImage.h"
#include "wdgImgSwitch.h"
#include "wdgTextBox.h"

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class myWin : public axWindow
{
  private:
    axBitmap      *bmp, *bmp2, *bmp3;
    axSurface     *srf, *srf2, *srf3;
    wdgPanel      *panel;
    wdgImgSwitch  *sw1,*sw2;
    wdgImage      *im1;
    wdgTextBox    *txt;

    int counter;
    axStrings textboxstrings;

  public:

    myWin(int aID, axRect aRect, int aFlags=0)
    : axWindow("test1_win",this,aID,aRect,aFlags)
      {
        panel = new wdgPanel(this,-1,NULL_RECT,wal_Client);
        appendWidget(panel);
        unsigned char* buffer = NULL;

          int size = loadFile((char*)"/home/cern/Pictures/cern256.png",&buffer); // 256x256
          axBitmap* bmp = decodePng(buffer,size);
          free(buffer);
          buffer=NULL;

          bmp->convertRgbaBgra(); // swap rgb <-> bgr
          bmp->setBackground(128,128,128); // replace background (w/alpha)
          bmp->prepare(); // prepare for blitting
          srf = uploadBitmap(bmp); // blit bitmap to surface

          //

          size = loadFile((char*)"/home/cern/Pictures/testbutton.png",&buffer); // 90*30 (like reaper themes), w/alpha
          axBitmap* bmp2 = decodePng(buffer,size);
          free(buffer);
          buffer=NULL;

          bmp2->convertRgbaBgra();
          bmp3 = new axBitmap(bmp2->getWidth(),bmp2->getHeight(),(unsigned char*)bmp2->getBuffer());

          bmp2->setBackground(bmp,axRect(30,30,30,30));  // replace background (w/alpha)
          bmp2->prepare();

          bmp3->setBackground(bmp,axRect(60,30,30,30));  // replace background (w/alpha)
          bmp3->prepare();

          srf2 = uploadBitmap(bmp2);
          srf3 = uploadBitmap(bmp3);

        delete bmp;
        delete bmp2;

        //----------

        counter = 0;

        panel->appendWidget( im1 = new wdgImage(    this,-1,axRect(10, 10,256,256),wal_None,  srf) );
        panel->appendWidget( sw1 = new wdgImgSwitch(this, 0,axRect(40, 40,30, 30 ),wal_None,  0, srf2) );
        panel->appendWidget( sw2 = new wdgImgSwitch(this, 1,axRect(70, 40,30, 30 ),wal_None,  0, srf3) );
        panel->appendWidget( txt = new wdgTextBox(  this, 2,axRect(200,50,150,256),wal_None   ) );

        wdgPanel *p = new wdgPanel(this,-1,NULL_RECT,wal_Client);
        p->setDrawMode(pdm_Border|pdm_Background);
        p->setBackgroundBrush(new axBrush(axColor(112,112,112)));
        p->setAlign(5,5);
        p->setFlag(wfl_Clip);
        txt->setContainer(p);

        doRealign();
      }

    //----------

    virtual ~myWin()
      {
        delete srf;
        delete srf2;
        // delete all strings we created by mouse-clicking
        for (int i=0; i<textboxstrings.size(); i++) delete[] textboxstrings[i].ptr();
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        char* buf;
        int i;
        int id = aWidget->mID;
        switch (id)
        {
          case 0:
            buf = new char[32];
            sprintf(buf,"click #%i",counter);
            counter++;
            txt->appendText(buf);
            textboxstrings.append(buf);
            break;
          case 1:
            txt->clearText();
            //counter = 0;
            for (i=0; i<textboxstrings.size(); i++)
            {
              delete[] textboxstrings[i].ptr();
              textboxstrings.clear();
            }
            break;
        }
        redrawWidget(aWidget);
      }

    //virtual void onHover(axWidget* aWidget)
    //  {
    //    trace("hover: " << aWidget->mID );
    //  }

    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->blit( srf->getCanvas(), 100,50, 0,0, 256,256);
        axWindow::doPaint(aCanvas,aRect);
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
        win->setTitle("test4");
        win->show();
        win->eventLoop();
        win->hide();
        delete win;
      }
};

//----------------------------------------------------------------------
#include "axMain.h"
