#ifndef axDemo_page_canvas_included
#define axDemo_page_canvas_included
//----------------------------------------------------------------------

#include "wdg/wdgPanel.h"
#include "axDemo_skin.h"

#include "core/axRand.h"

//----------------------------------------------------------------------

class axDemo_page_canvas : public wdgPanel
{
  // let us access widgets, etc, directly
  friend class axDemo;

  private:

  public:

    axDemo_page_canvas(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : wdgPanel(aListener,aRect,aAlignment)
      {
        //appendWidget( w_Panel = new wdgPanel(this,NULL_RECT,wa_Client));
      }

    virtual ~axDemo_page_canvas()
      {
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPanel::doPaint(aCanvas,aRect);
        axColor grey    = aCanvas->getColor(128,128,128);
        axColor black   = aCanvas->getColor(255,255,255);
        axColor white   = aCanvas->getColor(255,255,255);
        axColor cyan    = aCanvas->getColor(  0,255,255);
        axColor yellow  = aCanvas->getColor(255,255,  0);
        axColor green   = aCanvas->getColor(  0,255,  0);

        int x = mRect.x+16;
        int y = mRect.y+16;

        // pixels

        for (int i=0; i<100; i++)
        {
          aCanvas->setPenColor( aCanvas->getColor(axRandInt(255),
                                                  axRandInt(255),
                                                  axRandInt(255)) );
          
          aCanvas->drawPoint( x + axRandInt(15), y + axRandInt(15) );
        }
        
        aCanvas->setPenColor(white);
        aCanvas->drawLine( x+16,y,x+31,y+15);
        aCanvas->setPenColor(green);
        aCanvas->drawLine( x+31,y,x+15,y+15);

      }

};

//----------------------------------------------------------------------
#endif
