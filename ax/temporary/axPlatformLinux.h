#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

#ifndef AX_NOGUI
  //#include "Xlib.h"
#endif


class axPlatform
{
  private:
    int mFormat;
  public:
    axPlatform(int aFormat)
      {
        mFormat = aFormat;
        #ifndef AX_NOGUI
          //XInitThreads();
        #endif
      }
    virtual ~axPlatform()
      {
      }

    inline int  getOS(void)     { return pf_Linux; }
    inline int  getFormat(void) { return mFormat; }
};

//----------------------------------------------------------------------
#endif

