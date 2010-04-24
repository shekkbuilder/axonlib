#ifndef axDemo_page_sync_included
#define axDemo_page_sync_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

//----------------------------------------------------------------------

class axDemo_page_sync : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:

  public:

    axDemo_page_sync(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
      }

    virtual ~axDemo_page_sync()
      {
      }

};

//----------------------------------------------------------------------
#endif
