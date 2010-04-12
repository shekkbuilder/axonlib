#ifndef axFont_included
#define axFont_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "core/axString.h"

//----------------------------------------------------------------------

#ifdef AX_LINUX

struct axFont
{
  axString  mName;
  int       mSize;
  int       mStyle;
};

#endif

#ifdef AX_WIN32

struct axFont
{
  axString  mName;
  int       mSize;
  int       mStyle;
};

#endif

typedef axArray<axFont> axFonts;

//----------------------------------------------------------------------
#endif
