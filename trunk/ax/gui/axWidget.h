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
#define wa_Stacked      10

// widget flags
#define wf_None         0
#define wf_Active       1
#define wf_Visible      2
#define wf_Capture      4
#define wf_Hover        8
#define wf_Align        16
#define wf_Vertical     32
#define wf_Clip         64

//----------------------------------------------------------------------
//
// widget listener
//
//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

//----------

// used when you need to notify the 'owner' about something

class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
    virtual void onRedraw(axWidget* aWidget) {}
    virtual void onCursor(int aCursor=DEF_PENWIDTH) {}
    virtual void onHint(axString aHint) {}
    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY) {}
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class axWidget : public axWidgetListener
{
  friend class axPlugin;
  friend class axContainer;

  protected:

    axWidgetListener* mListener;
    axRect            mRect;
    int               mFlags;
    int               mAlignment;
    int               mConnection;    // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;     // direct access to the parameter (set in axEditor.connect)
    float             mValue;
    axRect            mOrig;
    axSkin*           mSkin;
    int               mMinWidth, mMinHeight;
    int               mMaxWidth, mMaxHeight;
  public:
    int     mId;
    void*   mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mListener   = aListener;
        mRect       = aRect;
        mFlags      = wf_Active|wf_Visible|wf_Capture;
        mAlignment  = aAlignment;
        mConnection = -1;
        mParameter  = NULL;
        mValue      = 0;
        mOrig       = mRect;
        mSkin       = NULL;
        mMinWidth   = 0;
        mMinHeight  = 0;
        mMaxWidth   = 999999;
        mMaxHeight  = 999999;
        mId         = 0;//aId;
        mPtr        = NULL;//aPtr;
      }

    //virtual ~axWidget()
    //  {
    //  }

    //----------

    inline void setFlag(int aFlag)      { mFlags |= aFlag; }
    inline void setAllFlags(int aFlags) { mFlags = aFlags; }
    inline void clearFlag(int aFlag)    { mFlags &= ~aFlag; }
    inline bool hasFlag(int aFlag)      { return (mFlags&aFlag); }

//    inline void setOption(int aOption)      { mOptions |= aOption; }
//    inline void setAllOptions(int aOptions) { mOptions = aOptions; }
//    inline void clearOption(int aOption)    { mOptions &= ~aOption; }
//    inline bool hasOption(int aOption)      { return (mOptions&aOption); }

    inline axRect       getRect(void) { return mRect; }
    inline int          getFlags(void) { return mFlags; }
    inline bool         isActive(void)  { return (mFlags&wf_Active); }
    inline bool         isVisible(void) { return (mFlags&wf_Visible); }
    inline bool         doCapture(void) { return (mFlags&wf_Capture); }
    virtual bool        intersects(axRect aRect) { return mRect.intersects(aRect); }
    virtual bool        contains(int aXpos, int aYpos) { return mRect.contains(aXpos,aYpos); }
    inline float        getValue(void) { return mValue; }
    inline void         setValue(float aValue) { mValue=aValue; }
    inline int          getConnection(void) { return mConnection; }
    inline void         setConnection(int aNum) { mConnection=aNum; }
    inline axParameter* getParameter(void) { return mParameter; }
    inline void         setParameter(axParameter* aParameter) { mParameter=aParameter; }

//    inline void setText(axString aText, int aAlign=ta_Center) { mText=aText; mTextAlign=aAlign; }
//    inline void setImage(axImage* aImage) { mImage=aImage; }
    inline void setSizeLimits(int minw, int minh, int maxw=999999, int maxh=999999) { mMinWidth=minw; mMinHeight=minh; mMaxWidth=maxw; mMaxHeight=maxh; }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    //virtual void  doSetValue(float aValue) { mValue=aValue; }
    //virtual float doGetValue(void) { return mValue; }

    virtual axWidget* doFindWidget(int aXpos, int aYpos)
      {
        return this;
      }

    //----------

    virtual void doSetSkin(axSkin* aSkin, bool aSubWidgets=false)
      {
        mSkin = aSkin;
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual void doSetPos(int aXpos, int aYpos)
      {
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        if (aWidth < mMinWidth) aWidth = mMinWidth;
        if (aWidth > mMaxWidth) aWidth = mMaxWidth;
        if (aHeight < mMinHeight) aHeight = mMinHeight;
        if (aHeight > mMaxHeight) aHeight = mMaxHeight;
        mRect.setSize(aWidth,aHeight);
      }

    //----------

    //virtual void doMove(int aDeltaX, int aDeltaY)
    //  {
    //    int x = mRect.x + aDeltaX;
    //    int y = mRect.y + aDeltaY;
    //    mRect.setPos(x,y);
    //  }

    //----------

    virtual void doResize(int aDeltaX, int aDeltaY)
      {
        int w = mRect.w + aDeltaX;
        int h = mRect.h + aDeltaY;
        this->doSetSize(w,h);
      }

    virtual void doRealign(void) {}

    virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}

    virtual void doMouseDown(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseUp(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseMove(int aXpos, int aYpos, int aButton) {}

    virtual void doKeyDown(int aKeyCode, int aState) {}
    virtual void doKeyUp(int aKeyCode, int aState) {}

    virtual void doEnter(axWidget* aWidget) {}
    virtual void doLeave(axWidget* aWidget) {}
};


//----------------------------------------------------------------------
#endif
