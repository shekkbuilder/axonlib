#ifndef axFormatVst_included
#define axFormatVst_included
//----------------------------------------------------------------------

#include "axPlatform.h"

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

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
    AEffect mEffect;
  public:
    axInstance(axDescriptor* aDescriptor) { mDescriptor=aDescriptor; }
    virtual ~axInstance() { }
    inline axDescriptor* getDescriptor(void) { return mDescriptor; }
    virtual AEffect* getEffect(void) { return &mEffect; };
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
  //private:
  //  AEffect mAEffect;
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

#ifdef AX_LINUX

  AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
  #define main main_plugin

#endif //LINUX

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                         \
                                                                                        \
AEffect* main(audioMasterCallback audioMaster)                                          \
{                                                                                       \
  axFormat<_desc,_inst,_iface,_plat>* plug =  new axFormat<_desc,_inst,_iface,_plat>(); \
  _inst* instance = (_inst*)plug->getInstance();                                        \
  if (!instance) return 0;                                                              \
  AEffect* ae = instance->getEffect();                                                  \
  return ae;                                                                            \
}

// XInitThreads(); -> interfacelinux

//----------------------------------------------------------------------
#endif


