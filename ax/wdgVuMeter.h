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
 * @file
 * \brief vu meter widget
 */

/**
 * \brief vu meter widget
 *
 * long desc
 *
 */

#ifndef wdgVuMeter_included
#define wdgVuMeter_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgVuMeter : public axWidget
{
  private:
    axPen* mRectPen;
    axBrush* mBarBrush;
  public:
    float mLevel;
  public:

    wdgVuMeter(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mLevel = 0;
        clearFlag(wfl_Active);
        setFlag(wfl_Vertical);
        mRectPen = new axPen(AX_GREY_LIGHT);
        mBarBrush = new axBrush(AX_GREY_LIGHT);
      }
    virtual ~wdgVuMeter()
      {
        delete mRectPen;
        delete mBarBrush;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
//aCanvas->setPenColor( AX_GREY_LIGHT );
aCanvas->selectPen(mRectPen);
        aCanvas->drawRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
//aCanvas->setBrushColor( AX_GREY_LIGHT );
aCanvas->selectBrush(mBarBrush);
        if (hasFlag(wfl_Vertical))
        {
          float h = (float)(mRect.h-4) * mLevel;
          int y2 = mRect.y2() - 2;
          int y1 = y2 - h;
          aCanvas->fillRect(mRect.x+2, y1, mRect.x2()-2, y2 );
        }
        else
        {
          float w = (float)(mRect.w-4) * mLevel;
          int x2 = mRect.x2() - 2;
          int x1 = x2 - w;
          aCanvas->fillRect(x1,mRect.y+2, x2, mRect.y2()-2 );
        }
      }
};

//----------------------------------------------------------------------
#endif

