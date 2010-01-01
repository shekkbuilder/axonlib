#ifndef axModule_included
#define axModule_included
//----------------------------------------------------------------------

#include "axArray.h"
#include "axString.h"

//----------

// mod pin type
#define mpt_Data    0
#define mpt_Signal  1
// mod pin dir
#define mpd_Input   0
#define mpd_Output  1
// mod pin resolution
#define mpr_Const   0
#define mpr_Static  1
#define mpr_Dynamic 2

//----------------------------------------------------------------------
//
// axPin
//
//----------------------------------------------------------------------

class axModule;

class axPin
{
  public:
    axModule* mOwner;
    axString  mName;
    int       mType;    // 0=data, 1=signal, 2=multi
    int       mDir;     // 0=input, 1=output
    int       mRes;     // 0=const, 1=static, 2=dynamic
  public:
    float*    mPtr;     // if input
    float     mValue;   // if output
  public:
    axPin(axModule* aOwner, axString aName, int aType, int aDir, int aRes=mpr_Dynamic, float* aPtr=NULL, float aValue=0)
      {
        mOwner  = aOwner;
        mName   = aName;
        mType   = aType;
        mDir    = aDir;
        mRes    = aRes;
        mPtr    = aPtr;
        mValue  = aValue;
      }

};

//----------

typedef axArray<axPin*> axPins;

//----------------------------------------------------------------------

// signal types
#define sty_noop  0
#define sty_reset 1
#define sty_exec  2
#define sty_sync  3
#define sty_val   4

//----------

class axModuleHandler
{
  public:
    virtual void doSignal(int aType, int aNum, float aVal, void* aPtr) {}
    virtual void doProcess(void) {}
    virtual void doCompile(void) {}
    virtual void doExecute(void) {}
    virtual void doNoteOn(int aNote, float aVelocity) {}
    virtual void doNoteOff(int aNote/*, float aVelocity*/) {}
    virtual void doControl(int aIndex, float aValue) {}
};

/*

onProcess - 'normal', non-compiled execution
onSignal  -
onCompile - cache pointers, everything you need into this module,
            so that the module network is independent of the graph
onExecute - exec compiled network

*/

//----------

class axModuleListener
{
  //virtual void onChangePins(void) {}      // number of pins (in/out) changed
  //virtual void onChangeParams(void) {}    // number of parameters changed
};

//----------------------------------------------------------------------
//
// axModule
//
//----------------------------------------------------------------------

// module flags
#define mfl_None    0
#define mfl_Hover   1

class axModule : public axModuleHandler
{
  public:
    axModuleListener* mOwner;
    int       mFlags;
    axString  mName;
    axPins    mPins;
  public:

    axModule(axModuleListener* aOwner, axString aName)
      {
        mOwner = aOwner;
        mFlags = mfl_None;
        mName = aName;
        mPins.clear();
      }

    //----------

    virtual ~axModule()
      {
        deletePins();
      }

    //----------------------------------------

    void deletePins(void)
      {
        for( int i=0; i<mPins.size(); i++ ) delete mPins[i];
      }

    //----------

    void connectDirect(int aDstPin, float* aPtr)
      {
        //TODO: check pintype, etc, if unconnected, etc.
        mPins[aDstPin]->mPtr = aPtr;
      }

    //----------

    bool connectPin( axModule* aSrcMod,  int aSrcPin, int aDstPin  )
      {
        //TODO: already connected? (pins/connection)
        axPin* src = aSrcMod->mPins[aSrcPin];
        axPin* dst = mPins[aDstPin];
        if (src->mDir  != mpd_Output) return false;
        if (dst->mDir  != mpd_Input)  return false;
        if (dst->mType != src->mType) return false;
        dst->mPtr = &src->mValue;
        return true;
      }

    //----------------------------------------
    // module handler
    //----------------------------------------

    //virtual void process(void) {}
    //virtual void signal(int aType, int aNum, float aVal, void* aPtr) {}

    //----------------------------------------

};

//----------------------------------------------------------------------

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
#endif
