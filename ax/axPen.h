#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------
//TODO

#include "axCanvas.h"

#define DEF_PENWIDTH 0

class axPen
{
  protected:

    axColor mColor;
    int     mWidth;
    int     mStyle;

  public:

    axPen( axColor aColor=AX_GREY_LIGHT, int aWidth=DEF_PENWIDTH, int aStyle=0 )
      {
        mColor = aColor;
        mWidth = aWidth;
        mStyle = aStyle;
      }
    virtual ~axPen() {}

};

//----------------------------------------------------------------------
#endif


