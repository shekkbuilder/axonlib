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
  \file ax.h
  \brief ...
*/

#ifndef wdgPaintBox_included
#define wdgPaintBox_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgPaintBox : public axWidget
{

  private:
    axPen*      m_BorderPen;
    axBrush*    m_BackgroundBrush;
  protected:
    axPen*      mBorderPen;
    axBrush*    mBackgroundBrush;
    int         mDrawMode;
    axSurface*  mImage;

  public:

    wdgPaintBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        m_BorderPen   = new axPen(AX_GREY_DARK);
        m_BackgroundBrush = new axBrush(AX_GREY);
        mDrawMode = pdm_Background;
        mBorderPen = m_BorderPen;
        mBackgroundBrush = m_BackgroundBrush;
        mImage = NULL;
      }

    //----------

    virtual ~wdgPaintBox()
      {
        delete m_BorderPen;
        delete m_BackgroundBrush;
      }

    //----------------------------------------

    inline axPen* getBorderPen(void) { return mBorderPen; }
    inline axBrush* getBackgroundBrush(void) { return mBackgroundBrush; }
    inline int getDrawMode(void) { return mDrawMode; }

    inline void setBorderPen(axPen* aPen) { mBorderPen = aPen; }
    inline void setBackgroundBrush(axBrush* aBrush) { mBackgroundBrush = aBrush; }
    inline void setDrawMode(int aMode) { mDrawMode = aMode; }

    inline axSurface* getImage(void) { return mImage; }
    inline void setImage(axSurface* srf) { mImage = srf; }

    //----------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mDrawMode&1) // background
        {
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
        //axWidget::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
#endif
