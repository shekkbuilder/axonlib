#ifndef axColor_included
#define axColor_included
//----------------------------------------------------------------------

/// platform/device dependent color

#include "axGlobals.h"

// some pre-defined colors

  #define AX_BLACK        axColor(   0,   0,   0 )
  #define AX_GREY_DARK    axColor(  64,  64,  64 )
  #define AX_GREY         axColor( 128, 128, 128 )
  #define AX_GREY_LIGHT   axColor( 192, 192, 192 )
  #define AX_WHITE        axColor( 255, 255, 255 )

  #define AX_RED          axColor( 255,   0,   0 )
  #define AX_ORANGE       axColor( 255, 128,   0 )
  #define AX_YELLOW       axColor( 255, 255,   0 )
  #define AX_GREEN        axColor(   0, 255,   0 )
  #define AX_CYAN         axColor(   0, 255, 255 )
  #define AX_BLUE         axColor(   0,   0, 255 )
  #define AX_MAGENTA      axColor( 255,   0, 255 )

  #define AX_RED_DARK     axColor( 128,   0,   0 )
  #define AX_ORANGE_DARK  axColor( 128,  64,   0 )
  #define AX_YELLOW_DARK  axColor( 128, 128,   0 )
  #define AX_GREEN_DARK   axColor(   0, 128,   0 )
  #define AX_CYAN_DARK    axColor(   0, 128, 128 )
  #define AX_BLUE_DARK    axColor(   0,   0, 128 )
  #define AX_MAGENTA_DARK axColor( 128,   0, 128 )

struct axColor
{
  //public:

    int mColor;

  //public:

    /// empty constructor

    axColor() { mColor=0; }

    /// constructor w/initial setup

    axColor(int aR,int aG,int aB) { set(aR,aG,aB); }
    //axColor(float aR,float aG,float aB) { setcolor(aR*255,aG*255,aB*255); }

    //void  operator = (axColor col)   { mColor = col.mColor; }
    //void  operator = (axColor* col)  { mColor = col->mColor; }
    //void  operator = (int  i)        { mColor = i; }
    //void  operator = (const int i)   { mColor = i; }

    /// create/set color

    void set(int aR, int aG, int aB)
      {

        #ifdef WIN32
        mColor = RGB(aR,aG,aB);
        #endif

        #ifdef linux
          XColor xcol;
          xcol.red   = aR << 8;
          xcol.green = aG << 8;
          xcol.blue  = aB << 8;
          xcol.flags = (DoRed or DoGreen or DoBlue);
          XAllocColor(gDP,XDefaultColormap(gDP,0),&xcol);
          mColor = xcol.pixel;
        #endif

      };

};

//----------------------------------------------------------------------

/// initialize a default set of colors

void initDefaultColors(void)
  {
  }

//----------------------------------------------------------------------
#endif
