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
 * \brief value widget
 */

/**
 * \brief value widget
 *
 * long desc
 *
 */

#ifndef wdgValue_included
#define wdgValue_included
//----------------------------------------------------------------------

#include "axWidget.h"

#define MAX_TEXT 32

class wdgValue : public axWidget
{
  private:
    bool    is_dragging;
    int     prev;
    char    buf[MAX_TEXT];
    char    hint[256];
  public:
    //float   mValue;
    axColor mTextColor;
    int     mTextAlign;
    float   mSens1, mSens2;

  public:

    wdgValue(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        setFlag(wfl_Vertical);
        setBackground(true,AX_GREY_DARK);
        memset(buf,0,MAX_TEXT);
        is_dragging = false;
        mTextAlign      = tal_Center;
        prev        = 0;
        mTextColor  = AX_GREY_LIGHT;
        mSens1      = 0.005;
        mSens2      = 0.1;
        if(mParameter) mValue = mParameter->doGetValue();
        else mValue = 0;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------


    // do we need to inherit these?


    virtual void doSetValue(float aValue)
      {
        mValue = aValue;
        if(mParameter) mParameter->doSetValue(aValue);
      }

    //----------

    virtual float doGetValue(void)
      {
        if(mParameter) return mParameter->doGetValue();
        else return mValue;
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas,aRect);
        aCanvas->setTextColor( mTextColor );
        if(mParameter) mParameter->doGetDisplay(buf);
        else sprintf(buf,"%.2f",mValue);
        aCanvas->drawText( mRect.x, mRect.y,mRect.x2()/*-5*/,mRect.y2(),buf,mTextAlign);
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        is_dragging = true;
        if( mFlags&wfl_Vertical)
        {
          prev = aY;
          mListener->onCursor(cuArrowUpDown);
        }
        else
        {
          prev = -aX;
          mListener->onCursor(cuArrowLeftRight);
        }
      }

    //----------

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        is_dragging = false;
          mListener->onCursor(DEF_CURSOR);
      }

    //----------

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if( is_dragging )
        {
          int cur;
          if( mFlags&wfl_Vertical) cur = aY;
          else cur = -aX;
          int dist = cur - prev;
          float s = mSens1;
          if( aB&but_Ctrl ) s*=mSens2;
          float delta = s * (float)dist;
          prev = cur;
          mValue -= delta;
          mValue = axMin(1,axMax(0,mValue));
          if(mParameter) mParameter->mValue = mValue;
          mListener->onChange( this );
          sprintf(hint," %.3f",mValue);
          mListener->onHint(hint);
        }
      }

    //----------

};

#undef MAX_TEXT

//----------------------------------------------------------------------
#endif


