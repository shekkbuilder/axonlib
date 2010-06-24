#ifndef axInterfaceNone_included
#define axInterfaceNone_included
//----------------------------------------------------------------------
/*

empty/no-op Interface (for AX_NOGUI)

*/

class axInterface
{
  public:
    axInterface(axInstance* aInstance) {}
    virtual ~axInterface() {}
    //axWindow* createWindow(int w, int h) { return NULL; }
};

// simplifies AX_MAIN/AX_ENTRYPOINT
// use NULL_EDITOR instead of axEditor to clarify that there is
// no editor

#define NULL_INTERFACE axInterface

//----------------------------------------------------------------------
#endif



