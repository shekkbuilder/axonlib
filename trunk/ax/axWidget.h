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
 * \brief main widget classes
 */

/**
 * \file axWidget.h
 * \brief main widget classes
 *
 * base class for gui elements
 * mainly contains a rect (pos/size), and some flags and data for reacting
 * to events. all messages up/down the hierarchy is done with the handler
 * and listener methods. a widget doesn't know anything about its owner /
 * container, and can only react to commands with the data/info passed as
 * parameters.
 *
 */


#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------

//TODO: mSurface in here, from axWindow
//  axWindow: double buffering
//  axWidget: bitmap for gui
//  (axContainer?)

#include "axString.h"
#include "axRect.h"
#include "axMath.h"
#include "axCanvas.h"

// widget flags

/** \defgroup widget_flags widget flags */
/* @{ */
#define wfl_Active    1       ///< is widget active and reacts to (mouse) events?
#define wfl_Visible   2       ///< is widget visible, and will be painteed?
#define wfl_Clear     4       ///< clear background if set
#define wfl_Clip      8       ///< set cliprect before painting
#define wfl_Capture   16      ///< captures mouse events?
#define wfl_Vertical  32      ///< vertical instead of horizontal (layout, drag direction, ..)
#define wfl_Align     64      ///< set if auto-align/size/pos
#define wfl_All       0xffff  ///< set if auto-align/size/pos
/* @} */

//widget alignment

/** \defgroup widget_alignment widget alignment */
/* @{ */
#define wal_None        0     ///< none, ignore realignment
#define wal_Client      1     ///< align to fill entire client area
#define wal_Left        2     ///< align left, set widget height to fill client area
#define wal_Top         3     ///< align top, set widget width to fill client area
#define wal_Right       4     ///< align right, set widget height to fill client area
#define wal_Bottom      5     ///< align top, set widget width to fill client area
#define wal_LeftTop     6     ///< align left top (no resize)
#define wal_RightTop    7     ///< align right top (no resize)
#define wal_LeftBottom  8     ///< align left bottom (no resize)
#define wal_RightBottom 9     ///< align right bottom (no resize)
#define wal_Stacked     10    ///< stacked
#define wal_Parent      11    ///< full parent size, regardless of current client size
/* @} */

//#define MAX_TEXT_LEN 16

#include "axParameter.h"

class axWidget;
#include "axArray.h"
typedef axArray<axWidget*> axWidgets;

//----------------------------------------------------------------------
//
// axWidgetBase
//
//----------------------------------------------------------------------

/**
  \class axWidgetBase
  \brief widget base

  the base class for all widgets (and containers). it contains of a set of (do-nothing) methods,
  that defines what a widget can (or is supposed to) do, and how to use these capabilities.
  all widgets inherit from this, and overload as many of the virtual 'do' methods as needed, to do their things.
*/

// Listener -> widget
class axWidgetBase
{
  public:
    /// reset to default
    /**
      reset thew widget to its default state/value
    */
    virtual void doReset(void) {}

    /// set internal state/value
    /**
      \param aValue new value (0..1)
    */
    virtual void doSetValue(float aValue) {} // 0..1

    /// get value/status
    /**
      \return current value (0..1)
    */
    virtual float doGetValue(void) { return 0; } // 0..1

    /// get widget name
    /**
      \return name string
    */
    virtual axString doGetName(void) { return STR_EMPTY; }

    /// move widget
    /**
      \param aX xpos
      \param aY ypos
    */
    virtual void doMove(int aX, int aY) {}

    /// scroll content of widget
    /**
      containers will also call doMove for all the sub-widgets
      \param dX number of pixels to scroll horizontally
      \param dY number of pixels to scroll vertically
    */
    virtual void doScroll(int dX, int dY) {}

    /// resizel widget
    /**
      \param aW new width
      \param aH new height
    */
    virtual void doResize(int aW, int aH) {}

    /// realign sub-widgets
    /**
      perform full realignment, including realigning sub-widgets
    */
    virtual void doRealign(void) {}

    /// paint widget to specified canvas. rect = update-rect
    /**
      paint widget to specified canvas.
      aRect specifies the area (in window/editor) that needs to be redrawn
      \param aCanvas canvas on which to draw
      \param aRect the update rectangle
    */
    virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}

    /// mouse has entered widget rect/area
    virtual void doEnter(void) {}

    /// mouse has leaved rect
    virtual void doLeave(void) {}

    /// a timer has fired
    virtual void doTimer(void) {}

    /// mouse button click
    /**
      \param aX mouse xpos
      \param aY mouse ypos
      \param aB mouse buttons
    */
    virtual void doMouseDown(int aX, int aY, int aB) {}

    /// mouse button up, x,y = pos, b = button/modifier keys
    /**
      \param aX mouse xpos
      \param aY mouse ypos
      \param aB mouse buttons
    */
    virtual void doMouseUp(int aX, int aY, int aB) {}

    /// mouse moved, x,y = pos, b = button/modifier keys
    /**
      \param aX mouse xpos
      \param aY mouse ypos
      \param aB mouse buttons
    */
    virtual void doMouseMove(int aX, int aY, int aB) {}

    /// key pressed.
    /**
      \param aK key code
      \param aS state (ctrl, shft, etc..)
    */
    virtual void doKeyDown(int aK, int aS) {}

    /// key released.
    /**
      \param aK key code
      \param aS state (ctrl, shft, etc..)
    */
    virtual void doKeyUp(int aK, int aS) {}

};

//----------------------------------------------------------------------
//
// axWidgetListener
//
//----------------------------------------------------------------------

/**
  \class axWidgetListener
  \brief widget listener

  a widget doesn't have any information about what container it belongs to, or its parent, or anything.
  the only way it can sends messages upwards in the hierarchy, is through this listener.

*/

class axWidgetListener
{
  public:

    /// widget value/state changed
    /**
      notifies the parent/listener that the internal state of the widget has changed.
      often this means that the widget has to be redrawn, to show  the new value/state
    */
    virtual void onChange(axWidget* aWidget) {}

    /// redraw widget
    virtual void onRedraw(axWidget* aWidget) {}

    /// redraw area
    virtual void onRedraw(axRect aRect) {}

    /// redraw all
    virtual void onRedrawAll(void) {}

    /// widget has moved
    virtual void onMove(axWidget* aWidget, int dX,int dY) {}  // delta x/y

    /// widget has resized
    virtual void onResize(axWidget* aWidget, int dX,int dY) {}  // delta x/y

    /// update hint/status text
    virtual void onHint(axString aHint) {}

    /// mouse hovering over widget
    virtual void onHover(axWidget* aWidget) {}

    /// change mouse cursor
    virtual void onCursor(int aCursor) {}
};

//----------------------------------------------------------------------
//
// axWidget
//
//----------------------------------------------------------------------

/**
  \class axWidget
  \brief main widget class

  the main widget class,

*/

class axWidget : public axWidgetBase
{
  protected:
    axWidgetListener* mListener;
    axRect mRect;
    int mAlignment;
    int mFlags;
    axColor mFillColor;
    float mValue;
    int mConIndex;              // hmmm..
    axParameter *mParameter;    // dobbelt opp?
  public:
    void* mUser;
    int mID;

  public:

    axWidget(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
      {
        mListener   = aListener;
        mID         = aID;
        mRect       = aRect;
        mParameter  = NULL;
        clearAllFlags();
        setFlag(wfl_Active);
        setFlag(wfl_Visible);
        setFlag(wfl_Capture);
        mFillColor  = AX_GREY;
        mAlignment  = aAlignment;
        mValue      = 0;
        mUser       = NULL;
        mConIndex   = -1;
      }

    //virtual ~axWidget() {}

    //--------------------------------------------------
    // accessors

    inline axParameter* getParameter(void) { return mParameter; }
    inline void setParameter(axParameter* p) { mParameter = p; }

    inline float getValueDirect(void) { return mValue; }
    inline void setValueDirect(float v) { mValue = v; }

    inline int getConnectionIndex(void) { return mConIndex; }
    inline void setConnectionIndex(int c) { mConIndex = c; }

    inline int getAlignment(void) { return mAlignment; }

    //--------------------------------------------------

    /// switch background fill on/off, and color
    /**
    */
    virtual void setBackground(bool aFill, axColor aColor=AX_GREY)
      {
        if( aFill) setFlag(wfl_Clear);
        else clearFlag(wfl_Clear);
        mFillColor = aColor;
      }

    // --- flags ---

    /// clear all widget flags
    inline void clearAllFlags(void) { mFlags=0; }

    /// set one specific flag
    /**
      see: \ref widget_flags
    */
    inline void setFlag(int aFlag) { mFlags|=aFlag; }

    /// clear (reset) a flag
    /**
      see: \ref widget_flags
    */
    inline void clearFlag(int aFlag) { mFlags &= ~aFlag; }

    /// return true if flag is set
    /**
      see: \ref widget_flags
    */
    inline bool hasFlag(int aFlag) { return (mFlags&aFlag); }

    // --- rect ---

    /// the widgets area, relative to window/editor
    inline axRect getRect(void) { return mRect; }

    /// return true if widget intersects (touches/overlaps) specified rect
    inline bool intersects(axRect aRect) { return (mRect.intersects(aRect)); }

    /// return true if specified point is inside widget
    inline bool contains(int aX, int aY) { return (mRect.contains(aX,aY));  }

    //--------------------------------------------------
    // widget handler
    //--------------------------------------------------

    //virtual void doReset(void) { mValue = 0; }
    /// set widget walue
    virtual void doSetValue(float aValue) { mValue = aValue; }
    /// get widget walue
    virtual float doGetValue(void) { return mValue; }
    /// get widget name
    virtual axString doGetName(void) { return STR_EMPTY; }

    /// move widget
    /**
      \param aX new x position
      \param aY new y position
    */
    virtual void doMove(int aX, int aY)
      {
        mRect.x = aX;
        mRect.y = aY;
      }

    //----------

    /// scroll (content of) widget
    /**
      \param dX number of pixels to scroll horizontally
      \param dY number of pixels to scroll vertically
    */
    virtual void doScroll(int dX, int dY)
      {
        mRect.x += dX;
        mRect.y += dY;
      }

    //----------

    /// resize widget
    /**
      set widget size
      \param aW width
      \param aH height
    */
    virtual void doResize(int aW, int aH)
      {
        mRect.w = aW;
        mRect.h = aH;
      }

    //virtual void doRealign(void) {}

    //----------

    /// paint widget
    /**
      background is filled (with mFillColor) if the widget has the wfl_Visible and wfl_Clear flags set
    */
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if( hasFlag(wfl_Visible) )
        {
          if( hasFlag(wfl_Clear) )
          {
            aCanvas->setBrushColor( mFillColor );
            aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
          }
        }
      }

};

#undef MAX_TEXT_LEN

//----------------------------------------------------------------------
#endif


