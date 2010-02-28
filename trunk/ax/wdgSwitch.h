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
  \file wdgSwitch.h
  \brief switch (or button) widget
*/

#ifndef wdgSwitch_included
#define wdgSwitch_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axWidget.h"

class wdgSwitch : public axWidget
{
  public:
    int       mAlign;
    axString  mTextOff;
    axString  mTextOn;
    axFont*   mFontOff;
    axFont*   mFontOn;
    axBrush*  mFillBrush;

  public:

    wdgSwitch(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None,
              float aValue=0)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        //setBackground(true,AX_GREY_DARK);
        mFillBrush = new axBrush(AX_GREY_DARK);
        //setFillBrush(mFillBrush);
        //setFlag(wfl_DefaultDraw);
        mAlign    = tal_Center;
        mFontOff  = new axFont(AX_GREY);
        mFontOn   = new axFont(AX_GREY_LIGHT);
        mTextOff  = "off";
        mTextOn   = "on";
        mValue    = aValue;
        if (mParameter) mValue = mParameter->doGetValue();
        else mValue = aValue;
      }

    virtual ~wdgSwitch()
      {
        delete mFontOff;
        delete mFontOn;
        delete mFillBrush;
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
        mValue = aValue;
        if (mParameter) mParameter->doSetValue(aValue);
      }

    //----------

    virtual float doGetValue(void)
      {
        if (mParameter) return mParameter->doGetValue();
        else return mValue;
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //axWidget::doPaint(aCanvas,aRect);
        aCanvas->clearPen();
        aCanvas->selectBrush(mFillBrush);
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
        if (mValue>0.5)
        {
          aCanvas->selectFont(mFontOn);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOn,mAlign);
        }
        else
        {
          aCanvas->selectFont(mFontOff);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOff,mAlign);
        }
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if (mValue>0.5) mValue=0;
        else mValue=1;
        if (mParameter) mParameter->setValueDirect(mValue);
        mListener->onChange(this);
      }

};

//----------------------------------------------------------------------
#endif


