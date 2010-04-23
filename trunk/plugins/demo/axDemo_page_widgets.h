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

//----------------------------------------------------------------------

class axDemo_page_widgets : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    wdgButton*    w1;
    wdgValue*     w2;
    wdgSlider*    w3;
    wdgScrollBar* w4;
    wdgKnob*      w5;

  public:
    axDemo_page_widgets(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        //appendWidget( new wdgLabel( aListener,axRect(10,10,120,20),wa_TopLeft,      "label",              ta_Left) );
        //appendWidget( new wdgButton(aListener,axRect(10,10,120,20),wa_TopLeft,false,"switch","[ switch ]",ta_Center,bm_Switch) );
        //appendWidget( new wdgButton(aListener,axRect(10,10,120,20),wa_TopLeft,false,"spring","[ spring ]",ta_Center,bm_Spring) );
      }
    virtual ~axDemo_page_widgets()
      {
      }
    void setupWidgets(axWidgetListener* aListener)
    {
      //axWidget *panel;
      wdgPanel* panel;
      //appendWidget( panel = new axWidget(aListener,axRect(200,0),wa_Left) );
      appendWidget( panel = new wdgPanel(aListener,axRect(200,0),wa_Left) );
      panel->setBorders(10,10,5,5);
        panel->appendWidget(      new wdgLabel(     aListener,axRect(150,20), wa_TopLeft, "label", ta_Left ) );
        panel->appendWidget(      new wdgButton(    aListener,axRect(150,20), wa_TopLeft, false, "spring", "[ spring ]", ta_Center, bm_Spring ) );
        panel->appendWidget( w1 = new wdgButton(    aListener,axRect(150,20), wa_TopLeft, false, "switch", "[ switch ]", ta_Center, bm_Switch ) );
        panel->appendWidget( w2 = new wdgValue(     aListener,axRect(150,20), wa_TopLeft, "value", 0 ) );
        panel->appendWidget( w3 = new wdgSlider(    aListener,axRect(150,20), wa_TopLeft, "slider", 0 ) );
        panel->appendWidget( w4 = new wdgScrollBar( aListener,axRect(150,20), wa_TopLeft  ) );
        panel->appendWidget( w5 = new wdgKnob(      aListener,axRect(150,32), wa_TopLeft, "knob", 0.3 ) );
        panel->appendWidget(      new wdgKnob(      aListener,axRect(150,16), wa_TopLeft, "knob", 0.2 ) );

        panel->appendWidget(      new wdgOctave(    aListener,axRect(150,50), wa_TopLeft  ) );

      //appendWidget( panel = new axWidget(aListener,axRect(200,0),wa_Left) );
      appendWidget( panel = new wdgPanel(aListener,axRect(200,0),wa_Left) );
      panel->setBorders(5,5,2,2);
        panel->appendWidget(      new wdgSlider(    aListener,axRect(20,150), wa_LeftTop, "v.slider", 0, true ) );
        panel->appendWidget(      new wdgScrollBar( aListener,axRect(20,150), wa_LeftTop, 0, true ) );



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
