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
 * \brief image widget
 */

/**
 * \brief image widget
 *
 * long desc
 *
 */

#ifndef wdgImage_included
#define wdgImage_included
//----------------------------------------------------------------------

#include "axWidget.h"
#include "axContainer.h"
#include "axSurface.h"

class wdgImage : public axWidget
{
  protected:
    axSurface* mSurface;

  public:

    wdgImage(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, axSurface* aSurface=NULL)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        clearFlag(wfl_Active);
        mSurface = aSurface;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if(hasFlag(wfl_Visible))
        {
          if (mRect.w>0 && mRect.h>0) aCanvas->blit( mSurface->mCanvas, mRect.x,mRect.y,0,0, mRect.w,mRect.h);
        }
      }

};

//----------------------------------------------------------------------
#endif





