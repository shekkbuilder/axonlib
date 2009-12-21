#ifndef axPoint_included
#define axPoint_included
//----------------------------------------------------------------------

struct axPoint
{

  union
    {
      struct { int x,y; };
      struct { int buf[2]; };
    };

  //----------

  axPoint()               {}
  axPoint(int aN)         { x=y=aN; }
  axPoint(int aX, int aY) { x=aX; y=aY; }

  inline void add(int aX,int aY)
    {
      x += aX;
      y += aY;
    };

  inline void add(axPoint aPoint)
    {
      x += aPoint.x;
      y += aPoint.y;
    };



};

//----------------------------------------------------------------------
#endif
