#ifndef axApplication_included
#define axApplication_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "axContext.h"

//----------------------------------------------------------------------

class axApplication
{
  public:
    axApplication(axContext* aContext) {}
    //virtual ~axApplication() {}
    virtual int main(void) {return 0;}
};

//----------------------------------------------------------------------

#ifdef AX_LINUX

  #define AX_APP_CONTEXT_INIT                       \
    XInitThreads();                                 \
    axContext ctx;                                  \
    Display* display = XOpenDisplay(NULL);          \
    Window   parent  = XDefaultRootWindow(display); \
    void*    audio   = NULL;                        \
    ctx.mDisplay = display;                         \
    ctx.mWindow  = parent;                          \
    ctx.mAudio   = audio;

  #define AX_APP_CONTEXT_EXIT                       \
    XCloseDisplay(display);

#endif

//----------------------------------------------------------------------

#ifdef AX_WIN32
#endif

//----------------------------------------------------------------------

//trace("display " << (int)display);
//trace("parent  " << (int)parent);
//trace("audio   " << (int)audio);

#define AX_APP_ENTRY(classname)             \
int main(void)                              \
{                                           \
  AX_APP_CONTEXT_INIT                       \
  axApplication* app = new classname(&ctx); \
  int ret = app->main();                    \
  AX_APP_CONTEXT_EXIT                       \
  delete app;                               \
  return ret;                               \
}

//----------------------------------------------------------------------
#endif
