#ifndef axBitmapBase_included
#define axBitmapBase_included
//----------------------------------------------------------------------

/*

  user-mem (local) bitmap. let yu access the pixels/ints directly,
  and draw to a canvas with drawImage()

  note that the rgb format is a bit shaky, more work should be done here

  some functions to convert from our 'universal' format,
  to a format accepted by bitblt, XPutImage would be nice.
  or a format specifier variable/flag, so we could check purselves,
  and do what we want..
  considerations: 16/24/32-bit screens, rgba, bgra, rgb, ...

  and/or check capabilities for automatic conversion by the os/platform.
  i think bitblt (win32) is doing some of that, but i don't know about
  xlib's XPutImage/XCopyArea..

*/

#include "platform/axContext.h"
#include "base/axImage.h"

//class axBitmapBase : public axImage
//{
//  public:
//    axBitmapBase(axContext* aContext) {}
//    virtual ~axBitmapBase() {}
//};

class axBitmapBase
{
  //public:
  protected:
    //int   mHandle;
    bool  mPrepared;
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

    axBitmapBase(axContext* aContext, int aWidth, int aHeight)
      {
        mPrepared = false;
        mWidth    = aWidth;
        mHeight   = aHeight;
        mDepth    = 24;         // !!!
        mBuffer   = NULL;
      }

    /// destructor
    virtual ~axBitmapBase()
      {
        //if (mBuffer) delete[] mBuffer;
      }

    // accessors

    virtual int   getWidth(void)    { return mWidth; }
    virtual int   getHeight(void)   { return mHeight; }
    virtual int   getDepth(void)    { return mDepth; }
    virtual char* getBuffer(void)   { return mBuffer; }

    // overloaded by w32/x11

    virtual int   getHandle(void) { return 0; }
    virtual void  prepare(void) {}

};


//----------------------------------------------------------------------
#endif