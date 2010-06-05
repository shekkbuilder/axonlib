//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_render.log"

//#define AX_ALPHA

#include "format/axFormat.h"
#include "gui/axEditor.h"
#include "core/axRand.h"
#include "gui/axBitmapRender.h"

//----------------------------------------------------------------------

// in release-build, i can render around 200.000 lines per second
// on my laptop (in win7)

#define NUM_LINES  1000
#define NUM_POLYS  100

//#define LINE_ALPHA 8

class myEditor : public axEditor
{
  private:
    axBitmap*         m_Bitmap;
    axBitmap*         m_Texture;
    axBitmapRender*   m_Render;
    axPolyVertex      p0[4],p1[4],p2[4],p3[4];
  public:
    myEditor(axFormat* aFormat, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aFormat,aContext,aRect,aWinFlags)
      {
        axRand(418);
        m_Bitmap = createBitmap(256,256,32);
        m_Bitmap->createBuffer();
        m_Bitmap->prepare();
        m_Render = new axBitmapRender();
        m_Render->setup(m_Bitmap);

        m_Texture = createBitmap(64,64,32);
        m_Texture->createBuffer();
        m_Texture->fillBuffer(0x20ff7f00);

        //p0[0].x =  127; p0[0].y =    0; p0[0].z =  1.0; p0[0].next = &p0[1];
        //p0[1].x =  255; p0[1].y =  127; p0[1].z =  1.0; p0[1].next = &p0[2];
        //p0[2].x =  127; p0[2].y =  255; p0[2].z =    0; p0[2].next = &p0[3];
        //p0[3].x =    0; p0[3].y =  127; p0[3].z =    0; p0[3].next = NULL;

      }
    virtual ~myEditor()
      {
        delete m_Render;
        delete m_Bitmap;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int i;
        int x0,y0,z0;
        int x1,y1,z1;
        int x2,y2,z2;
        int x3,y3,z3;
        unsigned char r,g,b,a;
        //axColor grey = aCanvas->getColor(128,192,128);
        //aCanvas->setBrushColor(grey);
        //aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
        //axEditor::doPaint(aCanvas,aRect);
        m_Render->clear();

        for (i=0; i<NUM_LINES; i++)
        {
          x1 = axRandInt(255);
          y1 = axRandInt(255);
          x2 = axRandInt(255);
          y2 = axRandInt(255);
          r = axRandInt(255);
          g = axRandInt(255);
          b = axRandInt(255);
          a = axRandInt(255); // LINE_ALPHA;
          m_Render->drawLineAA(x1,y1,x2,y2,((a<<24)+(r<<16)+(g<<8)+b));
        }
        for (i=0; i<NUM_POLYS; i++)
        {
          x0 = axRandInt(200);  y0 = axRandInt(200);  z0 = axRandInt(200);
          x1 = axRandInt(200);  y1 = axRandInt(200);  z1 = axRandInt(200);
          x2 = axRandInt(200);  y2 = axRandInt(200);  z2 = axRandInt(200);
          x3 = axRandInt(200);  y3 = axRandInt(200);  z3 = axRandInt(200);
          p0[0].x =  x0; p0[0].y = y0; p0[0].z = z0; p0[0].next = &p0[1];
          p0[1].x =  x1; p0[1].y = y1; p0[1].z = z1; p0[1].next = &p0[2];
          p0[2].x =  x2; p0[2].y = y2; p0[2].z = z2; p0[2].next = &p0[3];
          p0[3].x =  x3; p0[3].y = y3; p0[3].z = z3; p0[3].next = NULL;

//          p0[1].x =  255; p0[1].y =  127; p0[1].z =  1.0; p0[1].next = &p0[2];
//          p0[2].x =  127; p0[2].y =  255; p0[2].z =    0; p0[2].next = &p0[3];
//          p0[3].x =    0; p0[3].y =  127; p0[3].z =    0; p0[3].next = NULL;

          m_Render->drawAffineTexturedPolygon( p0 ,m_Texture );
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
