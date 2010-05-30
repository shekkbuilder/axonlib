#ifndef wdgMenuItem_included
#define wdgMenuItem_included
//----------------------------------------------------------------------

#include "wdg/wdgButton.h"

//class wdgMenuItem : public wdgButton
class wdgMenuItem : public wdgLabel
{
  private:
    bool isHovering;
    //int  mx,my;
  public:
    //popupButton(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axString aText="text", int aTextAlign=ta_Center)
    wdgMenuItem(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axString aText="text", int aTextAlign=ta_Center)
    : wdgLabel(aListener,aRect,aAlignment,aText,aTextAlign)
      {
        setFlag(wf_Active);
        isHovering = false;
      }
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axColor light = aCanvas->getColor(192,192,192);
        axColor dark  = aCanvas->getColor(64,64,64);
        axColor grey  = aCanvas->getColor(128,128,128);
        if (isHovering)
        {
          aCanvas->setBrushColor(dark);
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          aCanvas->setTextColor(light);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
        }
        else
        {
          //aCanvas->setBrushColor(grey);
          //aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          aCanvas->setTextColor(dark);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
        }
      }
    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          //mValue = 1;
          mListener->onChange(this);
        } //bu_Left
      }
    virtual void doEnter(axWidget* aCapture)
      {
        //wdgLabel::doEnter(aCapture);
        isHovering = true;
        mListener->onRedraw(this);
        //mListener->onCursor(cu_Hand);
      }
    virtual void doLeave(axWidget* aCapture)
      {
        //mListener->onCursor(DEF_CURSOR);
        //wdgLabel::doLeave(aCapture);
        isHovering = false;
        mListener->onRedraw(this);
      }
};

//----------------------------------------------------------------------
#endif
