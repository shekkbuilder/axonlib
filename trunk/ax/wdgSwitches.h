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
 * \brief switches widget
 */

/**
 * \brief switches widget
 *
 * long desc
 *
 */

#ifndef wdgSwitches_included
#define wdgSwitches_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axWidget.h"

class wdgSwitches : public axWidget
{
  public:
    int       mNum;
    char**    mStrings;
    int       mSelected;
    axColor   mOffColor;
    axColor   mOnColor;

  public:

    wdgSwitches(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        setBackground(true,AX_GREY_DARK);
        mNum      = 0;
        mStrings  = NULL;
        mSelected = 0;
        mOffColor = AX_GREY;
        mOnColor  = AX_GREY_LIGHT;
      }

    //virtual ~wdgSwitches() {}

    //--------------------------------------------------

    void setup(int aNum, char** aStrings)
      {
        mNum = aNum;
        mStrings = aStrings;
      }

    void setNum(int i)
      {
        mSelected = i;
        float siz = 1.0f/(float)mNum;
        mValue = (float)mSelected *siz + (siz*0.5);
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doSetValue(float aValue)
      {
        mValue = aValue;
        //if( aValue >= 0.5 ) mState=true;
        //else mState=false;
        int n = (int)axFloor( aValue * (float)mNum );
        mSelected = axMinInt( mNum-1, n );
      }

    //----------

    virtual float doGetValue(void)
      {
        return (float)mSelected / (float)mNum;
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas,aRect);
        int x = mRect.x;
        int w = mRect.w / mNum;
        for( int i=0; i<mNum; i++ )
        {
          if( i==mSelected ) aCanvas->setTextColor( mOnColor);
          else aCanvas->setTextColor( mOffColor);
          aCanvas->drawText( x, mRect.y, x+w-1, mRect.y2(), mStrings[i],tal_Center);
          x += w;
        }
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        int x = aX - mRect.x;   // x = 0..mRect.w
        doSetValue( (float)x / (float)mRect.w );
        mListener->onChange( this );
      }

};

//----------------------------------------------------------------------
#endif


