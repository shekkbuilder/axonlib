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
  \file axPoint.h
  \brief wrapper for x/y coordinate pair
*/

#ifndef axPoint_included
#define axPoint_included
//----------------------------------------------------------------------

/// 2d point

struct axPoint
{
  int x,y;

  axPoint()
    {
    }

  axPoint(int i)
    {
      x = i;
      y = i;
    }

  axPoint(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  //----------

  void clear(void)
    {
    }

  void set(int i)
    {
      x = i;
      y = i;
    }

  void set(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  inline void add(int aX, int aY)
    {
      x += aX;
      y += aY;
    }

};

#define NULL_POINT axPoint(0,0)

//----------------------------------------------------------------------
#endif


