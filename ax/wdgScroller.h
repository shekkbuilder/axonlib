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
 * \brief widget scroller 
 */

/**
 * \brief widget scroller
 *
 * long desc
 *
 */

#ifndef wdgScroller_included
#define wdgScroller_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgScroller : public axWidget
{
  private:
    bool is_dragging;
    float pixel_size;
    int clickx,clicky;
  protected:
    //axString  mText;
    //axColor   mTextColor;
    //int       mTextAlign;
    //char      txt[32];
    float mThumbSize;   // 0..1

  public:

    wdgScroller(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment /*axParameter* aParameter=NULL,*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        //clearFlag( wfl_Active );
        //mText       = aText;
        //mTextColor  = aColor;
        //mTextAlign  = aAlign;//tal_Center;
        //setBackground(true,AX_GREY_DARK);
        is_dragging = false;
        mThumbSize = 1;
      }

    //inline void setPos(float aPos) { mPos = aPos; }
    inline void setThumbSize(float aSize) { mThumbSize = aSize; }

    //inline void setThumb(float aValue, float aThumb)
    //  {
    //    mValue = aValue;
    //    mThumbSize = aThumb;
    //  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint( aCanvas, aRect );

        int thumb = (int)mThumbSize * mRect.h;
        int ipos   = (int)mValue * (mRect.h - thumb);

        aCanvas->setBrushColor( AX_GREY_DARK );
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
        aCanvas->setBrushColor( AX_GREY_LIGHT );
        if (hasFlag(wfl_Vertical))
        {
          int y = mRect.y+ipos;
          aCanvas->fillRect( mRect.x, y, mRect.x2(), y+thumb );
        }
        else
        {
          int x = mRect.x+ipos;
          aCanvas->fillRect( x, mRect.y, mRect.x+thumb, mRect.y2() );
        }
      }

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        is_dragging = true;
        int thumbsize = (int)((float)mRect.h*mThumbSize);
        //TRACE("thumbsize: %i\n",thumbsize);
        int numpixels = mRect.h - thumbsize;
        //TRACE("numpixels: %i\n",numpixels);
        if (numpixels<=0) numpixels = 1;  // HACK
        pixel_size = 1.0f/(float)numpixels;
        //TRACE("pixel_size: %f\n",pixel_size);
        clickx = aX;
        clicky = aY;
      }


    virtual void doMouseUp(int aX, int aY, int aB)
      {
        is_dragging = false;
      }

    virtual void doMouseMove(int aX, int aY, int aB)
      {
        if (is_dragging)
        {
          int deltax = aX - clickx;
          int deltay = aY - clicky;
          if (hasFlag(wfl_Vertical)) mValue += ((float)deltay * pixel_size);
          else mValue += ((float)deltax * pixel_size);
          mValue = axMax(0,axMin(mValue,1));
          mListener->onChange(this);
          clickx = aX;
          clicky = aY;
        }
      }


};

//----------------------------------------------------------------------
#endif





