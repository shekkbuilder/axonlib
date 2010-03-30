#ifndef axBrush_included
#define axBrush_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "gui/axColor.h"

//----------------------------------------------------------------------

#define DEF_BRUSHSTYLE 0

struct axBrush
{
  axColor mColor;
  int     mStyle;
  //
  axBrush() { mColor=0; mStyle=DEF_BRUSHSTYLE; }
  axBrush(axColor aColor, int aStyle=DEF_BRUSHSTYLE) { mColor=aColor; mStyle=aStyle; }

  inline void setColor(axColor aColor) { mColor=aColor; }
  inline void setStyle(int aStyle) { mStyle=aStyle; }

  inline unsigned long getColor(void) { return mColor.get(); }
  inline int getStyle(void) { return mStyle; }
};

typedef axArray<axBrush> axBrushes;

//----------------------------------------------------------------------
#endif
