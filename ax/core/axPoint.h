#ifndef axPoint_included
#define axPoint_included
//----------------------------------------------------------------------

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


