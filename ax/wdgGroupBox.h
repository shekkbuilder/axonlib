#ifndef wdgGroupBox_included
#define wdgGroupBox_included
//----------------------------------------------------------------------

//TODO:
//hierarch:
//  sub-container for 'main area'-
//  title-bar & button as (auto-aligned) widgets

class wdgGroupBox : public axContainer
{
  protected:
    axString  mTitleText;
    int       mTitleSize;
    axColor   mTitleColor;
    axColor   mTitleBack;
    bool      mClosable;
    bool      mClosed;
    int       mHeight;
  public:
    wdgGroupBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : axContainer(aListener,aID,aRect,aAlignment)
      {
        mTitleText  = "wdgGroupBox";
        mTitleSize  = 20;
        mTitleColor = AX_GREY_LIGHT;
        mTitleBack  = AX_GREY_DARK;
        mClosable   = false;//true;
        mClosed     = false;
        mHeight     = mRect.h;
      }

    virtual ~wdgGroupBox()
      {
      }

    void setup(axString aTitle, bool aClosed=false, bool aClosable=false)
      {
        mTitleText = aTitle;
        mClosed = aClosed;
        mClosable = aClosable;
      }

    void toggle_closed(void)
      {
        if (mClosed)
        {
          mClosed = false;
          mRect.h = mHeight;
        }
        else
        {
          mClosed = true;
          mRect.h = mTitleSize;
        }
      }

    //virtual void doResize(int aW, int aH)
    //  {
    //    //mHeight = aH;
    //    axContainer::doResize(aW,aH);
    //  }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( !mClosed )
        {
          aCanvas->setBrushColor( mFillColor );
          aCanvas->fillRect( mRect.x, mRect.y+mTitleSize, mRect.x2(), mRect.y2() );
          aCanvas->setPenColor( mTitleBack );
          aCanvas->drawRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
        }
        aCanvas->setBrushColor( mTitleBack );
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y+mTitleSize-1 );
        aCanvas->setTextColor( mTitleColor );
        aCanvas->drawText( mRect.x, mRect.y, mRect.x2(), mRect.y+mTitleSize-1, mTitleText, tal_Center );
        if( !mClosed ) axContainer::doPaint(aCanvas,aRect);
      }

    virtual void appendWidget(axWidget* aWidget, bool aRealign=true)
      {
        //aWidget->doMove( mRect.x + aWidget->mRect.x, mRect.y + aWidget->mRect.y );
        mWidgets.append(aWidget);
        if(aRealign)
        {
          aWidget->doMove( mRect.x + aWidget->mRect.x, mRect.y + aWidget->mRect.y + mTitleSize );
          //doRealign();
        }
      }

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        if (aB==1 && mClosable)
        {
          if (aY < (mRect.y+mTitleSize))
          {
            //if (mClosed) mClosed = false;
            //else mClosed = true;
            toggle_closed();
            mListener->onResize(this,0,0);
            //mListener->onRedrawAll();
          }
        }
      }

};

//----------------------------------------------------------------------
#endif

