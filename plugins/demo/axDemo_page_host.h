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
    axHostInfo* mHostInfo;

  public:

    axDemo_page_host(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        mHostInfo = NULL;
      }

    virtual ~axDemo_page_host()
      {
      }

    void setup(axHostInfo* aHostInfo)
      {
        mHostInfo = aHostInfo;
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPanel::doPaint(aCanvas,aRect);
        if (mHostInfo)
        {
          axColor text = aCanvas->getColor(AX_WHITE);
          int x = mRect.x;
          int y = mRect.y;
          aCanvas->setTextColor(text);
          aCanvas->drawText(x+10,y+10,"host");    aCanvas->drawText(x+80,y+10,mHostInfo->name);
          aCanvas->drawText(x+10,y+25,"format");  aCanvas->drawText(x+80,y+25,mHostInfo->format);
        }
      }


};

//----------------------------------------------------------------------
#endif
