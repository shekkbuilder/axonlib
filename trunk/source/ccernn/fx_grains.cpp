#define AX_ALPHA

#include "axFormat.h"
#include "axEditor.h"
#include "core/axRand.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
//#include "wdg/wdgPanel.h"
#include "wdg/wdgImage.h"
#include "wdg/wdgKnob.h"

#include "gui/axBitmapLoader.h"

#include "img/fx_grains_back.h"
#include "img/fx_grains_knob.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE 4000000
#define MAX_GRAINS  256

char* str_onoff[] =
{
  (char*)"off",
  (char*)"on"
};

struct GRAIN
{
  int   active;
  float pos;
  float start;
  float end;
  float duration;
  float speed;
  float phase;
  float phase_add;
  float phase2;
  float phase2_add;
};

//----------------------------------------------------------------------

class mySkin : public axSkin
{
  public:
    axSurface*  s_Back;
    axSurface*  s_Knob;
    int mKnobWidth,mKnobHeight,mKnobCount;
  public:
    mySkin(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        mKnobWidth  = 32;
        mKnobHeight = 32;
        mKnobCount  = 65;
      }
    virtual ~mySkin()
      {
      }
    //virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
    //  {
    //    aCanvas->setBrushColor( /*aCanvas->getColor(128,0,0)*/mFillColor );
    //    aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
    //  }
    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue)
      {
        if (s_Knob)
        {
          // bitmap
          int index = (int)axFloor(aValue*mKnobCount);
          index = axMinInt(index,mKnobCount-1);
          int ky = mKnobHeight * index;

          //#ifdef AX_ALPHA
            aCanvas->renderImage(s_Knob,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          //  //aCanvas->stretchImage(mKnobImage,aRect.x,aRect.y, aRect.w, aRect.h, 0,ky,mKnobWidth,mKnobHeight);
          //#else
          //  aCanvas->drawImage(mKnobImage,aRect.x,aRect.y, 0,ky,mKnobWidth,mKnobHeight);
          //#endif
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

class myPlugin : public axFormat
{
  private:
  // process
    float*      BUFFER;
    GRAIN       GRAINS[MAX_GRAINS];
    int         index;
    int         countdown;
  //'internal' parameters
    float       m_Master;
    int         m_NumGrains;
    float       m_BufferSize;
    float       m_Freeze;
    float       m_GrainDist;
    float       m_GrainSize;
    float       m_GrainDur;
    float       m_GrainPitch;
    float       m_Env;
    float       m_GrainEnv;
    float       m_StartJit;
    float       m_PitchJit;
    float       m_SizeJit;
    float       m_DurJit;
    float        _BufferSize;
    float        _GrainDist;
    float        _GrainSize;
    float        _GrainDur;
  //vst parameters
  //gui
    axEditor*   w_Editor;
    //wdgPanel*   w_Panel;
    wdgImage*   w_Panel;
    axSurface*  s_Back;
    axSurface*  s_Knob;
    mySkin*     m_Skin;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_None)
      {
        axRand(418);
        BUFFER = new float[BUFFER_SIZE];
        axMemset(BUFFER,0,BUFFER_SIZE*sizeof(float));
        axMemset(GRAINS,0,MAX_GRAINS*sizeof(GRAIN));
        index     = 0;
        countdown = 0;
        describe("fx_grains","ccernn","axonlib example",0,AX_MAGIC+0x1004);
        setupAudio(2,2,false);
        setupEditor(340,325);
        appendParameter( new parFloat(  this,"master volume",     "",   1,    0, 2   ) );
        appendParameter( new parInteger(this,"number of grains",  "",   10,   1, MAX_GRAINS ) );
        appendParameter( new parFloat(  this,"buffer size",       "ms", 1000, 1, 5000) );
        appendParameter( new parInteger(this,"freeze",            "",   0,    0, 1, str_onoff ) );
        appendParameter( new parFloat(  this,"grain distance",    "ms", 20,   1, 100 ) );
        appendParameter( new parFloat(  this,"grain size",        "ms", 30,   1, 100 ) );
        appendParameter( new parFloat(  this,"grain duration",    "ms", 300,  1, 2000) );
        appendParameter( new parFloat(  this,"grain pitch",       "",   1,    0, 2   ) );
        appendParameter( new parFloat(  this,"envelope",          "",   1   ) );
        appendParameter( new parFloat(  this,"grain envelope",    "",   1   ) );
        appendParameter( new parFloat(  this,"distance jitter",   "",   0.2 ) );
        appendParameter( new parFloat(  this,"pitch jitter",      "",   0.2 ) );
        appendParameter( new parFloat(  this,"size jitter",       "",   0.2 ) );
        appendParameter( new parFloat(  this,"duration jitter",   "",   0.2 ) );
        setupParameters();

      }

    //----------

    virtual ~myPlugin()
      {
        delete BUFFER;
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);

        axBitmapLoader* loader;
        axBitmap* bitmap;

        loader = new axBitmapLoader();
          loader->decode((unsigned char*)fx_grains_back,fx_grains_back_size);
          bitmap = editor->createBitmap(340,325,24);
            bitmap->createBuffer((char*)loader->getImage());

            bitmap->convertRgbaBgra();                                          // -> bgr.a
            bitmap->premultAlpha();
            bitmap->swizzle(
              1.1, 0.0, 0.0, 0.0,
              0.0, 1.1, 0.0, 0.0,
              0.0, 0.0, 0.8, 0.0,
              0.0, 0.0, 0.0, 1.0
            );

            bitmap->prepare();
            s_Back = editor->createSurface(340,325,24);
            s_Back->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,340,325 );
          delete bitmap;
        delete loader;

        loader = new axBitmapLoader();
          loader->decode((unsigned char*)fx_grains_knob,fx_grains_knob_size);
          bitmap = editor->createBitmap(32,32*65,32);
            bitmap->createBuffer((char*)loader->getImage());
            bitmap->convertRgbaBgra();                                          // -> bgr.a
            //bitmap->swizzle(
            //  1.0, 0.0, 0.0, 0.0,
            //  0.0, 1.0, 0.0, 0.0,
            //  0.0, 0.0, 1.0, 0.0,
            //  0.0, 0.0, 0.0, 1.0
            //);
            bitmap->premultAlpha();
            bitmap->prepare();
            s_Knob = editor->createSurface(32,32*65,32);
            s_Knob->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,32,32*65 );
          delete bitmap;
        delete loader;

        axCanvas* canvas = editor->getCanvas();
        m_Skin = new mySkin(canvas);
        m_Skin->s_Back = s_Back;
        m_Skin->s_Knob = s_Knob;
        editor->applySkin(m_Skin);

        editor->appendWidget( w_Panel = new wdgImage(editor,NULL_RECT,wa_Client,s_Back) );

          w_Panel->appendWidget( new wdgKnob(editor,axRect(230,255,100,32),wa_None,"master") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect( 10,155,100,32),wa_None,"num grains") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect( 10, 50,100,32),wa_None,"buf size") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect( 10, 85,100,32),wa_None,"freeze") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(120, 50,100,32),wa_None,"grain dist") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(120, 85,100,32),wa_None,"grain size") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(120,120,100,32),wa_None,"grain dur") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(120,155,100,32),wa_None,"grain pitch") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect( 10,255,100,32),wa_None,"dur env") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(120,255,100,32),wa_None,"grain env") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(230, 50,100,32),wa_None,"dist jitter") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(230,155,100,32),wa_None,"pitch jitter") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(230, 85,100,32),wa_None,"size jitter") );
          w_Panel->appendWidget( new wdgKnob(editor,axRect(230,120,100,32),wa_None,"dur jitter") );

          for (int i=0; i<w_Panel->getNumWidgets(); i++)
            editor->connect( w_Panel->getWidget(i), mParameters[i] );
          setupParameters();

        editor->doRealign();
        w_Editor = editor;
        editor->show();
        return editor;
      }

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor = NULL;
        delete editor;
        delete s_Back;
        delete s_Knob;
      }

    //virtual void doIdleEditor(void)
    //  {
    //  }

    //--------------------------------------------------
    // plugin
    //--------------------------------------------------

    virtual void doSetParameter(axParameter* aParameter)
      {
        float v = aParameter->getValue();
        switch ( aParameter->getIndex() )
        {
          case 0:   m_Master      = v*v;    break;
          case 1:   m_NumGrains   = (int)v; break;
          case 2:    _BufferSize  = v;      break;
          case 3:   m_Freeze      = v;      break;
          case 4:    _GrainDist   = v;      break;
          case 5:    _GrainSize   = v;      break;
          case 6:    _GrainDur    = v;      break;
          case 7:   m_GrainPitch  = v*v;    break;
          case 8:   m_Env         = v;      break;
          case 9:   m_GrainEnv    = v;      break;
          case 10:  m_StartJit    = v*v*v;  break;
          case 11:  m_PitchJit    = v*v*v;  break;
          case 12:  m_SizeJit     = v*v*v;  break;
          case 13:  m_DurJit      = v*v*v;  break;
        }
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        float ms  = getSampleRate() * 0.001;
        m_BufferSize = _BufferSize * ms;
        m_GrainDist  = _GrainDist  * ms;
        m_GrainSize  = _GrainSize  * ms;
        m_GrainDur   = _GrainDur   * ms;
        while (index>=(int)m_BufferSize) index -= (int)m_BufferSize;
        return false;
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];
        float in0 =  spl0;
        float in1 =  spl1;
        float out0 = 0;
        float out1 = 0;
        GRAIN* newgrain = NULL;
        GRAIN* G = GRAINS;
        // ----- update currently playing grains
        for( int i=0; i<m_NumGrains; i++ )
        {
          if( G->active==1 )
          {
            G->pos += G->speed;
            if (G->pos >= G->end) G->pos = G->start;
            if (G->pos >= m_BufferSize) G->pos -= m_BufferSize;
            //if( G->pos < 0 ) G->pos += m_BufferSize; // if we allow negative speeds
            G->phase += ( G->phase_add * 2 );
            if (G->phase >= 2) G->phase -= 2;
            G->phase2 += ( G->phase2_add * 2 );
            if (G->phase2 >= 2) G->phase2 -= 2;
            G->duration -= 1;
            if (G->duration <= 0) G->active = 0;
            float gvol = G->phase  * ( 2 - axAbs(G->phase ) );
            float dvol = G->phase2 * ( 2 - axAbs(G->phase2) );
            gvol = gvol*m_GrainEnv + (1-m_GrainEnv);
            dvol = dvol*m_Env      + (1-m_Env);
            int gpos = (int)G->pos * 2;
            out0 += BUFFER[gpos  ] * dvol * gvol;
            out1 += BUFFER[gpos+1] * dvol * gvol;
          } //on
          else if (!newgrain) newgrain = G;
          G += 1;//sizeof(GRAIN);
        } //for, numgrains
        // ----- time for a new grain?
        if( countdown <= 0 )
        {
          countdown = m_GrainDist;
          if( newgrain )
          {
            float startrnd = 1 + (m_StartJit * axRandSigned() ); // 0..2
            float pitchrnd = 1 + (m_PitchJit * axRandSigned() );
            float sizernd  = 1 + (m_SizeJit  * axRandSigned() );
            float durrnd   = 1 + (m_DurJit   * axRandSigned() );
            float siz = m_GrainSize * sizernd;
            //float st  = index * startrnd;
            float st =  index + startrnd*m_BufferSize;
            if( st >= m_BufferSize ) st -= m_BufferSize;
            if( st <  0            ) st += m_BufferSize;
            float en  = st + siz;
            if( en >= m_BufferSize ) en = m_BufferSize; // clamp
            if( en <  0           ) en = 0;
            float du = m_GrainDur * durrnd;
            newgrain->active      = 1;
            newgrain->pos         = st;
            newgrain->start       = st;
            newgrain->end         = en;
            newgrain->duration    = du;
            newgrain->speed       = m_GrainPitch * pitchrnd;
            newgrain->phase       = 0;
            newgrain->phase_add   = 1 / siz;
            newgrain->phase2      = 0;
            newgrain->phase2_add  = 1 / du;
          } //newgrain
        } //countdwn
        countdown -= 1;
        // ----- house-keeping & buffering
        int index2 = index*2;
        if( m_Freeze < 0.5 ) BUFFER[index2  ] = in0;
        if( m_Freeze < 0.5 ) BUFFER[index2+1] = in1;
        index += 1;
        if( index >= m_BufferSize ) index -= m_BufferSize;
        float mmm = m_Master*m_Master*m_Master;
        *aOutputs[0] = out0 * mmm;
        *aOutputs[1] = out1 * mmm;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
