#ifndef wdgImage_included
#define wdgImage_included
//----------------------------------------------------------------------

#include "axWidget.h"
#include "axContainer.h"
#include "axSurface.h"

class wdgImage : public axWidget
{
  protected:
    axSurface* mSurface;

  public:

    wdgImage(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, /*axParameter* aParameter=NULL,*/ axSurface* aSurface=NULL)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        clearFlag(wfl_Active);
        mSurface = aSurface;
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if(hasFlag(wfl_Visible))
        {
          axRect R = mRect;
          R.crop( aRect.x, aRect.y, aRect.w, aRect.h );
          //TRACE("x:%i  y:%i  w:%i  h:%i\n",R.x,R.y,R.w,R.h);
          if(R.w>0 && R.h>0) aCanvas->blit( mSurface->mCanvas, R.x,R.y, R.x,R.y,R.w,R.h );
        }
      }

};

//----------------------------------------------------------------------
#endif





