#ifndef axParameter_included
#define axParameter_included
//----------------------------------------------------------------------

#include <stdio.h> // sprintf
#include "axString.h"
#include "axMath.h"

#define MAX_PARAM_TXTLEN 16

//----------------------------------------------------------------------

class axParameterHandler
{
  public:
    virtual void  doReset(void)             {}
    virtual void  doSetValue(float aValue)  {}
    virtual float doGetValue(void)          {return 0;}
    virtual void  doGetName(char* buf)      {}
    virtual void  doGetLabel(char* buf)     {}
    virtual void  doGetDisplay(char* buf)   {}
};

//----------------------------------------------------------------------

class axParameter;
class axParameterListener
{
  public:
    virtual void onChange(axParameter* aParameter) {}
};

//----------------------------------------------------------------------

class axParameter : public axParameterHandler
{
  //protected:
  public:
    axParameterListener* mListener;
    int       mID;
    axString  mName;
    axString  mLabel;
    float     mValue;   // 0..1
    float     mDefault;
    void*     mUser;
    int       mCNum;

  public:

    axParameter(axParameterListener* aListener, int aID, axString aName, axString aLabel="")
      {
        mListener    = aListener;
        mID       = aID;
        mName     = aName;
        mLabel    = aLabel;
        mValue    = 0;
        mDefault  = mValue;
        mUser     = NULL;
        mCNum     = -1;
      }

    virtual void  setValue(float aValue)    { mValue=aValue; }
    virtual float getValue(void)            { return mValue; }

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


