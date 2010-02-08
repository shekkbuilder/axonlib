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
 * \brief rectangle class
 */

/**
 * \brief rectangle class
 *
 * \long has methods to combine rectangles and find intersections
 *
 */

#ifndef axRect_included
#define axRect_included
//----------------------------------------------------------------------

/// rectangle

struct axRect
{

  int x;  ///< xpos
  int y;  ///< ypos
  int w;  ///< width
  int h;  ///< height

  //union
  //{
  //  //struct { int x,y,w,h; };
  //  //struct { int data[4]; };
  //  int x,y,w,h;
  //  int data[4];
  //};

  // various constructors
  axRect() { }
  axRect(int i) { x=i; y=i; w=i; h=i; }
  axRect(int aX, int aY, int aW, int aH) { x=aX; y=aY; w=aW; h=aH; }

  //----------------------------------------

  //void clear(void) {}

  /// set all of x,y,w,h to parameter i
  void set(int i) { x=i; y=i; w=i; h=i; }
  /// set all member variables
  void set(int aX, int aY, int aW, int aH) { x=aX; y=aY; w=aW; h=aH; }
  /// set pos (x,y)
  void setPos(int aX, int aY) { x=aX; y=aY; }
  /// set size (w,h)
  void setSize(int aW, int aH) { w=aW; h=aH; }

  //----------------------------------------

  /// get x2 (calculated from x and w)
  inline int x2(void) { return x+w-1; }
  /// get y2 (calculated from y and h)
  inline int y2(void) { return y+h-1; }
  /// set w (calaulated from x and parameter)
  inline void x2(int i) { w=i-x+1; }
  /// set h (calaulated from y and parameter)
  inline void y2(int i) { h=i-y+1; }

  //----------

  /// add specified amounts to x and y
  inline void add(int aX, int aY) { x+=aX; y+=aY; }
  /// add specific amounts to each of x,y,w,h
  inline void add(int aX, int aY, int aW, int aH) { x+=aX; y+=aY; w+=aW; h+=aH; }

  //----------------------------------------

  /// returns true if point aX,aY is inside of rect
  inline bool contains(int aX, int aY)
    {
      if( aX < x ) return false;
      if( aY < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  /// returns true if the two rectngles intersects,
  /// if they touch or overlap
  bool intersects(int aX, int aY, int aW, int aH)
    {
      if( (aX+aW) < x ) return false;
      if( (aY+aH) < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  /// sets the rect to contain (encapsulate) itself and another rect
  //  __            _____
  // |  |          :  :  |
  // |__|          :..:  |
  //     __    ->  |   ..|
  //    |  |       |  :  :
  //    |__|       |__:__:
  //
  inline void combine(int aX, int aY, int aW, int aH)
    {
      if( aX < x ) { w += (x-aX); x=aX; }
      if( aY < y ) { h += (y-aY); y=aY; }
      if( (aX+aW) >= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) >= (y+h) ) h = (aY+aH) - y;
    }

  /// crop rect so that it is completely inside passed rect
  //    ___
  //   |  _|_          _:.
  //   | | | |   ->   | | :
  //   | |_|_|        |_|.:
  //   |___|            :
  //
  inline void crop(int aX, int aY, int aW, int aH)
    {
      if( aX > x ) x = aX;
      if( aY > y ) y = aY;
      if( (aX+aW) <= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) <= (y+h) ) h = (aY+aH) - y;
    }

  //----------------------------------------

  /// simplifications, so we can pass axRect as parameters instead of x,y,w,h
  inline bool intersects(axRect aRect) { return intersects( aRect.x, aRect.y, aRect.w, aRect.h ); }
  /// simplifications, so we can pass axRect as parameters instead of x,y,w,h
  inline void combine(axRect aRect)    { return combine( aRect.x, aRect.y, aRect.w, aRect.h ); }

  //void crop(int aX, int aY, int aW, int aH)

};

#define NULL_RECT axRect(0,0,0,0) ///< null rect.

typedef axArray<axRect*> axRects;

//----------------------------------------------------------------------
#endif

