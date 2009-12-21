#ifndef axSynth1_gui_included
#define axSynth1_gui_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgMultiPage.h"

class myTab1 : public axContainer
{
  public:
    myTab1(axWidgetListener* aOwner, int aID, axRect aRect, int aAlignment=wal_Client)
    : axContainer(aOwner, aID, aRect, aAlignment)
      {
        //setBackground( true, AX_GREY_DARK );
      }
};

//----------------------------------------------------------------------
#endif
