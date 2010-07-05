
class axPlatformLinux : public axPlatform
{
  public:
    axPlatformLinux(axBase* aBase) : axPlatform(aBase)
      {
        trace("- axPlatformLinux.constructor");
      }
};

//----------

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------



























//#ifndef axPlatformLinux_included
//#define axPlatformLinux_included
////----------------------------------------------------------------------
//
//#ifndef AX_NOGUI
//  //#include "Xlib.h"
//#endif
//
//class axPlatform : public axPlatformBase
//{
//  public:
//    axPlatform(axFormatBase* aFormat)
//    : axPlatformBase(aFormat)
//      {
//        trace("axPlatformLinux.constructor");
//      }
//    virtual ~axPlatform()
//      {
//      }
//
//};
//
////----------------------------------------------------------------------
//#endif
//
