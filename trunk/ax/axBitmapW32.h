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
 * @file axBitmapW32.h
 * \brief bitmap for win32
 */

#ifndef axBitmapW32_included
#define axBitmapW32_included
//----------------------------------------------------------------------
#ifdef WIN32
//to remove this from doxygene

//#include "axGlobals.h"
//#include "axBitmapBase.h"

class axBitmapW32 : public axBitmapBase
{
  protected:
    HBITMAP mBitmap;

  public:

    axBitmapW32(int aWidth, int aHeight)
    : axBitmapBase(aWidth,aHeight)
      {
        //mPrepared = false;
      }

    //----------

    virtual ~axBitmapW32()
      {
        if (mPrepared) DeleteObject(mBitmap); // deletes allocated buffer
        //else
        //  if (mBuffer) delete[] mBuffer;
      }

    //----------

    virtual void prepare(void)
      {
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
          bmi.bmiHeader.biCompression = BI_RGB;           // unvompressed
          bmi.bmiHeader.biSizeImage   = mWidth*mHeight*4; // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
          HDC tempdc = GetDC(0);
          void* ptr;
          mBitmap = CreateDIBSection(tempdc, &bmi, DIB_RGB_COLORS, &ptr, NULL, 0);

          if (mBuffer)
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

typedef axBitmapW32 axBitmapImpl;

#endif //WIN32
//----------------------------------------------------------------------
#endif
