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
  \file axBitmap.h
  \brief bitmaps (client/ram)
*/

#ifndef axBitmap_included
#define axBitmap_included
//----------------------------------------------------------------------
//#include "axBitmapBase.h"

/// bitmap base

class axBitmapBase
{
  //public:
  protected:
    //int   mHandle;
    int   mWidth;
    int   mHeight;
    int   mDepth;
    char* mBuffer;

  public:

    /// constructor
    /**
      creates a bitmap
      \param aWidth widht of bitmap (in pixels)
      \param aHeight height of bitmap (in pixels)
    */

    axBitmapBase(int aWidth, int aHeight)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mDepth  = 24;
        mBuffer = NULL;
      }

    /// destructor
    virtual ~axBitmapBase()
      {
      }

    virtual int   getWidth(void) { return mWidth; } ///< return width
    virtual int   getHeight(void) { return mHeight; } ///< return height
    virtual int   getDepth(void) { return mDepth; } ///< return depth (bits per pixel)
    virtual char* getBuffer(void) { return NULL; } ///< return memory buffer
    // overloaded by w32/x11
    virtual int   getHandle(void) { return 0; } ///< return bitmap handle

    //--------------------------------------------------

    /**
      mre-multiplies a value (r,g, or b) with a (alpha)
      this is in reality a 0.8 fixed point multiplication (for making color mixing easier)
    */
    inline unsigned char alpha(int c, int a)
      {
        int ret = (c*a) >> 8;
        return ret & 0xff;
      }

    //----------

    /**
      same as alpha(c,n), but with a float parameter for alpha
    */

    inline unsigned char scale(unsigned char c, float n )
      {
        float nc = n * (float)c;
        return (unsigned char)axMin(255,nc);
      }

    //--------------------------------------------------

    /// convert pixels from RGBA to BGRA
    /**

      the pixel array returned by lodepng is rgba, while both gdi and xlib requires (i think) BGRA,
      so we can use this to convert.

    */

    virtual void convertRgbaBgra(void)
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

    //----------

    /// set background color
    /**
      set the background color of the bitmap, taking alpha into account
    */
    virtual void setBackground(unsigned char aR, unsigned char aG, unsigned char aB)
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

    //----------

    //TODO:
    virtual void setBackground(axBitmapBase* aBitmap, axRect aRect) {}

    //----------

    /**
      color matrix.
      the convert RfbaBgra() could be done with:
      swizzle(0,0,1,0,
              0,1,0,0
              1,0,0,0
              0,0,0,1);
    */

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

//typedef axBitmapBase axBitmapImpl;

#ifdef linux
  #include "axBitmapX11.h"
#endif

#ifdef WIN32
  #include "axBitmapW32.h"
#endif

//----------------------------------------------------------------------

class axBitmapMem : public axBitmapBase
{
  public:
    char* mBuffer;
  public:
    axBitmapMem(int aWidth, int aHeight)
    : axBitmapBase(aWidth, aHeight)
      {
        mDepth  = 32;
        mBuffer = new char[mWidth*mHeight*4];
      }
    axBitmapMem(int aWidth, int aHeight, unsigned char* aData)
    : axBitmapBase(aWidth, aHeight)
      {
        if(mBuffer) memcpy(mBuffer,aData,mWidth*mHeight*4);
        //convert_rgba_bgra(aData);
      }
    virtual ~axBitmapMem()
      {
      }

};

//----------------------------------------------------------------------

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

//    /// convert pixels from RGBA to BGRA
//    /**
//
//      the pixel array returned by lodepng is rgba, while both gdi and xlib requires (i think) BGRA,
//      so we can use this to convert.
//
//    */
//
//    void convertRgbaBgra(void)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = b;
//              mBuffer[pos+1] = g;
//              mBuffer[pos+2] = r;
//              mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------------------------------------
//
//    /**
//      mre-multiplies a value (r,g, or b) with a (alpha)
//      this is in reality a 0.8 fixed point multiplication
//      (for making color mixing easier)
//    */
//
//    inline unsigned char alpha(int c, int a)
//      {
//        int ret = (c*a) >> 8;
//        return ret & 0xff;
//      }
//
//    //----------
//
//    /// set background color
//    /**
//      set the background color of the bitmap, taking alpha into account
//    */
//    void setBackground(unsigned char aR, unsigned char aG, unsigned char aB)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = alpha(r,a) + alpha(aR,(255-a));
//              mBuffer[pos+1] = alpha(g,a) + alpha(aG,(255-a));
//              mBuffer[pos+2] = alpha(b,a) + alpha(aB,(255-a));
//              //mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }
//
//    //----------
//
//    //TODO:
//    void setBackground(axBitmap* aBitmap, axRect aRect) {}
//
//    //----------
//
//    /**
//      same as alpha(c,n), but with a float parameter for alpha
//    */
//
//    unsigned char scale(unsigned char c, float n )
//      {
//        float nc = n * (float)c;
//        return (unsigned char)axMin(255,nc);
//      }
//
//    //----------
//
//    /**
//      color matrix.
//      the convert RfbaBgra() could be done with:
//      swizzle(0,0,1,0,
//              0,1,0,0
//              1,0,0,0
//              0,0,0,1);
//    */
//
//    void swizzle( float rr, float rg, float rb, float ra,
//                  float gr, float gg, float gb, float ga,
//                  float br, float bg, float bb, float ba,
//                  float ar, float ag, float ab, float aa)
//      {
//        if(mBuffer)
//        {
//          for(int y=0; y<mHeight; y++)
//          {
//            for(int x=0; x<mWidth; x++)
//            {
//              int pos = (y*mWidth + x) * 4;
//              unsigned char r = mBuffer[pos+0];
//              unsigned char g = mBuffer[pos+1];
//              unsigned char b = mBuffer[pos+2];
//              unsigned char a = mBuffer[pos+3];
//              mBuffer[pos+0] = scale(r,rr) + scale(g,rg) + scale(b,rb) + scale(a,ra);
//              mBuffer[pos+1] = scale(r,gr) + scale(g,gg) + scale(b,gb) + scale(a,ga);
//              mBuffer[pos+2] = scale(r,br) + scale(g,bg) + scale(b,bb) + scale(a,ba);
//              mBuffer[pos+3] = scale(r,ar) + scale(g,ag) + scale(b,ab) + scale(a,aa);
//              //mBuffer[pos+3] = a;
//            } //for x
//          } //for y
//        } //mBuffer
//      }


};

//----------------------------------------------------------------------
#endif

