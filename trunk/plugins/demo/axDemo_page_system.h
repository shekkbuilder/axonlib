#ifndef axDemo_page_system_included
#define axDemo_page_system_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"
#include "core/axUtils.h"

//----------------------------------------------------------------------

class axDemo_page_system : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:
    axSystemInfo* mSystemInfo;

  public:

    axDemo_page_system(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        mSystemInfo = NULL;
      }

    virtual ~axDemo_page_system()
      {
      }

    void setup(axSystemInfo* aSystemInfo)
      {
        mSystemInfo = aSystemInfo;
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPanel::doPaint(aCanvas,aRect);
        if (mSystemInfo)
        {
          axColor text = aCanvas->getColor(AX_WHITE);
          int x = mRect.x;
          int y = mRect.y;
          aCanvas->setTextColor(text);
          aCanvas->drawText(x+10,y+10,"cpuid"); aCanvas->drawText(x+80,y+10,mSystemInfo->cpuidstr);
          aCanvas->drawText(x+10,y+25,"os");    aCanvas->drawText(x+80,y+25,mSystemInfo->os);
        }
      }

};

//----------------------------------------------------------------------
#endif
