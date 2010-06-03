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

#ifndef axBitmapRender_included
#define axBitmapRender_included
//----------------------------------------------------------------------
// assumes 32-bit
// LOTS of optimization could be done here!"
// also, check format rgba vs bgra, etc...

class axBitmapRender
{
  private:
    axBitmap*     mBitmap;
    int           mWidth;
    int           mHeight;
    unsigned int* mBuffer;

  public:

    axBitmapRender()
      {
      }

    virtual ~axBitmapRender()
      {
      }

    //--------------------------------------------------

    void setup(axBitmap* aBitmap)
      {
        mBitmap = aBitmap;
        mWidth  = aBitmap->getWidth();
        mHeight = aBitmap->getHeight();
        mBuffer = (unsigned int*)aBitmap->getBuffer();
      }

    //--------------------------------------------------

    inline unsigned char _alpha(int c, int a)
      {
        int ret = (c*a) >> 8;
        return ret & 0xff;
      }

    //----------

    void clear(void) { axMemset(mBuffer,0,mWidth*mHeight*4); }
    unsigned int getPixel(int x, int y) { return mBuffer[y*mWidth+x]; }
    void setPixel(int x, int y, unsigned int col) { mBuffer[y*mWidth+x] = col; }

    void blendPixel(int x, int y, unsigned int color, unsigned char alpha)
      {
        unsigned int  b1 = (color&0x000000ff);
        unsigned int  g1 = (color&0x0000ff00) >> 8;
        unsigned int  r1 = (color&0x00ff0000) >> 16;
        unsigned int  a1 = (color&0xff000000) >> 24;
        alpha = _alpha(alpha,a1);
        unsigned int  c  = getPixel(x,y);
        unsigned int  b2 = (c&0x0000ff);
        unsigned int  g2 = (c&0x00ff00) >> 8;
        unsigned int  r2 = (c&0xff0000) >> 16;
        unsigned char b  = _alpha(b1,alpha) + _alpha(b2,255-alpha);
        unsigned char g  = _alpha(g1,alpha) + _alpha(g2,255-alpha);
        unsigned char r  = _alpha(r1,alpha) + _alpha(r2,255-alpha);
        setPixel(x,y,(r<<16)+(g<<8)+b);
      }

    //--------------------------------------------------

    // http://www.users.on.net/~swcheetah/sam/Prog.html

    float frac(float x) { return x - (int)x; }
    float invfrac(float x) { return 1 - frac(x); }

    void drawWuLine(/*BITMAP *screen1,*/ float x1, float y1, float x2, float y2, unsigned int color)
      {
        float grad, xd, yd, xm, ym, xend, yend, xf, yf, brightness1, brightness2;
        int x, y, ix1, ix2, iy1, iy2;
        unsigned char c1, c2;
        //Width and Height of the line
        xd = (x2 - x1);
        yd = (y2 - y1);
        if(abs(xd)>abs(yd))
        {
          //Horizontal(ish) lines
          if(x1>x2)
          {
            xm = x1; x1 = x2; x2 = xm; //Swap x1 with x2
            ym = y1; y1 = y2; y2 = ym; //Swap y1 with y2
            xd = (x2 - x1);            //Calculate line width
            yd = (y2 - y1);            //Calculate line height
          }
          grad = yd / xd;                    //Calculate gradient
          //End Point 1
          xend = (int)(x1+.5);              //Find nearest integer X-coordinate
          yend = y1 + grad*(xend-x1);       //and corresponding Y value
          ix1 = (int)(xend);                 //calc screen coordinates
          iy1 = (int)(yend);
          yf = yend+grad;                      //calc first Y-intersection for main loop
          //End Point 2
          xend = (int)(x2+.5);                 //find nearest integer X-coordinate
          yend = y2 + grad*(xend-x2);          //and corresponding Y value
          ix2 = (int)(xend);                   //calc screen coordinates
          iy2 = (int)(yend);
          //MAIN LOOP
          for(x=ix1+1;x<=ix2-1;x++)
          {
            brightness1 = invfrac(yf);    //calc pixel brightnesses
            brightness2 =    frac(yf);
            c1 = (unsigned char)(brightness1 * 255);//calc pixel values
            c2 = (unsigned char)(brightness2 * 255);
            //setPixel(x,(int)yf,c1*0x010101);      //draw the pair of pixels
            //setPixel(x,(int)yf+1,c2*0x010101);
            blendPixel(x,(int)yf,  color,c1);      //draw the pair of pixels
            blendPixel(x,(int)yf+1,color,c2);
            yf += grad;
          }
        }
        else
        {
          //Vertical(ish) lines
          if(y1>y2)
          {
            xm = x1; x1 = x2; x2 = xm; //Swap x1 with x2
            ym = y1; y1 = y2; y2 = ym; //Swap y1 with y2
            xd = (x2 - x1);            //Calculate line width
            yd = (y2 - y1);            //Calculate line height
          }
          grad = xd / yd;                    //Calculate gradient
          //End Point 1
          xend = (int)(x1+.5);              //Find nearest integer X-coordinate
          yend = y1 + grad*(xend-x1);       //and corresponding Y value
          ix1 = (int)(xend);                 //calc screen coordinates
          iy1 = (int)(yend);
          xf = xend+grad;                      //calc first Y-intersection for main loop
          //End Point 2
          xend = (int)(x2+.5);                 //find nearest integer X-coordinate
          yend = y2 + grad*(xend-x2);          //and corresponding Y value
          ix2 = (int)(xend);                   //calc screen coordinates
          iy2 = (int)(yend);
          //MAIN LOOP
          for(y=iy1+1;y<=iy2-1;y++)
          {
            brightness1 = invfrac(xf);    //calc pixel brightnesses
            brightness2 =    frac(xf);
            c1 = (unsigned char)(brightness1 * 255);//calc pixel values
            c2 = (unsigned char)(brightness2 * 255);
            //setPixel((int)xf,y,c1*0x010101);      //draw the pair of pixels
            //setPixel((int)xf+1,y,c2*0x010101);
            blendPixel((int)xf,  y,color,c1);      //draw the pair of pixels
            blendPixel((int)xf+1,y,color,c2);

            xf += grad;
          }
        }
      }

};

//----------------------------------------------------------------------
#endif
