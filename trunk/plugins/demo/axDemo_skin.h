#ifndef axDemo_skin_included
#define axDemo_skin_included
//----------------------------------------------------------------------

//#include "gui/axSkin.h"
#include "gui/axBitmapLoader.h"
#include "skins/axSkinBasic.h"

//----------------------------------------------------------------------

#define BACKGROUND_COLOR  192,192,192

//----------------------------------------------------------------------

class axDemo_skin : public axSkinBasic
{
  friend class axDemo;
  private:
    axBitmapLoader loader;
    //axBitmap*       bitmap;
    axSurface*     skinsrf;

  public:

    axDemo_skin(axCanvas* aCanvas)
    : axSkinBasic(aCanvas)
      {
        skinsrf = NULL;
      }

    virtual ~axDemo_skin()
      {
        if (skinsrf) delete skinsrf;
      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    axSurface* loadBitmap(axEditor* aEditor, unsigned char* buffer, int size/*, int w, int h, int n*/)
      {
        loader.decode(buffer,size);
        int width = loader.getWidth();
        int height = loader.getHeight();
        axBitmap* bitmap = aEditor->createBitmap(width,height);
        bitmap->createBuffer( (char*)loader.getImage() );    // create bitmap buffer & copy data
        bitmap->convertRgbaBgra();                           // -> bgr.a
        bitmap->setBackground(BACKGROUND_COLOR/*128,128,128*/);                  // replace alpha (bgr)
        bitmap->prepare();                                   // prepare bitmap for blitting
        axSurface* surface = aEditor->createSurface(width,height);
        axCanvas* canvas = surface->getCanvas();
        canvas->drawBitmap(bitmap,0,0,0,0,width,height);     // upload /blit) to surface
        delete bitmap;
        return surface;
      }
    //--------------------------------------------------
    //
    //--------------------------------------------------

    void loadSkinBitmap(axEditor* aEditor, unsigned char* buffer, int size)
      {
        skinsrf = loadBitmap(aEditor,buffer,size);
      }


    //--------------------------------------------------
    // draw
    //--------------------------------------------------

//    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
//      {
//      }
//
//      //----------
//
//    virtual void drawSizer(axCanvas* aCanvas, axRect aRect)
//      {
//      }
//
//      //----------
//
//    virtual void drawLabel(axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign)
//      {
//      }
//
//      //----------
//
//    virtual void drawValue(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue)
//      {
//      }
//
//      //----------
//
//    virtual void drawButton(axCanvas* aCanvas, axRect aRect,  axString aText, int aTextAlign, bool aState)
//      {
//      }
//
//      //----------
//
//    virtual void drawSlider(axCanvas* aCanvas, axRect aRect,  float aValue, axString aText1, axString aText2, bool aVertical)
//      {
//      }
//
//      //----------
//
//    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue)
//      {
//      }
//
//      //----------
//
//    virtual void drawScrollBar(axCanvas* aCanvas, axRect aRect,  float aValue, bool aVertical, float aThumbSize)
//      {
//      }

};

//----------------------------------------------------------------------
#endif
