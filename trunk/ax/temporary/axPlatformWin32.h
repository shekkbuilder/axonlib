#ifndef axPlatformWin32_included
#define axPlatformWin32_included
//----------------------------------------------------------------------

class axPlatform
{
  public:
    axPlatform(axFormat* aFormat)
      {
        printf("axPlatform win32\n");
        #ifndef AX_NOGUI
          //register windows class
        #endif
      }
    virtual ~axPlatform() { }
};

//----------------------------------------------------------------------
#endif


