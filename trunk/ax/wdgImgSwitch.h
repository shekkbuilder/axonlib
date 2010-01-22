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
 * \brief image based switch widget
 */

/**
 * \brief image based switch widget
 *
 * long desc
 *
 */

#ifndef wdgImgSwitch_included
#define wdgImgSwitch_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgImgSwitch : public axWidget
{
  private:
    axSurface*  mSurface;
    int         mImgWidth;
    int         mImgHeight;

  public:

    wdgImgSwitch(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, /*axParameter* aParameter=NULL,*/
                 float aValue=0, axSurface* aSurface=NULL)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mValue    = aValue;
        mSurface = aSurface;
        if(mSurface)
        {
          mImgWidth = mSurface->mWidth / 3;
          mImgHeight = mSurface->mHeight;
        }
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int num;
        if( mValue<0.5 ) num=0;
        else num=2;
        int bx = num * mImgWidth;
        int by = 0;
        aCanvas->blit(mSurface->mCanvas, mRect.x,mRect.y, bx,by,mImgWidth,mImgHeight );
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if( mValue>0.5 ) mValue=0;
        else mValue=1;
        if(mParameter) mParameter->mValue = mValue;
        mListener->onChange( this );
      }

};

//----------------------------------------------------------------------
#endif


