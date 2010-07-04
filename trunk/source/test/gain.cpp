//#define AX_NOGUI

#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_PNG
#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

//----------------------------------------------------------------------

#include "base/axFormat.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

//----------------------------------------------------------------------

static char* g_params[]  = { (char*)"gain" };

//----------------------------------------------------------------------

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axInterface* aInterface) : axDescriptor(aInterface) {}
    virtual char* getName(void)             { return (char*)"test_gain_nogui"; }
    virtual char* getAuthor(void)           { return (char*)"ccernn"; }
    virtual char* getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual int   getNumParams(void)        { return 1; }
    virtual char* getParamName(int aIndex)  { return g_params[aIndex]; }
};

//----------------------------------------------------------------------

class myInstance : public axInstance
{
  private:
    float         m_Gain;
    axParameter*  p_Gain;
    axSkinBasic*  mSkin;
  public:

    myInstance(axDescriptor* aDescriptor) : axInstance(aDescriptor)
      {
        m_Gain = 0.5;
        //describe("test_gain_gui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        //setupAudio(2,2,false);
        //setupEditor(200,200);
        //appendParameter( p_Gain = new axParameter(this,"gain","") );
        //setupParameters();
      }

    // called from editor
    virtual void  doSetParameter(axParameter* aParameter)
      {
        trace("doSetParameter");
        if (aParameter->getIndex()==0) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

  //----------------------------------------
  // main (only called if exe)
  //----------------------------------------

  public:

    #ifdef AX_FORMAT_EXE
    virtual int main(int argc, char** argv)
      {
        trace("main");
        #ifndef AX_NOGUI
        axInterface* interface = mDescriptor->getInterface();
        Display* display = (Display*)interface->getHandle();
        Window parent = XDefaultRootWindow(display);
        axWindow* window = new axWindow(interface,&parent,axRect(0,0,320,240),AX_WIN_DEFAULT);
        axCanvas* canvas = window->getCanvas();
        mSkin = new axSkinBasic(canvas);
        window->applySkin(mSkin);
        wdgPanel* panel = new wdgPanel( window,NULL_RECT,wa_Client);
        window->appendWidget( panel );
        panel->appendWidget( new wdgKnob( window,axRect(10,10,100,32),wa_None,"knob1"));
        panel->appendWidget( new wdgKnob( window,axRect(10,50,100,32),wa_None,"knob2"));
        window->doRealign();
        window->show();
        window->eventLoop();
        window->hide();
        delete window;
        delete mSkin;
        #endif //AX_NOGUI
        return 0;
      }
    #endif //AX_FORMAT_EXE


  //----------------------------------------
  // gui
  //----------------------------------------

//  private:
//    axEditor* mEditor;
//    wdgPanel* wPanel;
//    wdgKnob*  w_Gain;
//
//  public:
//
//    virtual void* doOpenEditor(axInterface* aInterface, void* aParent)
//      {
//        trace("doOpenEditor");
//        trace("aInterface: " << aInterface);
//        trace("aParent: " << aParent);
//        trace("AX_WIN_DEFAULT: " << AX_WIN_DEFAULT);
//        axEditor* editor = new axEditor(aInterface,aParent,mEditorRect,AX_WIN_DEFAULT);
//        trace("editor: " << (int)editor);
//        //editor->appendWidget( wPanel = new wdgPanel(editor,NULL_RECT,wa_Client) );
////        wPanel->appendWidget( w_Gain = new wdgKnob( editor,axRect(10,10,128,32),wa_None,"gain",0.75) );
////        editor->connect(w_Gain,p_Gain);
////        editor->doRealign();
//        editor->show();
//        mEditor = editor;
//        return mEditor;
//      }
////
////    //----------
////
//    virtual void doCloseEditor(void)
//      {
//        axEditor* editor = mEditor;
//        mEditor->hide();
//        mEditor = NULL;
//        delete editor;
//      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myDescriptor,myInstance,axInterface,axPlatform)

