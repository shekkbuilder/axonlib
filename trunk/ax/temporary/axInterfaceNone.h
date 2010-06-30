#ifndef axInterfaceNone_included
#define axInterfaceNone_included
//----------------------------------------------------------------------
/*

empty/no-op Interface (for AX_NOGUI)

*/

class axInterface
{
  public:
    axInterface(axFormat* aFormat) { trace("axInterface: none" ); }
    virtual ~axInterface() {}

};

#define NULL_INTERFACE axInterface

//----------------------------------------------------------------------
#endif



