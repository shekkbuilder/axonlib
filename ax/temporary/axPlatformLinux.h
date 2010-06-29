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
        printf("axPlatform linux\n");
      }
    virtual ~axPlatform()
      {
      }

};

//----------------------------------------------------------------------
#endif

