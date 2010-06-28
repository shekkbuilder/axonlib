#ifndef axPlatformWin32_included
#define axPlatformWin32_included
//----------------------------------------------------------------------

class axPlatform
{
  public:
    axPlatform(axFormat* aFormat)
      {
        printf("axPlatform win32\n");
        // hmodule, hinstance
      }
    virtual ~axPlatform() { }
};

//----------------------------------------------------------------------
#endif


