#ifndef wdgImgSwitch_included
#define wdgImgSwitch_included
//----------------------------------------------------------------------

#include "axWidget.h"

class wdgImgSwitch : public axWidget
{
  private:
    axSurface*  mSurface;
    int         mImgWidth;
    int         mImgHeight;

  public:

    wdgImgSwitch(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, /*axParameter* aParameter=NULL,*/
                 float aValue=0, axSurface* aSurface=NULL)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mValue    = aValue;
        mSurface = aSurface;
        if(mSurface)
        {
          mImgWidth = mSurface->mWidth / 3;
          mImgHeight = mSurface->mHeight;
        }
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int num;
        if( mValue<0.5 ) num=0;
        else num=2;
        int bx = num * mImgWidth;
        int by = 0;
        aCanvas->blit(mSurface->mCanvas, mRect.x,mRect.y, bx,by,mImgWidth,mImgHeight );
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if( mValue>0.5 ) mValue=0;
        else mValue=1;
        if(mParameter) mParameter->mValue = mValue;
        mListener->onChange( this );
      }

};

//----------------------------------------------------------------------
#endif


