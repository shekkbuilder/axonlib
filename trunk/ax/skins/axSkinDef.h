#ifndef axSkinDef_included
#define axSkinDef_included
//----------------------------------------------------------------------

#include "gui/axSkin.h"
//#include "gui/axBitmapLoader.h"
#include "img/skin1.h"
#include "img/knob1.h"

class axSkinDef : public axSkin
{
  private:
  //public:

    axBitmap*   m_Bitmap;
    axSurface*  m_SkinSrf;
    axSurface*  m_KnobSrf;
    axColor     m_BackCol;
    axColor     m_TextCol;
    axColor     m_ValueCol;
    axRect      m_SliderRect, m_ThumbRect;
    int         m_KnobWidth,m_KnobHeight,m_KnobCount;

  public:

    axSkinDef(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        m_KnobWidth = 32;
        m_KnobHeight = 32;
        m_KnobCount = 65;
      }

//    virtual ~axSkinDef()
//      {
//      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    void setup(axEditor* aEditor, char* aSkinBuf, char* aKnobBuf)
      {
        axCanvas* canvas = aEditor->getCanvas();

        // skin

        m_Bitmap = aEditor->createBitmap(256,256,32);
        m_SkinSrf = aEditor->createSurface(256,256,32);
        m_Bitmap->createBuffer(aSkinBuf);
        m_Bitmap->convertRgbaBgra();
        m_Bitmap->premultAlpha();
        m_Bitmap->prepare();
        m_SkinSrf->getCanvas()->drawBitmap( m_Bitmap, 0,0, 0,0,256,256 );
        delete m_Bitmap;

        // knob

        m_Bitmap  = aEditor->createBitmap(32,(32*65),32);
        m_KnobSrf = aEditor->createSurface(32,(32*65),32);
        m_Bitmap->createBuffer(aKnobBuf);
        m_Bitmap->convertRgbaBgra();
        m_Bitmap->premultAlpha();
        m_Bitmap->prepare();
        m_KnobSrf->getCanvas()->drawBitmap( m_Bitmap, 0,0, 0,0,32,(32*65) );
        delete m_Bitmap;

        m_BackCol  = canvas->getColor(0x5c,0x64,0x6a);
        m_ValueCol  = canvas->getColor(192,192,192);
        m_SliderRect  = axRect(0,0,16,64);
        m_ThumbRect = axRect(0,64,16,11);

      }

    //--------------------------------------------------
    //
    // draw...
    //
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(m_BackCol);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());

      }

    //virtual void drawLabel(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign)
    //  {
    //    aCanvas->setTextColor(m_TextCol);
    //    aCanvas->drawText(aRect.x,aRect.y,aRect.x2(),aRect.y2(),aText,aTextAlign);
    //  }

    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical)
      {
        if (aVertical)
        {
          float h = (float)(aRect.h - m_ThumbRect.h);
          int y   = aRect.y + (int)(h*(1-aValue));
          aCanvas->renderImage(m_SkinSrf, aRect.x, aRect.y, m_SliderRect.x, m_SliderRect.y, m_SliderRect.x2(), m_SliderRect.y2());
          aCanvas->renderImage(m_SkinSrf, aRect.x, y, m_ThumbRect.x, m_ThumbRect.y, m_ThumbRect.x2(), m_ThumbRect.y2());
        }
        else
        {
        }
      }

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect,  axString aName, axString aDisp, float aValue)
      {
        if (m_KnobSrf)
        {
          // bitmap
          int index = (int)axFloor(aValue*m_KnobCount);
          index = axMinInt(index,m_KnobCount-1);
          int ky = m_KnobHeight * index;
          aCanvas->renderImage(m_KnobSrf,aRect.x,aRect.y, 0,ky,m_KnobWidth,m_KnobHeight);
          int x  = aRect.x;
          int y  = aRect.y;
          int size = axMinInt(aRect.w,aRect.h);
          int th = aCanvas->textHeight("Xj");
          aCanvas->setTextColor(mTextColor);
          aCanvas->drawText(x+size+8,y,   aRect.x2(),aRect.y2(),aName,ta_Top|ta_Left);
          aCanvas->drawText(x+size+8,y+th,aRect.x2(),aRect.y2(),aDisp,ta_Top|ta_Left);
        }
      }
};

//----------------------------------------------------------------------
#endif
