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
  \file wdgValue.h
  \brief value widget
*/

#ifndef wdgValue_included
#define wdgValue_included
//----------------------------------------------------------------------

#define BUF_SIZE  256
#define HINT_SIZE 256

#include "axWidget.h"

class wdgValue : public axWidget
{
  private:
    bool      is_dragging;
    int       prev;
    char      buf[BUF_SIZE];
    char      hint[HINT_SIZE];
    axBrush*  mFillBrush;
    axFont*   mFont;

  //public:
    int     mTextAlign;
    float   mSens1, mSens2;

  public:

    wdgValue(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        setFlag(wfl_Vertical);
        memset(buf,0,BUF_SIZE);
        is_dragging = false;
        mTextAlign  = tal_Center;
        prev        = 0;
        mFillBrush  = new axBrush(AX_GREY_DARK);
        mFont       = new axFont(AX_GREY_LIGHT);
        mSens1      = 0.005;
        mSens2      = 0.1;
        if (mParameter) mValue = mParameter->doGetValue();
        else mValue = 0;
      }

    virtual ~wdgValue()
      {
        delete mFont;
        //delete mFillBrush;
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
        aCanvas->selectFont(mFont);
        if (mParameter) mParameter->doGetDisplay(buf);
        else sprintf(buf,"%.2f",mValue);
        aCanvas->drawText(mRect.x,mRect.y,mRect.x2()/*-5*/,mRect.y2(),buf,mTextAlign);
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
          if (hasFlag(wfl_Vertical)) cur = aY;
          else cur = -aX;
          int dist = cur - prev;
          float s = mSens1;
          if (aB&but_Ctrl) s*=mSens2;
          float delta = s * (float)dist;
          prev = cur;
          mValue -= delta;
          mValue = axMin(1,axMax(0,mValue));
          //if (mParameter) mParameter->setValue(mValue);
          if (mParameter) mParameter->setValueDirect(mValue);
          mListener->onChange(this);
          sprintf(hint," %.3f",mValue);
          mListener->onHint(hint);
        }
      }

};

#undef BUF_SIZE
#undef HINT_SIZE

//----------------------------------------------------------------------
#endif


