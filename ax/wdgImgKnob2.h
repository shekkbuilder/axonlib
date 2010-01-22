/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
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
