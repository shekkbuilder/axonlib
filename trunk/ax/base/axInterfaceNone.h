
class axInterfaceNone : public axInterface
{
  //private:
  //  axBase*   mBase;

  public:

    axInterfaceNone(axBase* aBase) : axInterface(aBase)
      {
        trace("- axInterfaceNone.constructor");
        //mBase = aBase;
      }

    virtual ~axInterfaceNone()
      {
        trace("- axInterfaceNone.destructor");
      }

    virtual void* createWindow(axInstance* aInstance,void* parent)
      {
        trace("- axInterfaceNone.createWindow");
        return NULL;
      }

};

//----------

typedef axInterfaceNone AX_INTERFACE;

//----------------------------------------------------------------------





































//#ifndef axInterfaceNone_included
//#define axInterfaceNone_included
////----------------------------------------------------------------------
///*
//
//empty/no-op Interface (for AX_NOGUI)
//
//*/
//
//class axInterface : public axInterfaceBase
//{
//  public:
//    axInterface(axPlatform* aPlatform)
//    : axInterfaceBase(aPlatform)
//      {
//        trace("axInterfaceNone,constructor" );
//      }
//    virtual ~axInterface() {}
//
//};
//
//#define NULL_INTERFACE axInterface
//
////----------------------------------------------------------------------
//#endif
//
//
//
