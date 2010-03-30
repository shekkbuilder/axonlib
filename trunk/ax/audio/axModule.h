#ifndef axModule_included
#define axModule_included
//----------------------------------------------------------------------
/*

  - a graph has a list of modules, and is-a (via inheritance) a module itself
  - modules have two lists of pins, inputs and outputs
  - modules are connected to eachother via these pins
  - inputs pins have pointers directly to the source-pins data
  - output pins have pointers to internally generated data
  - graph.connect() connects two such pins, from output to input,
  - initially, the modules are processed in the order they are appended to the graph
  - compile() genertes a new module list, sorted by 'execution order'
  - execute() executes this list (or the initial,
    non-sorted list if compile() has not been called
  - a module can have additional functions, like the process() from 'the old' dspXXX classes
  - a plugin has-a, or is-a graph, so that we just insert modules into the graph, and connect them to make a plugin
    (can later be done visually with an editor that has "save as .cpp")
    (or we can have this editor in a plugin)
  - todo: script module
  - todo: signal pins, going in the 'opposite direction', for control flow, events, etc..

                   __________________           __________________
                  |_____      _____  |         |_____      _____  |
                  |     |    |     | |         |     |    |     | |
                  | in  |    | out |<------------ in |    | out | |
                  |_____|    |_____| |         |_____|    |_____| |
                  |__________________|         |_____             |
                                               |     |            |
                                               |  in |            |
                                               |_____|            |
                                               |__________________|

    (i'm obviously not a graphical artist! - ccernn)

*/
//----------------------------------------------------------------------

#include "axDefines.h"
#include "core/axArray.h"

//----------------------------------------------------------------------

// input pins:  mPtr points directly to the value from the destination module/pin
// output pins: mPtr points to the pin output value (internally in module)

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
    axModule(axString aName)  { mName=aName; }
    virtual ~axModule()       { deletePins(); }

    inline void appendPin(axPin* aPin) { mPins.append(aPin); }
    inline void deletePins(void) { for (int i=0;i<mPins.size(); i++) delete mPins[i]; }

    virtual axPin*  getPin(int aIndex) { return mPins[aIndex]; }
    virtual SPL*    getPinPtr(int aIndex) { return mPins[aIndex]->getPtr(); }
    virtual void    setPinPtr(int aIndex, SPL* aPtr) { mPins[aIndex]->setPtr(aPtr); }

    virtual int     connectPin(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod) { return 0; }
    virtual int     connectDirect(axPin* mDstPin, SPL* aSource) { return 0; }

    virtual void    doProcess(SPL** aInputs, SPL** aOutputs) {}
    virtual int     doCoompile(void) { return 0;}
    virtual int     doExecute(void) { return 0; }

    //virtual float process(float in) { return 0; }

};

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
#endif
