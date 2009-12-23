#ifndef wdgImgKnob_included
#define wdgImgKnob_included
//----------------------------------------------------------------------

#include "axSurface.h"
#include "wdgValue.h"

class wdgImgKnob : public wdgValue
{
  //private:
  protected:
    bool  is_vertical;
    int   mImgWidth, mImgHeight;
    //char buf[32];
    float val;
    char  name[32];
    char  display[32];
  protected:
    int mNumImages;
    axSurface* mSurface;
    //

  public:

    wdgImgKnob(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment, /*axParameter* aParameter=NULL,*/
               int aNumImages=0, axSurface* aSurface=NULL)
    : wdgValue(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        mNumImages = aNumImages;
        mSurface = aSurface;
        if(mSurface && mNumImages>0)
        {
          if(mSurface->mWidth < mSurface->mHeight)
          {
            is_vertical = true;
            mImgWidth = mSurface->mWidth;
            mImgHeight = mSurface->mHeight / mNumImages;
          }
          else
          {
            is_vertical = false;
            mImgWidth = mSurface->mWidth / mNumImages;
            mImgHeight = mSurface->mHeight;
          }
        }
      }

    //--------------------------------------------------

    void drawKnob(axCanvas* aCanvas, axRect aRect, float aValue, axString aName, axString aDisplay, int aFlags)
      {
        float n = floor( aValue * mNumImages );
        int num = axMin(n,(mNumImages-1));
        int bx,by;
        if(is_vertical) { bx=0; by=num*mImgHeight; }
        else { bx=num*mImgWidth; by=0; }
        aCanvas->blit(mSurface->mCanvas, mRect.x,mRect.y, bx,by,mImgWidth,mImgHeight );
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //axWidget::doPaint( aCanvas, aRect );

        if(mParameter)
        {
          name[0] = '\0';
          display[0] = '\0';
          val = mParameter->doGetValue();
          mParameter->doGetName( name );
          mParameter->doGetDisplay( display );
        }
        else
        {
          val = mValue;// * 0.8;
          strcpy( name, "[name]" );
          sprintf( display, "%f", val );
        }
        drawKnob(aCanvas,aRect,val,name,display,0);
      }
};

//----------------------------------------------------------------------
#endif




