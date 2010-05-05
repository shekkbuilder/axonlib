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
  \file parFloat.h
  \brief parameter with floating point values
*/

#ifndef parFloat_included
#define parFloat_included
//----------------------------------------------------------------------

#include "axParameter.h"

class parFloat : public axParameter
{
  protected:
    float mRange;
    float mNumSteps;
    float mStepSize;
    float mHalfStep;
  public:
    float   mMin, mMax, mStep;

  public:

    parFloat( axParameterListener* aListener, axString aName, axString aLabel="",
              float aValue=0, float aMin=0, float aMax=1, float aStep=0)
    : axParameter(aListener,aName,aLabel)
      {
        setup(aValue,aMin,aMax,aStep);
      }

    //virtual ~parFloat() {}

    //--------------------------------------------------

    void setup(float aVal, float aMin, float aMax, float aStep)
      {
        mMin      = aMin;                 // 2
        mMax      = aMax;                 // 4
        mStep     = aStep;                // 0.5
        mRange    = mMax-mMin;            // 4-2 = 2
        if( mStep > 0 )
        {
          mNumSteps = 1 + (mRange/mStep); // 1+(2/0.5)  = 1+4   = 5
          mStepSize = 1 / (mNumSteps-1);  // 1/(5-1)    = 1/4   = 0.25
          mHalfStep = 0.5 * mStepSize;    // 0.5 * 0.25 = 0.125
        }
        else
        {
          mNumSteps = 1;
          mStepSize = 1;
          mHalfStep = 0;
        }
        setValue(aVal);
      }

    //----------

//    int getStep(void)
//    {
//      float n = mValue * mNumSteps;
//      return axMin(n,(mNumSteps-1));
//    }

    //--------------------------------------------------

    virtual void  setValue(float aValue)
      {
        mValue = ( (aValue-mMin) / mRange);
        //mValue += mHalfStep;
      }

    //----------

    virtual float getValue(void)
      {
        if( mStep>0 )
        {
          float n = mValue * mNumSteps;
          int st = (int)axMin(n,(mNumSteps-1));
          return mMin + (st*mStep);
        }
        else
        {
          return mMin + (mValue*mRange);
        }
      }

    //--------------------------------------------------

//    virtual void  doReset(void)             { mValue=mDefault; if(mListener) mListener->onChange(this); }
//    virtual void  doSetValue(float aValue)  { mValue=aValue;   if(mListener) mListener->onChange(this); }
//    virtual float doGetValue(void)          { return mValue; }
//    virtual void  doGetName(char* buf)      { strcpy(buf,mName.ptr()); }
//    virtual void  doGetLabel(char* buf)     { strcpy(buf,mLabel.ptr()); }

    virtual void  doGetDisplay(char* buf)
      {
        //sprintf(buf,"%f",getValue());
        __builtin_sprintf(buf,"%.2f",getValue());
      }

    //--------------------------------------------------

};

//----------------------------------------------------------------------
#endif
