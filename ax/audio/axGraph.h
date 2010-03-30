#ifndef axGraph_included
#define axGraph_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "audio/axModule.h"

class axGraph : public axModule
{
  protected:
    axModules mModules;
    axModules mExecList;
    bool      mCompiled;
  public:

    axGraph(axString aName)
    : axModule(aName)
      {
        mCompiled = false;
      }

    virtual ~axGraph()
      {
        deleteModules();
      }

    //----------

    virtual void appendModule(axModule* aModule) { mModules.append(aModule); }
    virtual void deleteModules(void) { for (int i=0; i<mModules.size(); i++) delete mModules[i]; }

    //----------

    // axModule[Base]

    virtual void doProcess(SPL** aInputs, SPL** aOutputs)
      {
        if (mCompiled) { for (int i=0; i<mExecList.size(); i++) mExecList[i]->doProcess(aInputs,aOutputs); }
        else { for (int i=0; i<mModules.size(); i++) mModules[i]->doProcess(aInputs,aOutputs); }
      }

};

//----------------------------------------------------------------------
#endif
