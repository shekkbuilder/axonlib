#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase)
      {
        trace("axPlatformLinux.constructor");
      }
    virtual ~axPlatformLinux()
      {
        trace("axPlatformLinux.destructor");
      }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------
#endif
