#ifndef axParameter_included
#define axParameter_included
//----------------------------------------------------------------------

#include <stdio.h> // sprintf (doGetDisplay)
#include "core/axString.h"

// kVstMaxParamStrLen     8
#define MAX_PARAM_TXTLEN  16

//----------------------------------------------------------------------

class axParameter;

class axParameterListener
{
  public:
    virtual void onChange(axParameter* aParameter) {}
};

//----------------------------------------------------------------------

// parameter flags
#define pf_Automate 1

#define AX_PAR_DEFAULT (pf_Automate)

//NOTE: plugin flags also has pf_ prefix

//----------------------------------------------------------------------

class axParameter// : public axParameterBase
{
  friend class axWindow;
  friend class axPlugin;

  private:
    int mConnection;      // which widget (if any) this is connected to
    int mIndex;           // index into the parameter list (axPluginBase)

  protected:
    axParameterListener* mListener;
    axString  mName;
    axString  mLabel;
    float     mValue;   // 0..1
    float     mDefault;
    int       mFlags;

  public:

    axParameter(axParameterListener* aListener, /*int aID,*/ axString aName, axString aLabel="")
      {
        mListener   = aListener;
        mName       = aName;
        mLabel      = aLabel;
        mValue      = 0;
        mDefault    = mValue;
        mConnection = -1;
        mFlags      = AX_PAR_DEFAULT;
        mIndex      = -1;
        //mID         = aID;
        //mUser       = NULL;
      }

    //--------------------------------------------------
    // inline
    //--------------------------------------------------

    inline int getFlags(void)                       { return mFlags; }
    inline void setListener(axParameterListener* l) { mListener = l; }
    inline axParameterListener* getListener(void)   { return mListener; }
    inline void setValueDirect(float v)             { mValue = v; }
    inline float getValueDirect(void)               { return mValue; }
    inline int  getConnection(void)                 { return mConnection; }
    inline void setConnection(int aNum)             { mConnection = aNum; }
    inline int  getIndex(void)                      { return mIndex; }
    inline void setIndex(int aNum)                  { mIndex = aNum; }

    inline axString getName(void) { return mName; }

    //----------

    virtual void setValue(float aValue) { mValue=aValue; }
    virtual float getValue(void)        { return mValue; }

    //----------

    virtual void doReset(void)
      {
        mValue = mDefault;
        if (mListener) mListener->onChange(this);
      }

    //----------

    virtual void doSetValue(float aValue)
      {
        mValue = aValue;
        if (mListener) mListener->onChange(this);
      }

    //----------

    virtual float doGetValue(void)
      {
        return mValue;
      }

    //----------

    virtual void doGetName(char* buf)
      {
        strcpy(buf,mName.ptr());
      }

    //----------

    virtual void doGetLabel(char* buf)
      {
        strcpy(buf,mLabel.ptr());
      }

    //----------

    virtual void doGetDisplay(char* buf)
      {
        float val = getValue();
        sprintf(buf,"%.3f",val);
      }

};

typedef axArray<axParameter*> axParameters;

//----------------------------------------------------------------------
#endif

