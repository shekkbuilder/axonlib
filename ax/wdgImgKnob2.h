#ifndef wdgImgKnob2_included
#define wdgImgKnob2_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgLabel.h"
#include "wdgImgKnob.h"

class wdgImgKnob2 : public axContainer
{
  //protected:
  public:
    wdgLabel*   name;
    wdgImgKnob* knob;
    wdgLabel*   display;
    char        buf[256];

  public:

    wdgImgKnob2(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment,
               int aNumImages=0, axSurface* aSurface=NULL)
    : axContainer(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        appendWidget( name    = new wdgLabel(   this,-1,axRect(aRect.x,aRect.y,    32,16),wal_None, "label", AX_GREY_LIGHT, tal_Center|tal_Top )  );
        appendWidget( knob    = new wdgImgKnob( this,-2,axRect(aRect.x,aRect.y+16, 32,32),wal_None, aNumImages, aSurface )  );
        appendWidget( display = new wdgLabel(   this,-3,axRect(aRect.x,aRect.y+48, 32,16),wal_None, "-", AX_GREY_LIGHT, tal_Center|tal_Bottom )  );
      }
    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget->mID == -2)
        {
          knob->mParameter->doGetDisplay(buf);
          display->setText( buf );
          mListener->onChange(knob);
          mListener->onChange(display);
        }
      }


};

//----------------------------------------------------------------------
#endif
