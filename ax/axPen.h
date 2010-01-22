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

#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------
//TODO

#include "axCanvas.h"

#define DEF_PENWIDTH 0

class axPen
{
  protected:

    axColor mColor;
    int     mWidth;
    int     mStyle;

  public:

    axPen( axColor aColor=AX_GREY_LIGHT, int aWidth=DEF_PENWIDTH, int aStyle=0 )
      {
        mColor = aColor;
        mWidth = aWidth;
        mStyle = aStyle;
      }
    virtual ~axPen() {}

};

//----------------------------------------------------------------------
#endif


