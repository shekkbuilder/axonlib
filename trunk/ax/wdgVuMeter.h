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
 * \brief vu meter widget
 */

/**
 * \brief vu meter widget
 *
 * long desc
 *
 */

#ifndef wdgVuMeter_included
#define wdgVuMeter_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgVuMeter : public axWidget
{
  public:
    float mLevel;
  public:

    wdgVuMeter(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mLevel = 0;
        clearFlag(wfl_Active);
        setFlag(wfl_Vertical);
      }
    virtual ~wdgVuMeter()
      {
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setPenColor( AX_GREY_LIGHT );
        aCanvas->drawRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
        aCanvas->setBrushColor( AX_GREY_LIGHT );
        if (hasFlag(wfl_Vertical))
        {
          float h = (float)(mRect.h-4) * mLevel;
          int y2 = mRect.y2() - 2;
          int y1 = y2 - h;
          aCanvas->fillRect(mRect.x+2, y1, mRect.x2()-2, y2 );
        }
        else
        {
          float w = (float)(mRect.w-4) * mLevel;
          int x2 = mRect.x2() - 2;
          int x1 = x2 - w;
          aCanvas->fillRect(x1,mRect.y+2, x2, mRect.y2()-2 );
        }
      }
};

//----------------------------------------------------------------------
#endif


