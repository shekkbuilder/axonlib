/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
 * \file axGraph.h
 * \brief graph
*/

#ifndef axGraph_included
#define axGraph_included
//----------------------------------------------------------------------

#include "axModule.h"

class axGraph : public axModule
{
  private:
    axModule *mOutMod;
    axPin *mIn0, *mIn1;
    axPin *mOut0, *mOut1;
  protected:
    axModules mModules;
    axModules mExecList;
  public:

    axGraph(axModuleListener* aOwner, axString aName)
    : axModule(aOwner, aName)
      {
        mOutMod = NULL;
        appendInput(  mIn0  = new axPin(this,"in0", mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1", mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out0",mpt_Data) );
        appendOutput( mOut1 = new axPin(this,"out1",mpt_Data) );
      }

    virtual ~axGraph()
      {
        deleteModules();
      }

    // append modules in the order they should be processed
    virtual void appendModule(axModule* aModule)
      {
        mModules.append(aModule);
      }

    virtual void deleteModules(void)
      {
        for (int i=0; i<mModules.size(); i++) delete mModules[i];
      }

    //virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0/*, void* aPtr*/)
    //  {
    //  }

    virtual void doProcess(void)
      {
        for (int i=0; i<mModules.size(); i++) mModules[i]->doProcess();
      }

    virtual void doCompile(void)
      {
        mExecList.clear();
        for (int i=0; i<mModules.size(); i++)
        {
          axModule* mo = mModules[i];
          mo->doCompile();
          if (mo->getMode()==mmo_Dynamic) mExecList.append(mo);
        }
      }

    virtual void doExecute(void)
      {
        for (int i=0; i<mExecList.size(); i++) mExecList[i]->doProcess();
      }

};

//----------------------------------------------------------------------
#endif
