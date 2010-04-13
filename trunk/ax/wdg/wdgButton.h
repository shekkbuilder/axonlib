#ifndef wdgButton_included
#define wdgButton_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

#define bm_Switch 0
#define bm_Spring 1

class wdgButton : public axWidget
{
  private:
    bool mButtonDown;
  protected:
    bool      mState;
    axString  mOffText, mOnText;
    int       mTextAlign;
    int       mMode;

  public:
    wdgButton(axWidgetListener* aListener, /*int aId, */axRect aRect, int aAlignment=wa_None,
              bool aState=false, axString aOffText="Off", axString aOnText="On", int aTextAlign=ta_Center, int aMode=bm_Switch)
    : axWidget(aListener,/*aId,*/aRect,aAlignment)
      {
        mButtonDown = false;
        mMode = aMode;//bm_Switch;
        mState = aState;
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;
      }

    //----------

    inline void setState(bool aState) { mState=aState; }
    inline bool getState(void) { return mState; }

    //----------

    inline void setText(axString aOffText, axString aOnText, int aTextAlign=ta_Center)
      {
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;
      }

    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawButton(aCanvas,mRect,mState?mOnText:mOffText,mTextAlign,mState);
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        switch (mMode)
        {
          case bm_Switch:
            if (mState)
            {
              mState = false;
              mValue = 0;
            }
            else
            {
              mState = true;
              //mValue = 1;
            }
            mListener->onChange(this);
            break;
          case bm_Spring:
            mButtonDown = true;
            mState = true;
            //mValue = 1;
            mListener->onRedraw(this);
            break;
        } //switch
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        switch (mMode)
        {
          case bm_Switch:
            break;
          case bm_Spring:
            mButtonDown = false;
            mState = false;
            //mValue = 0;
            if (mRect.contains(aXpos,aYpos)) mListener->onChange(this);
            else mListener->onRedraw(this);
            break;
        }
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        switch (mMode)
        {
          case bm_Switch:
            break;
          case bm_Spring:
            if (mButtonDown)
            {
              if (mRect.contains(aXpos,aYpos))
              {
                if (!mState)
                {
                  mState = true;
                  //mValue = 1;
                  mListener->onRedraw(this);
                }
              }
              else
              {
                mState = false;
                //mValue = 0;
                mListener->onRedraw(this);
              }
            }

            break;
        } //switch

      }

    //--------------------------------------------------

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

