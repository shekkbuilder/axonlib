#ifndef axModule_included
#define axModule_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "core/axArray.h"

//----------------------------------------------------------------------

class axPin
{
  protected:
    SPL*  mPtr;
  public:
    virtual SPL*  getPtr(void) { return mPtr; }
    virtual void  setPtr(SPL* aPtr) { mPtr=aPtr; }
};

typedef axArray<axPin*> axPins;

//----------------------------------------------------------------------

class axModule
{
  protected:
    axString  mName;
    axPins    mPins;
  //protected:
  //    void appendPin(axPin* aPin) { mPins.append(aPin); }
  //    void deletePins(void) { for (int i=0;i<mPins.size(); i++) delete mPins[i]; }
  public:
    axModule(axString aName) { mName=aName; }
    virtual ~axModule() { deletePins(); }

    void appendPin(axPin* aPin) { mPins.append(aPin); }
    void deletePins(void) { for (int i=0;i<mPins.size(); i++) delete mPins[i]; }

    virtual axPin*  getPin(int aIndex) { return mPins[aIndex]; }
    virtual SPL*    getPinPtr(int aIndex) { return mPins[aIndex]->getPtr(); }
    virtual void    setPinPtr(int aIndex, SPL* aPtr) { mPins[aIndex]->setPtr(aPtr); }

    virtual int     connect(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod) { return 0; }
    virtual void    process(SPL** aInputs, SPL** aOutputs) { *aOutputs[0]=*aInputs[0]; *aOutputs[1]=*aInputs[1]; }
    virtual int     compile(void) { return 0;}
    virtual int     execute(void) { return 0; }

};

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
#endif
