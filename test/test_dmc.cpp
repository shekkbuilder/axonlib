#include "format/axFormat.h"
#include "par/parInteger.h"
#include "par/parFloat.h"

#include "gui/axEditor.h"
#include "gui/axBitmapLoader.h"
#include "wdg/wdgBitmap.h"

#include "core/axCompress.h"

#define AX_ALPHA

//#define TEST_COMPRESSION
#define NUMSHIFT 0
#define BUFFERSIZE (1024*1024*4)

unsigned char cbuf[BUFFERSIZE];
unsigned char ebuf[BUFFERSIZE];
unsigned char tbuf[BUFFERSIZE];

//---------------------------------------------------------------------

class test_dmc : public axFormat
{
  private:
  private:
    axEditor*       w_Editor;
    wdgBitmap*      w_Bitmap;
    axBitmap*       m_Bitmap;
    axBitmapLoader  m_Png;
    #ifdef TEST_COMPRESSION
    axCompress      m_Dmc;
    #endif

  //--------------------------------------------------
  #ifdef TEST_COMPRESSION
  private:
    void fill_layer(char* buffer, int size, int layer, unsigned char value=0)
      {
        unsigned char* wb = (unsigned char*)buffer;
        int n = size/4;
        for (int i=0;i<n;i++) { wb[i*4+layer] = value; }
      }
    void shift_layers(char* buffer, int size, int shift)
      {
        unsigned char* wb = (unsigned char*)buffer;
        int n = size/4;
        if (shift>0)
        {
          for (int i=0;i<n;i++)
          {
            wb[i*4+0] <<= shift;
            wb[i*4+1] <<= shift;
            wb[i*4+2] <<= shift;
            wb[i*4+3] <<= shift;
          }
        }
        else
        {
          shift = -shift;
          for (int i=0;i<n;i++)
          {
            wb[i*4+0] >>= shift;
            wb[i*4+1] >>= shift;
            wb[i*4+2] >>= shift;
            wb[i*4+3] >>= shift;
          }
        }
      }
  #endif
  //--------------------------------------------------

  public:

    test_dmc(axContext* aContext)
    : axFormat(aContext)
      {
        w_Editor = NULL;
        describe("test_dmc","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        //setupEditor(256,256);
        setupEditor(456,108);

        //int res = m_Png.decodeLoadPng("img/ccernn2.png");
        int res = m_Png.decodeLoadPng("background.png");
        trace("decode = " << res);
        trace("width: "   << m_Png.getWidth());
        trace("height: "  << m_Png.getHeight());
        trace("image: "   << hex << (unsigned int)m_Png.getImage() << dec);

        //--------------------------------------------------
        #ifdef TEST_COMPRESSION
        char* img = (char*)m_Png.getImage();
        int size  = m_Png.getWidth() * m_Png.getHeight() * 4;
        axMemcpy(tbuf,img,size);                      trace("original size: " << size);
        //fill_layer((char*)tbuf,size,3,255);
        shift_layers((char*)tbuf,size,-NUMSHIFT);
        m_Dmc.split_layers((char*)tbuf,size);
        m_Dmc.delta_encode((char*)tbuf,size);
        int csize = m_Dmc.compress(tbuf,size,cbuf);   trace("compressed size: " << csize); // ccernn2.png = 101721, dmc = 98326, 92457 w/o alpha
        axFileWrite("img/ccernn2.dmc",(char*)cbuf,csize);
        int esize = m_Dmc.expand(cbuf,csize,ebuf);    trace("expanded size: " << esize);
        m_Dmc.delta_decode((char*)ebuf,size);
        m_Dmc.join_layers((char*)ebuf,size);
        shift_layers((char*)ebuf,size,NUMSHIFT);
        #endif
        //--------------------------------------------------

      }

    virtual ~test_dmc()
      {
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( w_Bitmap = new wdgBitmap(editor,NULL_RECT,wa_Client));
        //editor->setBorders(10,10,5,5);
        editor->doRealign();
        //transferParameters();

        //m_Bitmap = editor->createBitmap(256,256,24);    // !!!!!
        m_Bitmap = editor->createBitmap(456,108,24);    // !!!!!

        #ifdef TEST_COMPRESSION
        //m_Bitmap->createBuffer((char*)tbuf);
        m_Bitmap->createBuffer((char*)ebuf);
        #else
        m_Bitmap->createBuffer((char*)m_Png.getImage());
        #endif
        m_Bitmap->convertRgbaBgra();
        m_Bitmap->setBackground(255,255,255);
        m_Bitmap->prepare();

        w_Bitmap->setBitmap(m_Bitmap);
        editor->show();
        w_Editor = editor;
        return editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor->hide();
        w_Editor = NULL;
        delete editor;
        delete m_Bitmap;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(test_dmc)
