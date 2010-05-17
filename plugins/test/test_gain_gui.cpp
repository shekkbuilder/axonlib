//#define AX_DEBUG_MEMORY

#include "axPlugin.h"
#include "axEditor.h"
//#include "gui/axSkinDefault.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

class myPlugin : public axPlugin//,
                 //public axWidgetListener
{

    //----------------------------------------
    // gui
    //----------------------------------------

  private:
    axParameter*  p_Gain;
    float         m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_HasEditor)
      {
        m_Gain = 0;
        describe("test_gain_gui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(200,200);
        appendParameter( p_Gain = new axParameter(this,"gain","") );
        setupParameters();

        int a=1, b=1;
        axAssert(a-b);        
        //axStaticAssert(a-b);
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Gain) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  private:
    axEditor* mEditor;
    wdgPanel* wPanel;
    wdgKnob*  w_Gain;

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( wPanel = new wdgPanel(editor,NULL_RECT,wa_Client) );
        wPanel->appendWidget( w_Gain = new wdgKnob( editor,axRect(10,10,128,32),wa_None,"gain",0.75) );
        editor->connect(w_Gain,p_Gain);
        //editor->setSkin(mSkinDefault,true);
        editor->doRealign();
        editor->show();
        mEditor = editor;
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = mEditor;
        mEditor->hide();
        mEditor = NULL;
        delete editor;
      }

    //----------

    //virtual void doIdleEditor() {}

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
