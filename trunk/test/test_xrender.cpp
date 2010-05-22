
//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#include "axPlugin.h"
#include "axEditor.h"

#include "wdg/wdgImage.h"
#include "../extern/picopng.h"
//#include "testpng.h"
#include "testpng.h"

//----------------------------------------------------------------------
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class myImage : public wdgImage
{
  public:
    myImage(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axImage* aImage=NULL)
    : wdgImage(aListener,aRect,aAlignment,aImage)
      {


      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int dstx = mRect.x;
        int dsty = mRect.y;
        int srcx = 0;
        int srcy = 0;
        int srcw = mRect.w;
        int srch = mRect.h;


        aCanvas->setBrushColor( aCanvas->getColor(0,0,0));
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());

        aCanvas->renderImage(mImage,dstx+30,dsty+30,srcx,srcy,srcw,srch);

        aCanvas->setBrushColor( aCanvas->getColor(255,255,255) );
        aCanvas->fillCircle(10,10,200,200);
        aCanvas->setTextColor( aCanvas->getColor(255,0,255) );
        aCanvas->drawText(50,150,"dette er en test");

        aCanvas->renderImage(mImage,dstx,dsty,srcx,srcy,srcw,srch);

      }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat,
                 public axWidgetListener
{
  private:
    PNG_info_t* pnginfo;
    axEditor*   m_Editor;
    myImage*    w_Image;
    axSurface*  surface;
    axBitmap*   bitmap;
  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, /*pf_None*/AX_PLUG_DEFAULT)
      {
        describe("test_winsize","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(256,256);
        pnginfo = PNG_decode(testpng,testpng_size);
        //pnginfo = PNG_decode(png2,png2_size);
        trace("pnginfo = " << pnginfo);
        trace("png error: " << PNG_error);
      }

    virtual ~myPlugin()
      {
        png_alloc_free_all();
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        m_Editor  = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);

        int   w   = pnginfo->width;
        int   h   = pnginfo->height;
        char* buf = (char*)pnginfo->image->data;
        bitmap = m_Editor->createBitmap(w,h,32);      // 32
        bitmap->createBuffer(buf);
        bitmap->convertRgbaBgra();
        //bitmap->swizzle(
        //  1,  0,  0,  0,
        //  0,  1,  0,  0,
        //  0,  0,  1,  0,
        //  0,  0,  0,  1
        //);
        //bitmap->randomAlpha();

          int width = bitmap->getWidth();
          int height = bitmap->getHeight();
          char* buffer = bitmap->getBuffer();

          for(int y=0; y<height; y++)
          {
            for(int x=0; x<width; x++)
            {
              int pos = (y*width + x) * 4;
              unsigned char a = x;//axRandInt(255);
              buffer[pos+3] = a;
            } //for x
          } //for y

        bitmap->premultAlpha();
        bitmap->prepare();

        surface = m_Editor->createSurface(w,h,32);    // 32
        axCanvas* canvas = surface->getCanvas();
        canvas->drawBitmap(bitmap,0,0,0,0,w,h);
        delete bitmap;

        m_Editor->appendWidget( w_Image = new myImage(m_Editor,axRect(0,0,256,256),wa_None,surface) );
        m_Editor->doRealign();
        m_Editor->show();
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        delete m_Editor;
        m_Editor = NULL;
        delete surface;
        //delete bitmap;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

