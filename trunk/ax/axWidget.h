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
#include "axPoint.h"
#include "axMath.h"
#include "axCanvas.h"

// widget flags

/** \defgroup widget_flags widget flags */
/* @{ */
#define wfl_Active      1       ///< is widget active and reacts to (mouse) events?
#define wfl_Visible     2       ///< is widget visible, and will be painteed?
//#define wfl_DefaultDraw 4       ///< do default drawing
#define wfl_Clip        8       ///< set cliprect before painting
#define wfl_Capture     16      ///< captures mouse events?
#define wfl_Vertical    32      ///< vertical instead of horizontal (layout, drag direction, ..)
#define wfl_Align       64      ///< set if auto-align/size/pos
#define wfl_All         0xffff  ///< set if auto-align/size/pos
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

// panel/paintbox draw modes
#define pdm_None        0
#define pdm_Background  1
#define pdm_Border      2
#define pdm_Image       4
#define pdm_All         0xffff


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
  //friend class axContainer;
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
  friend class axContainer;
  friend class axEditor;
  protected:
    axWidgetListener* mListener;
    axRect            mRect;
    axPoint           mOrig;
    int               mAlignment;
    int               mFlags;
    int               mConnection;
    float             mValue;
    axParameter*      mParameter;             // for displaying. this shouldn't really be here...
    int               mMinWidth, mMinHeight;
    int               mMaxWidth, mMaxHeight;
  public:
    void*             mUser;
    int               mID;

  public:

    axWidget(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
      {
        mListener   = aListener;
        mRect       = aRect;
        mOrig.set(mRect.x,mRect.y);
        mAlignment  = aAlignment;
        clearAllFlags();
        setFlag(wfl_Active);
        setFlag(wfl_Visible);
        setFlag(wfl_Capture);
        mConnection = -1;
        mValue      = 0;
        mParameter  = NULL;
        mMinWidth   = mMinHeight = 0;
        mMaxWidth   = mMaxHeight = 999999;
        mUser       = NULL;
        mID         = aID;
      }

    //virtual ~axWidget()
    //  {
    //  }

    //--------------------------------------------------

    inline void setMinWidth(int w)  { mMinWidth = w; }
    inline void setMaxWidth(int w)  { mMaxWidth = w; }
    inline void setMinHeight(int h) { mMinHeight = h; }
    inline void setMaxHeight(int h) { mMaxHeight = h; }

    virtual axWidget* connect(axParameter* aParameter) { /*trace("axWidget.connect");*/ mParameter=aParameter; return this; }
    virtual axParameter* getParameter(void) { return mParameter; }

    //virtual void setParameter(axParameter* aParameter)
    //  {
    //    trace("axWidget.setParameter");
    //    mParameter = aParameter;
    //  }

    //--------------------------------------------------
    // inline
    //--------------------------------------------------

    inline axRect getRect(void)
      {
        return mRect;
      }

    // --- flags ---

    /// clear all widget flags
    inline void clearAllFlags(void)
      {
        mFlags=0;
      }

    /// set one specific flag
    /**
      see: \ref widget_flags
    */
    inline void setFlag(int aFlag)
      {
        mFlags|=aFlag;
      }

    /// clear (reset) a flag
    /**
      see: \ref widget_flags
    */
    inline void clearFlag(int aFlag)
      {
        mFlags &= ~aFlag;
      }

    /// return true if flag is set
    /**
      see: \ref widget_flags
    */
    inline bool hasFlag(int aFlag)
      {
        return (mFlags&aFlag);
      }

    // --- rect ---

    /// return true if widget intersects (touches/overlaps) specified rect
    inline bool intersects(axRect aRect)
      {
        return (mRect.intersects(aRect));
      }

    /// return true if specified point is inside widget
    inline bool contains(int aX, int aY)
      {
        return (mRect.contains(aX,aY));
      }

    //--------------------------------------------------

    //----------

    virtual axWidget* findHover(int aX, int aY)
      {
        //if (mRect.contains(aX,aY))
          return this;
        //else
        //  return NULL;
      }

    //--------------------------------------------------
    // widget base
    //--------------------------------------------------

    //virtual void doReset(void) { mValue = 0; }
    /// set widget walue
    virtual void doSetValue(float aValue)
      {
        mValue = aValue;
      }

    /// get widget walue
    virtual float doGetValue(void)
      {
        return mValue;
      }

    /// get widget name
    virtual axString doGetName(void)
      {
        return STR_EMPTY;
      }

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

//    /// scroll (content of) widget
//    /**
//      \param dX number of pixels to scroll horizontally
//      \param dY number of pixels to scroll vertically
//    */
//    virtual void doScroll(int dX, int dY)
//      {
//        // hmmm, this should not be here?
//        // doScroll means 'scroll content', not 'scroll yourself'
//        // move to axContainer?
//        mRect.x += dX;
//        mRect.y += dY;
//      }

    //----------

    /// resize widget
    /**
      set widget size
      \param aW width
      \param aH height
    */
    virtual void doResize(int aW, int aH)
      {
        if (aW < mMinWidth) aW = mMinWidth;
        if (aW > mMaxWidth) aW = mMaxWidth;
        if (aH < mMinHeight) aH = mMinHeight;
        if (aH > mMaxHeight) aH = mMaxHeight;
        mRect.w = aW;
        mRect.h = aH;
      }

};

#undef MAX_TEXT_LEN

//----------------------------------------------------------------------
#endif


