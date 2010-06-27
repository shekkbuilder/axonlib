#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------
/*
  variations (flags, see axInterface.h)
  - embedded/windowed
  - buffered/unbuffered
  - alpha/noalpha
*/

class axInterface
{
  public:
    axInterface(axFormat* aFormat)
      {
        printf("axInterface linux\n");
      }
    virtual ~axInterface() {}

    int       getScreenWidth(void) { return 0; }
    int       getScreenHeight(void) { return 0; }
    int       getScreenDepth(void) { return 0; }

    /* axWindow* */ void* createWindow(int aWidth, int aHeight) { return NULL; }
    /* axWindow* */ void* createEmbeddedWindow(int aParent, int aWidth, int aHeight) { return NULL; }
};

//----------------------------------------------------------------------
#endif


