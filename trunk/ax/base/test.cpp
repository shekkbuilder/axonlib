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

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { trace("myDescriptor.constructor"); }
    virtual ~myDescriptor() { trace("myDescriptor.destructor"); }
};

//----------

class myInstance : public AX_INSTANCE
{
  public:
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        trace("myInstance.constructor");
        char temp[256];
        trace( axGetBasePath(temp) );
      }
    virtual ~myInstance() { trace("myInstance.destructor"); }
};

//----------------------------------------------------------------------

//AX_MAIN(myPlugin)

AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
