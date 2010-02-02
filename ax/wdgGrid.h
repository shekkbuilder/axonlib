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
  \file wdgGrid.h
  \brief ...
*/

#ifndef wdgGrid_included
#define wdgGrid_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgGrid : public axWidget
{
  private:
    axBrush*  mBackBrush;
    axPen*    mGridPen;
  //
    int       mWidth;
    int       mHeight;

  public:

    wdgGrid(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        mBackBrush = new axBrush(AX_GREY_DARK);
        mGridPen = new axPen(AX_GREY);
      }

    virtual ~wdgGrid()
      {
        delete mBackBrush;
        delete mGridPen;
      }

    //----------

    void setup(int aWidth, int aHeight)
      {
        mWidth = aWidth;
        mHeight = aHeight;
      }

    //----------

    virtual void on_ClickCell(int aX, int aY, int aB) {}

    //--------------------------------------------------
    // ..base
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mWidth>0 && mHeight>0)
        {
          int i;
          float xcell = ((float)mRect.w / (float)mWidth);
          float ycell = ((float)mRect.h / (float)mHeight);
          // background
          aCanvas->selectBrush(mBackBrush);
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2()-1,mRect.y2()-1);
          // grid
          float x = (float)mRect.x + xcell - 1;
          float y = (float)mRect.y + ycell - 1;
          aCanvas->selectPen(mGridPen);
          for (i=0; i<mWidth-1; i++ )
          {
            aCanvas->drawLine( (int)x, mRect.y, (int)x, mRect.y2() );
            x += xcell;
          } //width
          for (i=0; i<mHeight-1; i++ )
          {
            aCanvas->drawLine( mRect.x, (int)y, mRect.x2(), (int)y );
            y += ycell;
          } //height
        } //w&h>0
      }

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        float xcell  = ((float)mRect.w / (float)mWidth);
        float ycell  = ((float)mRect.h / (float)mHeight);
        int x = (int)axFloor(  (float)(aX-mRect.x) / xcell  );
        int y = (int)axFloor(  (float)(aY-mRect.y) / ycell  );
        on_ClickCell(x,y,aB);
      }

};

//----------------------------------------------------------------------
#endif
