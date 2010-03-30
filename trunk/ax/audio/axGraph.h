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
  public:

    axGraph(axString aName)
    : axModule(aName)
      {
      }

    virtual ~axGraph()
      {
        deleteModules();
      }

    //----------

    virtual void appendModule(axModule* aModule) { mModules.append(aModule); }
    virtual void deleteModules(void) { for (int i=0; i<mModules.size(); i++) delete mModules[i]; }

    //----------

    // axModuleBase

    virtual void process(SPL** aInputs, SPL** aOutputs)
      {
        for (int i=0; i<mModules.size(); i++)
        {
          mModules[i]->process(aInputs,aOutputs);
        }
      }

};

//----------------------------------------------------------------------
#endif
