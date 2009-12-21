#ifndef axRect_included
#define axRect_included
//----------------------------------------------------------------------

#include "axPoint.h"

struct axRect
{

  union
    {
      struct { int x1,y1,x2,y2; };
      struct { int x, y, xx,yy; };
      struct { int buf[4];  };
    };

  //------------------------------

  axRect() {}

  axRect(int aX1, int aY1, int aX2, int aY2)
    {
      x1=aX1;
      y1=aY1;
      x2=aX2;
      y2=aY2;
    }

  //------------------------------

  inline int w(void) { return x2-x1+1; }
  inline int h(void) { return y2-y1+1; }

  inline void w(int n) { x2 = x1+n-1; }
  inline void h(int n) { y2 = y1+n-1; }

  //----------

  inline bool contains(int aX, int aY)
    {
      if ( aX >= x1 &&
           aX <= x2 &&
           aY >= y1 &&
           aY <= y2 ) return true;
      else return false;
    }

  inline bool contains(axPoint aPoint)
    {
      if ( aPoint.x >= x1 &&
           aPoint.x <= x2 &&
           aPoint.y >= y1 &&
           aPoint.y <= y2 ) return true;
      else return false;
    };

  inline bool intersect(axRect aRect)
    {
      if ( aRect.x1 > x2 ) return false;
      if ( aRect.y1 > y2 ) return false;
      if ( aRect.x2 < x1 ) return false;
      if ( aRect.y2 < y1 ) return false;
      return true;
    }

  inline void combine(axRect aRect)
    {
      if (aRect.x1 < x1) x1=aRect.x1;
      if (aRect.y1 < y1) y1=aRect.y1;
      if (aRect.x2 > x2) x2=aRect.x2;
      if (aRect.y2 > y2) y2=aRect.y2;
    }

  //----------

  inline void crop(axRect aRect)
    {
      if (aRect.x1 > x1) x1=aRect.x1;
      if (aRect.y1 > y1) y1=aRect.y1;
      if (aRect.x2 < x2) x2=aRect.x2;
      if (aRect.y2 < y2) y2=aRect.y2;
    }

  //----------

  inline void add(axRect aRect)
  {
    x1 += aRect.x1;
    y1 += aRect.y1;
    x2 += aRect.x2;
    y2 += aRect.y2;
  }

  inline void add(int aX1, int aY1, int aX2, int aY2)
  {
    x1 += aX1;
    y1 += aY1;
    x2 += aX2;
    y2 += aY2;
  }

  inline void add(axPoint aPoint)
  {
    x1 += aPoint.x;
    y1 += aPoint.y;
    x2 += aPoint.x;
    y2 += aPoint.y;
  }

  inline void add(int aX, int aY)
  {
    x1 += aX;
    y1 += aY;
    x2 += aX;
    y2 += aY;
  }

};

typedef axArray<axRect> axRects;

const axRect NULLRECT(0,0,0,0);

//----------------------------------------------------------------------
#endif
