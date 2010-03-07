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
  \file wdgTextBox.h
  \brief textbox widget
*/

#ifndef wdgTextBox_included
#define wdgTextBox_included
//----------------------------------------------------------------------

/*
  NOTE: you have to manage the strings you append to this yourself.
        only a pointer to the string is kept
        allocate them yourself,
        and remember to delete them when you're finished
*/

#include "wdgScrollBox.h"
#include "wdgLabel.h"

class wdgTextBox : public wdgScrollBox
{
  private:
  public:
    wdgTextBox(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : wdgScrollBox(aListener, aID, aRect, aAlignment)
      {
        wContainer->setAlign(5,5);
      }
    virtual ~wdgTextBox()
      {
      }
    virtual void appendText(axString aText)
      {
        appendWidget( new wdgLabel(this,-1,axRect(0,0,0,16),wal_Top,aText,AX_BLACK,tal_Left) );
        doRealign();
        mListener->onChange(this);
      }
    virtual void clearText(void)
      {
        wContainer->deleteWidgets();
        doRealign();
        mListener->onChange(this);
      }

};

//----------------------------------------------------------------------
#endif