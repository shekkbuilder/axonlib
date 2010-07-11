#ifndef axInterfaceNone_included
#define axInterfaceNone_included
//----------------------------------------------------------------------

#include "base/axBase.h"

class axInterfaceNone : public axInterface
{
  //private:
  //  axBase*   mBase;

  public:

    axInterfaceNone(axBase* aBase) : axInterface(aBase)
      {
        //trace("axInterfaceNone.constructor");
        //mBase = aBase;
      }

    virtual ~axInterfaceNone()
      {
        //trace("axInterfaceNone.destructor");
      }

    virtual void* createEditor(axInstance* aInstance,void* parent)
      {
        //trace("axInterfaceNone.createWindow");
        return NULL;
      }

};

//----------

typedef axInterfaceNone AX_INTERFACE;

//----------------------------------------------------------------------
#endif
