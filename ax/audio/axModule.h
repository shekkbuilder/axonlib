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
  - todo: signal pins, going in the 'opposite direction', for control flow, events, etc.. calls doSignal()

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

// pin direction
#define pd_Input  0
#define pd_Output 1

// pin type
#define pt_None   0   // const
#define pt_Signal 1   // static
#define pt_Audio  2   // dynamic

// signal types
#define st_Tick   0

// module flags
#define mf_None   0
#define mf_Active 1

//----------------------------------------------------------------------

/*
  a pin is essentially a ptr to a sample (and some helpers)
  input pins: points directly to the value from the destination module/pin
  output pins: points to the pin output value (internally in module)
  these are manipulated in connect(), and doCompile(), etc...
  to point as close as possibly to the 'real data'
*/

class axPin
{
  protected:
    SPL*  mPinPtr;
    int   mPinType;
    int   mPinDir;
  public:
    axPin() { mPinDir=pd_Output; mPinType=pt_Audio; }
    virtual void  setPtr(SPL* aPtr) { mPinPtr=aPtr; }
    virtual SPL*  getPtr(void) { return mPinPtr; }
    virtual int   getDir(void) { return mPinDir; }
    virtual int   getType(void) { return mPinType; }
};

typedef axArray<axPin*> axPins;

//----------------------------------------------------------------------

// a listener is a way for the module to communicate 'upwards',
// most probbaly a graph/macro, or our plugin
// (if we want to 'intercept' the messages)

class axModuleListener
{
  public:
    virtual void onSignal(int aType=0, PTR aValue=NULL) {}
};

//----------------------------------------------------------------------

/*
  the default setting is a do-nothing in everything, so its quite useless
  in a graph, but when you start filling in the virtual methods, it
  gets more and more functionality
*/

class axModule
{
  protected:
    axModuleListener* mListener;
    axString  mName;
    axPins    mPins;
    int       mFlags;

  public:

    axModule(axModuleListener* aListener, axString aName)
      {
        mListener = aListener;
        mName = aName;
        mFlags = mf_None;
      }

    virtual ~axModule()
      {
        deletePins();
      }

    // init

    inline void     appendPin(axPin* aPin)  { mPins.append(aPin); }
    inline void     deletePins(void)        { for (int i=0;i<mPins.size(); i++) delete mPins[i]; }
    inline bool     isActive(void)          { return (mFlags&mf_Active); }

    // access

    virtual axPin*  getPin(int aIndex)                { return mPins[aIndex]; }
    virtual SPL*    getPinPtr(int aIndex)             { return mPins[aIndex]->getPtr(); }
    virtual void    setPinPtr(int aIndex, SPL* aPtr)  { mPins[aIndex]->setPtr(aPtr); }

    // graph setup

    virtual int connectPin(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod) { return 0; }
    virtual int connectDirect(axPin* mDstPin, SPL* aSource) { return 0; }

    // runtime

    // send signal to module, aValue = ptr to 'something', depending on signal type.
    // events, execution flow opposite of audio.
    // for parameters, midi notes, etc,
    // ex: midi module can send note event to oscillator, gate event to envelope, etc..
    // also, for conditional execution
    // (ex: multiple signal outputs, one of them selected depending on some input)

    virtual void doSignal(int aType=0, PTR aValue=NULL) {}

    // process

    virtual void doProcess(SPL** aInputs, SPL** aOutputs) {}

    // prepares aExecList
    // cache pointers, check connected inputs
    // (todo: check pin rates, const, static dynamic)
    // setup runtime execution state/mode.

    virtual int doCompile(void) { return 0; }

    // compiled (prepared) execution.
    // use cached pointers, appropriate state/mode, etc..
    // optimized code-path
    //
    // via getPinPtr(), we can get quite close to the source,
    // so this kind of 'optimization' could be as effective as
    // the normal process() funtions with pointer to the samples

    virtual int doExecute(SPL** aInputs, SPL** aOutputs) { return 0; }

};

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
/*
  what if we require that the modules cache everything needed,
  so that the ptrs in the pins aren't used when calling the execlist?
  that could make editing the graph at runtime safer? or more predictable?

  modScript . public axModule
  opModule : public axOpcode



*/
//----------------------------------------------------------------------
#endif
