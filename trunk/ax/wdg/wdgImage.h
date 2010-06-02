#ifndef wdgImage_included
#define wdgImage_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgImage : public axWidget//axContainer
{
  protected:
    bool        mSurfaceLoaded;
    axSurface*  mSurface;
    axImage*    mImage;

  public:

    wdgImage(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
             axImage* aImage=NULL)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
        mSurfaceLoaded = false;
        mImage = aImage;
      }

    //----------

    virtual ~wdgImage()
      {
        if (mSurfaceLoaded) delete mSurface;
      }

    //----------

    inline void setImage(axImage* aImage) { mImage = aImage; }

    //----------

    virtual void loadBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
    {
      axBitmap* bitmap = aEditor->createBitmap(aWidth,aHeight,aDepth);
      bitmap->createBuffer(aBuffer);
      bitmap->convertRgbaBgra();
      bitmap->premultAlpha();
      bitmap->prepare();
      //return bitmap;
      /*axSurface**/ mSurface = aEditor->createSurface(aWidth,aHeight,aDepth);
      mSurface->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight );
      mImage = mSurface;
      mSurfaceLoaded = true;
      delete bitmap;
    }

    //----------

    // todo: clipping

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mImage)
        {
          int dstx = mRect.x;
          int dsty = mRect.y;
          int srcx = 0;
          int srcy = 0;
          int srcw = mRect.w;
          int srch = mRect.h;
          aCanvas->drawImage(mImage,dstx,dsty, srcx,srcy,srcw,srch);
        }
        axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

