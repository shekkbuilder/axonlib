/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0 
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See LICENSE_AX for more details.
 *  
 * You should have received a copy of the Axonlib License 
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file wdgPanel.h
  \brief panel container
*/

#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgPanel : public axContainer
{
  private:
  //axPen*      m_LightPen;
  //axPen*      m_DarkPen;
  //axBrush*    m_FillBrush;
    axPen*      m_BorderPen;
    axBrush*    m_BackgroundBrush;

  protected:
  //axPen*      mLightPen;
  //axPen*      mDarkPen;
  //axBrush*    mFillBrush;
    axPen*      mBorderPen;
    axBrush*    mBackgroundBrush;
    int         mDrawMode;
    axSurface*  mImage;

  public:

    wdgPanel(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axContainer(aListener,aID,aRect,aAlignment)
      {
        //m_LightPen  = new axPen(AX_GREY_LIGHT);
        //m_DarkPen   = new axPen(AX_GREY_DARK);
        //m_FillBrush = new axBrush(AX_GREY);
        m_BorderPen   = new axPen(AX_GREY_DARK);
        m_BackgroundBrush = new axBrush(AX_GREY);
        mDrawMode = pdm_Background;
        //mLightPen   = m_LightPen;
        //mDarkPen    = m_DarkPen;
        //mFillBrush  = m_FillBrush;
        mBorderPen = m_BorderPen;
        mBackgroundBrush = m_BackgroundBrush;
      }

    virtual ~wdgPanel()
      {
        //delete m_LightPen;
        //delete m_DarkPen;
        //delete m_FillBrush;
        delete m_BorderPen;
        delete m_BackgroundBrush;
      }

    //--------------------------------------------------

    //inline axPen* getLightPen(void) { return mLightPen; }
    //inline axPen* getDarkPen(void) { return mDarkPen; }
    //inline axBrush* getFillBrush(void) { return mFillBrush; }

    //inline void setLightPen(axPen* aPen) { mLightPen = aPen; }
    //inline void setDarkPen(axPen* aPen) { mDarkPen = aPen; }
    //inline void setFillBrush(axBrush* aBrush) { mFillBrush = aBrush; }

    inline axPen* getBorderPen(void) { return mBorderPen; }
    inline axBrush* getBackgroundBrush(void) { return mBackgroundBrush; }
    inline int getDrawMode(void) { return mDrawMode; }

    inline void setBorderPen(axPen* aPen) { mBorderPen = aPen; }
    inline void setBackgroundBrush(axBrush* aBrush) { mBackgroundBrush = aBrush; }
    inline void setDrawMode(int aMode) { mDrawMode = aMode; }

    inline axSurface* getImage(void) { return mImage; }
    inline void setImage(axSurface* srf) { mImage = srf; }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
//        // visible?
//        //if (hasFlag(wfl_DefaultDraw))
//        {
//          //if (hasFlag(wfl_DefaultDraw))
//          {
//            //  _
//            // |
//            //
//            aCanvas->selectPen(mLightPen);
//            aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x2(),mRect.y   );
//            aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x,   mRect.y2());
//            //
//            // _|
//            //
//            aCanvas->selectPen(mDarkPen);
//            aCanvas->drawLine(mRect.x2(),mRect.y,   mRect.x2(),mRect.y2());
//            aCanvas->drawLine(mRect.x,   mRect.y2(),mRect.x2(),mRect.y2());
//          }
//          aCanvas->clearPen();
//          aCanvas->selectBrush(mFillBrush);
//          aCanvas->fillRect(mRect.x+1, mRect.y+1, mRect.x2()-1, mRect.y2()-1);
//        }
        if (mDrawMode&1) // background
        {
          //aCanvas->clearPen();
          aCanvas->selectBrush(mBackgroundBrush);
          aCanvas->fillRect(mRect.x, mRect.y, mRect.x2(), mRect.y2());
        }
        if (mDrawMode&4) // image
        {
          //if (mRect.w>0 && mRect.h>0)
          aCanvas->blit( mImage->mCanvas, mRect.x,mRect.y,0,0, mRect.w,mRect.h);

        }
        if (mDrawMode&2) // border
        {
          aCanvas->selectPen(mBorderPen);
          aCanvas->drawRect(mRect.x, mRect.y,mRect.x2(),mRect.y2());
        }
        axContainer::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
#endif
