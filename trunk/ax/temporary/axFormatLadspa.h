#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

#include "axPlatform.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptor
{
  private:
    axPlatform* mPlatform;
  public:
    axDescriptor(axPlatform* aPlatform) { }
    virtual ~axDescriptor() { }
    inline axPlatform* getPlatform() { return mPlatform; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstance
{
  public:
    axInstance(axDescriptor* aDescriptor) { }
    virtual ~axInstance() { }
    virtual int getAEffect(void) { return 0; };
};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

// Descriptor
// Instance
// Platform
// {Editor]

template<class _D, class _I, class _P>
class axFormat
{
  private:
    _P* mPlatform;
    _D* mDescriptor;
    _I* mInstance;

  public:

    axFormat()
      {
        mPlatform   = new _P(ft_Ladspa);
        mDescriptor = new _D(mPlatform);
        mInstance   = new _I(mDescriptor);
      }

    //----------

    virtual ~axFormat()
      {
        delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }

    //----------

    inline axPlatform*   getPlatform(void)    { return mPlatform; }
    inline axDescriptor* getDescriptor(void)  { return mDescriptor; }
    inline axInstance*   getInstance(void)    { return mInstance; }

    //----------

};

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_plat)                                  \
                                                                          \


//----------------------------------------------------------------------
#endif

