//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test.log"

//#define AX_NOGUI
#include "base/axBase.h"

//----------------------------------------------------------------------

//class myPlugin : public AX_FORMAT
//{
//  public:
//    myPlugin(axBase* aBase) : AX_FORMAT(aBase)
//      {
//        trace("- myPlugin.constructor");
//      }
//};

//----------------------------------------------------------------------

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axBase* aBase) : axDescriptor(aBase) { trace("- myDescriptor.constructor"); }
};

class myInstance : public axInstance
{
  public:
    myInstance(axBase* aBase) : axInstance(aBase) { trace("- myInstance.constructor"); }
};

//----------------------------------------------------------------------

//AX_MAIN(myPlugin)

AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
