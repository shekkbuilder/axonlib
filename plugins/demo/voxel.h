/*
   Landscape rendering

   Ok.. i know that voxel is something else... but a lot of people is using
   the name "voxel" to mean this kind of rendering tecnique.
   I wrote this to explain the basic idea behind the rendering of newvox4;
   newvox4 is very badly written (it's named 4 because is the fourth of
   a sequel of experiments) and is coded in pascal + asm.
   Since i got a few request of an explanation i decided to write the kernel
   of the rendering in C hoping that this will be easier to understand.
   This implements only the base landscape (no sky or floating ball) and
   with keyboard only support but i think you can get the idea of how I
   implemented those other things.

   I'm releasing this code to the public domain for free... and as it's
   probably really obvious there's no warranty of any kind on it.
   You can do whatever you want with this source; however a credit in any
   program that uses part of this code would be really appreciated :)

   Any comment is welcome :)

                                    Andrea "6502" Griffini, programmer
                                           agriff@ix.netcom.com
                                        http://vv.val.net/~agriffini
*/

// modified by ccernn

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <string.h>
//#include "SDL.h"

typedef unsigned char byte;

#include "core/axMath.h"

// a good test for these approximations ?
#define sin axSin
#define cos axCos
#define tan axTan

//----------------------------------------------------------------------

//    // Draw the frame
//    View(x0,y0,a,screen);
//    // Update position/angle
//    x0 += ss*cos(a);
//    y0 += ss*sin(a);
//    a  += sa;
//    // Slowly reset the angle to 0
//    if (sa!=0)
//    {
//      if (sa<0) sa+=0.001;
//      else sa-=0.001;
//    }
//    // User input
//    if (keystate[SDLK_UP])      ss+=s;
//    if (keystate[SDLK_DOWN])    ss-=s;
//    if (keystate[SDLK_RIGHT])   sa+=0.003;
//    if (keystate[SDLK_LEFT])    sa-=0.003;

//----------------------------------------------------------------------

class CVoxel
{
  private:

    byte  HMap[256*256];  // Height field
    byte  CMap[256*256];  // Color map
    byte  Video[320*200*4]; // Off-screen buffer
    int   lasty[320];     // Last pixel drawn on a given column
    int   lastc[320];     // Color of last pixel on a column

    int   i,k;
    float a;      // angle
    int   x0,y0;  // current position

    float FOV;
    float s;      // speed constant
    float ss;     // current forward/backward speed
    float sa;     // angular speed

    //byte  colors[256*256*4];

  public:

    CVoxel()
      {
        FOV = 3.141592654 / 4;   /* half of the xy field of view */
        ComputeMap();
        a  = 0;
        x0 = 0;
        y0 = 0;
        k  = 0;
        s  = 1024; //4096;
        ss = 0;
        sa = 0;
      }

    unsigned char* getBuffer(void) { return Video; }

    void set(int xx0, int yy0, float aa)
      {
        x0 = xx0;
        y0 = yy0;
        a  = aa;
      }

  private:

    // Reduces a value to 0..255 (used in height field computation)
    int Clamp(int x)
      {
        return (x<0 ? 0 : (x>255 ? 255 : x));
      }

    //----------

    // Draw a "section" of the landscape; x0,y0 and x1,y1 and the xy coordinates
    // on the height field, hy is the viewpoint height, s is the scaling factor
    // for the distance. x0,y0,x1,y1 are 16.16 fixed point numbers and the
    // scaling factor is a 16.8 fixed point value.

    void Line(int x0,int y0,int x1,int y1,int hy,int s)
      {
        int i,sx,sy;
        // Compute xy speed
        sx = (x1-x0) / 320;
        sy = (y1-y0) / 320;
        for (i=0; i<320; i++)
        {
          // Compute the xy coordinates; a and b will be the position inside the
          // single map cell (0..255).
          int u0 = ( x0>>16)&   0xFF;
          int v0 = ((y0>>8) & 0xFF00);
          int u1 = ( u0+1)  &   0xFF;
          int v1 = ( v0+256)& 0xFF00;

          int a  = ( x0>>8) & 255;
          int b  = ( y0>>8) & 255;

          // Fetch the height at the four corners of the square the point is in
          int h0 = HMap[u0+v0];
          int h1 = HMap[u1+v0];
          int h2 = HMap[u0+v1];
          int h3 = HMap[u1+v1];
          // Compute the height using bilinear interpolation
          h0 = ( h0<<8) + a*(h1-h0);
          h2 = ( h2<<8) + a*(h3-h2);
          int h  = ((h0<<8) + b*(h2-h0)) >> 16;

          // Fetch the color at the four corners of the square the point is in
          h0 = CMap[u0+v0];
          h1 = CMap[u1+v0];
          h2 = CMap[u0+v1];
          h3 = CMap[u1+v1];
          // Compute the color using bilinear interpolation (in 16.16)
          h0 = ( h0<<8) + a*(h1-h0);
          h2 = ( h2<<8) + a*(h3-h2);
          int c  = ((h0<<8) + b*(h2-h0));

          //int blue  = colors[(u0+v0)*4];
          //int green = colors[(u0+v0)*4+1];
          //int red   = colors[(u0+v0)*4+2];

          // Compute screen height using the scaling factor
          int y = (((h-hy)*s)>>11) + 100;

          // Draw the column
          if ( y < (a=lasty[i]) )
          {
            unsigned char *b = Video + (a*320 + i) * 4;
            int sc,cc;
            if (lastc[i]==-1) lastc[i]=c;
            sc = (c-lastc[i]) / (a-y);
            cc = lastc[i];
            if (a>199)
            {
              b  -= (a-199)*320 * 4;
              cc += (a-199)*sc;
              a   =    199;
            }
            if (y<0) y=0;
            while (y<a)
            {
              *b     = (cc>>16);//(blue  * (cc>>16)) >> 8;
              *(b+1) = (cc>>16);//(green * (cc>>16)) >> 8;
              *(b+2) = (cc>>17);//(red   * (cc>>16)) >> 8;
              *(b+3) = 0;//cc>>16;
              cc += sc;
              b  -= 320 * 4;
              a--;
            }
            lasty[i] = y;
          }
          lastc[i] = c;
          // Advance to next xy position
          x0 += sx;
          y0 += sy;
        }
      }

  public:

    // Heightfield and colormap computation
    void ComputeMap(void)
      {
        int p,i,j,k,k2,p2;
        // Start from a plasma clouds fractal
        HMap[0] = 128;
        for (p=256; p>1; p=p2)
        {
          p2 = p >> 1;
          k = p*8 + 20;
          k2 = k >> 1;
          for (i=0; i<256; i+=p)
          {
            for (j=0; j<256; j+=p)
            {
              int a,b,c,d;
              a = HMap[ (  i        <<8) +   j ];
              b = HMap[ (((i+p)&255)<<8) +   j ];
              c = HMap[ (  i        <<8) + ((j+p) & 255) ];
              d = HMap[ (((i+p)&255)<<8) + ((j+p) & 255) ];
              HMap[ (  i         <<8) + ((j+p2)&255) ] = Clamp(((a+c)    >>1)+(axRandInt()%k-k2));
              HMap[ (((i+p2)&255)<<8) + ((j+p2)&255) ] = Clamp(((a+b+c+d)>>2)+(axRandInt()%k-k2));
              HMap[ (((i+p2)&255)<<8) +   j ]          = Clamp(((a+b)    >>1)+(axRandInt()%k-k2));
            }
          }
        }
        // Smoothing
        for (k=0; k<3; k++)
          for (i=0; i<256*256; i+=256)
            for (j=0; j<256; j++)
            {
              HMap[i+j] = (HMap[((i+256)  &0xFF00)+j]
                        +  HMap[  i+((j+1)&  0xFF)  ]
                        +  HMap[((i-256)  &0xFF00)+j]
                        +  HMap[  i+((j-1)&  0xFF)  ])
                        >> 2;
            }
        // Color computation (derivative of the height field)
        for (i=0; i<256*256; i+=256)
          for (j=0; j<256; j++)
          {
            k = 128 + ( HMap[((i+256)&0xFF00)+((j+1)&255)] - HMap[i+j] ) * 4;
            if (k<0) k=0;
            if (k>255) k=255;
            CMap[i+j] = k;
          }
      }

//    void makeColors(void)
//      {
//        for (int x=0; x<256; x++)
//        {
//          for (int y=0; y<256; y++)
//          {
//            colors[(y*256+x)*4]   = axRandomInt(255); // b
//            colors[(y*256+x)*4+1] = axRandomInt(255); // g
//            colors[(y*256+x)*4+2] = axRandomInt(255); // r
//            colors[(y*256+x)*4+3] = 255;//0;
//          }
//        }
//      }

    //----------

    // Draw the view from the point x0,y0 (16.16) looking at angle a

    void View(float x,float y,float aa/*,SDL_Surface *screen*/)
    //void View(int x0,int y0,float aa/*,SDL_Surface *screen*/)
      {
        // 0..1 -> 8.16 fixed point
        int x0 = (int)(x*65536.0f) * 256;
        int y0 = (int)(y*65536.0f) * 256;
        //trace("x:"<<x<<" y:"<<y);
        //trace("x0:"<<x0<<" y0:"<<y0);

        int d;
        int a,b,h,u0,v0,u1,v1,h0,h1,h2,h3;
        /* Clear offscreen buffer */
        //memset(Video,0,320*200*4);
        axMemset(Video,128,320*200*4);
        /* Initialize last-y and last-color arrays */
        for ( d=0; d<320; d++ )
        {
          lasty[d]=200;
          lastc[d]=-1;
        }
        // Compute viewpoint height value
        // Compute the xy coordinates; a and b will be the position inside the
        // single map cell (0..255).
        u0 = (x0>>16)&0xFF;    a = (x0>>8)&255;
        v0 = ((y0>>8)&0xFF00); b = (y0>>8)&255;
        u1 = (u0+1)&0xFF;
        v1 = (v0+256)&0xFF00;
        /* Fetch the height at the four corners of the square the point is in */
        h0 = HMap[u0+v0]; h2 = HMap[u0+v1];
        h1 = HMap[u1+v0]; h3 = HMap[u1+v1];
        /* Compute the height using bilinear interpolation */
        h0 = (h0<<8) + a*(h1-h0);
        h2 = (h2<<8) + a*(h3-h2);
        h = ((h0<<8) + b*(h2-h0)) >> 16;
        /* Draw the landscape from near to far without overdraw */
        for ( d=0; d<100; d+=1+(d>>6) )
        {
          Line(x0+d*65536*cos(aa-FOV),y0+d*65536*sin(aa-FOV),
               x0+d*65536*cos(aa+FOV),y0+d*65536*sin(aa+FOV),
               h-30,100*256/(d+1));
        }
//        // Blit the final image to the screen
//        if ( SDL_LockSurface(screen) == 0 )
//        {
//          int row;
//          Uint8 *src, *dst;
//          src = Video;
//          dst = (Uint8 *)screen->pixels;
//          for ( row=screen->h; row>0; --row )
//          {
//            memcpy(dst, src, 320);
//            src += 320;
//            dst += screen->pitch;
//          }
//          SDL_UnlockSurface(screen);
//        }
//        SDL_UpdateRect(screen, 0, 0, 0, 0);
      }

};
