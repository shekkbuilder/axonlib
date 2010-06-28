#ifndef axFormatVst_included
#define axFormatVst_included
//----------------------------------------------------------------------

#include <stdio.h> // printf

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

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
  public:
    axDescriptor(axFormat* aFormat) {}
    ~axDescriptor() {}
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

class axInstance// : public axParameterListener
{
  private:
    AEffect mAEffect; // vst-specific

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

    //----------

    static float getParameter_callback(AEffect* ae, VstInt32 index)
      {
        //printf("vst: getParameter\n");
        axInstance* instance = (axInstance*)(ae->object);
        return instance->getParameter(index);
      }

    //----------

    static void setParameter_callback(AEffect* ae, VstInt32 index, float value)
      {
        //printf("vst: setParameter\n");
        axInstance* instance = (axInstance*)(ae->object);
        instance->setParameter(index,value);
      }

    //----------

    static void processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        axInstance* instance = (axInstance*)(ae->object);
        instance->processReplacing(inputs,outputs,sampleFrames);
      }

    //----------

    //static void processDoubleReplacing_callback(AEffect* e, double** inputs, double** outputs, VstInt32 sampleFrames)
    //  {
    //    axInstance* instance = (axInstance*)(e->object);
    //    instance->processDoubleReplacing(inputs,outputs,sampleFrames);
    //  }

  //--------------------------------------------------
  public:
  //--------------------------------------------------

     axInstance(axFormat* aFormat)
      {
        axDescriptor* descriptor = aFormat->getDescriptor();
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
        mAEffect.version                 = descriptor->getVersion();
        mAEffect.uniqueID                = descriptor->getUniqueId();
        mAEffect.numPrograms             = descriptor->getNumProgs();
        mAEffect.numParams               = descriptor->getNumParams();
        mAEffect.numInputs               = descriptor->getNumInputs();
        mAEffect.numOutputs              = descriptor->getNumOutputs();
        mAEffect.initialDelay            = 0;
        //printf("name:    %s\n",mDescriptor->getName());
        //printf("author:  %s\n",mDescriptor->getAuthor());
        //printf("product: %s\n",mDescriptor->getProduct());
        //printf("version: %i\n",mDescriptor->getVersion());
        //printf("uid:     %i\n",mDescriptor->getUniqueId());
        //printf("inputs   %i\n",mDescriptor->getNumInputs());
        //printf("outputs  %i\n",mDescriptor->getNumOutputs());
        //printf("params   %i\n",mDescriptor->getNumParams());
        //printf("progs    %i\n",mDescriptor->getNumProgs());
      }

    //----------

    virtual ~axInstance()
      {
      }

    //----------------------------------------
    // accessors

    inline AEffect* getAEffect(void) { return &mAEffect; };

  //----------------------------------------
  // helpers / internal
  //----------------------------------------

  public:

    //sendMidi
    //programs/banks..

  //----------------------------------------
  // callbacks
  //----------------------------------------

  private: // hide them
  //protected:

    virtual VstIntPtr dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        // the big switch/case here...
        return 0;
      }

    //----------

    virtual float getParameter(VstInt32 aIndex)
      {
        return 0;
      }

    //----------

    virtual void setParameter(VstInt32 aIndex, float aValue)
      {
      }

    //----------

    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
      {
      }

    //----------

    virtual void processDoubleReplacing(double** aInputs, double** aOutputs, VstInt32 aLength)
      {
      }

  //----------------------------------------
  // do...
  //----------------------------------------

  public:
  //protected: // should other classes be able to call these?
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

template<class _D,class _I,class _In,class _P>
class axFormatImpl : public axFormat
{
  private:
    /* _P*  */ axPlatform*   mPlatform;
    /* _D*  */ axDescriptor* mDescriptor;
    /* _I*  */ axInstance*   mInstance;
    /* _In* */ axInterface*  mInterface;

  public:
    axFormatImpl() : axFormat()
      {
        printf("axFormatImpl: vst\n");
        mPlatform   = new _P(this);
        mDescriptor = new _D(this);
        mInstance   = new _I(this);
        mInterface  = NULL;//new _In(this);
        // audio?
      }
    virtual ~axFormatImpl()
      {
        delete mPlatform;
        delete mDescriptor;
        delete mInstance;
        if (mInterface) delete mInterface;
      }
  //protected: //TODO: friend func..
  public:
    virtual axFormat*     getFormat(void)     { return this; }
    virtual axPlatform*   getPlatform(void)   { return mPlatform; }
    virtual axDescriptor* getDescriptor(void) { return mDescriptor; }
    virtual axInstance*   getInstance(void)   { return mInstance; }
    virtual axInterface*  getInterface(void)  { return mInterface; }
};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------
// wrap it up into a dll/so

#ifdef AX_LINUX

  AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
  #define main main_plugin

  //----------

  #define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                                 \
                                                                                                  \
  AEffect* main(audioMasterCallback audioMaster)                                                  \
  {                                                                                               \
    axFormatImpl<_desc,_inst,_iface,_plat>* plug =  new axFormatImpl<_desc,_inst,_iface,_plat>(); \
    _inst* instance = (_inst*)plug->getInstance();                                                \
    if (!instance) return 0;                                                                      \
    AEffect* ae = instance->getAEffect();                                                         \
    return ae;                                                                                    \
  }

#endif //LINUX

//----------------------------------------------------------------------
// (only the 'main' part is different)

#ifdef AX_WIN32

  #define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                                 \
                                                                                                  \
  int main(int audioMaster, char** empty)                                                         \
  {                                                                                               \
    axFormatImpl<_desc,_inst,_iface,_plat>* plug =  new axFormatImpl<_desc,_inst,_iface,_plat>(); \
    _inst* instance = (_inst*)plug->getInstance();                                                \
    if (!instance) return 0;                                                                      \
    AEffect* ae = instance->getAEffect();                                                         \
    return (int)ae;                                                                               \
  }

#endif //WIN32

//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(_desc,_inst) AX_ENTRYPOINT(_desc,_inst,NULL_INTERFACE,axPlatform)
#else
  #define AX_MAIN(_desc,_inst,_iface) AX_ENTRYPOINT(_desc,_inst,_iface,axPlatform)
#endif

//----------------------------------------------------------------------

// the instance is deleted in the event handlers
//
// TODO: currently, the axFormat is not deleted anywhere???
// it shouldn't be deleted until the dll/so ios unloaded,
// but does the os handle this?
// windows could handle this with DllUnload (?) in axPlatform
// linux? _fini?

//----------------------------------------------------------------------
#endif
