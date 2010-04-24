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

  public:

    axDemo_page_widgets(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        //setupWidgets(aListener);
      }

    virtual ~axDemo_page_widgets()
      {
      }

    void setupWidgets(axWidgetListener* aListener)
    {
      axWidget* widget;
      wdgPanel* panel;
      wdgScrollBox* scroll;
      //appendWidget( panel = new wdgPanel(aListener,axRect(150,0),wa_Left) );

      appendWidget( scroll = new wdgScrollBox(aListener,axRect(150,0),wa_Left) );
        widget = scroll->getContainer();//->setBorders(10,10,5,5);
        widget->setFlag(wf_Clip);
        widget->setBorders(10,10,5,5);
        widget->appendWidget(      new wdgLabel(     aListener,axRect(128, 20), wa_Top, "label", ta_Left ) );
        widget->appendWidget(      new wdgButton(    aListener,axRect(128, 20), wa_Top, false, "spring", "spring", ta_Center, bm_Spring ) );
        widget->appendWidget( w1 = new wdgButton(    aListener,axRect(128, 20), wa_Top, false, "switch", "switch", ta_Center, bm_Switch ) );
        widget->appendWidget( w2 = new wdgValue(     aListener,axRect(128, 20), wa_Top, "value", 0 ) );
        widget->appendWidget( w3 = new wdgSlider(    aListener,axRect(128, 20), wa_Top, "slider", 0 ) );
        widget->appendWidget( w4 = new wdgScrollBar( aListener,axRect(128, 20), wa_Top  ) );
        widget->appendWidget( w5 = new wdgKnob(      aListener,axRect(128, 32), wa_Top, "knob", 0.3 ) );
        widget->appendWidget(      new wdgKnob(      aListener,axRect(128, 16), wa_Top, "knob", 0.2 ) );
        widget->appendWidget(      new wdgOctave(    aListener,axRect(128, 40), wa_Top  ) );
        widget->appendWidget(      new wdgGrid(      aListener,axRect(128,128), wa_Top) );

      appendWidget( w_Sizer = new wdgSizer(aListener,axRect(0,0,5,0),wa_Left) );
        w_Sizer->setTarget(scroll);

      appendWidget( panel = new wdgPanel(aListener,axRect(100,0),wa_Client) );
        panel->setBorders(10,10,5,5);
        panel->setFlag(wf_Clip);
        panel->appendWidget(      new wdgSlider(    aListener,axRect( 16,128), wa_Right, "v.slider", 0, true ) );
        panel->appendWidget(      new wdgScrollBar( aListener,axRect( 16,128), wa_Right, 0, true ) );
    }

    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    wdgPanel::doPaint(aCanvas,aRect);
    //  }
    //virtual void onChange(axWidget* aWidget)
    //  {
    //    mListener->onChange(aWidget);
    //  }
};

//----------------------------------------------------------------------
#endif
