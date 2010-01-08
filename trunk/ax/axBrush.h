#ifndef axBrush_included
#define axBrush_included
//----------------------------------------------------------------------
//TODO

#include "axCanvas.h"

class axBrush
{
  protected:

    axColor mColor;
    int     mStyle;

  public:

    axBrush( axColor aColor=AX_GREY, int aStyle=0 )
      {
        mColor = aColor;
        mStyle = aStyle;
      }
    virtual ~axBrush() {}

};

//----------------------------------------------------------------------
#endif


