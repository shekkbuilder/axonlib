#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

#include <dlfcn.h>

class axPlatformLinux : public axPlatform
{
  private:
    char mPath[AX_MAX_PATH];

  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase)
      {
        /*trace("axPlatformLinux.constructor");*/
        mPath[0] = 0;
        Dl_info dli;
        dladdr(__func__, &dli);
        axStrncpy(mPath,dli.dli_fname,AX_MAX_PATH);
      }
    virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
    virtual char* getPlatformName(void) { return (char*)"linux"; }
    virtual char* getPath(void) { return mPath; }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif
