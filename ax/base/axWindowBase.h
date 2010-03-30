#ifndef axWindowBase_included
#define axWindowBase_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "core/axRect.h"
#include "core/axString.h"
//#include "gui/axWidget.h"
#include "gui/axContainer.h"
#include "gui/axSurface.h"
#include "gui/axCanvas.h"
//#include "gui/axColor.h"
//#include "gui/axPen.h"
//#include "gui/axBrush.h"
//#include "gui/axFont.h"

#define AX_WIN_NOBORDER   1
#define AX_WIN_MSGTHREAD  2
#define AX_WIN_MSGPROC    4
#define AX_WIN_MSGDELETE  8
#define AX_WIN_BUFFERED   16

#define AX_WIN_APPDEFAULT   ( AX_WIN_BUFFERED | AX_WIN_MSGDELETE )
#define AX_WIN_PLUGDEFAULT  ( AX_WIN_BUFFERED | AX_WIN_MSGTHREAD | AX_WIN_NOBORDER)

class axWindowBase : public axContainer//axWidget
{
  protected:
    int         mWinFlags;
    axCanvas*   mCanvas;
    axSurface*  mSurface;
    axWidget*   mCapturedWidget;


    axWindowBase(axContext* aContext, axRect aRect, int aWinFlags)
    : axContainer(NULL,aRect)
    //: axWidget(NULL,aRect)
      {
        mRect     = aRect;
        mWinFlags = aWinFlags;
        mCanvas   = NULL;
        mSurface  = NULL;
      }
    virtual ~axWindowBase() {}

    // accessors

    virtual int getHandle(void) { return 0; }

    // low level

    virtual void    flush(void) {}
    virtual void    sync(void) {}
    virtual void    lock(void) {}
    virtual void    unlock(void) {}
    virtual void    show(void) {}
    virtual void    hide(void) {}
    virtual void    setPos(int aX, int aY) {}
    virtual void    setSize(int aW, int aH) {}
    virtual void    setTitle(axString aTitle) {}
    virtual void    reparent(int aParent) {}

    // medium level

    virtual void    invalidate(int aX, int aY, int aW, int aH) {}
    virtual void    resizeBuffer(int aWidth, int aHeight) {}
    virtual void    beginPaint(void) {}
    virtual void    endPaint(void) {}

    // events

    virtual void    sendEvent(unsigned int aValue=0) {}
    virtual void    eventLoop(void) {}

    // axWidgetListener

    virtual void onChange(axWidget* aWidget) { trace("axWindowBase.onChange(wdg)"); }
    virtual void onRedraw(axWidget* aWidget) { trace("axWindowBase.onRedraw(wdg)"); }

};

//----------------------------------------------------------------------
#endif
