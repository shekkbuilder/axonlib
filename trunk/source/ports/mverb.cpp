//#define NO_GUI
#define AX_ALPHA

//----------

#include "format/axFormat.h"
#include <math.h>
#include <memory.h>
#include "../../extern/mverb/MVerb.h"

//#ifndef NO_GUI
//  #include "gui/axEditor.h"
//  #include "gui/axBitmapLoader.h"
//  #include "skins/axSkinDef.h"
//  #include "wdg/wdgImage.h"
//  #include "wdg/wdgKnob.h"
//  #include "mverb_background.h"
//  #include "mverb_knob.h"
//#endif //gui

//----------------------------------------------------------------------

float prog1[] = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.5, 1.0, 0.15, 0.75 }; // Subtle
float prog2[] = { 0.0, 0.5,	1.0, 0.5, 0.0, 1.0, 1.0, 0.35, 0.75 }; // Stadium
float prog3[] = { 0.0, 0.5, 1.0, 0.5, 0.0, 0.25,1.0, 0.35, 0.75 }; // Cupboard
float prog4[] = { 0.9, 0.5, 0.1, 0.5, 0.0, 0.5, 1.0, 0.5,  0.75 }; // Dark
float prog5[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 0.5,  0.75 }; // Halves

//----------------------------------------------------------------------
//
// skin
//
//----------------------------------------------------------------------
#ifndef NO_GUI

  #include "gui/axEditor.h"
  #include "gui/axBitmapLoader.h"
  //#include "skins/axSkinDef.h"
  #include "gui/axSkin.h"
  #include "wdg/wdgImage.h"
  #include "wdg/wdgKnob.h"
  #include "mverb_background.h"
  #include "mverb_knob.h"

  #define KNOB_TEXTYOFFSET 35

class mverb_skin : public axSkin
{
  protected:
    bool        mKnobSrfLoaded;
    axSurface*  mKnobSrf;
    int         mKnobWidth;
    int         mKnobHeight;
    int         mKnobCount;
    axColor     mKnobTextCol;
    axColor     mKnobValCol;

  public:

    mverb_skin(axCanvas* aCanvas)
      {
        mKnobSrfLoaded    = false;
        mKnobSrf          = NULL;
        mKnobWidth        = 32;
        mKnobHeight       = 32;
        mKnobCount        = 129;
        mKnobTextCol      = aCanvas->getColor(0,0,0);
        mKnobValCol       = aCanvas->getColor(255,255,255);
      }

    virtual ~mverb_skin()
      {
        if (mKnobSrfLoaded) delete mKnobSrf;
      }

    virtual axBitmap* loadBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
    {
      axBitmap* bitmap = aEditor->createBitmap(aWidth,aHeight,aDepth);
      bitmap->createBuffer(aBuffer);
      bitmap->convertRgbaBgra();
      bitmap->premultAlpha();
      bitmap->prepare();
      return bitmap;
    }

    virtual void loadKnobBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth, int aNum=65)
      {
        axBitmap* bitmap = loadBitmap(aEditor,aBuffer,aWidth,aHeight,aDepth);
        mKnobSrf = aEditor->createSurface(aWidth,aHeight,aDepth);
        mKnobSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight);
        mKnobSrfLoaded = true;
        delete bitmap;
      }

    //TODO drawPanel

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue, int aMode=0)
      {
        if (mKnobSrf)
        {
          int index = (int)axFloor(aValue*mKnobCount);
          index = axMinInt(index,mKnobCount-1);
          int ky = mKnobHeight * index;
          aCanvas->renderSurface(mKnobSrf,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          int y  = aRect.y + KNOB_TEXTYOFFSET;
          int th = aCanvas->textHeight("Xj");
          aCanvas->setTextColor(mKnobValCol);
          aCanvas->drawText(aRect.x,y,aRect.x2(),y+th,aDisp,ta_Center);
        }
      }
};

#endif //gui
//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
    MVerb<float>    em_verb;
  #ifndef NO_GUI
  private:
    bool            m_GuiInitialized;
    axEditor*       m_Editor;
    mverb_skin*     m_Skin;
    wdgImage*       w_Panel;
    axBitmapLoader* m_BackLoader;
    axBitmapLoader* m_KnobLoader;
  #endif //gui

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext)
      {
        describe("mverb","martin eastwood","axonlib port",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        #ifndef NO_GUI
          m_GuiInitialized = false;
          setupEditor(456,108);
        #endif //gui
        appendParameter( new axParameter(this,"damping freq", "",0.5) );
        appendParameter( new axParameter(this,"density",      "",0.5) );
        appendParameter( new axParameter(this,"bandwidthfreq","",0.5) );
        appendParameter( new axParameter(this,"decay",        "",0.5) );
        appendParameter( new axParameter(this,"predelay",     "",0.5) );
        appendParameter( new axParameter(this,"size",         "",0.5) );
        appendParameter( new axParameter(this,"gain",         "",0.5) );
        appendParameter( new axParameter(this,"mix",          "",0.5) );
        appendParameter( new axParameter(this,"earlymix",     "",0.5) );
        setupParameters();
        appendProgram( new axProgram("subtle",  9, prog1) );
        appendProgram( new axProgram("stadium", 9, prog2) );
        appendProgram( new axProgram("cupboard",9, prog3) );
        appendProgram( new axProgram("dark",    9, prog4) );
        appendProgram( new axProgram("halves",  9, prog5) );
        setupPrograms();
      }

    virtual ~myPlugin()
      {
        #ifndef NO_GUI
        if (m_GuiInitialized)
        {
          delete m_BackLoader;
          delete m_KnobLoader;
        }
        #endif //gui
      }

    virtual void doPreProgram(int aProgram)
      {
        if (getCurrentProgram() != aProgram)    // should this be handled lower down, so we don't
          saveProgram( getCurrentProgram() );   // even get the the call if changing to same program?
      }                                         // could be handy, to allow "revert..."

    virtual void doStateChange(int aState)
      {
        switch (aState)
        {
          case fs_Resume:
            em_verb.setSampleRate(getSampleRate());
            break;
        }
      }

    virtual void doSetParameter(axParameter* aParameter)
      {
        int   index = aParameter->getIndex();
        float value = aParameter->getValue();
        em_verb.setParameter(index,value);
      }

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        em_verb.process(aInputs,aOutputs,aSize);
        return true;
      }

#ifndef NO_GUI

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        if (!m_GuiInitialized)
        {
          m_BackLoader = new axBitmapLoader();
          m_BackLoader->decodePng((unsigned char*)mverb_background,mverb_background_size);
          m_KnobLoader = new axBitmapLoader();
          m_KnobLoader->decodePng((unsigned char*)mverb_knob,mverb_knob_size);
          m_GuiInitialized = true;
        }
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        // skin
        axCanvas* canvas = editor->getCanvas();
        m_Skin = new mverb_skin(canvas);
        m_Skin->loadKnobBitmap(editor,(char*)m_KnobLoader->getImage(),32,(32*129),32);  // bitmap depth
        editor->applySkin(m_Skin);
        // background image
        editor->appendWidget( w_Panel = new wdgImage(editor,NULL_RECT,wa_Client) );
        w_Panel->loadBitmap(editor,(char*)m_BackLoader->getImage(),456,108, 24);        // screen depth
        // knobs

        // hopefully this is in order now :-)
        w_Panel->appendWidget( new wdgKnob( editor,axRect(336,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(216,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(256,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(296,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(96, 40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(176,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(376,40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(56, 40,40,64),wa_None) );
        w_Panel->appendWidget( new wdgKnob( editor,axRect(136,40,40,64),wa_None) );

        // setup
        for (int i=0; i<9; i++) editor->connect( w_Panel->getWidget(i), mParameters[i] );
        editor->doRealign();
        editor->show();
        m_Editor = editor;
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

#endif //gui

};

AX_ENTRYPOINT(myPlugin)
