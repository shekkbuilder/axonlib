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
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */
 
#ifndef axBitmap_included
#define axBitmap_included
//----------------------------------------------------------------------
//#include "axBitmapBase.h"

class axBitmapBase
{
  public:
    //int   mHandle;
    int   mWidth;
    int   mHeight;
    int   mDepth;

  public:
    axBitmapBase(int aWidth, int aHeight)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mDepth  = 24;
        //mBuffer = NULL;
      }
    virtual ~axBitmapBase() {}
    virtual int   getHandle(void) { return 0; }
    virtual char* getBuffer(void) { return NULL; }
    virtual int   getWidth(void) { return mWidth; }
    virtual int   getHeight(void) { return mHeight; }
};

//----------------------------------------------------------------------

//typedef axBitmapBase axBitmapImpl;

#ifdef linux
  #include "axBitmapX11.h"
#endif

#ifdef WIN32
  #include "axBitmapW32.h"
#endif

//----------------------------------------------------------------------

//class axMemBitmap : public axBitmapBase
//{
//  public:
//    char* mBuffer;
//  public:
//    axMemBitmap(int aWidth, int aHeight)
//    : axBitmapBase(aWidth, aHeight)
//      {
//        mDepth  = DefaultDepth(gDP,DefaultScreen(gDP)); // 32?
//        mBuffer = new char[mWidth*mHeight*4];
//      }
//    axMemBitmap(int aWidth, int aHeight, unsigned char* aData)
//    : axBitmapBase(aWidth, aHeight)
//      {
//        if(mBuffer) memcpy(mBuffer,aData,mWidth*mHeight*4);
//        //convert_rgba_bgra(aData);
//      }
//    virtual ~axMemBitmap()
//      {
//      }
//
//};

//----------------------------------------------------------------------

//----------

class axBitmap : public axBitmapImpl
{
  public:

    axBitmap(int aWidth, int aHeight)
    : axBitmapImpl(aWidth, aHeight)
      {
      }

    //----------

    axBitmap(int aWidth, int aHeight, unsigned char* aData)
    : axBitmapImpl(aWidth, aHeight)
      {
        if(mBuffer) memcpy(mBuffer,aData,mWidth*mHeight*4);
        //convert_rgba_bgra(aData);
      }

    //----------

    virtual ~axBitmap()
      {
      }

    //----------------------------------------

    void convertRgbaBgra(void)
      {
        if(mBuffer)
        {
          for(int y=0; y<mHeight; y++)
          {
            for(int x=0; x<mWidth; x++)
            {
              int pos = (y*mWidth + x) * 4;
              unsigned char r = mBuffer[pos+0];
              unsigned char g = mBuffer[pos+1];
              unsigned char b = mBuffer[pos+2];
              unsigned char a = mBuffer[pos+3];
              mBuffer[pos+0] = b;
              mBuffer[pos+1] = g;
              mBuffer[pos+2] = r;
              mBuffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

    //----------------------------------------

    inline unsigned char alpha(int c, int a)
      {
        int ret = (c*a) >> 8;
        return ret & 0xff;
      }

    //----------

    void setBackground(unsigned char aR, unsigned char aG, unsigned char aB)
      {
        if(mBuffer)
        {
          for(int y=0; y<mHeight; y++)
          {
            for(int x=0; x<mWidth; x++)
            {
              int pos = (y*mWidth + x) * 4;
              unsigned char r = mBuffer[pos+0];
              unsigned char g = mBuffer[pos+1];
              unsigned char b = mBuffer[pos+2];
              unsigned char a = mBuffer[pos+3];
              mBuffer[pos+0] = alpha(r,a) + alpha(aR,(255-a));
              mBuffer[pos+1] = alpha(g,a) + alpha(aG,(255-a));
              mBuffer[pos+2] = alpha(b,a) + alpha(aB,(255-a));
              //mBuffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

    unsigned char scale(unsigned char c, float n )
      {
        float nc = n * (float)c;
        return (unsigned char)axMin(255,nc);
      }

    void swizzle( float rr, float rg, float rb, float ra,
                  float gr, float gg, float gb, float ga,
                  float br, float bg, float bb, float ba,
                  float ar, float ag, float ab, float aa)
      {
        if(mBuffer)
        {
          for(int y=0; y<mHeight; y++)
          {
            for(int x=0; x<mWidth; x++)
            {
              int pos = (y*mWidth + x) * 4;
              unsigned char r = mBuffer[pos+0];
              unsigned char g = mBuffer[pos+1];
              unsigned char b = mBuffer[pos+2];
              unsigned char a = mBuffer[pos+3];
              mBuffer[pos+0] = scale(r,rr) + scale(g,rg) + scale(b,rb) + scale(a,ra);
              mBuffer[pos+1] = scale(r,gr) + scale(g,gg) + scale(b,gb) + scale(a,ga);
              mBuffer[pos+2] = scale(r,br) + scale(g,bg) + scale(b,bb) + scale(a,ba);
              mBuffer[pos+3] = scale(r,ar) + scale(g,ag) + scale(b,ab) + scale(a,aa);
              //mBuffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }


};

//----------------------------------------------------------------------
#endif

