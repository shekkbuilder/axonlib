#ifndef wdgTest_included
#define wdgTest_included
//----------------------------------------------------------------------

#include "gui/axContainer.h"

class myWidget : public axContainer//axWidget
{
  private:
    bool  mDragging;
    int   mYclick;
    float mSensitivity;
    float mClickVal;
  public:
    axString mName;
    bool mHighlight;

  public:

    //myWidget(axWidgetListener* aListener, axRect aRect=NULL_RECT, int aAlignment=wa_None)
    //: axWidget(aListener,aRect,aAlignment)
    myWidget(axWidgetListener* aListener, axRect aRect=NULL_RECT, int aAlignment=wa_None)
    : axContainer(aListener,aRect,aAlignment)
      {
        //mValue = 1;
        mDragging = false;
        mYclick = 0;
        mSensitivity = 0.005f;
        mClickVal = 0;
        //
        mHighlight = false;
      }

    //virtual ~myWidget() {}

    inline void highlight(bool aState) { mHighlight=aState; }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        char buf[256];
        //if (mSkin)
        //{
          aCanvas->selectBrush(1);
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          if (mHighlight)
          {
            aCanvas->selectPen(2);
            aCanvas->drawRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          }
          aCanvas->selectFont(0);
          if (mParameter) { mParameter->doGetDisplay(buf); }
          else sprintf(buf,"%.3f",mValue);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),buf,ta_Center);
        //}
        axContainer::doPaint(aCanvas,aRect);
      }

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        trace("[" << mName.ptr() << "] doMouseDown x,y " << aXpos << "," << aYpos << " but " << aButton);
        //if (mValue>0.5) mValue=0; else mValue=1;
        mDragging = true;
        mYclick = aYpos;
        mClickVal = mValue;
        //mListener->onChange(this);
        axContainer::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        trace("[" << mName.ptr() << "] doMouseUp x,y " << aXpos << "," << aYpos );
        mDragging = false;
        if (mHighlight)
        {
          mHighlight = false;
          mListener->onChange(this);
        }
        //if (mHighlight && !mRect.contains(aXpos,aYpos))
        //{
        //  mHighlight = false;
        //  mListener->onChange(this);
        //}

        //mHighlight = false;
        //mListener->onChange(this);
        axContainer::doMouseUp(aXpos,aYpos,aButton);
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        trace("[" << mName.ptr() << "] doMouseMove x,y " << aXpos << "," << aYpos );
        if (mDragging)
        {
          float ydist = (float)(mYclick - aYpos);
          mValue = mClickVal + (ydist * mSensitivity);
          if (mValue>1) mValue=1;
          if (mValue<0) mValue=0;
          mListener->onChange(this);
        }
        axContainer::doMouseMove(aXpos,aYpos,aButton);
      }

    virtual void doEnter(axWidget* aCapture)
      {
        trace("[" << mName.ptr() << "] doEnter");
        if (!aCapture)
        {
          mHighlight = true;
          mListener->onChange(this); //TODO: onRedraw(this);
        }
      }

    virtual void doLeave(axWidget* aCapture)
      {
        trace("[" << mName.ptr() << "] doLeave");
        if (!aCapture)
        {
          mHighlight = false;
          mListener->onChange(this);
        }
      }

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        trace("[" << mName.ptr() << "] doKeyDown " << aKeyCode << "," << aState);
      }

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        trace("[" << mName.ptr() << "] doKeyUp " << aKeyCode << "," << aState);
      }



};

//----------------------------------------------------------------------
#endif
