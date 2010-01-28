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

class wdgPanel : public axContainer
{
  protected:
    axPen*    mPenLight;
    axPen*    mPenDark;
    axBrush*  mBrushFill;

  public:

    wdgPanel(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axContainer(aListener,aID,aRect,aAlignment)
      {
        mPenLight  = new axPen(AX_GREY_LIGHT);
        mPenDark   = new axPen(AX_GREY_DARK);
        mBrushFill = new axBrush(AX_GREY);
        //clearFlag(wfl_DefaultDraw);
      }

    virtual ~wdgPanel()
      {
        delete mPenLight;
        delete mPenDark;
        delete mBrushFill;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        // visible?
        {
          //  _
          // |
          //
          aCanvas->selectPen(mPenLight);
          aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x2(),mRect.y   );
          aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x,   mRect.y2());
          //
          // _|
          //
          aCanvas->selectPen(mPenDark);
          aCanvas->drawLine(mRect.x2(),mRect.y,   mRect.x2(),mRect.y2());
          aCanvas->drawLine(mRect.x,   mRect.y2(),mRect.x2(),mRect.y2());
          // fill
          aCanvas->clearPen();
          aCanvas->selectBrush(mBrushFill);
          aCanvas->fillRect(mRect.x+1, mRect.y+1, mRect.x2()-1, mRect.y2()-1);
        }
        axContainer::doPaint(aCanvas,aRect);
      }

};

//----------------------------------------------------------------------
#endif
