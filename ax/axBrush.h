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
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */
 
#ifndef axBrush_included
#define axBrush_included
//----------------------------------------------------------------------
//TODO

#include "axCanvas.h"

#ifdef linux

class axBrush
{
  protected:

    axColor mColor;
    int     mStyle;

  public:

    axBrush( axColor aColor=AX_GREY, int aStyle=0 )
      {
        mColor = aColor;
        mStyle = aStyle;
      }

    virtual ~axBrush() {}

    //virtual void setBrushColor(axColor aColor)
    //  {
    //    mBrushColor = aColor;
    //    setforecolor(mBrushColor.mColor);
    //  };

    //virtual void setBrushStyle(int aStyle)
    //  {
    //    XSetFillStyle(gDP,mGC,aStyle); // FillSolid
    //  };

};

#endif

//----------------------------------------------------------------------
#endif


