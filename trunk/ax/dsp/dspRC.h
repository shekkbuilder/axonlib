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
  \file dspRC.h
  \brief RC filter
*/

#ifndef dspRC_included
#define dspRC_included
//----------------------------------------------------------------------

class dspRC
{
  private:
    float mValue;
    float mTarget;
    float mWeight;
  public:
    dspRC() { mValue=0; mTarget=0; mWeight=0; }
    void setup(float aValue=0, float aTarget=0, float aWeight=0) { mValue=aValue; mTarget=aTarget; mWeight=aWeight; }
    inline void setValue(float aValue) { mValue = aValue; }
    inline void setTarget(float aTarget) { mTarget = aTarget; }
    inline void setWeight( float aWeight) { mWeight = aWeight; }
    inline float getValue(void) { return mValue; }
    inline float getTarget(void) { return mTarget; }
    inline float getWeight(void) { return mWeight; }
    virtual float process(void)
    {
      mValue += (mTarget-mValue) * mWeight;
      return mValue;
    }
    virtual float process(float in)
    {
      mValue += (in-mValue) * mWeight;
      return mValue;
    }
};

//----------------------------------------------------------------------
#endif
