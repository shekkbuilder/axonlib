
//----------

#define AX_PLUGIN     myEffect
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  0
#define AX_WIDTH      600
#define AX_HEIGHT     400
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"
#include "wdgPanel.h"
#include "wdgResizer.h"
#include "wdgKnob.h"
#include "wdgButton.h"

#include "axBitmapLoader.h"
#include "images/fibonacci.h"

#include "axHost.h"

//----------------------------------------------------------------------

class myEffect : public axEffect
{
  private:
    bool        mGuiInitialized;

    wdgResizer  *wSplit;
    wdgResizer  *wResizer;
    wdgPanel    *wTop, *wBottom, *wLeft, *wMain;

    axBrush*    mLeftBrush;
    axBrush*    mSizerBrush;
    axSurface*  mBackSrf;

  public:

    myEffect(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mGuiInitialized = false;
        describe("ax_demo","ccernn","axonlib demo",0,AX_MAGIC);
        setupAudio(2,2,false);
        setupEditor(AX_WIDTH,AX_HEIGHT);
      }

    virtual ~myEffect()
      {
        if (mGuiInitialized)
        {
          delete mLeftBrush;
          delete mSizerBrush;
          delete mBackSrf;
        }
      }

    //--------------------------------------------------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        if (!mGuiInitialized)
        {
          mLeftBrush = new axBrush(axColor(112,112,112));
          mSizerBrush = new axBrush(AX_GREY_DARK);
          axBitmap* bmp = decodePng((unsigned char*)fibonacci,fibonacci_size);
            bmp->convertRgbaBgra();           // swap rgb <-> bgr
            bmp->setBackground(128,128,128);  // replace background (w/alpha)
            bmp->prepare();                   // prepare for blitting
          mBackSrf = uploadBitmap(bmp);       // blit bitmap to surface
          delete bmp;
          mGuiInitialized = true;
        }

        /*
        mID:
          1-4     main panels
          10-11   sizers
          100-104 buttons
        */

        aEditor->setMinWidth(200);
        aEditor->setMaxWidth(800);
        aEditor->setMinHeight(200);
        aEditor->setMaxHeight(600);

        // --- bottom panel (status) ---

        aEditor->appendWidget( wBottom = new wdgPanel(  this,1,axRect(0,0,0,20),wal_Bottom) );
          wBottom->setDrawMode(pdm_Background);

          // window resizer
          wBottom->setAlign(4,4);
          wBottom->appendWidget( wResizer = new wdgResizer(this,10,axRect(0,0,10,10),wal_RightBottom ) );
          wResizer->setDrawMode(pdm_Background);
          wResizer->setBackgroundBrush(mSizerBrush);


        // --- left

        aEditor->appendWidget( wLeft   = new wdgPanel(  this,2,axRect(0,0,150,0),wal_Left) );
          wLeft->setDrawMode(  pdm_Border|pdm_Background);
          wLeft->setBackgroundBrush( mLeftBrush );
          wLeft->setMinWidth(100);
          wLeft->setMaxWidth(500);

        // left/right slitter
        aEditor->appendWidget( wSplit  = new wdgResizer(this,11,axRect(0,0,5,0),wal_Left) );
          wSplit->setDrawMode(pdm_Background);
          wSplit->setBackgroundBrush(mSizerBrush);

        // --- top ---

        aEditor->appendWidget( wTop    = new wdgPanel(  this,3,axRect(0,0,0,50),wal_Top) );
          wTop->setDrawMode(pdm_Border|pdm_Background);
          //wTop->setBackgroundBrush( mBackBrush );
          wTop->setAlign(10,10,5,5);
          wTop->appendWidget( new wdgButton(this,100,axRect(0,0,30,30),wal_LeftTop, "1" ) );
          wTop->appendWidget( new wdgButton(this,101,axRect(0,0,30,30),wal_LeftTop, "2" ) );
          wTop->appendWidget( new wdgButton(this,102,axRect(0,0,30,30),wal_LeftTop, "3" ) );
          wTop->appendWidget( new wdgButton(this,103,axRect(0,0,30,30),wal_LeftTop, "4" ) );
          wTop->appendWidget( new wdgButton(this,104,axRect(0,0,30,30),wal_LeftTop, "5" ) );

        // --- main ---

        aEditor->appendWidget( wMain   = new wdgPanel(  this,4,axRect(0,0,0,0),wal_Client) );
          wMain->setDrawMode( pdm_Image );
          wMain->setImage( mBackSrf );
          //wMain->setBackgroundBrush( mMainBrush );

        aEditor->doRealign();
      }

    //----------

    virtual void onResize(axWidget* aWidget,int dX, int dY)   // delta x,y
      {
        if (aWidget==wResizer) // window resizer
        {
          int w = mEditor->getRect().w + dX;
          int h = mEditor->getRect().h + dY;
          mEditor->resizeWindow(w,h);
          //recalc_scroller();
          //mEditor->onChange(scr);
        }
        if (aWidget==wSplit) // left/right splitter
        {
          int w = wLeft->getRect().w + dX;
          int h = wLeft->getRect().h;// + dY;
          wLeft->doResize(w,h);
          mEditor->doRealign();
          //recalc_scroller();
          mEditor->redraw();
        }
        //if (aWidget->mID == 1000) // group boxes
        //{
        //  wLeft->doRealign();
        //  //recalc_scroller();
        //  mEditor->onChange(wLeft);
        //}
      }



};

//----------------------------------------------------------------------
#include "axMain.h"

