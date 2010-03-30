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

    virtual axCanvas* getCanvas(void) { return mCanvas; }
    virtual axSurface* getSurface(void) { return mSurface; }

    //inline void redraw(axWidget* aWidget) { axRect R = aWidget->getRect(); invalidate(R.x,R.y,R.x2(),R.y2()); }

    //----------------------------------------
    //
    //----------------------------------------

//    virtual void doMove(int aXpos, int aYpos) {}

    virtual void doResize(int aWidth, int aHeight)
      {
        //trace("axWindow.DoResize " << aWidth << "," << aHeight);
        //TODO: doRealign
        mRect.w = aWidth;
        mRect.h = aHeight;
      }

//    virtual void doRealign(void) {}
//    virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}
//    virtual void doMouseDown(int aXpos, int aYpos, int aButton) {}
//    virtual void doMouseUp(int aXpos, int aYpos, int aButton) {}
//    virtual void doMouseMove(int aXpos, int aYpos, int aButton) {}
//    virtual void doKeyDown(int aKeyCode, int aState) {}
//    virtual void doKeyUp(int aKeyCode, int aState) {}

//    // axWidgetListener

      virtual void redrawRect(axRect aRect)         { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
      virtual void redrawWidget(axWidget* aWidget)  { redrawRect(aWidget->rect()); }

      virtual void paramChanged(axParameter* aParameter) { trace("axWindow.paramChanged"); }

};

//----------------------------------------------------------------------
#endif
