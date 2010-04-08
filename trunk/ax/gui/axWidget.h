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

// widget flags
#define wf_Active   0x0001
#define wf_Visible  0x0002
#define wf_Capture  0x0004
#define wf_Hover    0x0008
#define wf_Align    0x0010
#define wf_Vertical 0x0020
#define wf_Clip     0x0040

//#define wf_Fill     0x0100
//#define wf_Border   0x0200
//#define wf_Bevel    0x0400
//#define wf_Image    0x0800
//#define wf_Text     0x1000

// widget options
#define wo_Fill     0x01
#define wo_Border   0x02
#define wo_Bevel    0x04
#define wo_Image    0x08
#define wo_Text     0x10

//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

//----------

class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
    //virtual void onRedraw(axWidget* aWidget) {}
    virtual void onCursor(int aCursor=DEF_PENWIDTH) {}
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
    int               mFlags;
    int               mAlignment;
    int               mConnection;    // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;     // direct access to the parameter (set in axEditor.connect)
    float             mValue;
    axRect            mOrig;
    axSkin*           mSkin;
    int               mMinWidth;
    int               mMaxWidth;
    int               mMinHeight;
    int               mMaxHeight;

  protected: // painting
    int       mOptions;
    //axColor   mLightColor;
    //axColor   mDarkColor;
    //axColor   mFillColor;
    //axColor   mTextColor;
    axImage*  mImage;
    axString  mText;
    int       mTextAlign;

  //public:
  //  int     mId;
  //  void*   mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mListener   = aListener;
        mRect       = aRect;
        mFlags      = wf_Active | wf_Visible | wf_Capture;
        mAlignment  = aAlignment;
        mConnection = -1;
        mParameter  = NULL;
        mValue      = 0;
        mOrig       = mRect;
        mSkin       = NULL;
        mMinWidth  = 0;
        mMaxWidth  = 999999;
        mMinHeight = 0;
        mMaxHeight = 999999;
        //mId = aId;
        //mPtr = aPtr;
        mOptions    = wo_Fill | wo_Bevel;
        mImage      = NULL;
        mText       = "wdgPanel";
        mTextAlign  = ta_Center;

      }

    //virtual ~axWidget()
    //  {
    //  }

    //----------

    inline void setFlag(int aFlag) { mFlags |= aFlag; }
    inline void setAllFlags(int aFlags) { mFlags = aFlags; }
    inline void clearFlag(int aFlag) { mFlags &= ~aFlag; }
    inline bool hasFlag(int aFlag) { return (mFlags&aFlag); }

    inline void setOption(int aOption)      { mOptions |= aOption; }
    inline void setAllOptions(int aOptions) { mOptions = aOptions; }
    inline void clearOption(int aOption)    { mOptions &= ~aOption; }
    inline bool hasOption(int aOption)      { return (mOptions&aOption); }

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
    inline void         setConnection(int aNum) { mConnection = aNum; }
    inline axParameter* getParameter(void) { return mParameter; }
    inline void         setParameter(axParameter* aParameter) { mParameter = aParameter; }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual axWidget* doFindWidget(int aXpos, int aYpos)
      {
        return this;
      }

    //----------

    virtual void doMove(int aXpos, int aYpos)
      {
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doResize(int aWidth, int aHeight)
      {
        if (aWidth < mMinWidth) aWidth = mMinWidth;
        if (aWidth > mMaxWidth) aWidth = mMaxWidth;
        if (aHeight < mMinHeight) aHeight = mMinHeight;
        if (aHeight > mMaxHeight) aHeight = mMaxHeight;
        mRect.setSize(aWidth,aHeight);
      }

    //----------

    virtual void doRealign(void) {}

    //----------

    virtual void doSetSkin(axSkin* aSkin, bool aSubWidgets=false)
      {
        mSkin = aSkin;
        //mLightColor = aSkin->getColor(3);
        //mDarkColor  = aSkin->getColor(1);
        //mFillColor  = aSkin->getColor(2);
        //mTextColor  = aSkin->getColor(4);
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        // assert skin?
        // --- fill
        if (mOptions&wo_Fill)
        {
          //aCanvas->setBrushColor(mFillColor);
          aCanvas->setPenColor( mSkin->getColor(2) );
          aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        }
        // --- image
        if (mOptions&wo_Image)
        {
          aCanvas->drawImage(mImage,0,0,mRect.x,mRect.y,mRect.w,mRect.h);
        }
        // --- border
        if (mOptions&wo_Border)
        {
          //aCanvas->setPenColor(mLightColor);
          aCanvas->setPenColor( mSkin->getColor(3) );
          aCanvas->drawRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        }
        else
        // --- bevel
        if (mOptions&wo_Bevel)
        {
          //aCanvas->setPenColor(mLightColor);
          aCanvas->setPenColor( mSkin->getColor(3) );
          aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x2()-1,mRect.y     );
          aCanvas->drawLine(mRect.x,   mRect.y,   mRect.x,     mRect.y2()-1);
          //aCanvas->setPenColor(mDarkColor);
          aCanvas->setPenColor( mSkin->getColor(1) );
          aCanvas->drawLine(mRect.x+1, mRect.y2(),mRect.x2(),  mRect.y2()  );
          aCanvas->drawLine(mRect.x2(),mRect.y+1, mRect.x2(),  mRect.y2()  );
        }
        // --- text
        if (mOptions&wo_Text)
        {
          //aCanvas->setPenColor(mTextColor);
          aCanvas->setPenColor( mSkin->getColor(4) );
          aCanvas->drawText(mRect.x,mRect.y,mRect.x2(),mRect.y2(),mText,mTextAlign);
        }
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseUp(int aXpos, int aYpos, int aButton) {}
    virtual void doMouseMove(int aXpos, int aYpos, int aButton) {}
    virtual void doKeyDown(int aKeyCode, int aState) {}
    virtual void doKeyUp(int aKeyCode, int aState) {}
    virtual void doEnter(axWidget* aCapture) {}
    virtual void doLeave(axWidget* aCapture) {}
};


//----------------------------------------------------------------------
#endif
