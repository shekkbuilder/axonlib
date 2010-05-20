#ifndef axBitmap_included
#define axBitmap_included
//----------------------------------------------------------------------

// - linux/ubuntu (32bit), amd phenom (x86 quadcore), nvidia (geforce 8600 gt)
//   32 bits, bgra
//   0xAARRGGBB
//   little endian
//   buffer[0] = b
//   buffer[1] = g
//   buffer[2] = r
//   buffer[3] = a

#include "axDefines.h"
#include "core/axMath.h"
#include "core/axRand.h"
#include "platform/axContext.h"
#include "base/axBitmapBase.h"

//----------

#ifdef AX_LINUX
  #include "platform/linux/axBitmapLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axBitmapWin32.h"
#endif


class axBitmap : public axBitmapImpl
{
  public:

    axBitmap(axContext* aContext, int aWidth, int aHeight, int aDepth)
    : axBitmapImpl(aContext,aWidth, aHeight, aDepth)
      {
        //wtrace("axBitmap.constructor");
      }

    //----------

    // the mBuffer is handled a bit differently for win32/linux,
    // depending on if we prepare() the bitmap.
    // so we delete it in axBitmaplinux/Win32 destructors..

    virtual ~axBitmap()
      {
        //if (mBuffer) delete[] mBuffer;
        //mBuffer = NULL;
      }

    //----------------------------------------

    char* createBuffer(char* aData=NULL)
      {
        int size = mWidth*mHeight*4;
        // malloc ?
        mBuffer = new char[size];

        if (aData) axMemcpy(mBuffer,(int)aData,size);
        else axMemset(mBuffer,0,size);
        return mBuffer;
      }

    //----------------------------------------

    inline unsigned char alpha(int c, int a)
      {
        int ret = (c*a) >> 8;
        return ret & 0xff;
      }

    //----------------------------------------

    unsigned char scale(unsigned char c, float n )
      {
        float nc = n * (float)c;
        return (unsigned char)axMin(255,nc);
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
              mBuffer[pos+3] = a;
            } //for x
          } //for y
        } //mBuffer
      }

    //----------

    //TODO:
    void setBackground(axBitmap* aBitmap, axRect aRect)
      {
        char* buf2 = aBitmap->getBuffer();
        int h = aBitmap->getHeight();
        //trace("aBitmap->getHeight() = " << h );
        if (mBuffer)
        {
          int x2 = aRect.x;
          int y2 = aRect.y;
          int w2 = aRect.x2();
          int h2 = aRect.y2();
          for(int y=0; y<mHeight; y++)
          {
            x2 = aRect.x;
            for(int x=0; x<mWidth; x++)
            {
              int pos = (y*mWidth + x) * 4;
              unsigned char r = mBuffer[pos+0];
              unsigned char g = mBuffer[pos+1];
              unsigned char b = mBuffer[pos+2];
              unsigned char a = mBuffer[pos+3];
              int pos2 = (y2*h + x2) * 4;
              //trace("pos2 = " << pos2 );
              unsigned char r2 = buf2[pos2+0];
              unsigned char g2 = buf2[pos2+1];
              unsigned char b2 = buf2[pos2+2];
              //unsigned char a2 = buf2[pos+3];

              mBuffer[pos+0] = alpha(r,a) + alpha(r2,(255-a));
              mBuffer[pos+1] = alpha(g,a) + alpha(g2,(255-a));
              mBuffer[pos+2] = alpha(b,a) + alpha(b2,(255-a));
              mBuffer[pos+3] = a;

              x2++;
              if (x2>=w2) x2 = aRect.x;
            } //for x
            y2++;
            if (y2>=h2) y2 = aRect.y;
          } //for y

        } //mBuffer
      }

    //----------

    void swizzle( float rr, float rg, float rb, float ra,
                  float gr, float gg, float gb, float ga,
                  float br, float bg, float bb, float ba,
                  float ar, float ag, float ab, float aa)
      {
        if(mBuffer)
        {
          trace("swizzle");
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
