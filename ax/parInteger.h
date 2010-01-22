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

#ifndef parInteger_included
#define parInteger_included
//----------------------------------------------------------------------

#include "axParameter.h"

class parInteger : public axParameter
{
  public:
    int     mMin, mMax, mStep;
    char**  mStrings;
    float   mRange;
    float   mHalfStep;

  public:

    parInteger( axParameterListener* aListener, int aID, axString aName, axString aLabel="",
                int aValue=0, int aMin=0, int aMax=100, char** aStrings=NULL)
    : axParameter(aListener,aID,aName,aLabel)
      {
        setup(aValue,aMin,aMax,aStrings);
      }

    //virtual ~parFloat() {}

    //--------------------------------------------------

    void setup(int aVal, int aMin, int aMax, char** aStrings)
      {
        mMin      = aMin;
        mMax      = aMax;
        mRange    = mMax - mMin + 1;      // 4
        mHalfStep = (1/mRange)*0.5;
        mStrings  = aStrings;
        setInt(aVal);
      }

    //----------

    virtual void setInt(int aValue)
      {
        mValue = (float)(aValue-mMin) / mRange;
        mValue += mHalfStep;
      }

    //----------

    virtual int getInt(void)
      {
        //return mRange * (float)(mValue + mMin);
        float n = floor (mValue * mRange );
        return (mMin + (int)axMin(n,(mRange-1)));
      }

    //--------------------------------------------------

    virtual void  setValue(float aValue)  { setInt((int)aValue); }
    virtual float getValue(void)          { return (float)getInt(); }

    //--------------------------------------------------

    //virtual void  doReset(void)             { mValue=mDefault; if(mListener) mListener->onChange(this); }
    //virtual void  doSetValue(float aValue)  { mValue=axMax(0,axMin(1,aValue)); if(mListener) mListener->onChange(this); }
    //virtual float doGetValue(void)          { return mValue; }
    //virtual void  doGetName(char* buf)      { strcpy(buf,mName.ptr()); }
    //virtual void  doGetLabel(char* buf)     { strcpy(buf,mLabel.ptr()); }

    virtual void  doGetDisplay(char* buf)
      {
        int i = getInt();
        if( mStrings ) strcpy( buf, mStrings[i] );
        else sprintf( buf, "%i", (int)i );
      }

    //--------------------------------------------------

};

//----------------------------------------------------------------------
#endif
