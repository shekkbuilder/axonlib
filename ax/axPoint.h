#ifndef axPoint_included
#define axPoint_included
//----------------------------------------------------------------------

struct axPoint
{

  int x,y;
  //union
  //{
  //  //struct { int x,y; };
  //  //struct { int w,h; };
  //};
  axPoint()               { }
  axPoint(int i)          { x=i; y=i; }
  axPoint(int aX, int aY) { x=aX; y=aY; }

  //----------------------------------------

  void clear(void)          { }
  void set(int i)           { x=i; y=i; }
  void set(int aX, int aY)  { x=aX; y=aY; w=aW; h=aH; }

  //----------------------------------------

  inline void add(int aX, int aY)
    {
      x+=aX;
      y+=aY;
    }

//  inline bool contains(int aX, int aY)
//    {
//      if( aX < x ) return false;
//      if( aY < y ) return false;
//      if( aX >= (x+w) ) return false;
//      if( aY >= (y+h) ) return false;
//      return true;
//    }
//
//  bool intersects(int aX, int aY, int aW, int aH)
//    {
//      if( (aX+aW) < x ) return false;
//      if( (aY+aH) < y ) return false;
//      if( aX >= (x+w) ) return false;
//      if( aY >= (y+h) ) return false;
//      return true;
//    }
//
//  inline void combine(int aX, int aY, int aW, int aH)
//    {
//      if( aX < x ) { w += (x-aX); x=aX; }
//      if( aY < y ) { h += (y-aY); y=aY; }
//      if( (aX+aW) >= (x+w) ) w = (aX+aW) - x;
//      if( (aY+aH) >= (y+h) ) h = (aY+aH) - y;
//    }
//
//  inline void crop(int aX, int aY, int aW, int aH)
//    {
//      if( aX > x ) x = aX;
//      if( aY > y ) y = aY;
//      if( (aX+aW) <= (x+w) ) w = (aX+aW) - x;
//      if( (aY+aH) <= (y+h) ) h = (aY+aH) - y;
//    }
//
//  //----------------------------------------
//
//  inline bool intersects(axRect aRect) { return intersects( aRect.x, aRect.y, aRect.w, aRect.h ); }
//  inline void combine(axRect aRect)    { return combine( aRect.x, aRect.y, aRect.w, aRect.h ); }
//  //void crop(int aX, int aY, int aW, int aH)

};

//#define NULL_RECT axRect(0,0,0,0)

//----------------------------------------------------------------------
#endif


