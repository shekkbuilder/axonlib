#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

#ifndef AX_NOGUI
  //#include "Xlib.h"
#endif

class axPlatform : public axPlatformBase
{
  public:
    axPlatform(axFormat* aFormat)
    : axPlatformBase(aFormat)
      {
      }
    virtual ~axPlatform()
      {
      }

};

//----------------------------------------------------------------------
#endif

