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
  \file wdgSwitch,h
  \brief switch (or button) widget
*/

#ifndef wdgButton_included
#define wdgButton_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axWidget.h"

class wdgButton : public axWidget
{
  private:
    bool      mIsOn;
    axBrush*  m_BrushOff;
    axBrush*  m_BrushOn;
    axPen*    m_BorderPen;
    axFont*   m_FontOff;
    axFont*   m_FontOn;
  protected:
    int       mAlign;
    axString  mText;
    axBrush*  mBrushOff;
    axBrush*  mBrushOn;
    axPen*    mBorderPen;
    axFont*   mFontOff;
    axFont*   mFontOn;

  public:

    wdgButton(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None,
              axString aText="click", bool aIsOn=false)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        mAlign      = tal_Center;
        mText       = aText;//"[]";
        mIsOn       = aIsOn;
        m_BrushOff  = new axBrush(AX_GREY_DARK);
        m_BrushOn   = new axBrush(AX_GREY);
        m_BorderPen = new axPen(AX_BLACK);
        m_FontOff   = new axFont(AX_GREY);
        m_FontOn    = new axFont(AX_GREY_LIGHT);
        mBrushOff   = m_BrushOff;
        mBrushOn    = m_BrushOn;
        mBorderPen  = m_BorderPen;
        mFontOff    = m_FontOff;
        mFontOn     = m_FontOn;
      }

    virtual ~wdgButton()
      {
        delete m_BrushOff;
        delete m_BrushOn;
        delete m_BorderPen;
        delete m_FontOff;
        delete m_FontOn;
      }

    //--------------------------------------------------

    virtual void on_Click(wdgButton* btn) {} // override this

    //--------------------------------------------------

    inline void setBrushOff(axBrush* brush) { mBrushOff=brush; }
    inline void setBrushOn(axBrush* brush) { mBrushOn=brush; }
    inline void setBorderPen(axPen* pen) { mBorderPen=pen; }
    inline void setFontOff(axFont* font) { mFontOff=font; }
    inline void setFontOn(axFont* font) { mFontOn=font; }

    inline bool isOn(void) { return mIsOn; }

    //--------------------------------------------------

    //void setup(axString aOff, axString aOn)
    //  {
    //    mTextOff = aOff;
    //    mTextOn = aOn;
    //  }

    //void setup(char** aStrings)
    //  {
    //    mTextOff = aStrings[0];
    //    mTextOn = aStrings[1];
    //  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    //virtual void doSetValue(float aValue)
    //  {
    //    mValue = aValue;
    //    if (mParameter) mParameter->doSetValue(aValue);
    //  }

    //----------

    //virtual float doGetValue(void)
    //  {
    //    if (mParameter) return mParameter->doGetValue();
    //    else return mValue;
    //  }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //axWidget::doPaint(aCanvas,aRect);
        //aCanvas->clearPen();
        if (mIsOn)
        {
          aCanvas->selectBrush(mBrushOn);
          aCanvas->fillRect( mRect.x,mRect.y,mRect.x2(),mRect.y2() );
          aCanvas->selectPen(mBorderPen);
          aCanvas->drawRect( mRect.x,mRect.y,mRect.x2(),mRect.y2() );
          aCanvas->selectFont(mFontOn);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mAlign);
        }
        else
        {
          aCanvas->selectBrush(mBrushOff);
          aCanvas->fillRect( mRect.x,mRect.y,mRect.x2(),mRect.y2() );
          aCanvas->selectPen(mBorderPen);
          aCanvas->drawRect( mRect.x,mRect.y,mRect.x2(),mRect.y2() );
          aCanvas->selectFont(mFontOff);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mAlign);
        }
//        aCanvas->selectBrush(mFillBrush);
//        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
//        if (mValue>0.5)
//        {
//          aCanvas->selectFont(mFontOn);
//          aCanvas->drawText(mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOn,mAlign);
//        }
//        else
//        {
//          aCanvas->selectFont(mFontOff);
//          aCanvas->drawText(mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOff,mAlign);
//        }
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        mIsOn = true;
        mListener->onChange(this);
        //mListener->onRedraw(this);
      }

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if (mIsOn)
        {
          if (!mRect.contains(aX,aY))
          {
            mIsOn = false;
            mListener->onChange(this);
          }
        } // ison
        else
        {
          if (mRect.contains(aX,aY))
          {
            mIsOn = true;
            mListener->onChange(this);
          }
        }
      }

    virtual void doMouseUp(int aX, int aY, int aB)
      {
        if (mIsOn)
        {
          //trace("click");
          on_Click(this);
        }
        mIsOn = false;
        mListener->onChange(this);
        //mListener->onRedraw(this);
      }

};

//----------------------------------------------------------------------
#endif


