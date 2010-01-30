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

/// parameter base class
/**
  the parameter base class. defines which methods are available in all parameters.
  in this base class they are all empty and virtual. subclasses will override
  these to do their functionality
*/

class axParameterBase
{
  public:

    /// reset value
    virtual void  doReset(void) {}

    /// set value
    virtual void  doSetValue(float aValue) {}

    /// get value
    virtual float doGetValue(void) {return 0;}

    /// get name
    virtual void  doGetName(char* buf) {}

    /// get label
    virtual void  doGetLabel(char* buf) {}

    /// get display
    virtual void  doGetDisplay(char* buf) {}

};

//----------------------------------------------------------------------

class axParameter;
class axParameterListener
{
  public:

    /// value has changed
    /**
      called when the value of aParameter has changed.
      \param aParameter the parameter that has changed
    */
    virtual void onChange(axParameter* aParameter) {}
};

//----------------------------------------------------------------------

/**
  \brief parameter

  a parameter is a value that can be automated (or otherwise controlled) by the plugin host.
  it also has a name ("lowpass"), a label ("hz"),
  and a method for getting a text description of the current value ("251.5").
  the value is internally a floating point number, ranging from 0 to 1,
  but subclasses (parFloat/parInteger) add functionality for different steps/ranges,
  and methods for setting and getting the value in the 'external' format.

 */

class axParameter : public axParameterBase
{
  friend class axEditor;
  private:
    int mConnection;

  protected:
    axParameterListener* mListener;
    axString  mName;
    axString  mLabel;
    float     mValue;   // 0..1
    float     mDefault;

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
        mListener   = aListener;
        mID         = aID;
        mName       = aName;
        mLabel      = aLabel;
        mValue      = 0;
        mDefault    = mValue;
        mUser       = NULL;
        mConnection = -1;
      }

    //--------------------------------------------------
    // inline
    //--------------------------------------------------

    inline void setListener(axParameterListener* l)
      {
        mListener = l;
      }

    inline axParameterListener* getListener(void)
      {
        return mListener;
      }

    //TODO: do we need the two following? (they were introduced as a 'make-it-compile' hack)

    inline void setValueDirect(float v)
      {
        mValue = v;
      }

    inline float getValueDirect(void)
      {
        return mValue;
      }

    //----------

    /// set value, external format
    /**
      this will set the parameter value, using its 'external' format. it will be converted back into the 0..1.
      the listener will _NOT_ be notified about the change.
      \param aValue new parameter value. range 0..1
    */
    virtual void setValue(float aValue)
      {
        mValue=aValue;
      }

    /// get value, external format
    /**
      this will return the value, converted from 0..1 to to its external format.
      \return parameter value. converted to its proper range/format.
    */
    virtual float getValue(void)
      {
        return mValue;
      }

    // axParameterBase

    /// reset parameter to default
    /**
      reset the parameter to its default value (normally set in constructor).
      and notify the listener about the change (onChange)
    */
    virtual void doReset(void)
      {
        mValue=mDefault;
        if(mListener) mListener->onChange(this);
      }

    /// set value
    /**
      set new value, and notify listener (onChange)
      \param aValue new value, range 0..1
    */
    virtual void doSetValue(float aValue)
      {
        mValue=aValue;
        if(mListener) mListener->onChange(this);
      }

    /// get value
    /**
      returns current value
      \return current value, range 0..1
    */
    virtual float doGetValue(void)
      {
        return mValue;
      }

    /// get name
    /**
      copies the parameter name ("lowpass", "distortion", etc) to the provided buffer.
      note: that the buffer is handled by the caller. be careful with buffer overflows!
      \param buf buffer to write string to
    */
    virtual void doGetName(char* buf)
      {
        strcpy(buf,mName.ptr());
      }

    /// get label
    /**
      copies the label ("db","ms",..) to the provided buffer.
      \param buf buffer to write string to
      \note the buffer is handled by the caller.
    */
    virtual void doGetLabel(char* buf)
      {
        strcpy(buf,mLabel.ptr());
      }

    /// get display
    /**
      writes the value, as a text string, to the buffer. "123.5", "off", etc..
      \param buf buffer to write string to
      \note the buffer is handled by the caller.
    */
    virtual void doGetDisplay(char* buf)
      {
        sprintf(buf,"%f",mValue);
      }

};

typedef axArray<axParameter*> axParameters;

//----------------------------------------------------------------------
#endif


