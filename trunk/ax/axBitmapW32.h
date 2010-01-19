#ifndef axBitmapW32_included
#define axBitmapW32_included
//----------------------------------------------------------------------

//#include "axGlobals.h"
//#include "axBitmapBase.h"

class axBitmapImpl : public axBitmapBase
{
  public:
    HBITMAP mBitmap;
    char*   mBuffer;

  public:

    axBitmapImpl(int aWidth, int aHeight)
    : axBitmapBase(aWidth,aHeight)
      {
        // http://msdn.microsoft.com/en-us/library/dd183375%28VS.85%29.aspx
        // If the height of the bitmap is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner.
        // If the height is negative, the bitmap is a top-down DIB and its origin is the upper left corner.
        //TODO: cleanup if already mPrepared?
        BITMAPINFO bmi;
        memset(&bmi,0,sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = mWidth;           // width of the bitmap, in pixels.
        bmi.bmiHeader.biHeight      = -mHeight;         // height (negative)
        bmi.bmiHeader.biPlanes      = 1;                // number of planes for target device. must be set to 1.
        bmi.bmiHeader.biBitCount    = 32;               // bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB;           // unvompressed
        bmi.bmiHeader.biSizeImage   = mWidth*mHeight*4; // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
        HDC tempdc = GetDC(0);
        void* ptr;
        mBitmap = CreateDIBSection(tempdc, &bmi, DIB_RGB_COLORS, &ptr, NULL, 0);
        mBuffer = (char*)ptr;
        ReleaseDC(0,tempdc);
      }

    //----------

    virtual ~axBitmapImpl()
      {
        DeleteObject(mBitmap); // deletes allocated buffer
      }

    //----------

    virtual int getHandle(void) { return (int)mBitmap; }
    virtual char* getBuffer(void) { return (char*)mBuffer; }

};


//----------------------------------------------------------------------
#endif