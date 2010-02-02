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
  \file wdgKnob.h
  \brief knob widget
*/

#ifndef wdgKnob_included
#define wdgKnob_included
//----------------------------------------------------------------------

#include "wdgValue.h"

#define NAME_SIZE 32
#define DISP_SIZE 32

class wdgKnob : public wdgValue
{
  private:
    float     val;
    char      name[NAME_SIZE];
    char      display[DISP_SIZE];
    int       mKnobSize;
    axBrush*  brush_name_back;
    axBrush*  brush_disp_back;
    axBrush*  brush_knob_back_outer;
    axBrush*  brush_knob_back_inner;
    axBrush*  brush_knob_pie;
    axFont*   font_name;
    axFont*   font_disp;
    axPen*    pen_knob_arc_full;
    axPen*    pen_knob_arc_outer;
    // small knob
    axPen*    sm_pen_knob_arc_full;
    axPen*    sm_pen_knob_arc_outer;
    axBrush*  sm_brush_knob_pie;
    axFont*   sm_font_disp;

  public:

    wdgKnob(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None, int aSize=1)
    : wdgValue(aListener,aID,aRect,aAlignment)
      {
        mKnobSize             = aSize;
        brush_name_back       = new axBrush(AX_GREY_DARK);
        brush_disp_back       = new axBrush(axColor(96,96,96));
        brush_knob_back_outer = new axBrush(AX_GREY);
        brush_knob_back_inner = new axBrush(axColor(112,112,112));
        brush_knob_pie        = new axBrush(AX_GREY_LIGHT);
        font_name             = new axFont(AX_GREY);
        font_disp             = new axFont(AX_GREY_LIGHT);
        pen_knob_arc_full     = new axPen(AX_GREY_DARK,4);
        pen_knob_arc_outer    = new axPen(AX_YELLOW,4);
        sm_pen_knob_arc_full  = new axPen(AX_GREY_DARK,3);
        sm_pen_knob_arc_outer = new axPen(AX_WHITE,3);
        sm_brush_knob_pie     = new axBrush(AX_GREY_LIGHT);
        sm_font_disp          = new axFont(AX_GREY_DARK);
      }

    virtual ~wdgKnob()
      {
        delete brush_name_back;
        delete brush_disp_back;
        delete brush_knob_back_outer;
        delete brush_knob_back_inner;
        delete brush_knob_pie;
        delete font_name;
        delete font_disp;
        delete pen_knob_arc_full;
        delete pen_knob_arc_outer;
        delete sm_pen_knob_arc_full;
        delete sm_pen_knob_arc_outer;
        delete sm_brush_knob_pie;
        delete sm_font_disp;
      }

    //--------------------------------------------------

    void drawKnob(axCanvas* aCanvas, axRect aRect, float aValue, axString aName, axString aDisplay, int aFlags)
      {
        int x  = mRect.x;
        int y  = mRect.y;
        int x2 = mRect.x2();
        //int y2 = mRect.y2();
        // text rectangles
        aCanvas->clearPen();
        aCanvas->selectBrush(brush_name_back);
        aCanvas->fillRect(x+16,y,x2,y+14);
        aCanvas->selectBrush(brush_disp_back);
        aCanvas->fillRect(x+16,y+17,x2,y+31);
        aCanvas->selectFont(font_name);
        aCanvas->drawText(x+40,y,x2,y+14,aName,tal_Left);
        aCanvas->selectFont(font_disp);
        aCanvas->drawText(x+40,y+17,x2,y+31,aDisplay,tal_Left);
        // erase circle behind knob
        aCanvas->selectBrush(brush_knob_back_outer);
        aCanvas->fillCircle(x-4,y-4,x+35,y+35);
        aCanvas->selectBrush(brush_knob_back_inner);
        aCanvas->fillCircle(x,y,x+31,y+31);
        // draw full outer arc
        aCanvas->selectPen(pen_knob_arc_full);
        aCanvas->drawArc(x+2,y+2,x+29,y+29,0.6,0.8);
        // outer arc
        aCanvas->selectPen(pen_knob_arc_outer);
        aCanvas->drawArc(x+2,y+2,x+29,y+29,0.6,aValue*0.8);
        // draw pie (value)
        aCanvas->clearPen();
        aCanvas->selectBrush(brush_knob_pie);
        aCanvas->fillArc(x+6,y+6,x+25,y+25,0.6,aValue*0.8);
      }

    void drawKnobSmall(axCanvas* aCanvas, axRect aRect, float aValue, axString aName, axString aDisplay, int aFlags)
      {
        int x  = mRect.x;
        int y  = mRect.y;
        aCanvas->selectPen(sm_pen_knob_arc_full);
        aCanvas->drawArc(x+1,y+1,x+15,y+15,0.6,0.8);
        aCanvas->selectPen(sm_pen_knob_arc_outer);
        aCanvas->drawArc(x+1,y+1,x+15,y+15,0.6,aValue*0.8);
        aCanvas->clearPen();
        aCanvas->selectBrush(sm_brush_knob_pie);
        aCanvas->fillArc(x+4,y+4,x+12,y+12,0.6,aValue*0.8);
        aCanvas->selectFont(sm_font_disp);
        aCanvas->drawText(x+22,y,x+63,y+15,aDisplay,tal_Left);
      }

    //--------------------------------------------------
    // widget base
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( mParameter )
        {
          name[0] = '\0';
          display[0] = '\0';
          val = mParameter->doGetValue();
          mParameter->doGetName(name);
          mParameter->doGetDisplay(display);
        }
        else
        {
          val = mValue;
          strcpy(name,"[name]");
          sprintf(display,"%f",val);
        }
        switch(mKnobSize)
        {
          case 0: drawKnobSmall(aCanvas,aRect,val,name,display,0); break;
          case 1: drawKnob(aCanvas,aRect,val,name,display,0); break;
        }
      }
};

#undef NAME_SIZE
#undef DISP_SIZE

//----------------------------------------------------------------------
#endif
