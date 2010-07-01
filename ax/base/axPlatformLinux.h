#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

#ifndef AX_NOGUI
  //#include "Xlib.h"
#endif

class axPlatform
{
  public:
    axPlatform(axFormat* aFormat)
      {
        trace("axPlatform linux");
      }
    virtual ~axPlatform()
      {
      }

};

//----------------------------------------------------------------------
#endif

