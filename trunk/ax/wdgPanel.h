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
 * \brief panel widget
 */

/**
 * \brief panel widget
 *
 * long desc
 *
 */

#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgPanel : public axContainer//Widget
{
  protected:
    axColor   mLightColor;
    axColor   mDarkColor;
    //axColor   mFillColor;

  public:

    wdgPanel(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axContainer/*Widget*/(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        //clearFlag( wfl_Active );
        mLightColor = AX_GREY_LIGHT;
        mDarkColor = AX_GREY_DARK;
        mFillColor = AX_GREY;
        //setBackground(true,AX_GREY_DARK);
      }

    //inline void setText(axString aText)
    //  {
    //    mText = aText;
    //  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //  _
        // |
        //
        aCanvas->setPenColor( mLightColor );
        aCanvas->drawLine( mRect.x,      mRect.y,      mRect.x2(),   mRect.y      );
        aCanvas->drawLine( mRect.x,      mRect.y,      mRect.x,      mRect.y2()   );
        aCanvas->setPenColor( mDarkColor );
        aCanvas->drawLine( mRect.x2(),   mRect.y,      mRect.x2(),   mRect.y2()   );
        aCanvas->drawLine( mRect.x,      mRect.y2(),  mRect.x2(),    mRect.y2()   );
        //
        // _|
        //
        //aCanvas->setPenColor( mFillColor );
        aCanvas->setBrushColor( mFillColor );
        aCanvas->fillRect( mRect.x+1, mRect.y+1, mRect.x2()-1, mRect.y2()-1 );
        //
        axContainer::doPaint( aCanvas, aRect );

      }

    //virtual void doMouseDown(int aX, int aY, int aB) { wdgValue::doMouseDown(aX,aY,aB); }
    //virtual void doMouseUp(int aX, int aY, int aB) { wdgValue::doMouseUp(aX,aY,aB); }
    //virtual void doMouseMove(int aX, int aY, int aB) { wdgValue::doMouseMove(aX,aY,aB); }


};

//----------------------------------------------------------------------
#endif




