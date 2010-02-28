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
 * \file wdgImage.h
 * \brief image widget
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





