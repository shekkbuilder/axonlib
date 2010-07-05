#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

//TODO: remove these when using axDefines, etc...

#include <memory.h>

#ifndef __externc
  #define __externc extern "C"
#endif

#ifndef __dllexport
  #ifdef AX_WIN32
    #define __dllexport __attribute__ ((dllexport))
  #endif
  #ifdef AX_LINUX
    #define __dllexport
  #endif
#endif

//----------

//TODO: proper ladspa sdk
#include "../extern/ladspa.h"

//----------------------------------------------------------------------

class axFormatLadspa : public axFormat
{
  private:
    LADSPA_Descriptor ladspadescr;

  private:

    //--------------------------------------------------
    // static callback functions, host -> plugin
    //--------------------------------------------------

    // redirect to instantiate() in axDescriptor
    static LADSPA_Handle lad_instantiate_callback(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        trace("lad_instantiate_callback");
        axFormatLadspa* desc = (axFormatLadspa*)Descriptor->ImplementationData;
        return desc->lad_instantiate(SampleRate);
      }

    //----------

    // the rest are redirected to the instance(s)

    static void lad_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        trace("lad_connect_port_callback");
        axFormatLadspa* inst = (axFormatLadspa*)Instance;
        inst->lad_connect_port(Port,DataLocation);
      }

    //----------

    static void lad_activate_callback(LADSPA_Handle Instance)
      {
        trace("lad_activate_callback");
        axFormatLadspa* inst = (axFormatLadspa*)Instance;
        inst->lad_activate();
      }

    //----------

    static void lad_run_callback(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        //trace("lad_run_callback");
        axFormatLadspa* inst = (axFormatLadspa*)Instance;
        inst->lad_run(SampleCount);
      }

    //----------

    //static void lad_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axFormatLadspa* inst = (axFormatLadspa*)Instance;
    //    inst->lad_run_adding(SampleCount);
    //  }

    //----------

    //static void lad_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axFormatLadspa* inst = (axFormatLadspa*)Instance;
    //    inst->lad_set_run_adding_gain(Gain);
    //  }

    //----------

    static void lad_deactivate_callback(LADSPA_Handle Instance)
      {
        trace("lad_deactivate_callback");
        axFormatLadspa* inst = (axFormatLadspa*)Instance;
        inst->lad_deactivate();
      }

    //----------

    static void lad_cleanup_callback(LADSPA_Handle Instance)
      {
        trace("lad_cleanup_callback");
        axFormatLadspa* inst = (axFormatLadspa*)Instance;
        inst->lad_cleanup();
        //delete inst; // !!!
      }

    //--------------------------------------------------
    // redirected callback functions
    // (these could be in an instance class)
    //--------------------------------------------------

    // called from static callback function (above)
    virtual LADSPA_Handle lad_instantiate(unsigned long SampleRate)
      {
        trace("axFormatLadspa.lad_instantiate");
        return this;//mFormat->getInstance(); // create instance
      }

    //----------

    virtual void lad_connect_port(unsigned long Port, LADSPA_Data* DataLocation)
      {
        trace("axFormatLadspa.lad_connect_port");
      }

    virtual void lad_activate(void)
      {
        trace("axFormatLadspa.lad_activate");
      }

    virtual void lad_run(unsigned long SampleCount)
      {
        //trace("axFormatLadspa.lad_run");
      }

  //virtual void lad_run_adding(unsigned long SampleCount) {}
  //virtual void lad_set_run_adding_gain(LADSPA_Data Gain) {}

    virtual void lad_deactivate(void)
      {
        trace("axFormatLadspa.lad_deactivate");
      }

    virtual void lad_cleanup(void)
      {
        trace("axFormatLadspa.lad_cleanup");
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  protected:
    axBase*   mBase;

    //--------------------------------------------------
    //
    //--------------------------------------------------

  public:

    axFormatLadspa(axBase* aBase) : axFormat(aBase)
      {
        trace("- axFormatLadspa.constructor");
        mBase = aBase;
      }

    virtual ~axFormatLadspa()
      {
        trace("- axFormatLadspa.destructor");
      }

    virtual void* entrypoint(void* ptr)
      {
        trace("* axFormatLadspa.entrypoint");
        memset(&ladspadescr,0,sizeof(ladspadescr)); // axMemset
        ladspadescr.UniqueID            = 0;//mUniqueId;
        ladspadescr.Label               = (char*)"label";
        ladspadescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        ladspadescr.Name                = (char*)"name";
        ladspadescr.Maker               = (char*)"maker";
        ladspadescr.Copyright           = (char*)"copyright";
        ladspadescr.PortCount           = 0;
        ladspadescr.PortDescriptors     = NULL;
        ladspadescr.PortNames           = NULL;//g_stereo_ports;
        ladspadescr.PortRangeHints      = NULL;
        ladspadescr.ImplementationData  = this;
        ladspadescr.instantiate         = lad_instantiate_callback;
        ladspadescr.connect_port        = lad_connect_port_callback;
        ladspadescr.activate            = lad_activate_callback;
        ladspadescr.run                 = lad_run_callback;
        ladspadescr.run_adding          = NULL;//lad_run_adding_callback;            // ala process, optional
        ladspadescr.set_run_adding_gain = NULL;//lad_set_run_adding_gain_callback;   // if above
        ladspadescr.deactivate          = lad_deactivate_callback;
        ladspadescr.cleanup             = lad_cleanup_callback;
        return (void*)&ladspadescr;
        //TODO: fix
      }

//    virtual axDescriptor* createDescriptor(void)
//      {
//        return NULL;
//      }

//    virtual axInstance* createInstance(void)
//      {
//        return NULL;
//      }

};

//----------

typedef axFormatLadspa AX_FORMAT;

//----------------------------------------------------------------------

#ifdef AX_WIN32

  //---

  // this could (should?) be moved to axPlatform
  static __thread HINSTANCE gInstance;

  #define _AX_LADSPA_DLLMAIN                                    \
    __externc BOOL APIENTRY                                     \
    DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) \
    {                                                           \
      trace("ladspa DllMain()");                                \
      gInstance = hModule;                                      \
      return TRUE;                                              \
    }

  //---

  /*
  yes no need for the __externc {}
   __externc
  {
  */

  //---

#endif

//----------

#ifdef AX_LINUX

  #define _AX_LADSPA_DLLMAIN

#endif

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_PL,_IF,_FO)                                          \
                                                                            \
_AX_LADSPA_DLLMAIN                                                          \
__externc __dllexport                                                       \
const LADSPA_Descriptor* ladspa_descriptor (unsigned long index)            \
{                                                                           \
  if (index>0) return NULL;                                                 \
  axBaseImpl<_PL,_IF,_FO>* base = new axBaseImpl<_PL,_IF,_FO>();            \
  g_GlobalScope.setBase(base);                                              \
  _FO* format = (_FO*)base->getFormat();                                    \
  LADSPA_Descriptor* descr = (LADSPA_Descriptor*)format->entrypoint(NULL);  \
  return descr;                                                             \
}

//----------

/*
#ifdef AX_WIN32
  } // extern "C"
#endif
*/

//----------------------------------------------------------------------
#endif


































//#ifndef axFormatLadspa_included
//#define axFormatLadspa_included
////----------------------------------------------------------------------
//
//#include <memory.h> // memset
//
//#include "../extern/ladspa.h" //TODO: proper ladspa sdk
//
//// g_ = global
//
//// default names for (default) stereo in/out
//static char* g_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
//static char* g_stereo_outputs[] = { (char*)"out1",(char*)"out2" };
//
//static char* g_stereo_ports[] = { (char*)"in1", (char*)"in2",(char*)"out1",(char*)"out2" };
//
////----------------------------------------------------------------------
////
//// instance
////
////----------------------------------------------------------------------
//
//class axInstance
//{
//  private:
//    axFormat* mFormat;
//
//  public:
//
//    axInstance(axFormat* aFormat)
//      {
//        trace("axInstance.constructor [ladspa]");
//        mFormat=aFormat;
//      }
//
//    virtual ~axInstance()
//      {
//        trace("axInstance.destructor");
//      }
//
//    virtual void connect_port(unsigned long Port, LADSPA_Data* DataLocation)
//      {
//        trace("axInstance.connect_port");
//      }
//
//    virtual void activate(void)
//      {
//        trace("axInstance.activate");
//      }
//
//    virtual void run(unsigned long SampleCount)
//      {
//        //trace("axInstance.run");
//      }
//
//  //virtual void run_adding(unsigned long SampleCount) {}
//  //virtual void set_run_adding_gain(LADSPA_Data Gain) {}
//
//    virtual void deactivate(void)
//      {
//        trace("axInstance.deactivate");
//      }
//
//    virtual void cleanup(void)
//      {
//        trace("axInstance.cleanup");
//      }
//
//};
//
////----------------------------------------------------------------------
////
//// descriptor
////
////----------------------------------------------------------------------
//
//class axDescriptor
//{
//
//  // static callbacks
//
//  private:
//
//    // redirect to instantiate() in axDescriptor
//    static LADSPA_Handle lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
//      {
//        trace("lad_instantiate");
//        axDescriptor* desc = (axDescriptor*)Descriptor->ImplementationData;
//        return desc->instantiate(SampleRate);
//      }
//
//    //----------
//
//    // the rest are redirected to the instance(s)
//
//    static void lad_connect_port(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
//      {
//        trace("lad_connect_port");
//        axInstance* inst = (axInstance*)Instance;
//        inst->connect_port(Port,DataLocation);
//      }
//
//    //----------
//
//    static void lad_activate(LADSPA_Handle Instance)
//      {
//        trace("lad_activate");
//        axInstance* inst = (axInstance*)Instance;
//        inst->activate();
//      }
//
//    //----------
//
//    static void lad_run(LADSPA_Handle Instance, unsigned long SampleCount)
//      {
//        //trace("lad_run");
//        axInstance* inst = (axInstance*)Instance;
//        inst->run(SampleCount);
//      }
//
//    //----------
//
//    //static void lad_run_adding(LADSPA_Handle Instance, unsigned long SampleCount)
//    //  {
//    //    axInstance* inst = (axInstance*)Instance;
//    //    inst->run_adding(SampleCount);
//    //  }
//
//    //----------
//
//    //static void lad_set_run_adding_gain(LADSPA_Handle Instance, LADSPA_Data Gain)
//    //  {
//    //    axInstance* inst = (axInstance*)Instance;
//    //    inst->set_run_adding_gain(Gain);
//    //  }
//
//    //----------
//
//    static void lad_deactivate(LADSPA_Handle Instance)
//      {
//        //trace("lad_deactivate");
//        axInstance* inst = (axInstance*)Instance;
//        inst->deactivate();
//      }
//
//    //----------
//
//    static void lad_cleanup(LADSPA_Handle Instance)
//      {
//        //trace("lad_cleanup");
//        axInstance* inst = (axInstance*)Instance;
//        inst->cleanup();
//        delete inst;                                        // !!!
//      }
//
//  //----------
//
//  private:
//    axFormat*         mFormat;
//    LADSPA_Descriptor mLDescr;
//
//  public:
//
//    axDescriptor(axFormat* aFormat)
//      {
//        trace("axDescriptor [ladspa]");
//        mFormat     = aFormat;
//        memset(&mLDescr,0,sizeof(mLDescr)); // axMemset
//        mLDescr.UniqueID            = 0;//mUniqueId;
//        mLDescr.Label               = this->getName();
//        mLDescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
//        mLDescr.Name                = this->getName();
//        mLDescr.Maker               = this->getAuthor();
//        mLDescr.Copyright           = this->getProduct();;
//        mLDescr.PortCount           = 0;
//        mLDescr.PortDescriptors     = NULL;
//        mLDescr.PortNames           = NULL;//g_stereo_ports;
//        mLDescr.PortRangeHints      = NULL;
//        mLDescr.ImplementationData  = this;
//        mLDescr.instantiate         = lad_instantiate;
//        mLDescr.connect_port        = lad_connect_port;
//        mLDescr.activate            = lad_activate;
//        mLDescr.run                 = lad_run;                          // ala processReplacing
//        mLDescr.run_adding          = NULL;//lad_run_adding;            // ala process, optional
//        mLDescr.set_run_adding_gain = NULL;//lad_set_run_adding_gain;   // if above
//        mLDescr.deactivate          = lad_deactivate;
//        mLDescr.cleanup             = lad_cleanup;
//        //TODO: fix
//      }
//
//    //----------
//
//    ~axDescriptor()
//      {
//        trace("axDescriptor.constructor [ladspa]");
//      }
//
//    //----------------------------------------
//    // accessors
//
//    inline LADSPA_Descriptor* getLDescr(void) { return &mLDescr; };
//
//  //----------------------------------------
//  // helpers / internal
//  //----------------------------------------
//
//  public:
//
//  //----------------------------------------
//  // callbacks
//  //----------------------------------------
//
//  private: // hide them
//  //protected:
//
//    // called from static callback function (above)
//    virtual LADSPA_Handle instantiate(unsigned long SampleRate)
//      {
//        trace("axDescriptor.instantiate");
//        return mFormat->getInstance(); // create instance
//      }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    virtual char*         getName(void)             { return (char*)"plugin"; }
//    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
//    virtual char*         getProduct(void)          { return (char*)"unknwon plugin"; }
//    virtual int           getVersion(void)          { return 0; }
//    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
//    virtual int           getNumInputs(void)        { return 2; }
//    virtual int           getNumOutputs(void)       { return 2; }
//    virtual int           getNumParams(void)        { return 0; }
//    virtual int           getNumProgs(void)         { return 0; }
//    virtual char*         getInputName(int aIndex)  { return g_stereo_inputs[aIndex]; }
//    virtual char*         getOutputName(int aIndex) { return g_stereo_outputs[aIndex]; }
//    virtual char*         getParamName(int aIndex)  { return (char*)"param"; }
//    // TODO: double-check with ladspa (and exe)
//};
//
////----------------------------------------------------------------------
////
//// main
////
////----------------------------------------------------------------------
//
//template<class _D, class _I, class _If, class _P>
//class axFormatImpl : public axFormat//Base
//{
//  private:
//    _P* mPlatform;
//    _D* mDescriptor;
//
//  public:
//
//    axFormatImpl() : axFormat()//Base()
//      {
//        mPlatform   = new _P(this);
//        mDescriptor = new _D(this);
//      }
//
//    //----------
//
//    virtual ~axFormatImpl()
//      {
//        delete mDescriptor;
//        delete mPlatform;
//      }
//
//    //----------
//
//    virtual axPlatform*   getPlatform(void)    { return mPlatform; }
//    virtual axDescriptor* getDescriptor(void)  { return mDescriptor; }
//    virtual axInstance*   getInstance(void)    { return new _I(this); }
//
//    //----------
//
//};
//
////----------------------------------------------------------------------
//
//class axGlobalScope
//{
//  public:
//    axFormat* mFormat;
//    axGlobalScope()  { trace("axGlobalScope.constructor"); mFormat=NULL; }
//    ~axGlobalScope() { trace("axGlobalScope.destructor");  if (mFormat) delete mFormat; }
//};
//
//axGlobalScope g_Scope;
//
////----------------------------------------------------------------------
////
//// entry point
////
////----------------------------------------------------------------------
//
////TODO: test windows version...
//
///*
//- audacity does not recognizes the dll with the new structure.
//- the "rtladspa.exe" debug tool reads the descriptor but returns 0 ports
//and crashes soon after.
//- also something broke mverb-ladspa-win32 (strange?) with the old structure
//*/
//
///*
//
//  ah, ok, well, no ports are reported back to ladspa host at the moment,
//  only the comiplastion and plugin loading is active...
//  but, should a hopst really crash if zero ports? even if a plugin is
//  completely useless without inputs/outputs, it shouldn't crash?
//  seems like audacity has no errror checking or fallbacks for unsupported
//  number of ports, or something..
//  any, will be fixed as soon as we add back the proper code from earlier
//  revisions
//
//*/
//
//#ifdef AX_WIN32
//  // this could (should?) be moved to axPlatform
//  static __thread HINSTANCE gInstance;
//  #define _AX_LADSPA_DLLMAIN                                    \
//    __externc BOOL APIENTRY                                     \
//    DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) \
//    {                                                           \
//      trace("ladspa DllMain()");                                \
//      gInstance = hModule;                                      \
//      return TRUE;                                              \
//    }
//  // should this, and the } be -inside- the AX_ENTRYPOINT define? or is it only the DLLMAIN
//  // doesn't (only) the AX_ENTRYPOINT expands wherever we put the macro?
//
//  /*
//  yes no need for the __externc {}
//   __externc
//  {
//  */
//
//#else // AX_WIN32
//  #define _AX_LADSPA_DLLMAIN
//#endif
//
////----------
//
//#define AX_ENTRYPOINT(_desc,_inst,_iface,_plat)                                                   \
//                                                                                                  \
//  _AX_LADSPA_DLLMAIN                                                                              \
//  __externc __dllexport                                                                           \
//  const LADSPA_Descriptor* ladspa_descriptor (unsigned long index)                                \
//  {                                                                                               \
//    if (index>0) return NULL;                                                                     \
//    axFormatImpl<_desc,_inst,_iface,_plat>* plug =  new axFormatImpl<_desc,_inst,_iface,_plat>(); \
//    g_Scope.mFormat = plug;                                                                       \
//    _inst* instance = (_inst*)plug->getInstance();                                                \
//    if (!instance) return 0;                                                                      \
//    LADSPA_Descriptor* descr = plug->getDescriptor()->getLDescr();                                \
//    return descr;                                                                                 \
//  }                                                                                               \
//
////----------
//
///*
//#ifdef AX_WIN32
//  } // extern "C"
//#endif
//*/
//
////----------------------------------------------------------------------
//#endif
