#define AX_DEBUG
#include "core/axDebug.h"
#include "axPlugin.h"
#include "axEditor.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    axEditor*     mEditor;
    axContainer*  wClient;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, AX_PLUG_DEFAULT)
      {
        describe("test_widgets","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(512,384);
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        //axCanvas* canvas = getCanvas();
        wClient = new axContainer(this,NULL_RECT,wa_Client);
        wClient->setBorders(10,10,5,5);
        mEditor->appendWidget(wClient);
        axWidget*     wdg;
        axContainer*  cnt;

        // --- left ---

        wClient->appendWidget( cnt = new axContainer(this,axRect(0,0,200,0),wa_Left) );
          cnt->appendWidget( wdg = new axWidget(this,axRect(0,0,0,40),wa_Top) );
          cnt->setFlag(wf_Clip);
            wdg->setOption(wo_Text);
            wdg->setText("wdg");
          cnt->appendWidget( wdg = new axWidget(this,axRect(0,0,0,50),wa_Top) );
            wdg->clearOption(wo_Border);
            wdg->setOption(wo_Text|wo_Bevel);
            wdg->setText("wdg");

        // --- right ---

        wClient->appendWidget( cnt = new axContainer(this,axRect(0,0,150,0),wa_Right) );
          cnt->setBorders(5,5,5,5);
          cnt->setFlag(wf_Clip);
          cnt->setOption(wo_Text);
          cnt->setText("axContainer"/*,ta_Top*/);
          cnt->appendWidget( wdg = new axWidget(this,axRect(0,0,0,25),wa_Bottom) );
            wdg->setOption(wo_Text);
            wdg->setText("axWidget");
          cnt->appendWidget( wdg = new axWidget(this,axRect(0,0,0,25),wa_Bottom) );
            wdg->clearOption(wo_Border);
            wdg->setOption(wo_Text|wo_Bevel);
            wdg->setText("axWidget");

        // --- middle ---

        wClient->appendWidget( cnt = new axContainer(this,axRect(110,10,120,120),wa_Client) );
          cnt->setBorders(5,5,5,5);
          cnt->setFlag(wf_Clip|wf_Vertical);
          cnt->setOption(wo_Text);
          cnt->setText("axContainer"/*,ta_Top*/);

          for (int i=0; i<32; i++)
          {
            cnt->appendWidget( wdg = new axWidget(this,axRect(0,0,70,20),wa_Stacked) );
              wdg->clearOption(wo_Border);
              wdg->setOption(wo_Text|wo_Bevel);
              wdg->setText("axWidget");
          }


        // ---

        mEditor->doRealign();
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
    //    wtrace("...doIdleEditor");
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
