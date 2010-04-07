#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------

/*
  a widget is the base class for the gui.
  it is mainly a rectangle in a window, and can receive messages
  (mouse, keys, paint, ..) from its 'owner' to do something, and can
  send messages back to notify its handler that something has changed.

  the rect coords are kept in screen-space, relative to the window.
  this simplifies quite a few calculations at runtime,
  but also mean that you have to be a little bit careful when setting
  up the editor/window...
*/

#include "core/axRect.h"
#include "gui/axCanvas.h"
#include "gui/axSkin.h"
#include "axParameter.h"

//----------------------------------------------------------------------

// widget alignment
#define wa_None         0
#define wa_Client       1
#define wa_Left         2
#define wa_Right        3
#define wa_Top          4
#define wa_Bottom       5
#define wa_LeftTop      6
#define wa_RightTop     7
#define wa_LeftBottom   8
#define wa_RightBottom  9
#define wa_Stacked     10
//#define wa_Parent      11

// widget flags
#define wf_Active   0x0001
#define wf_Visible  0x0002
#define wf_Capture  0x0004
#define wf_Hover    0x0008
#define wf_Align    0x0010
#define wf_Vertical 0x0020
#define wf_Clip     0x0040

#define wf_Fill     0x0100
#define wf_Border   0x0200
#define wf_Bevel    0x0400
#define wf_Image    0x0800
#define wf_Text     0x1000


//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
    //virtual void onRedraw(axWidget* aWidget) {}
};

//----------------------------------------------------------------------

//----------------------------------------------------------------------

class axWidget : public axWidgetListener
{
  friend class axPlugin;
  friend class axContainer;

  protected:

    axWidgetListener* mListener;
    axRect            mRect;
    axFlags           mFlags;
    int               mAlignment;
    int               mConnection;    // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;     // direct access to the parameter (set in axEditor.connect)
    float             mValue;
    // alignment
    //axPoint           mOrig;
    axRect            mOrig;


  public:
    //int     mId;
    //void*   mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mListener = aListener;
        mRect = aRect;
        mFlags.mBits = wf_Active | wf_Visible | wf_Capture;
        mAlignment = aAlignment;
        mConnection = -1;
        mParameter = NULL;
        mValue = 0;

        mOrig = mRect;//.set(aRect.x,aRect.y);

        //mId = aId;
        //mPtr = aPtr;
      }

    virtual ~axWidget()
      {
        //deleteWidgets();
      }

    //----------

    inline axRect   getRect(void) { return mRect; }
    inline axFlags  getFlags(void) { return mFlags; }

    inline bool isActive(void)  { return mFlags.hasFlag(wf_Active); }
    inline bool isVisible(void) { return mFlags.hasFlag(wf_Visible); }
    inline bool doCapture(void) { return mFlags.hasFlag(wf_Capture); }

    virtual bool intersects(axRect aRect) { return mRect.intersects(aRect); }
    virtual bool contains(int aXpos, int aYpos) { return mRect.contains(aXpos,aYpos); }

    //----------

    inline float getValue(void) { return mValue; }
    inline void  setValue(float aValue) { mValue=aValue; }

    inline int  getConnection(void)     { return mConnection; }
    inline void setConnection(int aNum) { mConnection = aNum; }

    inline axParameter* getParameter(void) { return mParameter; }
    inline void setParameter(axParameter* aParameter) { mParameter = aParameter; }

    //----------

    virtual void setSkin(axSkin* aSkin, bool aSubWidgets=false) {}
    virtual axWidget* findWidget(int aXpos, int aYpos) { return this; }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual void doMove(int aXpos, int aYpos)
      {
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doResize(int aWidth, int aHeight)
      {
        //if (aW < mMinWidth) aW = mMinWidth;
        //if (aW > mMaxWidth) aW = mMaxWidth;
        //if (aH < mMinHeight) aH = mMinHeight;
        //if (aH > mMaxHeight) aH = mMaxHeight;
        mRect.setSize(aWidth,aHeight);
      }

    //----------

    virtual void doRealign(void) {}
    virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}
    virtual void doMouseDown(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseUp(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseMove(int aXpos, int aYpos, int aButton) {}
    virtual void doKeyDown(int aKeyCode, int aState) {}
    virtual void doKeyUp(int aKeyCode, int aState) {}
    virtual void doEnter(axWidget* aCapture) {}
    virtual void doLeave(axWidget* aCapture) {}
    // axWidgetListener
    //virtual void onChange(axWidget* aWidget)

};


//----------------------------------------------------------------------
#endif
