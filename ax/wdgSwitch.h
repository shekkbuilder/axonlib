#ifndef wdgSwitch_included
#define wdgSwitch_included
//----------------------------------------------------------------------

#include "axString.h"
#include "axWidget.h"

class wdgSwitch : public axWidget
{
  public:
    //bool      mState;
    int       mAlign;
    axColor   mOffColor;
    axColor   mOnColor;
    axString  mTextOff;
    axString  mTextOn;

  public:

    wdgSwitch(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, /*axParameter* aParameter=NULL,*/
              float aValue=0)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        setBackground(true,AX_GREY_DARK);
        mAlign    = tal_Center;
        mOffColor = AX_GREY;
        mOnColor  = AX_GREY_LIGHT;
        mTextOff  = "off";
        mTextOn   = "on";
        mValue    = aValue;
        if(mParameter) mValue = mParameter->doGetValue();
        else mValue = aValue;

      }

    //--------------------------------------------------

    void setup(axString aOff, axString aOn)
      {
        mTextOff = aOff;
        mTextOn = aOn;
      }

    void setup(char** aStrings)
      {
        mTextOff = aStrings[0];
        mTextOn = aStrings[1];
      }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    virtual void doSetValue(float aValue)
      {
        //axWidget::doSetValue(aValue);
        mValue = aValue;
        if(mParameter) mParameter->doSetValue(aValue);
        //if( mValue >= 0.5 ) mState=true;
        //else mState=false;
      }

    //----------

    virtual float doGetValue(void)
      {
        if(mParameter) return mParameter->doGetValue();
        else return mValue;
        //{
        //  if( mState) return 1;
        //  else return 0;
        //}

      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axWidget::doPaint(aCanvas,aRect);
        if( mValue>0.5 )
        {
          aCanvas->setTextColor( mOnColor );
          aCanvas->drawText( mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOn,mAlign);
        }
        else
        {
          aCanvas->setTextColor( mOffColor );
          aCanvas->drawText( mRect.x,mRect.y,mRect.x2()-5,mRect.y2(),mTextOff,mAlign);
        }
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


