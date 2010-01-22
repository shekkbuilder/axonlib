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
 * @file
 * \brief resizer widget
 */

/**
 * \brief resizer widget
 *
 * long desc
 *
 */

#ifndef wdgResizer_included
#define wdgResizer_included
//----------------------------------------------------------------------

#include "axWidget.h"
#include "axContainer.h"
#include "axSurface.h"

class wdgResizer : public axWidget
{
  private:
    char    hint[256];

  protected:
    //axSurface* mSurface;
    bool is_dragging;
    int prevx, prevy;
    //int sizerx,sizery;

  public:

    wdgResizer(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment/*,axParameter* aParameter=NULL,*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        is_dragging = false;
        // assume lower right corner
        //clearFlag(wfl_Active);
        //mSurface = aSurface;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //TRACE("wdgResizer.doPaint\n");
        if(hasFlag(wfl_Visible))
        {
          aCanvas->setBrushColor( AX_GREY_DARK );
          aCanvas->fillRect( mRect.x,mRect.y,mRect.x2(),mRect.y2());
        }
      }

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        is_dragging = true;
        prevx = aX;
        prevy = aY;
        mListener->onCursor(cuMove);

        //sizerx = mRect.x;
        //sizery = mRect.y;
      }

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        is_dragging = false;
        mListener->onCursor(DEF_CURSOR);
      }

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if (is_dragging)
        {
          int deltax = aX - prevx;
          int deltay = aY - prevy;
          mListener->onResize(this,deltax,deltay);
          prevx = aX;
          prevy = aY;
          sprintf(hint,"<resizing>");
          mListener->onHint(hint);
        }
      }

};

//----------------------------------------------------------------------
#endif





