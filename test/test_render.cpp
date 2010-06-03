//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_render.log"

//#define AX_ALPHA

#include "axFormat.h"
#include "axEditor.h"
#include "core/axRand.h"
#include "gui/axBitmapRender.h"

//----------------------------------------------------------------------

// in release-build, i can render around 200.000 lines per second
// on my laptop (in win7)

#define NUM_LINES  200000
//#define LINE_ALPHA 8

class myEditor : public axEditor
{
  private:
    axBitmap*       m_Bitmap;
    axBitmapRender* m_Render;
  public:
    myEditor(axFormat* aFormat, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aFormat,aContext,aRect,aWinFlags)
      {
        axRand(418);
        m_Bitmap = createBitmap(256,256,32);
        m_Render = new axBitmapRender();
        m_Bitmap->createBuffer();
        m_Bitmap->prepare();
        m_Render->setup(m_Bitmap);
      }
    virtual ~myEditor()
      {
        delete m_Render;
        delete m_Bitmap;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //axColor grey = aCanvas->getColor(128,192,128);
        //aCanvas->setBrushColor(grey);
        //aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
        //axEditor::doPaint(aCanvas,aRect);
        m_Render->clear();
        //m_Render->drawWuLine( 10,10,245,30,0xffffff);
        for (int i=0; i<NUM_LINES; i++)
        {
          int x1 = axRandInt(255);
          int y1 = axRandInt(255);
          int x2 = axRandInt(255);
          int y2 = axRandInt(255);
          unsigned char r = axRandInt(255);
          unsigned char g = axRandInt(255);
          unsigned char b = axRandInt(255);
          unsigned char a = axRandInt(255); // LINE_ALPHA;
          m_Render->drawWuLine(x1,y1,x2,y2,((a<<24)+(r<<16)+(g<<8)+b));
        }
        aCanvas->drawBitmap(m_Bitmap,0,0, 0,0,256,256);
      }
};

//----------------------------------------------------------------------

class myPlugin : public axFormat
{

    //----------------------------------------
    // plugin
    //----------------------------------------

  private:
    myEditor* m_Editor;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext/*, pf_HasEditor*/)
      {
        describe("test_render","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2/*,false*/);
        setupEditor(256,256);
        //appendParameter( p_Gain = new axParameter(this,"gain","") );
        setupParameters();
      }

    virtual ~myPlugin()
      {
      }

    //----------------------------------------
    // editor
    //----------------------------------------

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        myEditor* editor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        //axCanvas* canvas = editor->getCanvas();
        editor->show();
        m_Editor = editor;
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        myEditor* editor = m_Editor;
        m_Editor = NULL;
        delete editor;
      }

    //----------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
