#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#include "core/axStdlib.h"
#include "core/axRect.h"
#include "platform/axContext.h"
#include "gui/axWindow.h"
#include "base/axFormatBase.h"

#define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGDELETE)

//----------------------------------------------------------------------

class axFormatExe : public axFormatBase//,
                    //public axWidgetListener
{
  friend int main(void);

  protected:
    axString    mTitle;
    char        mTitleBuffer[256];
    axHostInfo  mHostInfo;

  protected:

    axFormatExe(axContext* aContext, int aFormatFlags)
    : axFormatBase(aContext, aFormatFlags)
      {
        //wtrace("axFormatExe.constructor");
        mTitle = "axFormatExe";
        mTitleBuffer[0] = 0;
      }

  //public:

    virtual ~axFormatExe()
      {
        //wtrace("axFormatExe.destructor");
      }

    //--------------------------------------------------

  public:

    virtual int main(axContext* aContext)
      {
        //wtrace("axFormatExe.main");
        // can this fail if we create the class as axEditor
        // meaning, can we safely typecase a axEditor* to a axWindow?
        if (mFormatFlags&pf_HasEditor)
        {
          axWindow* win = (axWindow*)doOpenEditor(aContext);
          //wtrace("win = " << win);
          win->setTitle(mTitle);
          win->eventLoop();
          doCloseEditor();
        }
        return 0;
      }

    //TODO:
    // - "pretend to be host"
    //- audio/midi io (plibaam/portaudio)
    //- timer thread for doIdleEditor

    //--------------------------------------------------

    virtual axHostInfo* getHostInfo(void)
      {
        mHostInfo.name    = "standalone";
        mHostInfo.id      = 0;
        mHostInfo.format  = "exe";
        return &mHostInfo;
      }

    virtual void describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        char* pos = mTitleBuffer;
        axStrcpy(pos,aName.ptr());
        pos += aName.length();
        axStrcpy(pos," :: ");
        pos += 4;
        axStrcpy(pos,aVendor.ptr());
        mTitle = mTitleBuffer;
      }

      //virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      //  {
      //  }

    virtual void setupEditor(int aWidth, int aHeight)
      {
        mEditorRect.set(0,0,aWidth,aHeight);
        mFormatFlags |= pf_HasEditor;
      }

//    virtual void setupParameters(void)
//      {
//        int num = mParameters.size();
//        for (int i=0; i<num; i++)
//        {
//          doSetParameter(mParameters[i]);
//        }
//      }

    virtual void setupParameters(void)
      {
        int num = mParameters.size();
        //setNumParams(num); // vst
        for (int i=0; i<num; i++)
        {
          axParameter* par = mParameters[i];
          par->setIndex(i);
          doSetParameter(par);
        }
      }


    //virtual void  notifyParamChanged(axParameter* aParameter)
    //  {
    //  }

    virtual void  notifyResizeEditor(int aWidth, int aHeight)
      {
        //trace("axFormatExe.notifyResizeEditor: " << aWidth << "," << aHeight);
        mEditorRect.w = aWidth;
        mEditorRect.h = aHeight;
        //sizeWindow(aWidth, aHeight); // vst
      }


};

typedef axFormatExe axFormatImpl;

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#ifdef AX_LINUX

//  axContext* axInitialize(void)
//    {
//      XInitThreads();
//      Display*  display = XOpenDisplay(NULL);
//      Window    parent  = XDefaultRootWindow(display);
//      AX_PTRCAST  audio   = NULL;
//      ctx.mDisplay = display;
//      ctx.mWindow  = parent;
//      ctx.mAudio   = audio;
//    }

  #define AX_CONTEXT_INIT(name)                         \
    XInitThreads();                                     \
    Display*    display = XOpenDisplay(NULL);           \
    Window      parent  = XDefaultRootWindow(display);  \
    axContext ctx(display,parent);

  #define AX_CONTEXT_EXIT                               \
    XCloseDisplay(display);

#endif

//----------

//----------

#ifdef AX_WIN32

  #define MAKESTRING2(s) #s
  #define MAKESTRING(s) MAKESTRING2(s)
  #define MAKE_NAME(name) MAKESTRING(name) "_window"

  #define AX_CONTEXT_INIT(name)                             \
    HINSTANCE instance  = (HINSTANCE)GetModuleHandle(NULL); \
    char*       winname = (char*)MAKE_NAME(name);           \
    axContext ctx(instance,winname);

  #define AX_CONTEXT_EXIT ;
  // unregister window?
  // what if multiple instances is using the same window?

#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define AX_ENTRYPOINT(classname)            \
int main(void)                              \
{                                           \
  AX_CONTEXT_INIT(classname)                \
  axFormatImpl* app = new classname(&ctx);  \
  int ret = app->main(&ctx);                \
  AX_CONTEXT_EXIT                           \
  delete app;                               \
  return ret;                               \
}

//----------------------------------------------------------------------
#endif
