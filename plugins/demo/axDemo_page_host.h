#ifndef axDemo_page_host_included
#define axDemo_page_host_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

//----------------------------------------------------------------------

class axDemo_page_host : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:

  public:

    axDemo_page_host(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
      }

    virtual ~axDemo_page_host()
      {
      }

};

//----------------------------------------------------------------------
#endif
