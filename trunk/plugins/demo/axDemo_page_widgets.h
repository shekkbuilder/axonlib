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

//----------------------------------------------------------------------

class axDemo_page_widgets : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    wdgButton*    w1;
    wdgValue*     w2;
    wdgSlider*    w3;
    wdgKnob*      w4;
    wdgScrollBar* w5;

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
      appendWidget(       new wdgLabel(     aListener,axRect(10,10,150,20),wa_TopLeft, "label",ta_Left ) );
      appendWidget( w1 =  new wdgButton(    aListener,axRect(10,10,150,20),wa_TopLeft, false,"switch","[ switch ]",ta_Center,bm_Switch ) );
      appendWidget(       new wdgButton(    aListener,axRect(10,10,150,20),wa_TopLeft, false,"spring","[ spring ]",ta_Center,bm_Spring ) );
      appendWidget( w2  = new wdgValue(     aListener,axRect(10,10,150,20),wa_TopLeft, "value", 0 ) );
      appendWidget( w3  = new wdgSlider(    aListener,axRect(10,10,150,20),wa_TopLeft, "slider",0 ) );
      appendWidget( w4 =  new wdgKnob(      aListener,axRect(10,10,158,32),wa_TopLeft, "knob",  0.3 ) );
      appendWidget( w5 =  new wdgScrollBar( aListener,axRect(10,10,150,20),wa_TopLeft  ) );
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
