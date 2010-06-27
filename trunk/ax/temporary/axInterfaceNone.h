#ifndef axInterfaceNone_included
#define axInterfaceNone_included
//----------------------------------------------------------------------
/*

empty/no-op Interface (for AX_NOGUI)

*/

class axInterface
{
  public:
    axInterface(axFormat* aFormat) { printf("axInterface: none\n" ); }
    virtual ~axInterface() {}

    int       getScreenWidth(void) { return 0; }
    int       getScreenHeight(void) { return 0; }
    int       getScreenDepth(void) { return 0; }

    /* axWindow* */ void* createWindow(int aWidth, int aHeight) { return NULL; }
    /* axWindow* */ void* createEmbeddedWindow(int aWidth, int aHeight) { return NULL; }

};

#define NULL_INTERFACE axInterface

//----------------------------------------------------------------------
#endif



