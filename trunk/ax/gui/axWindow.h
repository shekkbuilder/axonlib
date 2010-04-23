#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "base/axWindowBase.h"
//#include "gui/axSkin.h"
#include "gui/axSkinDefault.h"
#include "platform/axContext.h"
#include "axParameter.h"

//----------

#ifdef AX_LINUX
  #include "platform/linux/axWindowLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axWindowWin32.h"
#endif

//----------------------------------------------------------------------

class axWindow : public axWindowImpl

{
  //protected:
  //  axSkinDefault*  mDefaultSkin;

  public:

    axWindow(axContext* aContext, axRect aRect, int aWinFlags)
    : axWindowImpl(aContext,aRect,aWinFlags)
      {
        //trace("- axWindow.constructor");
        //axCanvas* can = getCanvas();
        //mDefaultSkin = new axSkinDefault(can);
        //setSkin(mDefaultSkin/*,true*/); // no sub-widgets yet?
        //trace("- axWindow.constructor OK");
      }

    virtual ~axWindow()
      {
        //delete mDefaultSkin;
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
    virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }
    virtual void paramChanged(axParameter* aParameter)  { /*wtrace("axWindow.paramChanged");*/ }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual void doSetPos(int aXpos, int aYpos) {}

    virtual void doSetSize(int aWidth, int aHeight)
      {
        //mRect.w = aWidth;
        //mRect.h = aHeight;
        mRect.setSize(aWidth,aHeight);
        if (mFlags&wf_Align) doRealign();
      }

    //virtual void doMove(int aDeltaX, int aDeltaY) {}
    //virtual void doResize(int aDeltaX, int aDeltaY) {}
    //virtual void doRealign(void) {}
    //virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}
    //virtual void doMouseDown(int aXpos, int aYpos, int aButton) {}
    //virtual void doMouseUp(int aXpos, int aYpos, int aButton) {}
    //virtual void doMouseMove(int aXpos, int aYpos, int aButton) {}
    //virtual void doKeyDown(int aKeyCode, int aState) {}
    //virtual void doKeyUp(int aKeyCode, int aState) {}
    //virtual void doEnter(axWidget* aCapture) {}
    //virtual void doLeave(axWidget* aCapture) {}

    // axWidgetListener

    virtual void onChange(axWidget* aWidget) { redrawWidget(aWidget); }
    virtual void onRedraw(axWidget* aWidget) { redrawWidget(aWidget); }
    virtual void onCursor(int aCursor) { setCursor(aCursor); }
    virtual void onHint(axString aHint) {}

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY)
      {
        if (mFlags&wf_Align) doRealign();
        /*if (mFlags&wf_Visible)*/ redrawAll();
      }

};

//----------------------------------------------------------------------
#endif
