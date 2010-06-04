#ifndef axDemo_skin_included
#define axDemo_skin_included
//----------------------------------------------------------------------

//#include "gui/axSkin.h"
#include "gui/axBitmapLoader.h"

#include "skins/axSkinBasic.h"
#include "skins/axSkinDef.h"

//----------------------------------------------------------------------

#define BACKGROUND_COLOR  192,192,192

//----------------------------------------------------------------------

class axDemo_skin : public axSkinDef
{
  friend class axDemo;
  private:
    //axBitmapLoader m_Loader;
    //axSurface*     m_SkinSrf;

    axRect  mPanelTopLeftRect;
    axRect  mPanelTopRect;
    axRect  mPanelTopRightRect;
    axRect  mPanelLeftRect;
    axRect  mPanelCenterRect;
    axRect  mPanelRightRect;
    axRect  mPanelBottomLeftRect;
    axRect  mPanelBottomRect;
    axRect  mPanelBottomRightRect;

    axRect  mSliderVBackRect;
    axRect  mSliderVThumbRect;

  public:

    axDemo_skin(axCanvas* aCanvas)
    : axSkinDef(aCanvas)
      {
        //m_SkinSrf = NULL;
        mPanelBackCol = aCanvas->getColor(128,128,128);
        mPanelTopLeftRect     = axRect( 0, 0,8,8);
        mPanelTopRect         = axRect( 8, 0,8,8);
        mPanelTopRightRect    = axRect(16, 0,8,8);
        mPanelLeftRect        = axRect( 0, 8,8,8);
        mPanelCenterRect      = axRect( 0, 8,8,8);
        mPanelRightRect       = axRect(16, 8,8,8);
        mPanelBottomLeftRect  = axRect( 0,16,8,8);
        mPanelBottomRect      = axRect( 8,16,8,8);
        mPanelBottomRightRect = axRect(16,16,8,8);

        mTextColor        = aCanvas->getColor(AX_WHITE);
        //mButtonOffRect    = axRect(16, 0,16,16);
        //mButtonOnRect     = axRect(16,16,16,16);
        mKnobWidth        = 32;
        mKnobHeight       = 32;
        mKnobCount        = 65;
        mKnobTextCol      = aCanvas->getColor(0,0,0);
        mKnobValCol       = aCanvas->getColor(255,255,255);
        mSliderVBackRect  = axRect( 24,  0,16,64);
        mSliderVThumbRect = axRect( 24, 64,16,11);

      }

    virtual ~axDemo_skin()
      {
        //if (m_SkinSrf) delete m_SkinSrf;
      }

    //--------------------------------------------------

    //axSurface* getSurface(void) { return m_SkinSrf; }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

//    axSurface* loadBitmap(axEditor* aEditor, unsigned char* aBuffer, int aSize)
//      {
//        m_Loader.decodePng(aBuffer,aSize);
//        int width = m_Loader.getWidth();
//        int height = m_Loader.getHeight();
//        axBitmap* bitmap = aEditor->createBitmap(width,height,24);
//        bitmap->createBuffer( (char*)m_Loader.getImage() );    // create bitmap buffer & copy data
//        bitmap->convertRgbaBgra();                           // -> bgr.a
//        bitmap->setBackground(BACKGROUND_COLOR/*128,128,128*/);                  // replace alpha (bgr)
//        bitmap->prepare();                                   // prepare bitmap for blitting
//        axSurface* surface = aEditor->createSurface(width,height,24);
//        axCanvas* canvas = surface->getCanvas();
//        canvas->drawBitmap(bitmap,0,0,0,0,width,height);     // upload /blit) to surface
//        delete bitmap;
//        return surface;
//      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

//    void loadSkinBitmap(axEditor* aEditor, unsigned char* buffer, int size)
//      {
//        m_SkinSrf = loadBitmap(aEditor,buffer,size);
//      }


    //--------------------------------------------------
    // draw
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect, int aMode=0)
      {
        aCanvas->setBrushColor( mPanelBackCol );
        aCanvas->fillRect(aRect.x    + mPanelLeftRect.w,
                          aRect.y    + mPanelTopRect.h,
                          aRect.x2() - mPanelRightRect.w,
                          aRect.y2() - mPanelBottomRect.h );
        aCanvas->renderImage( mSkinSrf,
                              aRect.x,aRect.y,
                              mPanelTopLeftRect.x,mPanelTopLeftRect.y,mPanelTopLeftRect.w,mPanelTopLeftRect.h);
        aCanvas->renderImage( mSkinSrf,
                              aRect.x2()-mPanelTopRightRect.w ,aRect.y,
                              mPanelTopRightRect.x,mPanelTopRightRect.y,mPanelTopRightRect.w,mPanelTopRightRect.h);
        aCanvas->renderImage( mSkinSrf,
                              aRect.x, aRect.y2()-mPanelTopRightRect.h,
                              mPanelBottomLeftRect.x,mPanelBottomLeftRect.y,mPanelBottomLeftRect.w,mPanelBottomLeftRect.h);
        aCanvas->renderImage( mSkinSrf,
                              aRect.x2()-mPanelBottomRightRect.w ,aRect.y2()-mPanelBottomRightRect.h,
                              mPanelBottomRightRect.x,mPanelBottomRightRect.y,mPanelBottomRightRect.w,mPanelBottomRightRect.h);

        int x = aRect.x + mPanelTopLeftRect.w;
        int w  = mPanelTopRect.w;
        int wleft = aRect.w - mPanelTopLeftRect.w - mPanelTopRightRect.w;
        while (wleft>=w)
        {
          aCanvas->renderImage( mSkinSrf,
                                x, aRect.y,
                                mPanelTopRect.x,mPanelTopRect.y,w,mPanelLeftRect.h );
          aCanvas->renderImage( mSkinSrf,
                                x, aRect.y2()-mPanelBottomRect.h,
                                mPanelBottomRect.x,mPanelBottomRect.y,w,mPanelBottomRect.w );
          x += w;
          wleft -= w;
        }
        if (wleft>0)
        {
          aCanvas->renderImage( mSkinSrf,
                                x, aRect.y,
                                mPanelTopRect.x,mPanelTopRect.y,wleft,mPanelLeftRect.h );
          aCanvas->renderImage( mSkinSrf,
                                x, aRect.y2()-mPanelBottomRect.h,
                                mPanelBottomRect.x,mPanelBottomRect.y,wleft,mPanelBottomRect.w );
        }

        int y = aRect.y + mPanelTopLeftRect.h;
        int h  = mPanelLeftRect.h;
        int hleft = aRect.h - mPanelTopLeftRect.h - mPanelBottomLeftRect.h;
        while (hleft>=h)
        {
          aCanvas->renderImage( mSkinSrf,
                                aRect.x, y,
                                mPanelLeftRect.x,mPanelLeftRect.y,mPanelLeftRect.w, h );
          aCanvas->renderImage( mSkinSrf,
                                aRect.x2()-mPanelRightRect.w, y,
                                mPanelRightRect.x,mPanelRightRect.y,mPanelRightRect.w, h );
          y += h;
          hleft -= h;
        }
        if (hleft>0)
        {
          aCanvas->renderImage( mSkinSrf,
                                aRect.x, y,
                                mPanelLeftRect.x,mPanelLeftRect.y,mPanelLeftRect.w,hleft);
          aCanvas->renderImage( mSkinSrf,
                                aRect.x2()-mPanelRightRect.w, y,
                                mPanelRightRect.x,mPanelRightRect.y,mPanelRightRect.w, hleft );
        }
      }

    //----------

};

//----------------------------------------------------------------------
#endif
