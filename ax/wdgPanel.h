/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
  \file wdgPanel.h
  \brief panel container
*/

#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

#include "axWidget.h"

// panel draw modes
#define pdm_None        0
#define pdm_Background  1
#define pdm_Border      2
#define pdm_All         0xffff

//----------

class wdgPanel : public axContainer
{
  private:
  //axPen*    m_LightPen;
  //axPen*    m_DarkPen;
  //axBrush*  m_FillBrush;
    axPen*    m_BorderPen;
    axBrush*  m_BackgroundBrush;

  protected:
  //axPen*    mLightPen;
  //axPen*    mDarkPen;
  //axBrush*  mFillBrush;
    axPen*    mBorderPen;
    axBrush*  mBackgroundBrush;
    int       mDrawMode;

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
