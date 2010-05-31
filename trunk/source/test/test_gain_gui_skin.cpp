//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#define AX_ALPHA

#include "axFormat.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

#include "skins/axSkinDef.h"
#include "gui/axBitmapLoader.h"
#include "../../ax/skins/img/skin1.h"
#include "../../ax/skins/img/knob1.h"

//----------------------------------------------------------------------

class myPlugin : public axFormat
{

    //----------------------------------------
    // plugin
    //----------------------------------------

  private:
    axParameter*  p_Gain;
    float         m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_HasEditor)
      {
        gui_initialized = false;
        m_Gain = 0;
        describe("test_gain_gui_skin","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(128,64);
        appendParameter( p_Gain = new axParameter(this,"gain","") );
        setupParameters();
        /*
          // test
          char* ptr = (char*)axCalloc(11, sizeof(char));
          trace ( axSqrt(21.f) );
        */
        //trace(axLogf(11.f));
      }

    virtual ~myPlugin()
      {
        if (gui_initialized)
        {
          delete skinloader;
          delete knobloader;
        }
        axDstdDestroy();
        //axDwinDestroy();
      }

    virtual void  doSetParameter(axParameter* aParameter)
      { // or aParameter->getIndex()==0
        if (aParameter==p_Gain) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

    //----------------------------------------
    // editor
    //----------------------------------------

  private:
    bool            gui_initialized;
    axBitmapLoader* skinloader;
    axBitmapLoader* knobloader;
    axEditor*       m_Editor;
    axSkinDef*      m_Skin;
    wdgPanel*       w_Panel;
    wdgKnob*        w_Gain;

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        if (!gui_initialized)
        {
          skinloader = new axBitmapLoader();
          skinloader->decode((unsigned char*)skin1,skin1_size);
          knobloader = new axBitmapLoader();
          knobloader->decode((unsigned char*)knob1,knob1_size);
          gui_initialized = true;
        }
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axCanvas* canvas = editor->getCanvas();
        m_Skin = new axSkinDef(canvas);
        m_Skin->loadSkinBitmap(editor,(char*)skinloader->getImage());
        m_Skin->loadKnobBitmap(editor,(char*)knobloader->getImage());
        editor->applySkin(m_Skin);
        editor->appendWidget( w_Panel = new wdgPanel(editor,NULL_RECT,wa_Client) );
        w_Panel->appendWidget( w_Gain = new wdgKnob( editor,axRect(10,10,100,32),wa_None,"gain",0.75) );
        editor->connect(w_Gain,p_Gain);
        editor->doRealign();
        editor->show();
        m_Editor = editor;
        //int* test_axrlc = NULL;
        //int c = 64; // warning: unused variable
        //test_axrlc = (int*)axRealloc (test_axrlc, 64 * sizeof(int));
        //if (test_axrlc)
        //  trace("test: *** realloc_ok");
        //axFree(test_axrlc);
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        axEditor* editor = m_Editor;
        m_Editor = NULL;
        delete editor;
        delete m_Skin;
      }

    //----------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
