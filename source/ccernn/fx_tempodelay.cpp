#define AX_ALPHA
//#define AX_WIDGET_NOUPDATELIST

#include "format/axFormat.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
#include "par/parFloat.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"
//#include "wdg/wdgButton.h"
//#include "wdg/wdgSlider.h"
#include "skins/axSkinDef.h"
#include "gui/axBitmapLoader.h"
#include "../../ax/skins/img/skin1.h"
#include "../../ax/skins/img/knob1.h"
//#include "../../ax/skins/img/knob32.h"

//----------------------------------------------------------------------

#define MAX_BEATS             4
#define MIN_BPM               30
#define MAX_SRATE             192000
#define MAX_SECONDS_PER_BEAT  60 / MIN_BPM
#define MAX_SAMPLES_PER_BEAT  MAX_SRATE * MAX_SECONDS_PER_BEAT
#define MAX_DELAY_LENGTH      MAX_BEATS * MAX_SAMPLES_PER_BEAT
//#define MAX_DELAY_LENGTH    1536000

//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
  // process
    SPL             m_Buffer[MAX_DELAY_LENGTH*2]; // totally 12mb (1.5m*sizeof(float)*2)
    int             m_Index;
    int             m_Size;
  //'internal' parameters
    float           m_Delay;
    float           m_Feedback;
    float           m_Dry;
    float           m_Wet;
  //vst parameters
    parFloat*       p_Delay;
    parFloat*       p_Feedback;
    parFloat*       p_Dry;
    parFloat*       p_Wet;
  //editor
    axEditor*       m_Editor;
    axSkinDef*      m_Skin;
    //axSkinDef*    m_Skin2;
    wdgPanel*       w_Panel;
    wdgKnob*        w_Delay;
    wdgKnob*        w_Feedback;
    wdgKnob*        w_Dry;
    wdgKnob*        w_Wet;
    bool            gui_initialized;
    axBitmapLoader* skinloader;
    axBitmapLoader* knobloader;
    //axBitmapLoader* knob2loader;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext)
      {
        if (!gui_initialized)
        {
          skinloader = new axBitmapLoader();
          skinloader->decodePng((unsigned char*)skin1,skin1_size);
          knobloader = new axBitmapLoader();
          knobloader->decodePng((unsigned char*)knob1,knob1_size);
          //knob2loader = new axBitmapLoader();
          //knob2loader->decodePng((unsigned char*)knob32,knob32_size);
          gui_initialized = true;
        }
        m_Index = 0;
        m_Size  = 0;
        describe("fx_tempodelay","ccernn","axonlib example",2,AX_MAGIC+0x1001);
        setupAudio(2,2,false);
        setupEditor(100+20,(32*4)+15+20);
        //setupEditor(320,400);
        appendParameter( p_Delay    = new parFloat(this,"delay",   "",0.75, 0.25, MAX_BEATS, 0.25 ) );
        appendParameter( p_Feedback = new parFloat(this,"feedback","",0.75 ) );
        appendParameter( p_Dry      = new parFloat(this,"dry",     "",0.75 ) );
        appendParameter( p_Wet      = new parFloat(this,"wet",     "",0.4  ) );
        setupParameters();
      }

    //----------

    ~myPlugin()
      {
        if (gui_initialized)
        {
          delete skinloader;
          delete knobloader;
          //delete knob2loader;
        }
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //[internal]
    inline int calcSize(float delay, float tempo, float srate)
      {
        float sec_per_beat = 60 / tempo;
        float spl_per_beat = srate * sec_per_beat;
        return (int)(delay * spl_per_beat);
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual void doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float val = aParameter->getValue();
        axString name = aParameter->getName();
        trace("doSetParameter(" << index << ") = " << val << "  [" <<  name.ptr() << "]" );
        switch(index)
        {
          // be careful with that index, eugene!
          case 0: m_Delay    = val; break;
          case 1: m_Feedback = val; break;
          case 2: m_Dry      = val; break;
          case 3: m_Wet      = val; break;
        }
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        updateTimeInfo(); // get timing info from host
        m_Size = calcSize( m_Delay, getTempo(), getSampleRate() );
        while (m_Index>=m_Size) m_Index-=m_Size;
        return false; // we want doProcessSample
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        int i2 = m_Index*2;
        float in0 = *aInputs[0];
        float in1 = *aInputs[1];
        float dly0 = m_Buffer[i2  ];
        float dly1 = m_Buffer[i2+1];
        m_Buffer[i2  ] = in0 + dly0*m_Feedback;
        m_Buffer[i2+1] = in1 + dly1*m_Feedback;
        m_Index++;
        if (m_Index>=m_Size) m_Index = 0;
        *aOutputs[0] = in0*m_Dry + dly0*m_Wet;
        *aOutputs[1] = in1*m_Dry + dly1*m_Wet;
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axCanvas* canvas = editor->getCanvas();
        m_Skin = new axSkinDef(canvas);
        m_Skin->loadSkinBitmap(editor,(char*)skinloader->getImage(),256,256,32);
        m_Skin->loadKnobBitmap(editor,(char*)knobloader->getImage(),32,(32*65),32);
        editor->applySkin(m_Skin);
        editor->appendWidget(  w_Panel = new wdgPanel(editor,NULL_RECT,wa_Client) );
        w_Panel->setBorders(10,10,5,5);
        w_Panel->appendWidget( w_Delay    = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"delay"    ) );
        w_Panel->appendWidget( w_Feedback = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"feedback" ) );
        w_Panel->appendWidget( w_Dry      = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"dry"      ) );
        w_Panel->appendWidget( w_Wet      = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"wet"      ) );
        //----------
        //m_Skin2 = new axSkinDef(canvas);
        //m_Skin2->copyFrom(m_Skin);
        //m_Skin2->loadKnobBitmap(editor,(char*)knob2loader->getImage());
        //m_Skin2->setVariation(1);
        //w_Delay->applySkin(m_Skin2);
        //wdgButton* but;
        //w_Panel->appendWidget( but = new wdgButton(editor,axRect(16,16),wa_StackedHoriz/*,true, "off","on",ta_Center,bm_Switch*/) );
        //w_Panel->appendWidget( but = new wdgButton(editor,axRect(16,16),wa_StackedHoriz,true/*, "off","on",ta_Center,bm_Switch*/) );
        //but->applySkin(m_Skin2);
        //wdgSlider* sli;
        //w_Panel->appendWidget( sli = new wdgSlider(editor,axRect(16,64),wa_StackedHoriz,"slider",0,true) );
        //sli->setSensitivity( 1.0f/(64.0f-11.0f) );
        //w_Panel->appendWidget( sli = new wdgSlider(editor,axRect(16,64),wa_StackedHoriz,"slider",0,true) );
        //sli->setSensitivity( 1.0f/(64.0f-11.0f) );
        //sli->applySkin(m_Skin2);
        //----------
        editor->connect(w_Delay,   p_Delay);
        editor->connect(w_Feedback,p_Feedback);
        editor->connect(w_Dry,     p_Dry);
        editor->connect(w_Wet,     p_Wet);
        editor->doRealign();
        editor->show();
        //editor->startTimer(500);
        m_Editor = editor;
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        //mEditor->stopTimer();
        m_Editor->hide();
        delete m_Editor;
        m_Editor = NULL;
        delete m_Skin;
        //delete m_Skin2;
      }

    //----------

    // call axWindow::doIdleEditor() to redraw update list
    //
    //virtual void doIdleEditor()
    //  {
    //    #ifndef AX_WIDGET_NOUPDATELIST
    //    if (m_Editor) m_Editor->redrawUpdates();
    //    #endif
    //  }

};

//----------------------------------------------------------------------

//#undef MAX_BEATS
//#undef MIN_BPM
//#undef MAX_SRATE
//#undef MAX_SECONDS_PER_BEAT
//#undef MAX_SAMPLES_PER_BEAT
//#undef MAX_DELAY_LENGTH

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
