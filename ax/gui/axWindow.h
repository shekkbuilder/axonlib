#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "base/axWindowBase.h"
#include "gui/axSkin.h"
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
  //  axSkin* mSkin;

  public:
    axWindow(axContext* aContext, axRect aRect, int aWinFlags)
    : axWindowImpl(aContext,aRect,aWinFlags)
      {
        //mSkin = NULL;
      }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual axCanvas* getCanvas(void) { return mCanvas; }
    //virtual axSurface* getSurface(void) { return mSurface; }

    //inline void redraw(axWidget* aWidget) { axRect R = aWidget->getRect(); invalidate(R.x,R.y,R.x2(),R.y2()); }

    //----------------------------------------
    //
    //----------------------------------------

      //virtual void doSetPos(int aXpos, int aYpos) {}

      virtual void doSetSize(int aWidth, int aHeight)
        {
          mRect.w = aWidth;
          mRect.h = aHeight;
          if (mFlags&wf_Align) doRealign(); // widget flags
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

      virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
      virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
      virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }
      virtual void paramChanged(axParameter* aParameter)  { /*wtrace("axWindow.paramChanged");*/ }

      // axWidgetListener

      virtual void onChange(axWidget* aWidget) { redrawWidget(aWidget); }
      virtual void onRedraw(axWidget* aWidget) { redrawWidget(aWidget); }
      virtual void onCursor(int aCursor) { setCursor(aCursor); }
      virtual void onHint(axString aHint) {}
      virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY) { doRealign(); redrawAll(); }

};

//----------------------------------------------------------------------
#endif
