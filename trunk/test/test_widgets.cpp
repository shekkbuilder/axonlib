
#define AX_DEBUG

#include "core/axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgLabel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgSlider.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  friend class myPlugin;
  private:
    wdgPanel*   wClient;
    wdgSizer*   wSizer;
    wdgPanel*   wLeft;
    wdgPanel*   wCenter;
    wdgPanel*   wRight;
    char        label_buf[256][8];
    axBitmap*   mBitmap;
  protected:
    int       mNumTimer;
    char      timerText[32];

    wdgLabel *wNumTimer;
    wdgLabel *wNumIdle;
    wdgLabel *wNumBlock;

  public:

    myEditor(axPlugin* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {
        mNumTimer = 0;

//        int x,y;
//        char* buffer;
//        mBitmap = createBitmap(100,100);
//        buffer = mBitmap->createBuffer();
//        for (x=0; x<100; x++)
//        {
//          for (y=0; y<100; y++)
//          {
//            *buffer++ = (int)((float)x * 2.55);
//            *buffer++ = (int)((float)y * 2.55);
//            *buffer++ = 0;
//            *buffer++ = 0;
//          }
//        }
//        mBitmap->prepare();

        appendWidget( wClient = new wdgPanel(this,NULL_RECT,wa_Client) );
          wClient->setBorders(10,10,5,5);

          axContainer* con;
          axWidget* wdg;
          wdgLabel* lab;

          // --- left ---

          wClient->appendWidget( wLeft = new wdgPanel(this,axRect(0,0,200,0),wa_Left) );
            wLeft->setBorders(10,10,5,5);
            wLeft->setFlag(wf_Clip);
            //wLeft->setSizeLimits(100,-1,400,300);

            wLeft->appendWidget(       new wdgLabel( this,axRect(0,0,0,20), wa_Top,"label ") );
            wLeft->appendWidget(       new wdgLabel( this,axRect(0,0,0,20), wa_Top,"label ",ta_Right) );
            wLeft->appendWidget(       new wdgPanel( this,axRect(0,0,0,20), wa_Top) );
            wLeft->appendWidget( con = new wdgPanel( this,axRect(0,0,0,20), wa_Top) );
              con->appendWidget(       new wdgLabel( this,NULL_RECT,wa_Client,"label inside panel") );
            wLeft->appendWidget(       new wdgButton(this,axRect(0,0,0,20),wa_Top,false,"button: off","button: on") );
            wLeft->appendWidget(       new wdgSlider(this,axRect(0,0,0,20),wa_Top,"slider") );
            wLeft->appendWidget( wdg = new wdgSlider(this,axRect(0,0,20,0),wa_Left,"slider") );
              wdg->setFlag(wf_Vertical);



          // --- sizer ---

          wClient->appendWidget( wSizer = new wdgSizer(this,axRect(0,0,5,0),wa_Left) );
            wSizer->setTarget(wLeft);

          // --- right ---

          wClient->appendWidget( wRight = new wdgPanel(this,axRect(0,0,180,0),wa_Right) );
            wRight->setBorders(10,10,5,5);
            //wRight->setFlag(wf_Clip);
            wRight->appendWidget( wNumTimer = new wdgLabel( this,axRect(0,0,0,20), wa_Top,"doTimer : 0",    ta_Left) );
            wRight->appendWidget( wNumIdle  = new wdgLabel( this,axRect(0,0,0,20), wa_Top,"effEditIdle : 0",ta_Left) );
            wRight->appendWidget( wNumBlock = new wdgLabel( this,axRect(0,0,0,20), wa_Top,"doProcessBlock : 0",ta_Left) );

          // --- center ---

          wClient->appendWidget( wCenter = new wdgPanel(this,NULL_RECT/*axRect(110,10,120,120)*/,wa_Client) );
            wCenter->setBorders(10,10,5,5);
            wCenter->setFlag(wf_Clip);
            for (int i=0; i<50; i++)
            {
              wCenter->appendWidget( lab = new wdgLabel(this,axRect(0,0,20,20),wa_Stacked) );
                sprintf(label_buf[i],"%i",i+1);
                lab->setText(label_buf[i],ta_Center);
            }

          // ---

        doRealign();
        startTimer(100);
      }

    virtual ~myEditor(void)
      {
        stopTimer();
      }

    virtual void doTimer(void)
      {
        mNumTimer++;
        sprintf(timerText,"doTimer : %i",mNumTimer);
        wNumTimer->setText(timerText,ta_Left);
        onRedraw(wNumTimer);
      }

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    axEditor::doPaint(aCanvas,aRect);
    //    aCanvas->drawBitmap(mBitmap, mRect.x2()-120,mRect.y2()-120, 0,0,100,100);
    //  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    myEditor* mEditor;
    int       mNumIdle;
    int       mNumBlock;
    char      idleText[32];
    char      blockText[32];

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, AX_PLUG_DEFAULT)
      {
        mNumIdle = 0;
        mNumBlock = 0;
        describe("test_widgets","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(600,300);
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //--------------------------------------------------

    virtual void  doStateChange(int aState)               { wtrace("doStateChange " << aState); }
    virtual void  doTransportChange(int aState)           { wtrace("doTransportChange " << aState); }
    virtual void  doSetProgram(int aProgram)              { wtrace("doSetProgram " << aProgram); }
    virtual void  doSetParameter(axParameter* aParameter) { wtrace("doSetParameter " << aParameter->getName().ptr() ); }
    //virtual bool  doProcessEvents(void)                   { wtrace("doProcessEvents"); return false;}
    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) { wtrace("doProcessMidi "<<ofs<<" : "<<msg1<<","<<msg2<<","<<msg3); }
    virtual bool  doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        mNumBlock++;
        return false;
      }
    //virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs) {}
    //virtual void  doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize) {}

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        wtrace("doOpenEditor");
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        wtrace("doCloseEditor");
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //----------

    virtual void doIdleEditor()
      {
        mNumIdle++;
        sprintf(idleText,"doIdleEditor : %i",mNumIdle);
        mEditor->wNumIdle->setText(idleText,ta_Left);
        mEditor->onRedraw(mEditor->wNumIdle);

        sprintf(blockText,"doProcessBlock : %i",mNumBlock);
        mEditor->wNumBlock->setText(blockText,ta_Left);
        mEditor->onRedraw(mEditor->wNumBlock);

      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
