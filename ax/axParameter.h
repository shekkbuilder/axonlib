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
  \file axParameter.h
  \brief vst-parameter handling
*/

#ifndef axParameter_included
#define axParameter_included
//----------------------------------------------------------------------

#include <stdio.h> // sprintf
#include "axString.h"
#include "axMath.h"

//TODO: ???
#define MAX_PARAM_TXTLEN 16

//----------------------------------------------------------------------

class axParameterBase
{
  public:
    /// reset value to default, and notify listener
    virtual void  doReset(void)             {}
    /// set value to aValue (0..1), and notify listener
    virtual void  doSetValue(float aValue)  {}
    /// get value (0..1)
    virtual float doGetValue(void)          {return 0;}
    /// get ptr to name text string ("lowpass", "distortion", etc)
    virtual void  doGetName(char* buf)      {}
    /// get ptr to label text string ("db","ms",..))
    virtual void  doGetLabel(char* buf)     {}
    /// get text representation of value (for gui, etc)
    virtual void  doGetDisplay(char* buf)   {}
};

//----------------------------------------------------------------------

class axParameter;
class axParameterListener
{
  public:
    /// value has changed
    virtual void onChange(axParameter* aParameter) {}
};

//----------------------------------------------------------------------

/**
  \brief parameter

  a parameter is a value that can be automated by the plugin host,
  and tweaked with a knob or other widget.
  the internal range of the value is always from 0 to 1.
  subclasses of axParameter (like parFloat and parInteger) add different ranges and intervals.
  the parameter has a name ("lowpass", "gain" ..), a label ("db", "%", "ms" ..),
  and a text description of the internal value ("-32", "off", "2:1" ..).

  when the parameter is changed (from host automation or knob-tweaking), the parameter's listener is notified
  (calling mListener->onChange(parameter)).

  add a parameter to a plugin by calling axPlugin::appendParameter

 */

class axParameter : public axParameterBase
{
  protected:
    axParameterListener* mListener;
    axString  mName;
    axString  mLabel;
    float     mValue;   // 0..1
    float     mDefault;
    int       mConIndex; // mCNum

  public:

    int       mID;
    void*     mUser;

    // aListener  listener
    // aID        id (for your own use)
    // aName      name ("lowpass",..)
    // aLabel     label ("hz,"db",..)

    //TODO: default value
    axParameter(axParameterListener* aListener, int aID, axString aName, axString aLabel="")
      {
        mListener = aListener;
        mID       = aID;
        mName     = aName;
        mLabel    = aLabel;
        mValue    = 0;
        mDefault  = mValue;
        mUser     = NULL;
        mConIndex = -1;
      }

    //----------

    inline void setListener(axParameterListener* l) { mListener = l; }
    inline axParameterListener* getListener(void) { return mListener; }
    inline void setConnectionIndex(int num) { mConIndex = num; }
    inline void setValueDirect(float v) { mValue = v; }

    inline int getConnectionIndex(void) { return mConIndex; }
    inline float getValueDirect(void) { return mValue; }

    //----------

    /// set value, with remapping/conversion
    virtual void  setValue(float aValue)    { mValue=aValue; }
    /// read value, with conversion/remapping
    virtual float getValue(void)            { return mValue; }

    // axParameterBase

    virtual void  doReset(void)             { mValue=mDefault; if(mListener) mListener->onChange(this); }
    virtual void  doSetValue(float aValue)  { mValue=aValue;   if(mListener) mListener->onChange(this); }
    virtual float doGetValue(void)          { return mValue; }
    virtual void  doGetName(char* buf)      { strcpy(buf,mName.ptr()); }
    virtual void  doGetLabel(char* buf)     { strcpy(buf,mLabel.ptr()); }
    virtual void  doGetDisplay(char* buf)   { sprintf(buf,"%f",mValue); }

};

typedef axArray<axParameter*> axParameters;

//----------------------------------------------------------------------
#endif


