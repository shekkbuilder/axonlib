#ifndef axFormatVst_included
#define axFormatVst_included
//----------------------------------------------------------------------

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

#include "axPlatform.h"

//----------

class axDescriptor;
class axInstance;
class axInterface;

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
    /*virtual*/ ~axDescriptor() {}
    //inline axPlatform* getPlatform(void) { return mPlatform; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------
/*

- axDescriptor holds (const/shared) info abourt parameters, i/o, names, etc..
- axInterface class is used to create windows/editor

a plugin is a sub-class of this, so it needs the descriptor to prepare the parameters,
and the interface to create an eventual editor.

*/

class axInstance
{
  private:
    AEffect       mAEffect;
    axDescriptor* mDescriptor;
    axInterface*  mInterface;

  public:

    // we could have the interface as an argument too?
    axInstance(axDescriptor* aDescriptor)
      {
        mDescriptor = aDescriptor;
        mInterface  = NULL;
      }

    virtual ~axInstance()
      {
      }

    //----------------------------------------

    inline axDescriptor*  getDescriptor(void) { return mDescriptor; }
    inline axInterface*   getInterface(void)  { return mInterface; }
    inline AEffect*       getAEffect(void)    { return &mAEffect; };
    inline void           setInterface(axInterface* aInterface) { mInterface=aInterface; }

    //----------------------------------------
    // lib-user overrides these:
    //----------------------------------------

    // programs & parameters
    virtual void  doSetupParameters(void) {}
    virtual void  doSetupPrograms(void) {}

    // editor
    virtual void  doSetupEditor(void* aWindow, int aWidth, int aHeight) {} // todo: void* -> axWindow*
    virtual void  doFinishEditor(void) {}
    virtual void  doOpenEditor(void) {}
    virtual void  doCloseEditor(void) {}
    virtual void  doIdleEditor(void) {}

};

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

#include "axInterface.h"

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
        mInstance   = new _I(mDescriptor);    // for vst, automatically create the instance (ladspa will create it later)
        mInterface  = new _In(mInstance);     // the interface/"editor-manager"
        mInstance->setInterface(mInterface);  // so the pugin/instace can create its editor
      }

    /*virtual*/ ~axFormat()
      {
        delete mInterface;
        delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }

  public:

    inline axPlatform*   getPlatform(void)    { return mPlatform; }
    inline axDescriptor* getDescriptor(void)  { return mDescriptor; }
    inline axInstance*   getInstance(void)    { return mInstance; } // vst: return the one instance, ladspa create & return new instance
    inline axInterface*  getInterface(void)   { return mInterface; }

    //----------

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,NULL_INTERFACE,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------

#ifdef AX_LINUX
  AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
  #define main main_plugin
#endif //LINUX

//----------

#ifdef AX_WIN32
  //TODO
#endif //WIN32

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                         \
                                                                                        \
AEffect* main(audioMasterCallback audioMaster)                                          \
{                                                                                       \
  axFormat<_desc,_inst,_iface,_plat>* plug =  new axFormat<_desc,_inst,_iface,_plat>(); \
  _inst* instance = (_inst*)plug->getInstance();                                        \
  if (!instance) return 0;                                                              \
  AEffect* ae = instance->getAEffect();                                                 \
  return ae;                                                                            \
}

//----------------------------------------------------------------------
#endif


