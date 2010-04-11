#ifndef wdgButton_included
#define wdgButton_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgButton : public axWidget
{
  protected:
    bool      mState;
    axString  mOffText,mOnText;
    int       mTextAlign;

  public:
    wdgButton(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None,
              bool aState=false, axString aOffText="Off", axString aOnText="On", int aTextAlign=ta_Center)
    : axWidget(aListener,/*aId,*/aRect,aAlignment)
      {
        mState = aState;
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;

      }

    //----------

    inline void setState(bool aState) { mState=aState; }
    inline bool getState(void) { return mState; }

    inline void setText(axString aOffText, axString aOnText, int aTextAlign=ta_Center)
      {
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawButton(aCanvas,mRect,mState?mOnText:mOffText,mTextAlign,mState);
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mState)
        {
          mState = false;
        }
        else
        {
          mState = true;
        }
        mListener->onChange(this);
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        mListener->onCursor(cu_Hand);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

    //----------

};

//----------------------------------------------------------------------
#endif

