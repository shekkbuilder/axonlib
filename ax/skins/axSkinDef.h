#ifndef axSkinDef_included
#define axSkinDef_included
//----------------------------------------------------------------------

#include "gui/axSkin.h"
//#include "gui/axBitmapLoader.h"
//#include "img/skin1.h"
//#include "img/knob1.h"

class axSkinDef : public axSkin
{
  private:
  //public:
    bool        m_SkinSrfLoaded;
    bool        m_KnobSrfLoaded;
    axSurface*  m_SkinSrf;
    axColor     m_PanelBackCol;
    axRect      m_ButtonOffRect;
    axRect      m_ButtonOnRect;
    axRect      m_Button2OffRect;
    axRect      m_Button2OnRect;
    axSurface*  m_KnobSrf;
    int         m_KnobWidth;
    int         m_KnobHeight;
    int         m_KnobCount;
    axColor     m_KnobTextCol;
    axColor     m_KnobValCol;
    axRect      m_SliderBackRect;
    axRect      m_SliderThumbRect;
    axRect      m_Slider2BackRect;
    axRect      m_Slider2ThumbRect;

  public:

    axSkinDef(axCanvas* aCanvas)
    : axSkin(aCanvas)
      {
        m_SkinSrfLoaded     = false;
        m_KnobSrfLoaded     = false;
        m_SkinSrf           = NULL;
        m_PanelBackCol      = aCanvas->getColor(0x5c,0x64,0x6a);
        m_ButtonOffRect     = axRect(16, 0,16,16);
        m_ButtonOnRect      = axRect(16,16,16,16);
        m_Button2OffRect    = axRect(32, 0,16,16);
        m_Button2OnRect     = axRect(32,16,16,16);
        m_KnobSrf           = NULL;
        m_KnobWidth         = 32;
        m_KnobHeight        = 32;
        m_KnobCount         = 65;
        m_KnobTextCol       = aCanvas->getColor(0,0,0);
        m_KnobValCol        = aCanvas->getColor(255,255,255);
        m_SliderBackRect    = axRect( 0,  0,16,64);
        m_SliderThumbRect   = axRect( 0, 64,16,11);
        m_Slider2BackRect   = axRect( 0, 80,16,64);
        m_Slider2ThumbRect  = axRect( 0,144,16,11);
      }

    //----------

    virtual ~axSkinDef()
      {
        if (m_SkinSrfLoaded) delete m_SkinSrf;
        if (m_KnobSrfLoaded) delete m_KnobSrf;
      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    // be careful about this....
    // be sure we copy all fields!!!

    void copyFrom(axSkinDef* aSkin)
      {
        // set to false, so we don't try to delete the surface in the desctructor
        // (will be done in the 'original' skin)
        m_SkinSrfLoaded     = false;
        m_KnobSrfLoaded     = false;
        m_SkinSrf           = aSkin->m_SkinSrf;
        m_KnobSrf           = aSkin->m_KnobSrf;
        m_PanelBackCol      = aSkin->m_PanelBackCol;
        m_ButtonOffRect     = aSkin->m_ButtonOffRect;
        m_ButtonOnRect      = aSkin->m_ButtonOnRect;
        m_Button2OffRect    = aSkin->m_Button2OffRect;
        m_Button2OnRect     = aSkin->m_Button2OnRect;
        m_KnobWidth         = aSkin->m_KnobWidth;
        m_KnobHeight        = aSkin->m_KnobHeight;
        m_KnobCount         = aSkin->m_KnobCount;
        m_KnobTextCol       = aSkin->m_KnobTextCol;
        m_KnobValCol        = aSkin->m_KnobValCol;
        m_SliderBackRect    = aSkin->m_SliderBackRect;
        m_SliderThumbRect   = aSkin->m_SliderThumbRect;
        m_Slider2BackRect   = aSkin->m_Slider2BackRect;
        m_Slider2ThumbRect  = aSkin->m_Slider2ThumbRect;
      }

    //----------

    void loadSkinBitmap(axEditor* aEditor, char* aBuffer)
      {
        m_SkinSrfLoaded = true;
        axBitmap* bitmap;
        bitmap = aEditor->createBitmap(256,256,32);
        m_SkinSrf = aEditor->createSurface(256,256,32);
        bitmap->createBuffer(aBuffer);
        bitmap->convertRgbaBgra();
        bitmap->premultAlpha();
        bitmap->prepare();
        m_SkinSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,256,256 );
        delete bitmap;
      }

    //----------

    void loadKnobBitmap(axEditor* aEditor, char* aBuffer)
      {
        m_KnobSrfLoaded = true;
        axBitmap* bitmap;
        bitmap  = aEditor->createBitmap(32,(32*65),32);
        m_KnobSrf = aEditor->createSurface(32,(32*65),32);
        bitmap->createBuffer(aBuffer);
        bitmap->convertRgbaBgra();
        bitmap->premultAlpha();
        bitmap->prepare();
        m_KnobSrf->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,32,(32*65) );
        delete bitmap;
      }

    //--------------------------------------------------
    //
    // draw...
    //
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(m_PanelBackCol);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());

      }

    //----------

    //virtual void drawLabel(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign)
    //  {
    //    aCanvas->setTextColor(m_TextCol);
    //    aCanvas->drawText(aRect.x,aRect.y,aRect.x2(),aRect.y2(),aText,aTextAlign);
    //  }

    //----------

    virtual void drawButton(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, bool aState)
      {
        switch (mVariation)
        {
          case 0:
            if (aState) aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_ButtonOnRect.x, m_ButtonOnRect.y, m_ButtonOnRect.w, m_ButtonOnRect.h );
                   else aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_ButtonOffRect.x,m_ButtonOffRect.y,m_ButtonOffRect.w,m_ButtonOffRect.h);
            break;
          case 1:
            if (aState) aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_Button2OnRect.x, m_Button2OnRect.y, m_Button2OnRect.w, m_Button2OnRect.h );
                   else aCanvas->renderImage(m_SkinSrf, aRect.x,aRect.y, m_Button2OffRect.x,m_Button2OffRect.y,m_Button2OffRect.w,m_Button2OffRect.h);
            break;
        }
      }

    //----------

    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical)
      {
        float h;
        int y;
        //trace("drawSlider");
        if (aVertical)
        {
          switch (mVariation)
          {
            case 0:
              h = (float)(aRect.h - m_SliderThumbRect.h);
              y   = aRect.y + (int)(h*(1-aValue));
              aCanvas->renderImage(m_SkinSrf, aRect.x, aRect.y, m_SliderBackRect.x, m_SliderBackRect.y, m_SliderBackRect.w, m_SliderBackRect.h );
              aCanvas->renderImage(m_SkinSrf, aRect.x, y,       m_SliderThumbRect.x,m_SliderThumbRect.y,m_SliderThumbRect.w,m_SliderThumbRect.h);
              break;
            case 1:
              h = (float)(aRect.h - m_Slider2ThumbRect.h);
              y   = aRect.y + (int)(h*(1-aValue));
              aCanvas->renderImage(m_SkinSrf, aRect.x, aRect.y, m_Slider2BackRect.x, m_Slider2BackRect.y, m_Slider2BackRect.w, m_Slider2BackRect.h );
              aCanvas->renderImage(m_SkinSrf, aRect.x, y,       m_Slider2ThumbRect.x,m_Slider2ThumbRect.y,m_Slider2ThumbRect.w,m_Slider2ThumbRect.h);
              break;
          }
        }
        else
        {
        }
      }

    //----------

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
