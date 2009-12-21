#ifndef wdgGroupBox_included
#define wdgGroupBox_included
//----------------------------------------------------------------------

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
        mClosed = false;
      }

    virtual ~wdgGroupBox()
      {
      }

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

//    inline void appendWidget(axWidget* aWidget)
//      {
//        //aWidget->mRect.y += mTitleSize;
//        //aWidget->doMove( mRect.x + aWidget->mRect.x, mRect.y + aWidget->mRect.y + mTitleSize );
//        //mWidgets.append(aWidget);
//        axContainer::appendWidget(aWidget);
//      }

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

//    virtual void doMouseDown(int aX, int aY, int aB)
//      {
//        if( aY-mRect.y < mTitleSize )
//        {
//          if( mClosed )
//          {
//            mRect.h = mHeight;
//            mClosed = false;
//            mListener->onResized(mRect.w, mTitleSize);
//          }
//          else
//          {
//            mHeight = mRect.h;
//            mRect.h = mTitleSize;
//            mListener->onResized(mRect.w, mTitleSize);
//            mClosed = true;
//          }
//        } //<20
//        axContainer::doMouseDown(aX,aY,aB);
//      }

};

//----------------------------------------------------------------------
#endif

