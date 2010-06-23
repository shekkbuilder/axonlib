#ifndef axFormatVst_included
#define axFormatVst_included
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
    //AEffect mEffect;
  public:
    axDescriptor(axPlatform* aPlatform) { mPlatform=aPlatform; }
    virtual ~axDescriptor() { }
    inline axPlatform* getPlatform(void) { return mPlatform; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstance
{
  private:
    axDescriptor* mDescriptor;
  public:
    axInstance(axDescriptor* aDescriptor) { mDescriptor=aDescriptor; }
    virtual ~axInstance() { }
    inline axDescriptor* getDescriptor(void) { return mDescriptor; }
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
        mPlatform   = new _P(ft_Vst);
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
//
// entrypoint
//
//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_plat)                                  \
                                                                          \
int main(int argc, char** argv)                                           \
{                                                                         \
  axFormat<_desc,_inst,_plat>* plug = new axFormat<_desc,_inst,_plat>();  \
  _inst* instance   = (_inst*)plug->getInstance();                        \
  return instance->getAEffect();                                          \
}

//----------------------------------------------------------------------
#endif

