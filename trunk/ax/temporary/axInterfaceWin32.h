#ifndef axInterfaceWin32_included
#define axInterfaceWin32_included
//----------------------------------------------------------------------
/*
  variations:
  - embedded/windowed
  - buffered/unbuffered
  - alpha/noalpha
*/

class axInterface
{
  public:
    axInterface(axInstance* aInstance) {}
    virtual ~axInterface() {}
    int       getScreenWidth(void) { return 0; }
    int       getScreenHeight(void) { return 0; }
    int       getScreenDepth(void) { return 0; }
    //axWindow* createWindow(int aWidth, int aHeight) { return NULL; }
    //axWindow* createEmbeddedWindow(int aParent, int aWidth, int aHeight) { return NULL; }
};

//----------------------------------------------------------------------
#endif


