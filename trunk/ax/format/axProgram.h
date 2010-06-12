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

#ifndef axPrograms_included
#define axPrograms_included
//----------------------------------------------------------------------

class axProgram
{
  private:
    int       mNumVal;
    //axString  mName;
    char      mName[33];
    float*    mValues;
    //bool      mDidAlloc;

  public:

    axProgram(axString aName, int aNumVal, float* aValues)
      {
        //mName   = aName;//"default";
        axStrncpy(mName,aName.ptr(),32);
        mNumVal = aNumVal;//0;
        //mValues = aValues;//NULL;
        mValues = (float*)axMalloc( sizeof(aNumVal*sizeof(float*)) );
        axMemcpy(mValues,aValues,aNumVal*sizeof(float));
        //mDidAlloc = true;
      }

    axProgram(axString aName, int aNumVal)
      {
        //mName   = aName;//"default";
        axStrncpy(mName,aName.ptr(),32);
        mNumVal = aNumVal;//0;
        mValues = (float*)axMalloc( sizeof(aNumVal*sizeof(float*)) );//NULL;
        //mDidAlloc = true;
      }

    ~axProgram()
      {
        if (/*mDidAlloc &&*/ mValues) axFree(mValues);
      }

    void      setName(axString aName)             { axStrncpy(mName,aName.ptr(),32);/*mName=aName;*/ }
    axString  getName(void)                       { return mName; }
    void      setValue(int aIndex, float aValue)  { mValues[aIndex]=aValue; }
    float     getValue(int aIndex)                { return mValues[aIndex]; }

};

typedef axArray<axProgram*> axPrograms;

//----------------------------------------------------------------------
#endif
