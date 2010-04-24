#ifndef axDemo_page_system_included
#define axDemo_page_system_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

//----------------------------------------------------------------------

class axDemo_page_system : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:

  public:

    axDemo_page_system(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
      }

    virtual ~axDemo_page_system()
      {
      }

};

//----------------------------------------------------------------------
#endif
