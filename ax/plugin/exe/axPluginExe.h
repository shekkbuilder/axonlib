#ifndef axPluginExe_included
#define axPluginExe_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "core/axRect.h"
#include "gui/axWindow.h"
#include "base/axPluginBase.h"

//----------------------------------------------------------------------

class axPluginExe : public axPluginBase,
                    public axWidgetListener
{
  friend int main(void);

  protected:
    axString  mTitle;
    char      mTitleBuffer[256];

  protected:

    axPluginExe(axContext* aContext, int aPluginFlags)
    : axPluginBase(aContext, aPluginFlags)
      {
        mTitle = "axPluginExe";
        mTitleBuffer[0] = 0;
      }

  //public:

    virtual ~axPluginExe()
      {
      }

    //--------------------------------------------------

  public:

    virtual int main(axContext* aContext)
      {
        // can this fail if we create the class as axEditor
        // meaning, can we safely typecase a axEditor* to a axWindow?
        axWindow* win = (axWindow*)doOpenEditor(aContext);
        win->setTitle(mTitle);
        win->eventLoop();
        doCloseEditor();
        return 0;
      }

    //TODO:
    // - "pretend to be host"
    //- audio/midi io (plibaam/portaudio)
    //- timer thread for doIdleEditor

    //--------------------------------------------------

    virtual void describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        char* pos = mTitleBuffer;
        strcpy(pos,aName.ptr());
        pos += aName.length();
        strcpy(pos," :: ");
        pos += 4;
        strcpy(pos,aVendor.ptr());
        mTitle = mTitleBuffer;
      }

      //virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      //  {
      //  }

    virtual void setupEditor(int aWidth, int aHeight)
      {
        mEditorRect.set(0,0,aWidth,aHeight);
        mPluginFlags.setFlag(pf_HasEditor);
      }

    virtual void setupParameters(void)
      {
        int num = mParameters.size();
        for (int i=0; i<num; i++)
        {
          doSetParameter(mParameters[i]);
        }
      }

    //virtual void  notifyParamChanged(axParameter* aParameter)
    //  {
    //  }


};

typedef axPluginExe axPluginImpl;

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#ifdef AX_LINUX

  #define AX_CONTEXT_INIT                           \
    XInitThreads();                                 \
    axContext ctx;                                  \
    Display* display = XOpenDisplay(NULL);          \
    Window   parent  = XDefaultRootWindow(display); \
    void*    audio   = NULL;                        \
    ctx.mDisplay = display;                         \
    ctx.mWindow  = parent;                          \
    ctx.mAudio   = audio;

  #define AX_CONTEXT_EXIT                           \
    XCloseDisplay(display);

#endif

#define AX_ENTRYPOINT(classname)            \
int main(void)                              \
{                                           \
  AX_CONTEXT_INIT                           \
  axPluginImpl* plug = new classname(&ctx); \
  int ret = plug->main(&ctx);               \
  AX_CONTEXT_EXIT                           \
  delete plug;                              \
  return ret;                               \
}


//----------------------------------------------------------------------
#endif
