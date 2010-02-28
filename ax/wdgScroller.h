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
 * \file wdgScroller.h
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
    axBrush*  brush_back;
    axBrush*  brush_thumb;

  protected:
    //axString  mText;
    //axColor   mTextColor;
    //int       mTextAlign;
    //char      txt[32];
    float mThumbSize;   // 0..1

  public:

    wdgScroller(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment)
    : axWidget(aListener,aID,aRect,aAlignment)
      {
        //clearFlag( wfl_Active );
        //mText       = aText;
        //mTextColor  = aColor;
        //mTextAlign  = aAlign;//tal_Center;
        //setBackground(true,AX_GREY_DARK);
        is_dragging = false;
        mThumbSize = 1;
        if ( aRect.w < aRect.h   ) setFlag(wfl_Vertical);
        else clearFlag(wfl_Vertical);
        brush_back = new axBrush(AX_GREY_DARK);
        brush_thumb = new axBrush(AX_GREEN_DARK);
      }

    virtual ~wdgScroller()
      {
        delete brush_back;
        delete brush_thumb;
      }

    //inline void setPos(float aPos) { mPos = aPos; }
    inline void setThumbSize(float aSize)
      {
        mThumbSize = aSize;
      }

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

        aCanvas->selectBrush(brush_back);
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );

        aCanvas->selectBrush(brush_thumb);
        if (hasFlag(wfl_Vertical))
        {
          int thumb = (int)((float)mRect.h*mThumbSize);
          int ipos  = (int)((float)(mRect.h-thumb)*mValue);
          int y = mRect.y+ipos;
          aCanvas->fillRect( mRect.x, y, mRect.x2(), y+thumb );
        }
        else
        {
          int thumb = (int)((float)mRect.w*mThumbSize);
          int ipos  = (int)((float)(mRect.w-thumb)*mValue);
          int x = mRect.x+ipos;
          aCanvas->fillRect( x, mRect.y, x+thumb, mRect.y2() );
        }
      }

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        is_dragging = true;
        int thumbsize,numpixels;
        if (hasFlag(wfl_Vertical))
        {
          thumbsize = (int)((float)mRect.h*mThumbSize);
          numpixels = mRect.h - thumbsize;
        }
        else
        {
          thumbsize = (int)((float)mRect.w*mThumbSize);
          numpixels = mRect.w - thumbsize;
        }
        if (numpixels<=0) numpixels = 1;  // HACK
        pixel_size = 1.0f/(float)numpixels;
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





