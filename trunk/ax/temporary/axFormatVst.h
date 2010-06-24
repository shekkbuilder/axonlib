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

#include "axInterface.h"

// Descriptor
// Instance
// Platform
// {Editor]

template<class _D, class _I, class _In, class _P>
class axFormat
{
  private:
    _P*   mPlatform;
    _D*   mDescriptor;
    _I*   mInstance;
    _In*  mInterface;
  public:
    axFormat()
      {
        mPlatform   = new _P(ft_Vst);
        mDescriptor = new _D(mPlatform);
        mInstance   = new _I(mDescriptor);
        mInterface  = new _In(mInstance);
      }
    virtual ~axFormat()
      {
        delete mInterface;
        delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }
  public:
    inline axPlatform*   getPlatform(void)    { return mPlatform; }
    inline axDescriptor* getDescriptor(void)  { return mDescriptor; }
    inline axInstance*   getInstance(void)    { return mInstance; }
    inline axInterface*  getInterface(void)   { return mInterface; }

    //----------

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,axEditor,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                         \
                                                                                        \
int main(int argc, char** argv)                                                         \
{                                                                                       \
  axFormat<_desc,_inst,_iface,_plat>* plug = new axFormat<_desc,_inst,_iface,_plat>();  \
  _inst* instance = (_inst*)plug->getInstance();                                      \
  return instance->getAEffect();                                                        \
}

//----------------------------------------------------------------------
#endif

