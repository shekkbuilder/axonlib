#define AX_NOGUI
//#define AX_WIN32
#define AX_LINUX

#include <stdio.h> // printf
#include "core/axDefines.h"
#include "temporary/axFormat.h"

//----------

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axFormat* aFormat) : axDescriptor(aFormat) { printf("myDescriptor\n"); }
    virtual char*         getName(void)             { return (char*)"test_template"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
};

//----------

class myInstance : public axInstance
{
  public:
    myInstance(axFormat* aFormat) : axInstance(aFormat) { printf("myInstance\n"); }
    int main(int argc,char** argv) { printf("myInstance.main\n"); return 0; }
};

//----------

//AX_MAIN(DEFAULT_DESCRIPTOR,myInstance)
//AX_ENTRYPOINT(myDescriptor,myInstance,axInterface,axPlatform)

AX_ENTRYPOINT( myDescriptor,
               myInstance,
               DEFAULT_INTERFACE,
               DEFAULT_PLATFORM )
