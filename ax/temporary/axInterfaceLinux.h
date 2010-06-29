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
  private:
    //Display* mDisplay;
  public:
    axInterface(axFormat* aFormat)
      {
        printf("axInterface linux\n");
        //XInitThreads();
      }
    virtual ~axInterface() {}

    int       getScreenWidth(void)  { return 0; }
    int       getScreenHeight(void) { return 0; }
    int       getScreenDepth(void)  { return 0; }

    axWindow* createWindow(int aWidth, int aHeight, int aDepth, int aFlags) { return NULL; }
    //axSurface*  createSurface(int aWidth, int aHeight, int aDepth, int aFlags) { return NULL; }
    //axBitmap*   createBitmap(int aWidth, int aHeight, int aDepth, int aFlags) { return NULL; }
};

//----------------------------------------------------------------------
#endif


