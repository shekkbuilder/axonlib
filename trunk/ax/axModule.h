/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */
 

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
  protected:
  //public:
    axModule* mOwner;
    axString  mName;
    int       mType;    // 0=data, 1=signal, 2=multi
    int       mDir;     // 0=input, 1=output
    int       mRes;     // 0=const, 1=static, 2=dynamic
  public:
    union
    {
      float*    mPtr;     // if input
      float     mValue;   // if output
    };
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

#define sty_gate    1
#define sty_num     2
#define sty_value   3
#define sty_noteon  4
#define sty_noteoff 5
#define sty_ctrl    6

//----------

class axModuleHandler
{
  public:
    virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0/*, void* aPtr*/) {}
    virtual void doProcess(void) {}
    virtual void doCompile(void) {}
    virtual void doExecute(void) {}
    //virtual void doNoteOn(int aNote, float aVelocity) {}
    //virtual void doNoteOff(int aNote/*, float aVelocity*/) {}
    //virtual void doControl(int aIndex, float aValue) {}
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
  //public:
  protected:
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

    void appendPin( axPin* pin)
      {
        mPins.append(pin);
      }

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
