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

class axModule;

//----------------------------------------------------------------------
//
// pin
//
//----------------------------------------------------------------------

// mod pin type
#define mpt_Data    0
#define mpt_Signal  1

// mod pin mode
#define mpm_Const   0
#define mpm_Dynamic 1 // audio-rate
#define mpm_Static  2 //

//----------

class axPin
{
  protected:
    axModule* mOwner;
    axString  mName;
    int       mType;          // 0=data, 1=signal, 2=multi
    int       mMode;          // 0=const, 1=static, 2=dynamic
    float     mDataValue;     // data output: put calculated output here
    float*    mDataPtr;       // data input: ptr directly to value
    axModule* mSignalModule;  // signal output: destination module
    int       mSignalIndex;   // signal output: destination signal pin number
  public:
    axPin(axModule* aOwner, axString aName, int aType, int aMode=mpm_Dynamic, float aValue=0, float* aPtr=NULL)
      {
        mOwner        = aOwner;
        mName         = aName;
        mType         = aType;
        mMode         = aMode;
        mDataValue    = aValue;
        mDataPtr      = aPtr;
        mSignalModule = NULL;
        mSignalIndex  = 0;
      }
    inline axModule* getOwner(void)           { return mOwner; }
    inline axString  getName(void)            { return mName; }
    inline  int      getType(void)            { return mType; }
    inline int       getMode(void)            { return mMode; }
    inline float     getValue(void)           { return mDataValue; }
    inline float*    getValueAdr(void)        { return &mDataValue; }
    inline float*    getPtr(void)             { return mDataPtr; }
    inline float**   getPtrAdr(void)          { return &mDataPtr; }
    inline void      setPtr(float* aPtr)      { mDataPtr=aPtr; }
  //inline void      setValue(float aValue)   { mDataValue=aValue; }
    inline float     readValue(void)          { return *mDataPtr; }
    inline void      writeValue(float aValue) { mDataValue=aValue; }

};

//----------

typedef axArray<axPin*> axPins;

//----------------------------------------------------------------------
//
// module base
//
//----------------------------------------------------------------------

// // module signal types
// #define mst_gate    1
// #define mst_num     2
// #define mst_value   3
// #define mst_noteon  4
// #define mst_noteoff 5
// #define mst_ctrl    6

//----------

class axModuleBase
{
  public:
    virtual void doProcess(void) {}       // 'normal', non-compiled execution
    virtual void doCompile(void) {}       // prepare/compile graph (prepare execlist)
    virtual void doExecute(void) {}       // execute compiled network (execlist)
    virtual void doSignal(int aIndex) {}  // send signal
};

//----------------------------------------------------------------------

class axModuleListener
{
  public:
    virtual void onChangePins(void) {}      // number of pins (in/out) changed
    virtual void onChangeParams(void) {}    // number of parameters changed
};

//----------------------------------------------------------------------
//
// module
//
//----------------------------------------------------------------------

// module Modes
#define mmo_Const   0   // const/static
#define mmo_Dynamic 1   // updated every sample (or 'tick')

//----------

class axModule : public axModuleBase
{
  protected:
    axModuleListener* mListener;
    axString          mName;
    int               mMode;
    axPins            mInputs;
    axPins            mOutputs;

  public:

    axModule(axModuleListener* aListener, axString aName/*, int aMode=mmo_Dynamic*/)
      {
        mListener = aListener;
        mName     = aName;
        mMode     = mmo_Dynamic;//aMode;
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

    virtual void connectDataInput( int aInput, float* aSource)
      {
        axPin* dst = mInputs[aInput];
        if (dst->getType()==mpt_Data)
        {
          dst->setPtr(aSource);
        }
      }

    //----------

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


    //----------------------------------------

    //virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0/*, void* aPtr*/) {}
    //virtual void doProcess(void) {}
    //virtual void doCompile(void) {}
    //virtual void doExecute(void) {}

};

//----------------------------------------------------------------------

typedef axArray<axModule*> axModules;

//----------------------------------------------------------------------
#endif
