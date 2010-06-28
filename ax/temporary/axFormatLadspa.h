#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

#include "../extern/ladspa.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

// g_ = global

// default names for (default) stereo in/out
static char* g_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
static char* g_stereo_outputs[] = { (char*)"out1",(char*)"out2" };

//----------

class axDescriptor
{
  private:

    // redirect to instantiate() in axDescriptor
    static LADSPA_Handle lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        axDescriptor* ladspa = (axDescriptor*)Descriptor->ImplementationData;
        return ladspa->instantiate(SampleRate);
      }

    //----------

    // the rest are redirected to the instance(s)

    static void lad_connect_port(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        axInstance* ladspa = (axInstance*)Instance;
        ladspa->connect_port(Port,DataLocation);
      }

    //----------

    static void lad_activate(LADSPA_Handle Instance)
      {
        axInstance* ladspa = (axInstance*)Instance;
        ladspa->activate();
      }

    //----------

    static void lad_run(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        axInstance* ladspa = (axInstance*)Instance;
        ladspa->run(SampleCount);
      }

    //----------

    //static void lad_run_adding(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axInstance* ladspa = (axInstance*)Instance;
    //    ladspa->run_adding(SampleCount);
    //  }

    //----------

    //static void lad_set_run_adding_gain(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axInstance* ladspa = (axInstance*)Instance;
    //    ladspa->set_run_adding_gain(Gain);
    //  }

    //----------

    static void lad_deactivate(LADSPA_Handle Instance)
      {
        axInstance* ladspa = (axInstance*)Instance;
        ladspa->deactivate();
      }

    //----------

    static void lad_cleanup(LADSPA_Handle Instance)
      {
        axInstance* ladspa = (axInstance*)Instance;
        ladspa->cleanup();
        delete ladspa; // !!!
      }

  //----------

  private:
    LADSPA_Descriptor ladescr;

  public:

    axDescriptor(axFormat* aFormat)
      {
        ladescr.UniqueID            = 0;//mUniqueId;
        ladescr.Label               = mLabel;//"label";
        ladescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        ladescr.Name                = mName;//"name";
        ladescr.Maker               = mMaker;//"maker";
        ladescr.Copyright           = mCopyright;//"copyright";
        ladescr.PortCount           = 0;//mNumPorts;
        ladescr.PortDescriptors     = mPortDesc; // NULL
        ladescr.PortNames           = mPortNames; // NULL
        ladescr.PortRangeHints      = mPortHint; // NULL
        ladescr.ImplementationData  = this;
        ladescr.instantiate         = lad_instantiate;
        ladescr.connect_port        = lad_connect_port;
        ladescr.activate            = lad_activate;
        ladescr.run                 = lad_run;                  // ala processReplacing
        ladescr.run_adding          = NULL;//lad_run_adding;           // ala process, optional
        ladescr.set_run_adding_gain = NULL;//lad_set_run_adding_gain;  // if above
        ladescr.deactivate          = lad_deactivate;
        ladescr.cleanup             = lad_cleanup;
      }

    ~axDescriptor()
      {
      }

    private:

    // called from static callback function (above)
    virtual LADSPA_Handle instantiate(unsigned long SampleRate)
      {
        return mFormat->getInstance(); // create instance
      }

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

class axInstance
{
  private:
    axFormat* mFormat;
  public:
    axInstance(axFormat* aFormat) { mFormat=aFormat; }
    virtual ~axInstance() { }
    //virtual int getAEffect(void) { return 0; };
    virtual void connect_port(unsigned long Port, LADSPA_Data* DataLocation) {}
    virtual void activate(void) {}
    virtual void run(unsigned long SampleCount) {}
    //virtual void run_adding(unsigned long SampleCount) {}
    //virtual void set_run_adding_gain(LADSPA_Data Gain) {}
    virtual void deactivate(void) {}
    virtual void cleanup(void) {}

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

template<class _D, class _I, class _If, class _P>
class axFormatImpl : public axFormat//Base
{
  private:
    _P* mPlatform;
    _D* mDescriptor;

  public:

    axFormatImpl() : axFormat()//Base()
      {
        mPlatform   = new _P(0); // todo: this (axFormat* param in platform constructor
        mDescriptor = new _D(this);
        //mInstance   = new _I(mDescriptor);
      }

    //----------

    virtual ~axFormatImpl()
      {
        //delete mInstance;
        delete mDescriptor;
        delete mPlatform;
      }

    //----------

    virtual axPlatform*   getPlatform(void)    { return mPlatform; }
    virtual axDescriptor* getDescriptor(void)  { return mDescriptor; }
    //virtual axInstance*   getInstance(void)    { return mInstance; }

    //----------

};

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat) \
  axFormatImpl<_desc,_inst,_iface,_plat>* plug =  new axFormatImpl<_desc,_inst,_iface,_plat>(); \

  //return plug->lad_descr;

// creates an axFormatImpl class..

//----------------------------------------------------------------------
#endif













//TODO: move the things below into the AX_ENTRYPOINT above

#if 0













#define AX_CONTEXT_INIT(name) \
  axContext ctx(0,0,0);

#ifdef AX_WIN32
  static __thread HINSTANCE gInstance;

  #define _AX_LADSPA_DLLMAIN                                    \
    __externc BOOL APIENTRY                                     \
    DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) \
    {                                                           \
      trace("ladspa DllMain()");                                \
      gInstance = hModule;                                      \
      return TRUE;                                              \
    }

  __externc
  {

#else // AX_WIN32
  #define _AX_LADSPA_DLLMAIN
#endif

#define AX_ENTRYPOINT(plugclass)                                      \
    _AX_LADSPA_DLLMAIN                                                \
    __externc __dllexport                                             \
    const LADSPA_Descriptor* ladspa_descriptor (unsigned long index)  \
    {                                                                 \
      trace("ladspa_descriptor( " << index << " )" );                 \
      if (index)                                                      \
        return NULL;                                                  \
      AX_CONTEXT_INIT(plugclass)                                      \
      plugclass* plug = new plugclass(&ctx,ff_Enumerate);             \
      LADSPA_Descriptor* descriptor = plug->getDescriptor();          \
      return descriptor;                                              \
    }                                                                 \
                                                                      \
                                                                      \
                                                                      \
    void* create_ladspa_instance(axContext* aContext)                 \
    {                                                                 \
      plugclass* plug = new plugclass(aContext,ff_None);              \
      return (void*)plug;                                             \
    }


#ifdef AX_WIN32
  } // extern "C"
#endif










#endif // 0
