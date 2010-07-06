// some of the debug stuff doesn't work properly yet.
// might be something with global or platform-specific
// things that are handled a bit different now...

//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test.log"

#define AX_NOGUI

#include "base/axBase.h"
//#include "base/test_Window.h"

class myPlugin : public AX_FORMAT
{
  //private:
  //  axWindow* mWindow;
  public:

    myPlugin(axBase* aBase) : AX_FORMAT(aBase)
      {
        wtrace("- myPlugin.constructor");
      }

    //virtual void* entrypoint(void* ptr)
    //  {
    //    wtrace("> myPlugin.entrypoint");
    //    return AX_FORMAT::entrypoint(ptr);
    //  }

    //virtual axInstance* createInstance(void)
    //  {
    //    wtrace("> myPlugin.createInstance");
    //    axInstance*  instance  = new axInstance(mBase);
    //    axInterface* interface = mBase->getInterface();
    //    mWindow = (axWindow*)interface->createWindow(instance,NULL);
    //    return instance;
    //  }

};

//----------------------------------------------------------------------
//AX_MAIN(myPlugin)
AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,myPlugin)
