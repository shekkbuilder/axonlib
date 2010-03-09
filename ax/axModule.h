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
  \file axModule.h
  \brief module class
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

// mod pin resolution
#define mpr_Const   0
#define mpr_Static  1
#define mpr_Dynamic 2

class axModule;

//----------

class axPin
{
  protected:
    axModule* mOwner;
    axString  mName;
    int       mType;    // 0=data, 1=signal, 2=multi
    int       mRes;     // 0=const, 1=static, 2=dynamic
    float     mValue;
    float*    mPtr;
  public:
    axPin(axModule* aOwner, axString aName, int aType, int aRes=mpr_Dynamic, float aValue=0, float* aPtr=NULL)
      {
        mOwner  = aOwner;
        mName   = aName;
        mType   = aType;
        mRes    = aRes;
        mValue  = aValue;
        mPtr    = aPtr;
      }

    inline axModule* getOwner(void)         { return mOwner; }
    inline axString  getName(void)          { return mName; }
    inline  int      getType(void)          { return mType; }
    inline int       getRes(void)           { return mRes; }

    inline float     getValue(void)         { return mValue; }
    inline float*    getValueAdr(void)      { return &mValue; }
    inline float*    getPtr(void)           { return mPtr; }
    inline float**   getPtrAdr(void)        { return &mPtr; }

    inline void      setPtr(float* aPtr)    { mPtr=aPtr; }
    inline void      setValue(float aValue) { mValue=aValue; } // duplicate: set/write

    inline float     readValue(void)        { return *mPtr; }
    inline void      writeValue(float aValue) { mValue=aValue; }

};

//----------

typedef axArray<axPin*> axPins;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// module signal types
#define mst_gate    1
#define mst_num     2
#define mst_value   3
#define mst_noteon  4
#define mst_noteoff 5
#define mst_ctrl    6

//----------

class axModuleBase
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
            check which inputs is connected, ..
            so that the module network is independent of the graph
onExecute - exec compiled network

*/

//------------------------------

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

// module Modes
#define mmo_Const   0
#define mmo_Static  1
#define mmo_Dynamic 2

class axModule : public axModuleBase
{
  protected:
    axModuleListener* mListener;
    int       mMode;
    axString  mName;
    axPins    mInputs;
    axPins    mOutputs;
    //bool      mIsDynamic;
    //bool      mIsStatic;

  public:

    axModule(axModuleListener* aListener, axString aName)
      {
        mListener = aListener;
        mMode = mmo_Const;
        mName = aName;
        mInputs.clear();
        mOutputs.clear();
      }

    //----------

    virtual ~axModule()
      {
        deletePins();
      }

    //----------------------------------------

    inline axModuleListener* getListener(void)  { return mListener; }
    inline  int     getMode(void)               { return mMode; }
    inline axString getName(void)               { return mName; }
    inline axPin*   getInPin(int aIndex)        { return mInputs[aIndex]; }
    inline axPin*   getOutPin(int aIndex)       { return mOutputs[aIndex]; }

    inline float    getInput(int aIndex)        { return mInputs[aIndex]->readValue(); }
    inline float    getOutput(int aIndex)       { return mOutputs[aIndex]->getValue(); }

    void appendInput(axPin* pin)  { mInputs.append(pin); }
    void appendOutput(axPin* pin) { mOutputs.append(pin); }
    void deletePins(void)
      {
        for( int i=0; i<mInputs.size(); i++ ) delete mInputs[i];
        for( int i=0; i<mOutputs.size(); i++ ) delete mOutputs[i];
        mInputs.clear();
        mOutputs.clear();
      }

    //----------------------------------------

    virtual void connectDataInput( int aInput, axModule* aSource, int aOutput)
      {
        axPin* src = aSource->getOutPin(aOutput);
        axPin* dst = mInputs[aInput];
        if ( dst->getType()==mpt_Data && src->getType()==mpt_Data )
        {
          float* ptr = src->getValueAdr();
          dst->setPtr(ptr);
        }
      }

    //----------

    virtual void connectDataInput( int aInput, float* aSource)
      {
        axPin* dst = mInputs[aInput];
        if (dst->getType()==mpt_Data)
        {
          dst->setPtr(aSource);
        }
      }

    //virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0/*, void* aPtr*/) {}
    //virtual void doProcess(void) {}
    //virtual void doCompile(void) {}
    //virtual void doExecute(void) {}

};

//----------------------------------------------------------------------

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
#endif
