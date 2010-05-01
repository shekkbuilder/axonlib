#ifndef axDemo_page_widgets_included
#define axDemo_page_widgets_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "wdg/wdgLabel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgValue.h"
#include "wdg/wdgSlider.h"
#include "wdg/wdgScrollBar.h"
#include "wdg/wdgKnob.h"
#include "wdg/wdgOctave.h"
#include "wdg/wdgGrid.h"

//----------------------------------------------------------------------

class popupButton : public wdgButton
{
  private:
    bool isHovering;
    //int  mx,my;
  public:
    popupButton(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axString aText="text", int aTextAlign=ta_Center)
    : wdgButton(aListener,aRect,aAlignment,false,aText,aText,aTextAlign,bm_Spring)
      {
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
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mOnText,mTextAlign);
        }
        else
        {
          //aCanvas->setBrushColor(grey);
          //aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
          aCanvas->setTextColor(dark);
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mOnText,mTextAlign);
        }
      }
    virtual void doEnter(axWidget* aCapture)
      {
        wdgButton::doEnter(aCapture);
        isHovering = true;
        mListener->onRedraw(this);
        //mListener->onCursor(cu_Hand);
      }
    virtual void doLeave(axWidget* aCapture)
      {
        //mListener->onCursor(DEF_CURSOR);
        wdgButton::doLeave(aCapture);
        isHovering = false;
        mListener->onRedraw(this);
      }
};

//----------------------------------------------------------------------

class axDemo_page_widgets : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    wdgSizer*     w_Sizer;
    wdgButton*    w1;
    wdgValue*     w2;
    wdgSlider*    w3;
    wdgScrollBar* w4;
    wdgKnob*      w5;
    wdgButton* bu;

    int mx,my;
    axWidget* modal;
    popupButton *mb1,*mb2,*mb3,*mb4,*mb5;

  public:

    axDemo_page_widgets(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        modal = NULL;
        setBorders(5,5,2,2);
        //setupWidgets(aListener);
        //wdgButton* bu;
        axWidget* widget;
        wdgPanel* panel;
        wdgScrollBox* scroll;

        appendWidget( scroll = new wdgScrollBox(aListener,axRect(0,200),wa_Top) );
          widget = scroll->getContainer();//->setBorders(10,10,5,5);
          //widget->setFlag(wf_Clip);
          scroll->setFlag(wf_Clip);
          widget->setBorders(10,10,5,5);
          widget->appendWidget(      new wdgSlider(    aListener,axRect( 16,200), wa_Left,"v.slider", 0.5, true ) );
          widget->appendWidget(      new wdgLabel(     aListener,axRect(128, 20), wa_Top, "label", ta_Left ) );
          widget->appendWidget( bu = new wdgButton(    this/*aListener*/,axRect(128, 20), wa_Top, false, "spring", "spring", ta_Center, bm_Spring ) );
          widget->appendWidget( w1 = new wdgButton(    aListener,axRect(128, 20), wa_Top, false, "switch", "switch", ta_Center, bm_Switch ) );
          widget->appendWidget( w2 = new wdgValue(     aListener,axRect( 80, 20), wa_TopLeft, "value", 0 ) );
          widget->appendWidget( w3 = new wdgSlider(    aListener,axRect(128, 20), wa_Top, "slider", 0 ) );
          widget->appendWidget( w4 = new wdgScrollBar( aListener,axRect(128, 20), wa_Top  ) );
          widget->appendWidget( w5 = new wdgKnob(      aListener,axRect(128, 32), wa_Top, "knob", 0.3 ) );
          widget->appendWidget(      new wdgKnob(      aListener,axRect(128, 16), wa_Top, "knob", 0.2 ) );
          widget->appendWidget(      new wdgOctave(    aListener,axRect(128, 40), wa_Top  ) );
          widget->appendWidget(      new wdgGrid(      aListener,axRect(128,128), wa_Top) );

        appendWidget( w_Sizer = new wdgSizer(aListener,axRect(0,5),wa_Top,sd_Vertical) );
          w_Sizer->setTarget(scroll);

        appendWidget( panel = new wdgPanel(aListener,axRect(0,100),wa_Client) );

      }

    virtual ~axDemo_page_widgets()
      {
      }

    virtual void onChange(axWidget* aWidget)
      {
        mListener->onChange(aWidget);
        if (aWidget==bu)
        {
          int x = bu->getRect().x + 10;
          int y = bu->getRect().y + 10;
          //TODO: wdgButtons
          modal = new wdgPanel(this,axRect(x,y,80,110),wa_None);
          modal->setBorders(5,5,0,0);
          modal->appendWidget( mb1 = new popupButton(this,axRect(0,20),wa_Top) );
          modal->appendWidget( mb2 = new popupButton(this,axRect(0,20),wa_Top) );
          modal->appendWidget( mb3 = new popupButton(this,axRect(0,20),wa_Top) );
          modal->appendWidget( mb4 = new popupButton(this,axRect(0,20),wa_Top) );
          modal->appendWidget( mb5 = new popupButton(this,axRect(0,20),wa_Top) );
          modal->doRealign();
          mListener->onCursor(DEF_CURSOR);
          mListener->onModal(true,modal);
        }
        if (aWidget==mb1 || aWidget==mb2 || aWidget==mb3 || aWidget==mb4 || aWidget==mb5)
        {
          mListener->onModal(false,NULL);
        }
      }

};

//----------------------------------------------------------------------
#endif
