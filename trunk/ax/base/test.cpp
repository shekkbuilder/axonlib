//#define AX_NOGUI

#include "base/axBase.h"
//#include "base/test_Window.h"

class myPlugin : public AX_FORMAT
{
  //private:
  //  axWindow* mWindow;
  public:

    myPlugin(axBase* aBase) : AX_FORMAT(aBase)
      {
        trace("- myPlugin.constructor");
      }

    //virtual void* entrypoint(void* ptr)
    //  {
    //    trace("> myPlugin.entrypoint");
    //    return AX_FORMAT::entrypoint(ptr);
    //  }

    //virtual axInstance* createInstance(void)
    //  {
    //    trace("> myPlugin.createInstance");
    //    axInstance*  instance  = new axInstance(mBase);
    //    axInterface* interface = mBase->getInterface();
    //    mWindow = (axWindow*)interface->createWindow(instance,NULL);
    //    return instance;
    //  }

};

//----------------------------------------------------------------------
AX_MAIN(myPlugin)
