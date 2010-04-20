#ifndef axBitmapWin32_included
#define axBitmapWin32_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axBitmapBase.h"

class axBitmapWin32 : public axBitmapBase
{
  //protected:
  //  Display* mDisplay;
  protected:
    HBITMAP mBitmap;
    HWND mWin;

  public:

    axBitmapWin32(axContext* aContext, int aWidth, int aHeight)
    : axBitmapBase(aContext,aWidth,aHeight)
      {
        mWin = aContext->mWindow;
      }

    virtual ~axBitmapWin32()
      {
        if (mPrepared)
        {
          DeleteObject(mBitmap); // deletes allocated buffer
        }
      }

    //----------

    virtual void prepare(void)
      {
        //trace("prepare");
        if (!mPrepared)
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
          bmi.bmiHeader.biCompression = BI_RGB;           // uncompressed
          bmi.bmiHeader.biSizeImage   = 0;//mWidth*mHeight*4; // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.

          //HDC tempdc = GetDC(0);
          HDC tempdc = GetDC(mWin);

          void* ptr;

          //http://msdn.microsoft.com/en-us/library/dd183494%28VS.85%29.aspx
          //
          //HBITMAP CreateDIBSection(
          //  HDC hdc,
          //  const BITMAPINFO *pbmi,
          //  UINT iUsage,
          //  VOID **ppvBits,
          //  HANDLE hSection,
          //  DWORD dwOffset
          //);
          //
          //The CreateDIBSection function creates a DIB that applications can write to directly.
          //The function gives you a pointer to the location of the bitmap bit values.
          //You can supply a handle to a file-mapping object that the function will use to create the bitmap,
          //or you can let the system allocate the memory for the bitmap.
          //
          //If hSection is NULL, the system allocates memory for the DIB.
          //In this case, the CreateDIBSection function ignores the dwOffset parameter.
          //An application cannot later obtain a handle to this memory.
          //The dshSection member of the DIBSECTION structure filled in
          //by calling the GetObject function will be NULL.
          //
          //If the function succeeds, the return value is a handle to the newly created DIB,
          //and *ppvBits points to the bitmap bit values.
          //If the function fails, the return value is NULL, and *ppvBits is NULL.

          mBitmap = CreateDIBSection(tempdc, &bmi, DIB_RGB_COLORS, &ptr, NULL, 0);

          // this next memcpy seems to crash in w7

          if (ptr && mBuffer)
          {
            memcpy(ptr,mBuffer,mWidth*mHeight*4);
            //delete[] mBuffer; //caller's responsibility?
            //mBuffer = (char*)ptr;
          }
          /*else*/ mBuffer = (char*)ptr;

          ReleaseDC(0,tempdc);
          mPrepared = true;
        } // !prepared
      }

    //----------

    virtual int getHandle(void) { return (int)mBitmap; }
    virtual char* getBuffer(void) { return (char*)mBuffer; }


};

typedef axBitmapWin32 axBitmapImpl;

#endif
//----------------------------------------------------------------------
