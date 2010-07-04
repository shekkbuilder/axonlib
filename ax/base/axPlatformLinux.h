#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

#ifndef AX_NOGUI
  //#include "Xlib.h"
#endif

class axPlatform : public axPlatformBase
{
  public:
    axPlatform(axFormatBase* aFormat)
    : axPlatformBase(aFormat)
      {
      }
    virtual ~axPlatform()
      {
      }

};

//----------------------------------------------------------------------
#endif

