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
 * \brief switch widget 
 */

/**
 * \brief switch widget 
 *
 * long desc
 *
 */

#ifndef wdgSwitch_included
#define wdgSwitch_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axWidget.h"

class wdgSwitch : public axWidget
{
  public:
    //bool      mState;
    int       mAlign;
    axColor   mOffColor;
    axColor   mOnColor;
    axString  mTextOff;
    axString  mTextOn;

  public:

    wdgSwitch(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, /*axParameter* aParameter=NULL,*/
              float aValue=0)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        setBackground(true,AX_GREY_DARK);
        mAlign    = tal_Center;
        mOffColor = AX_GREY;
        mOnColor  = AX_GREY_LIGHT;
        mTextOff  = "off";
        mTextOn   = "on";
        mValue    = aValue;
        if(mParameter) mValue = mParameter->doGetValue();
        else mValue = aValue;

      }

    //--------------------------------------------------

    void setup(axString aOff, axString aOn)
      {
        mTextOff = aOff;
        mTextOn = aOn;
      }

    void setup(char** aStrings)
      {
        mTextOff = aStrings[0];
        mTextOn = aStrings[1];
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doSetValue(float aValue)
      {
        //axWidget::doSetValue(aValue);
        mValue = aValue;
        if(mParameter) mParameter->doSetValue(aValue);
        //if( mValue >= 0.5 ) mState=true;
        //else mState=false;
      }

    //----------

    virtual float doGetValue(void)
      {
        if(mParameter) return mParameter->doGetValue();
        else return mValue;
        //{
        //  if( mState) return 1;
        //  else return 0;
        //}

      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas,aRect);
        if( mValue>0.5 )
        {
          aCanvas->setTextColor( mOnColor );
          aCanvas->drawText( mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOn,mAlign);
        }
        else
        {
          aCanvas->setTextColor( mOffColor );
          aCanvas->drawText( mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOff,mAlign);
        }
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


