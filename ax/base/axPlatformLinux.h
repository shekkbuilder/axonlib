#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase) { /*trace("axPlatformLinux.constructor");*/ }
    virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
    virtual char* getPlatformName(void) { return (char*)"linux"; }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------
//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif
