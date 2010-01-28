/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file axColor.h
 * \brief color class
 */

/**
 * \brief color class 
 */
 
#ifndef axColor_included
#define axColor_included
//----------------------------------------------------------------------

/// color class

#include "axGlobals.h"

/// pre-defined colors

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

      }
    
    /**
     * get random color and set
     * @param[in] flag int 0=normal, 1=light, 2=dark
     * @param[in] thd int threshold [0..255]
     */
    void random(const int flag=0, int thd=0)
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
