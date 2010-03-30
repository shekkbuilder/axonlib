#ifndef axFont_included
#define axFont_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "core/axString.h"

//----------------------------------------------------------------------

struct axFont
{
  axString  mName;
  axColor   mColor;
  int       mSize;
  int       mStyle;
  //
  axFont() { mName=""; mColor=0; mSize=-1; mStyle=0; }
  axFont(axString aName, axColor aColor, int aSize=-1, int aStyle=0) { mName=aName; mColor=aColor; mSize=aSize; mStyle=aStyle; }

  inline void setName(axString aName) { mName=aName; }
  inline void setColor(axColor aColor) { mColor=aColor; }
  inline void setSize(int aSize) { mSize=aSize; }
  inline void setStyle(int aStyle) { mStyle=aStyle; }

  inline axString getName(void) { return mName; }
  inline unsigned long getColor(void) { return mColor.get(); }
  inline int getSize(void) { return mSize; }
  inline int getStyle(void) { return mStyle; }

};

typedef axArray<axFont> axFonts;

//----------------------------------------------------------------------
#endif
