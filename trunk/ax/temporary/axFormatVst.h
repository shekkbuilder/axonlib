#ifndef axFormatVst_included
#define axFormatVst_included
//----------------------------------------------------------------------

#include <stdio.h> // printf

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

#include "axPlatform.h"

//----------

// some forward declarations, because some classes have references to
// eachother (cross-connected)

class axDescriptor;
class axInstance;
class axInterface;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

// default names for (default) stereo in/out
static char* g_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
static char* g_stereo_outputs[] = { (char*)"out1",(char*)"out2" };

//----------

class axDescriptor
{
  private:
    axPlatform* mPlatform;
  public:
    axDescriptor(axPlatform* aPlatform) { mPlatform=aPlatform; }
    ~axDescriptor() {}
    inline axPlatform* getPlatform(void) { return mPlatform; }
  public:
    virtual char*         getName(void)             { return (char*)"plugin"; }
    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
    virtual char*         getProduct(void)          { return (char*)"unknwon plugin"; }
    virtual int           getVersion(void)          { return 0; }
    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
    virtual int           getNumInputs(void)        { return 2; }
    virtual int           getNumOutputs(void)       { return 2; }
    virtual int           getNumParams(void)        { return 0; }
    virtual int           getNumProgs(void)         { return 0; }
    virtual char*         getInputName(int aIndex)  { return g_stereo_inputs[aIndex]; }
    virtual char*         getOutputName(int aIndex) { return g_stereo_outputs[aIndex]; }
    virtual char*         getParamName(int aIndex)  { return (char*)"param"; }
    // TODO: double-check with ladspa (and exe)
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

- hmmm, we could actually have axInstanceListener things in here too...
  and have some kind of instance communication/control.
  and if we can make a ladspa and a vst plugin (or other formats) co-exist in the same .so or .dll,
  and have them communicate, we could do interesting things..


*/

class axInstance// : public axParameterListener
{
  private:
    AEffect       mAEffect; // vst-specific
    axDescriptor* mDescriptor;
    axInterface*  mInterface;

  //--------------------------------------------------
  private:
  //--------------------------------------------------

    // static callbacks
    // called from vst dispatcher (via aeffect)

    static VstIntPtr dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        //printf("vst: dispatcher\n");
        axInstance* instance = (axInstance*)(ae->object);
        if (opCode==effClose)
        {
          instance->dispatcher(opCode,index,value,ptr,opt);
          delete instance;
          return 1;
        }
        return instance->dispatcher(opCode,index,value,ptr,opt);
      }

    static float getParameter_callback(AEffect* ae, VstInt32 index)
      {
        //printf("vst: getParameter\n");
        axInstance* instance = (axInstance*)(ae->object);
        return instance->getParameter(index);
      }

    static void setParameter_callback(AEffect* ae, VstInt32 index, float value)
      {
        //printf("vst: setParameter\n");
        axInstance* instance = (axInstance*)(ae->object);
        instance->setParameter(index,value);
      }

    static void processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        axInstance* instance = (axInstance*)(ae->object);
        instance->processReplacing(inputs,outputs,sampleFrames);
      }

    //static void processDoubleReplacing_callback(AEffect* e, double** inputs, double** outputs, VstInt32 sampleFrames)
    //  {
    //    axInstance* instance = (axInstance*)(e->object);
    //    instance->processDoubleReplacing(inputs,outputs,sampleFrames);
    //  }

  //--------------------------------------------------
  public:
  //--------------------------------------------------

    /*

    the instanbce is created once, when the dll/so is first created.
    todo: examine when it's desctroyed/freed..

    */

     axInstance(axDescriptor* aDescriptor)
      {
        mDescriptor = aDescriptor;
        mInterface  = NULL;
        memset(&mAEffect,0,sizeof(mAEffect));
        mAEffect.magic                   = kEffectMagic;
        mAEffect.object                  = this;
        mAEffect.user                    = NULL;
        mAEffect.dispatcher              = dispatcher_callback;
        mAEffect.setParameter            = setParameter_callback;
        mAEffect.getParameter            = getParameter_callback;
        mAEffect.processReplacing        = processReplacing_callback;
        mAEffect.processDoubleReplacing  = NULL;// processDoubleReplacing_callback;
        mAEffect.flags                   = effFlagsCanReplacing;
        mAEffect.version                 = mDescriptor->getVersion();
        mAEffect.uniqueID                = mDescriptor->getUniqueId();
        mAEffect.numPrograms             = mDescriptor->getNumProgs();
        mAEffect.numParams               = mDescriptor->getNumParams();
        mAEffect.numInputs               = mDescriptor->getNumInputs();
        mAEffect.numOutputs              = mDescriptor->getNumOutputs();
        mAEffect.initialDelay            = 0;

//        printf("name:    %s\n",mDescriptor->getName());
//        printf("author:  %s\n",mDescriptor->getAuthor());
//        printf("product: %s\n",mDescriptor->getProduct());
//        printf("version: %i\n",mDescriptor->getVersion());
//        printf("uid:     %i\n",mDescriptor->getUniqueId());
//        printf("inputs   %i\n",mDescriptor->getNumInputs());
//        printf("outputs  %i\n",mDescriptor->getNumOutputs());
//        printf("params   %i\n",mDescriptor->getNumParams());
//        printf("progs    %i\n",mDescriptor->getNumProgs());

      }

    virtual ~axInstance()
      {
      }

    //----------------------------------------
    // accessors

    inline axDescriptor*  getDescriptor(void) { return mDescriptor; }
    inline axInterface*   getInterface(void) { return mInterface; }
    inline AEffect*       getAEffect(void) { return &mAEffect; };

    // todo: friend func and private/protected instead
    inline void setInterface(axInterface* aInterface) { mInterface=aInterface; } // dangerous if public

  //----------------------------------------
  // internal functions
  //----------------------------------------

  public:

    //sendMidi
    //programs/banks..

  //----------------------------------------
  // callbacks
  //----------------------------------------

  private: // hide them
  //protected:

    // these will be more or less direct copy/paste from previous revision.
    // same with functions called by dispatcher,
    // editor is handled a little bit different (not much), so it might
    // need some attention

    virtual VstIntPtr dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        // the big switch/case here...
        return 0;
      }

    virtual float getParameter(VstInt32 aIndex)
      {
        return 0;
      }

    virtual void setParameter(VstInt32 aIndex, float aValue)
      {
      }

    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
      {
      }

    virtual void processDoubleReplacing(double** aInputs, double** aOutputs, VstInt32 aLength)
      {
      }

    //----------------------------------------
    // do...
    //----------------------------------------

  //protected: // should other classes be able to call these?
  public:

    // lib-user overrides these:

    // programs & parameters
    //virtual void  doSetupParameters(void) {}
    //virtual void  doSetupPrograms(void) {}

    // editor
    //virtual void  doSetupEditor(void* aWindow, int aWidth, int aHeight) {} // todo: void* -> axWindow*
    //virtual void  doFinishEditor(void) {}
    //virtual void  doOpenEditor(void) {}
    //virtual void  doCloseEditor(void) {}
    //virtual void  doIdleEditor(void) {}

};

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------
// combine everything...

// for vst, automatically create the instance (ladspa will create it later)
// so the pugin/instace can create its editor
// after we create the Interface, we give the ptr to the Instance

// this is kind of replacable building blocks,
// you can switch between different implementatons..
// the Instance/Descriptor is separated because of ladspa and relatives...

#include "axInterface.h"

template<class _D, class _I, class _In, class _P>
class axFormat
{
  private:
    _P*   mPlatform;    // the under-lying os/system
    _D*   mDescriptor;  // vst:one (with instance), ladspa:one (pre-instance)
    _I*   mInstance;    // vst:one (Aeffect / 'main' call), ladspa:many ('create_instance')
    _In*  mInterface;   // how-to-create-windows,

  public:

    axFormat()
      {
        mPlatform   = new _P(ft_Vst);
        mDescriptor = new _D(mPlatform);
        mInstance   = new _I(mDescriptor);
        mInterface  = new _In(mInstance);
        mInstance->setInterface(mInterface);
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
// wrap it up into a dll/so

/*

axFormat is created (and with that, all four lover leve classes, like
axPlatform, axDescriptor.. and ax axInstance
and we return the AEffect from this instance
the linux version has some additonal defines and trickery, to fix
calling conventions (?), vst sdk issues..
'main' is the function that is exported to 'the outer world'

*/

// some simplification macros
// you can use AX_ENTRYPOINT directly, so, ignore them if you don't use them,

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,NULL_INTERFACE,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------

#ifdef AX_LINUX

  AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
  #define main main_plugin

  //----------

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

#endif //LINUX

//----------------------------------------------------------------------

#ifdef AX_WIN32

  #define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                         \
                                                                                          \
  int main(int audioMaster, char** empty)                                                 \
  {                                                                                       \
    axFormat<_desc,_inst,_iface,_plat>* plug =  new axFormat<_desc,_inst,_iface,_plat>(); \
    _inst* instance = (_inst*)plug->getInstance();                                        \
    if (!instance) return 0;                                                              \
    AEffect* ae = instance->getAEffect();                                                 \
    return (int)ae;                                                                       \
  }

#endif //WIN32

// the instance is deleted in the event handlers
//
// TODO: currently, the axFormat is not deleted anywhere???
// it shouldn't be deleted until the dll/so ios unloaded,
// but does the os handle this?
// windows could handle this with DllUnload (?) in axPlatform
// linux? _fini?

//----------------------------------------------------------------------
#endif
