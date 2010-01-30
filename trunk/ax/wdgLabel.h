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
  \file wdgLabel.h
  \brief label widget
*/

#ifndef wdgLabel_included
#define wdgLabel_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgLabel : public axWidget
{
  protected:
    axString  mText;
    axFont*   mFont;
    int       mTextAlign;
    char      txt[32];
    //axBrush*  mBackBrush;

  public:

    wdgLabel(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment,
             axString aText="", axColor aColor=AX_GREY_DARK, int aAlign=tal_Left)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        clearFlag( wfl_Active );
        mText = aText;
        mFont = new axFont( aColor );
        mTextAlign  = aAlign;
        //mBackBrush = new axBrush(AX_GREY_DARK);
        //setFillBrush(mBackBrush);
        //setFlag(wfl_DefaultDraw);
      }

    virtual ~wdgLabel()
      {
        //delete mBackBrush;
      }

    inline void setText(axString aText)
      {
        mText = aText;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas,aRect);
        aCanvas->selectFont(mFont);
//        if (mParameter)
//        {
//          mParameter->doGetDisplay(txt);
//          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),txt,mTextAlign);
//        }
//        else
        aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
      }

};

//----------------------------------------------------------------------
#endif




