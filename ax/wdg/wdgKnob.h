#ifndef wdgKnob_included
#define wdgKnob_included
//----------------------------------------------------------------------

#include "wdg/wdgValue.h"

class wdgKnob : public wdgValue
{
  private:
    bool  mIsDragging;
    int   mClickX;
    int   mClickY;
    float mClickVal;
    char  mDisp[16];
  protected:
    //axString  mName;
    float     mSens1;
    float     mSens2;

  public:
    wdgKnob(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
            axString aName="", float aValue=0)
    : wdgValue(aListener,/*aId, */aRect,aAlignment,aName,aValue)
      {
        mName = aName;
        mValue = aValue;
        mIsDragging = false;
        mSens1 = 0.005;
        //if (hasFlag(wf_Vertical)) { if (aRect.w>0) mSens1 = 1.0f/(float)aRect.h; }
        //else                      { if (aRect.w>0) mSens1 = 1.0f/(float)aRect.w; }
        mSens2 = 0.05;
        //if (aRect.h<aRect.w) setFlag(wf_Vertical);
      }

    //----------

    //inline void setName(axString aName) { mName=aName; }
    inline void setSensitivity(float aSens1, float aSens2) { mSens1=aSens1; mSens2=aSens2; }

    //----------

    //virtual void doSetSize(int aWidth, int aHeight)
    //  {
    //    if (hasFlag(wf_Vertical)) { if (aHeight>0) mSens1 = 1.0f/(float)aHeight; }
    //    else                      { if (aWidth >0) mSens1 = 1.0f/(float)aWidth; }
    //    axWidget::doSetSize(aWidth,aHeight);
    //  }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          mIsDragging = true;
          mClickX = aXpos;
          mClickY = aYpos;
          mClickVal = mValue;
        }
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left) mIsDragging = false;
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mIsDragging)
        {
          //int dx = aXpos - mClickX;
          int dy = aYpos - mClickY;
          float v;
          /*if (hasFlag(wf_Vertical))*/ v = -dy;
          //else v = dx;
          float s = mSens1;
          if (aButton&bu_Ctrl) s*=mSens2;
          v *= s;
          mValue = mClickVal + v;
          mValue = axMin(1,mValue);
          mValue = axMax(0,mValue);
          mListener->onChange(this);
        }
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          //sprintf(mDisp,"%.3f",mValue);
          //if (mParameter) __builtin_sprintf(mDisp,"%.2f",mParameter->getValue());
          //else __builtin_sprintf(mDisp,"%.2f",mValue);
          if (mParameter) mParameter->doGetDisplay(mDisp);//axFtoa(mDisp,mParameter->getValue());
          else axFtoa(mDisp,mValue);
          mSkin->drawKnob(aCanvas,mRect,mName,mDisp,mValue);
        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        //if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
        //else mListener->onCursor(cu_ArrowLeftRight);
        mListener->onCursor(cu_ArrowUpDown);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

};

//----------------------------------------------------------------------
#endif

