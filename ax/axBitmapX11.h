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
 * @file axBitmapX11.h
 * \brief bitmap for X11
 */

#ifndef axBitmapX11_included
#define axBitmapX11_included
//----------------------------------------------------------------------
#ifdef linux
//to remove this from doxygene

#include "axGlobals.h"

class axBitmapX11 : public axBitmapBase
{
  //private:
  //  bool mPrepared;
  protected:
    XImage* mImage;
    //char*   mBuffer;
  public:

    axBitmapX11(int aWidth, int aHeight)
    : axBitmapBase(aWidth,aHeight)
      {
        //mPrepared = false;
        //prepare();
      }

    virtual ~axBitmapX11()
      {
        //trace("axBitmapX11.destructor");
        if (mPrepared)
        {
          //((XImage*)mImage)->data = NULL;    // we want to delete it ourselves...
          mImage->data = NULL;    // we want to delete it ourselves...
          XDestroyImage(mImage);  // frees data too
        }
        //if (mBuffer) delete[] mBuffer;
        //mBuffer = NULL;
      }

    //----------

    virtual void prepare(void)
      {
        //trace("prepare");
        if (!mPrepared)
        {
          mDepth  = DefaultDepth(gDP,DefaultScreen(gDP)); // 32?

          //mBuffer = new char[mWidth*mHeight*4];
          if (!mBuffer) mBuffer = new char[mWidth*mHeight*4];

          mImage = XCreateImage(
            gDP,
            //CopyFromParent,     // DefaultVisual(gDP,DefaultScreen(gDP)),  // visual
            DefaultVisual(gDP,DefaultScreen(gDP)),  // visual
            mDepth,             // depth
            ZPixmap,            // format
            0,                  // offset
            mBuffer,            // data
            mWidth, mHeight,    // size
            32,                 // pad
            0                   // bytes per line
          );
          mPrepared = true;
        } // !prepared
      }

    //----------

    virtual int getHandle(void) { return (int)mImage; }
    //virtual char* getBuffer(void) { return (char*)mBuffer; }

};

typedef axBitmapX11 axBitmapImpl;

        // http://www.sbin.org/doc/Xlib/chapt_06.html
        //
        // XDestroyImage()
        // Frees the data field in an image structure if the image structure was allocated in the application.
        // If the image was created using XCreateImage(), XGetImage(), or XGetSubImage(),
        // XDestroyImage() frees both the data and the image structure. Note that if the image data is stored
        // in static memory in the application, it cannot be freed
        // --to free an image created with XCreateImage() that has statically allocated data, you must set NULL
        // nto the data field before calling XDestroyImage().

//        if (mImage->data)
//        {
//          //delete mImage->data;           // we allocated it, so we free it
//          delete mBuffer;
//          //mImage->data = (char *)NULL;  //  instead of XDestroyImage()
//        }
        //if (mPrepared)
        //{
        //  mHandle->data = NULL;  // we want to delete it ourselves...
        //  XDestroyImage(mHandle); // frees data too
        //}

//    virtual bool prepare(void)
//      {
//        //TODO: cleanup if already mPrepared?
//        mDepth  = DefaultDepth(gDP,DefaultScreen(gDP)); // 32?
//        // http://tronche.com/gui/x/xlib/utilities/XCreateImage.html
//        // does not allocate space for the image itself
//        // initializes the structure byte-order, bit-order, and bitmap-unit values from the display
//        //
//        // http://www.sbin.org/doc/Xlib/chapt_06.html
//        // XCreateImage()
//        // Allocates memory for an XImage structure and sets various members.
//        // Note that it uses the server's data format, which is often not appropriate.
//        // The byte- and bit-order fields should usually be changed directly to the client-native format.
//        // However, then the call _XInitImageFuncPtrs(image) should be issued to reset the mapping
//        // to the appropriate versions of the functions for manipulating the image.
//        // This call is supposed to be private to Xlib and, therefore, should be watched for changes
//        // in later releases, but this is currently the accepted method.
//        mHandle = XCreateImage(
//          gDP,
//          CopyFromParent,     // DefaultVisual(gDP,DefaultScreen(gDP)),  // visual
//          mDepth,             // depth
//          ZPixmap,            // format
//          0,                  // offset
//          mBuffer,            // data
//          mWidth, mHeight,    // size
//          32,                 // pad
//          0                   // bytes per line
//        );
//        //mImage->byte_order = get_byte_order();
//        mPrepared = true;
//        return true;
//      };

//    virtual void  unprepare(void) {}
//    virtual void* ptr(void) {return NULL;}
//    virtual void* ptr(int aXofs, int aYofs) {return NULL;}
//    virtual void  clear(int aValue=0) {}
//    virtual bool  load(axString aFilename) {return false;}
//    virtual bool  save(axString aFilename) {return false;}

#endif //linux
//----------------------------------------------------------------------
#endif


//cw->base_image  = XCreateImage(cw->display, visual, depth, ZPixmap, 0, 0, width, height, depth, 0);
//if (cw->base_image)
//{
//  cw->base_image->data =
//  malloc(cw->base_image->bytes_per_line * height);
//}
//
//if (cw->base_image) XDestroyImage(cw->base_image);
//cw->base_image = NULL;
//
