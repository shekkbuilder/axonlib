/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
 * @file
 * \brief image based knob widget 2
 */

/**
 * \brief image based knob widget 2
 *
 * long desc
 *
 */

#ifndef wdgImgKnob2_included
#define wdgImgKnob2_included
//----------------------------------------------------------------------

#include "axContainer.h"
#include "wdgLabel.h"
#include "wdgImgKnob.h"

class wdgImgKnob2 : public axContainer
{
  private:
  //public:
    wdgLabel*   name;
    wdgImgKnob* knob;
    wdgLabel*   display;
    char        dispbuf[256];
    char        namebuf[256];

  public:

    wdgImgKnob2(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment,
               int aNumImages=0, axSurface* aSurface=NULL)
    : axContainer(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
//        appendWidget( name    = new wdgLabel(   this,-1,axRect(aRect.x,aRect.y,    32,16),wal_None, "label", AX_GREY_LIGHT, tal_Center|tal_Top )  );
//        appendWidget( knob    = new wdgImgKnob( this,-2,axRect(aRect.x,aRect.y+16, 32,32),wal_None, aNumImages, aSurface )  );
//        appendWidget( display = new wdgLabel(   this,-3,axRect(aRect.x,aRect.y+48, 32,16),wal_None, "-", AX_GREY_LIGHT, tal_Center|tal_Bottom )  );
//        //doRealign();
        appendWidget( name    = new wdgLabel(   this,-100,axRect(0,0,  32,16),wal_None, "label", AX_GREY_LIGHT, tal_Center|tal_Top )  );
        appendWidget( knob    = new wdgImgKnob( this,-200,axRect(0,16, 32,32),wal_None, aNumImages, aSurface )  );
        appendWidget( display = new wdgLabel(   this,-300,axRect(0,48, 32,16),wal_None, "000", AX_GREY_LIGHT, tal_Center|tal_Bottom )  );
        //doRealign();
        //clearFlag(wfl_Capture);
      }

    //inline axWidget* getKnob(void) { return knob; }

    virtual axWidget* connect(axParameter* aParameter)
      {
        //trace("wdgImgKnob2.connect");
        knob->connect(aParameter);
        knob->mID = mID;
        knob->mUser = mUser;
        aParameter->doGetName(namebuf);
        aParameter->doGetDisplay(dispbuf);
        name->setText(namebuf);
        display->setText(dispbuf);
        return knob;
      }

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==knob)
        {
          //trace("tweaking " << aWidget->mID);
          //trace("  value: " << aWidget->doGetValue() );
          dispbuf[0] = 0;
          axParameter* P = knob->getParameter();
          if (P) P->doGetDisplay(dispbuf);
          display->setText( dispbuf );
          //mListener->onRedraw(display);
          //mListener->onChange(this);
          mListener->onChange(display);
        }
        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif
