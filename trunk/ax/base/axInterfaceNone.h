#ifndef axInterfaceNone_included
#define axInterfaceNone_included
//----------------------------------------------------------------------
/*

empty/no-op Interface (for AX_NOGUI)

*/

class axInterface : public axInterfaceBase
{
  public:
    axInterface(axPlatform* aPlatform)
    : axInterfaceBase(aPlatform)
      {
        trace("axInterfaceNone" );
      }
    virtual ~axInterface() {}

};

#define NULL_INTERFACE axInterface

//----------------------------------------------------------------------
#endif



