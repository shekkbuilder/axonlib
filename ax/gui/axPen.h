#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------

#include "axDefines.h"

//----------------------------------------------------------------------

#define DEF_PENWIDTH 0

struct axPen
{
  axColor mColor;
  int     mSize;
  //
  axPen() { mColor=0; mSize=DEF_PENWIDTH; }
  axPen(axColor aColor, int aSize=DEF_PENWIDTH) { mColor=aColor; mSize=aSize; }

  inline void setColor(axColor aColor) { mColor=aColor; }
  inline void setSize(int aSize) { mSize=aSize; }

  inline unsigned long getColor(void) { return mColor.get(); }
  inline int getSize(void) { return mSize; }
};

typedef axArray<axPen> axPens;

//----------------------------------------------------------------------
#endif
