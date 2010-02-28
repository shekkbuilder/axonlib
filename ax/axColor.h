/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0 
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See LICENSE_AX for more details.
 *  
 * You should have received a copy of the Axonlib License 
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
 * @file axColor.h
 * \brief platform/device dependent color
 */

#ifndef axColor_included
#define axColor_included
//----------------------------------------------------------------------

#include "axGlobals.h"

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

/**
 * \brief platform/device dependent color
 */
struct axColor
{
    //public:
    int mColor;

    /// empty constructor
    axColor() { mColor=0; }

    /**
     constructor w/initial setup         
     * @param[in] aR int red value
     * @param[in] aG int green value
     * @param[in] aB int blue value 
    */
    axColor(int aR,int aG,int aB) { set(aR,aG,aB); }
    //axColor(float aR,float aG,float aB) { setcolor(aR*255,aG*255,aB*255); }

    //void  operator = (axColor col)   { mColor = col.mColor; }
    //void  operator = (axColor* col)  { mColor = col->mColor; }
    //void  operator = (int  i)        { mColor = i; }
    //void  operator = (const int i)   { mColor = i; }

    /**
     create/set color
     * @param[in] aR int red value
     * @param[in] aG int green value
     * @param[in] aB int blue value     
     * @return void
     */
    void set(int aR, int aG, int aB)
      {
        // win32
        #ifdef WIN32
        mColor = RGB(aR,aG,aB);
        #endif
        // linux
        #ifdef linux
          XColor xcol;
          xcol.red   = aR << 8;
          xcol.green = aG << 8;
          xcol.blue  = aB << 8;
          xcol.flags = (DoRed or DoGreen or DoBlue);
          XAllocColor(gDP,XDefaultColormap(gDP,0),&xcol);
          mColor = xcol.pixel;
        #endif
      }

    /**
     * get random color and set <br>
     * default threshold value for dark color is 96 and for light 192.     
     * @param[in] flag int 0=normal, 1=light, 2=dark
     * @param[in] thd int threshold [0..255]
     * @return void     
     */
    // fixes "warning: comparison between signed and unsigned integer expression"
    //void random(const int flag=0, int thd=0)
    void random(const int flag=0, unsigned int thd=0)
      {
        unsigned int r,g,b;
        switch(flag)
        {
          // normal
          case 0:
            r = axRandomInt(255);
            g = axRandomInt(255);
            b = axRandomInt(255);
          break;
          // light
          case 1:
            r = g = b = 0;
            if (thd == 0) thd = 192;
            while (true)
            {
              if (r < thd) r = axRandomInt(255);
              if (g < thd) g = axRandomInt(255);
              if (b < thd) b = axRandomInt(255);
              if (r > thd || g > thd || b > thd) break;
            }
          break;
          // dark
          case 2:
            r = g = b = 255;
            if (thd == 0) thd = 96;
            while (r > thd || g > thd || b > thd)
            {
              if (r > thd) r = axRandomInt(255);
              if (g > thd) g = axRandomInt(255);
              if (b > thd) b = axRandomInt(255);
            }
          break;
        }
        // set
        set(r, g, b);
      }
};

//----------------------------------------------------------------------

/// initialize a default set of colors

void initDefaultColors(void)
  {
  }

//----------------------------------------------------------------------
#endif
