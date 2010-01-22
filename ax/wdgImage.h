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

    wdgImage(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, /*axParameter* aParameter=NULL,*/ axSurface* aSurface=NULL)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
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
          axRect R = mRect;
          R.crop( aRect.x, aRect.y, aRect.w, aRect.h );
          //TRACE("x:%i  y:%i  w:%i  h:%i\n",R.x,R.y,R.w,R.h);
          if(R.w>0 && R.h>0) aCanvas->blit( mSurface->mCanvas, R.x,R.y, R.x,R.y,R.w,R.h );
        }
      }

};

//----------------------------------------------------------------------
#endif





