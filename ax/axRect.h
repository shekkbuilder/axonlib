#ifndef axRect_included
#define axRect_included
//----------------------------------------------------------------------

struct axRect
{

  union
  {
    struct { int x,y,w,h; };
    struct { int data[4]; };
  };
  axRect()                                { }
  axRect(int i)                           { x=i; y=i; w=i; h=i; }
  axRect(int aX, int aY, int aW, int aH)  { x=aX; y=aY; w=aW; h=aH; }

  //----------------------------------------

  void clear(void)                          { }
  void set(int i)                           { x=i; y=i; w=i; h=i; }
  void set(int aX, int aY, int aW, int aH)  { x=aX; y=aY; w=aW; h=aH; }
  void setPos(int aX, int aY)               { x=aX; y=aY; }
  void setSize(int aW, int aH)              { w=aW; h=aH; }

  //----------------------------------------

  //inline int x1(void) { return x; }
  //inline int y1(void) { return y; }
  inline int x2(void) { return x+w-1; }
  inline int y2(void) { return y+h-1; }

  //inline void x1(int i) { x=i; }
  //inline void y1(int i) { y=i; }
  inline void x2(int i) { w=i-x+1; }
  inline void y2(int i) { h=i-y+1; }

  //----------------------------------------

  inline void add(int aX, int aY)
    {
      x+=aX;
      y+=aY;
    }

  inline void add(int aX, int aY, int aW, int aH)
    {
      x+=aX;
      y+=aY;
      w+=aW;
      h+=aH;
    }

  //----------------------------------------

  inline bool contains(int aX, int aY)
    {
      if( aX < x ) return false;
      if( aY < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  bool intersects(int aX, int aY, int aW, int aH)
    {
      if( (aX+aW) < x ) return false;
      if( (aY+aH) < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  inline void combine(int aX, int aY, int aW, int aH)
    {
      if( aX < x ) x = aX;
      if( aY < y ) y = aY;
      if( (aX+aW) >= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) >= (y+h) ) h = (aY+aH) - y;
    }

  inline void crop(int aX, int aY, int aW, int aH)
    {
      if( aX > x ) x = aX;
      if( aY > y ) y = aY;
      if( (aX+aW) <= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) <= (y+h) ) h = (aY+aH) - y;
    }

  //----------------------------------------

  inline bool intersects(axRect aRect) { return intersects( aRect.x, aRect.y, aRect.w, aRect.h ); }
  inline void combine(axRect aRect)    { return combine( aRect.x, aRect.y, aRect.w, aRect.h ); }
  //void crop(int aX, int aY, int aW, int aH)

};

#define NULL_RECT axRect(0,0,0,0)

//----------------------------------------------------------------------
#endif

