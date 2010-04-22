//#define AX_DEBUG

#include "axPlugin.h"
#include "axEditor.h"
#include "gui/axSkinDefault.h"
//#include "gui/axSkin.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

#include "gui/axBitmapLoader.h"
#include "../img/knob32.h"

//----------------------------------------------------------------------
//
// skin
//
//----------------------------------------------------------------------

class mySkin : public axSkinDefault
{
  public:
    axImage*  mKnobImage;
    int       mKnobCount, mKnobWidth, mKnobHeight;
  public:
    mySkin(axCanvas* aCanvas)
    : axSkinDefault(aCanvas)
      {
        mKnobImage  = NULL;
        mKnobCount  = 0;
        mKnobWidth  = 0;
        mKnobHeight = 0;
      }
    inline void setKnobImage(axImage* aImage, int aCount, int aWidth, int aHeight)
      {
        mKnobImage  = aImage;
        mKnobCount  = aCount;
        mKnobWidth  = aWidth;
        mKnobHeight = aHeight;
      }
    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
      {
        fill_back(aCanvas,aRect);
        //draw_frame(aCanvas,aRect);
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
        else

        axSkinDefault::drawKnob(aCanvas,aRect,aName,aDisp,aValue);
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
        setupEditor(100,52);
        appendParameter( p_Gain = new axParameter(this,"gain","") );
        setupParameters();

        axDwinCreate();
        wdebug("hello dbg");        
        
        axDstdCreate();
        trace("hello dbg");
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
    axEditor*       mEditor;
    wdgPanel*       wPanel;
    wdgKnob*        w_Gain;
    axSurface*      srf;
    mySkin*         skin;
    axBitmapLoader  loader;
    axBitmap*       bitmap;

  public:

    // from axPluginVst.dispatcher  -  context: HWND
    // axPluginExe.main = main  -  context: instance, winname

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axCanvas* canvas = editor->getCanvas();
        skin = new mySkin(canvas);
        editor->setSkin(skin);
//      //--- decode & initialize bitmap
        srf = editor->createSurface(32,32*65);
        loader.decode((unsigned char*)knob32,knob32_size);
        /*axBitmap**/ bitmap = editor->createBitmap( loader.getWidth(), loader.getHeight() );
        bitmap->createBuffer( (char*)loader.getImage() );   // create bitmap buffer & copy data
        bitmap->convertRgbaBgra();                          // -> bgr.a
        bitmap->setBackground(128,128,128);                 // replace alpha
        bitmap->prepare();                                  // prepare bitmap for blitting
        axCanvas* can = srf->getCanvas();
        can->drawBitmap(bitmap,0,0,0,0,32,32*65);           // upload to surface
        delete bitmap;
//        //---
        skin->setKnobImage(srf,65,32,32);
//        editor->setSkin(skin);
        editor->appendWidget( wPanel = new wdgPanel(editor,NULL_RECT,wa_Client) );
        wPanel->appendWidget( w_Gain = new wdgKnob( editor,axRect(10,10,100,32),wa_None,"gain",0.75) );
        editor->connect(w_Gain,p_Gain);
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
        delete skin;
        delete srf;
        //delete bitmap;
      }

    //----------

    //virtual void doIdleEditor() {}

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
