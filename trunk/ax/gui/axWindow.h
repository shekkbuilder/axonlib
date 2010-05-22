#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "base/axWindowBase.h"
//#include "gui/axSkin.h"
//#include "gui/axSkinDefault.h"
#include "platform/axContext.h"
#include "axParameter.h"

//----------

#ifdef AX_LINUX
  //#include "platform/linux/axWindowLinux.h"
  #include "gui/linux/axWindowLinux.h"
#endif

#ifdef AX_WIN32
  //#include "platform/win32/axWindowWin32.h"
  #include "gui/win32/axWindowWin32.h"
#endif

//----------------------------------------------------------------------

class axWindow : public axWindowImpl

{
  protected:
  //  axSkinDefault*  mDefaultSkin;
    axWidget*         mModalWidget;
    int               mModalIndex;

  public:

    axWindow(axContext* aContext, axRect aRect, int aWinFlags)
    : axWindowImpl(aContext,aRect,aWinFlags)
      {
        //trace("- axWindow.constructor");
        //axCanvas* can = getCanvas();
        //mDefaultSkin = new axSkinDefault(can);
        //setSkin(mDefaultSkin/*,true*/); // no sub-widgets yet?
        //trace("- axWindow.constructor OK");
        mModalWidget = NULL;
        mModalIndex  = -1;
      }

    virtual ~axWindow()
      {
        //delete mDefaultSkin;
      }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.x2(), mRect.y2() ); }
    //virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.x2(), aRect.y2() ); }
    virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.w, mRect.h ); }
    virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.w, aRect.h ); }
    virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }
    virtual void paramChanged(axParameter* aParameter)  { /*wtrace("axWindow.paramChanged");*/ }

    //----------------------------------------

    virtual void goModal(axWidget* aWidget)
      {
        mModalWidget = aWidget;
        mModalIndex = appendWidget(mModalWidget);
        redrawAll();
      }

    //----------

    void unModal(void)
      {
        removeWidget(mModalIndex);
        delete mModalWidget;
        mModalWidget = NULL;
        redrawAll();
      }


    //----------------------------------------
    //
    //----------------------------------------

    //virtual void doSetPos(int aXpos, int aYpos)
    //  {
    //  }

    virtual void doSetSize(int aWidth, int aHeight)
      {
        //trace("axWindow.doSetSize: " << aWidth << "," << aHeight);
        //mRect.w = aWidth;
        //mRect.h = aHeight;
        mRect.setSize(aWidth,aHeight);
        if (mFlags&wf_Align) doRealign();
      }

    //virtual void doMove(int aDeltaX, int aDeltaY)
    //  {
    //  }
    //virtual void doResize(int aDeltaX, int aDeltaY)
    //  {
    //  }
    //virtual void doRealign(void)
    //  {
    //  }
    //virtual void doPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //  }

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
        else axWidget::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
        else axWidget::doMouseUp(aXpos,aYpos,aButton);
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
        else axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        else axWidget::doKeyDown(aKeyCode,aState);
      }

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        else axWidget::doKeyUp(aKeyCode,aState);
      }

    //virtual void doEnter(axWidget* aCapture)
    //  {
    //  }
    //virtual void doLeave(axWidget* aCapture)
    //  {
    //  }

    //----------------------------------------
    // axWidgetListener
    //----------------------------------------

    virtual void onChange(axWidget* aWidget)  { redrawWidget(aWidget); }
    virtual void onRedraw(axWidget* aWidget)  { redrawWidget(aWidget); }
    virtual void onCursor(int aCursor)        { setCursor(aCursor); }
    virtual void onHint(axString aHint)       {}

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
      {
        if (mFlags&wf_Align) doRealign();
        //if (mFlags&wf_Visible)
          redrawAll();
      }

    virtual void onModal(bool aModal, axWidget* aWidget)
      {
        trace("onModal");
        if (aModal) goModal(aWidget);
        else unModal();
      }


};

//----------------------------------------------------------------------
#endif
