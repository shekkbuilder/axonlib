//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_PNG
#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#include "axPlugin.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

#include "gui/axSkin.h"
#include "gui/axBitmapLoader.h"
#include "../img/knob32.h"

//----------------------------------------------------------------------
//
// skin
//
//----------------------------------------------------------------------

class mySkin : public axSkin//Default
{
  private:
    axBitmapLoader  loader;
    axBitmap*       bitmap;
  public:
    axSurface*  mKnobImage;
    int         mKnobCount, mKnobWidth, mKnobHeight;
  public:
    mySkin(axCanvas* aCanvas)
    : axSkin/*Default*/(aCanvas)
      {
        mKnobImage  = NULL;
        mKnobCount  = 0;
        mKnobWidth  = 0;
        mKnobHeight = 0;
      }
    virtual ~mySkin()
      {
        if (mKnobImage) delete mKnobImage;
      }
    inline void loadKnobBitmap(axEditor* aEditor, unsigned char* buffer, int size, int w, int h, int n)
      {
        mKnobWidth  = w;//32;
        mKnobHeight = h;//32;
        mKnobCount  = n;//65;
        mKnobImage = aEditor->createSurface(mKnobWidth,mKnobHeight*mKnobCount,24);
        loader.decode(buffer,size);
        bitmap = aEditor->createBitmap( loader.getWidth(), loader.getHeight(), 24 );
        bitmap->createBuffer( (char*)loader.getImage() );                   // create bitmap buffer & copy data
        bitmap->convertRgbaBgra();                                          // -> bgr.a
        bitmap->setBackground(128,128,128);                                 // replace alpha (bgr)
        bitmap->prepare();                                                  // prepare bitmap for blitting
        axCanvas* can = mKnobImage->getCanvas();
        can->drawBitmap(bitmap,0,0,0,0,mKnobWidth,mKnobHeight*mKnobCount);  // upload to surface
        delete bitmap;
      }
    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mFillColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }
    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue)
      {
        if (mKnobImage)
        {
          // bitmap
          int index = (int)axFloor(aValue*mKnobCount);
          index = axMinInt(index,mKnobCount-1);
          int ky = mKnobHeight * index;
          aCanvas->drawImage(mKnobImage,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          // text (copy/paste from wdgKnob)
          int x  = aRect.x;
          int y  = aRect.y;
          int size = axMinInt(aRect.w,aRect.h);
          int th = aCanvas->textHeight("Xj");
          if (aRect.h >= (2*th))
          {
            aCanvas->setTextColor(mTextColor);
            aCanvas->drawText(x+size+8,y,   aRect.x2(),aRect.y2(),aName,ta_Top|ta_Left);
            aCanvas->drawText(x+size+8,y+th,aRect.x2(),aRect.y2(),aDisp,ta_Top|ta_Left);
          }
          else
          {
            aCanvas->setTextColor(mTextColor);
            aCanvas->drawText(x+size+5,y,aRect.x2(),aRect.y2(),aDisp,ta_Left);
          }
        } //knobimage
        //else
        //  axSkinDefault::drawKnob(aCanvas,aRect,aName,aDisp,aValue);
      }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axPlugin
{

    //----------------------------------------
    // plugin
    //----------------------------------------

  private:
    axParameter*  p_Gain;
    float         m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_HasEditor)
      {
        trace("myPlugin.constructor()");
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
        trace ( axLogf(11.f) );
      }

    virtual ~myPlugin()
      {
        axDstdDestroy();
        //axDwinDestroy();
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
    // editor
    //----------------------------------------

  private:
    axEditor*       m_Editor;
    mySkin*         m_Skin;
    wdgPanel*       w_Panel;
    wdgKnob*        w_Gain;

  public:

    // from axPluginVst.dispatcher  -  context: HWND
    // axPluginExe.main = main  -  context: instance, winname

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axCanvas* canvas = editor->getCanvas();
        m_Skin = new mySkin(canvas);
        m_Skin->loadKnobBitmap(editor,(unsigned char*)knob32,knob32_size,32,32,65);
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

    //virtual void doIdleEditor() {}

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
