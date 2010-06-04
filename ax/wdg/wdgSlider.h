/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef wdgSlider_included
#define wdgSlider_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"
#include "core/axDefines.h"

class wdgSlider : public axWidget
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
    wdgSlider(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
              axString aName="", float aValue=0, bool aVertical=false)
    : axWidget(aListener,aRect,aAlignment)
      {
        mName = aName;
        mValue = aValue;
        mIsDragging = false;
        mSens1 = 0.005;
        //if (hasFlag(wf_Vertical)) { if (aRect.h>0) mSens1 = 1.0f/(float)aRect.h; }
        //else                      { if (aRect.w>0) mSens1 = 1.0f/(float)aRect.w; }
        if (aVertical)
        {
          setFlag(wf_Vertical);
          if (aRect.w>0) mSens1 = 1.0f/(float)aRect.h;
        }
        else
        {
          clearFlag(wf_Vertical);
          if (aRect.w>0) mSens1 = 1.0f/(float)aRect.w;
        }
        mSens2 = 0.05;
        //if (aRect.h<aRect.w) setFlag(wf_Vertical);
      }

    //----------

    //inline void setName(axString aName) { mName=aName; }
    inline void setSensitivity(float aSens1, float aSens2=0.05)
      {
        mSens1=aSens1;
        mSens2=aSens2;
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        axWidget::doSetSize(aWidth,aHeight);
        if (hasFlag(wf_Vertical)) { if (aHeight>0) mSens1 = 1.0f/(float)aHeight; }
        else                      { if (aWidth >0) mSens1 = 1.0f/(float)aWidth; }
      }

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
          int dx = aXpos - mClickX;
          int dy = aYpos - mClickY;
          float v;
          //if (hasFlag(wf_Vertical)) v = dy;
          if (hasFlag(wf_Vertical)) v = -dy;
          else v = dx;
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
          //__builtin_sprintf(mDisp,"%.3f",mValue);
          axFtoa(mDisp,mValue);
          mSkin->drawSlider(aCanvas,mRect,mValue,mName,mDisp,hasFlag(wf_Vertical)/*,mSkinMode*/);
        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
        else mListener->onCursor(cu_ArrowLeftRight);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

};

//----------------------------------------------------------------------
#endif

