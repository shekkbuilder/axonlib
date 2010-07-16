#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

//TODO: proper ladspa sdk
#include "../extern/ladspa.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorLadspa : public axDescriptor
{
  public:
    axDescriptorLadspa(axBase* aBase)// : axDescriptor(aBase)
      { /*trace("  axDescriptorLadspa.constructor");*/ }
    virtual ~axDescriptorLadspa()
      { /*trace("  axDescriptorLadspa.destructor");*/ }
};

typedef axDescriptorLadspa AX_DESCRIPTOR;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstanceLadspa : public axInstance
{
  protected:
    axBase* mBase;
  public:
    axInstanceLadspa(axBase* aBase) /*: axInstance(aBase)*/
      {
        mBase = aBase;
        /*trace("  axInstanceLadspa.constructor");*/
      }
    virtual ~axInstanceLadspa() { /*trace("  axInstanceLadspa.destructor");*/ }
    // callbacks
    virtual void lad_connect_port(unsigned long Port, LADSPA_Data* DataLocation) { /*trace("axFormatLadspa.lad_connect_port");*/ }
    virtual void lad_activate(void) { /*trace("axFormatLadspa.lad_activate");*/ }
    virtual void lad_run(unsigned long SampleCount) { /*trace("axFormatLadspa.lad_run");*/ }
  //virtual void lad_run_adding(unsigned long SampleCount) {}
  //virtual void lad_set_run_adding_gain(LADSPA_Data Gain) {}
    virtual void lad_deactivate(void) { /*trace("axFormatLadspa.lad_deactivate");*/ }
    virtual void lad_cleanup(void) { /*trace("axFormatLadspa.lad_cleanup");*/ }
    //
    virtual void appendParameter(axParameter* aParameter) {}
    virtual void setupParameters(void) {}
};

typedef axInstanceLadspa AX_INSTANCE;

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

class axFormatLadspa : public axFormat
{

  friend const LADSPA_Descriptor* ladspa_descriptor(unsigned long index);

  private:
    LADSPA_Descriptor ladspadescr;

  private:

    //--------------------------------------------------
    // static callback functions, host -> plugin
    //--------------------------------------------------

    // called by host to instantiate a plugin..
    // return the ptr to the instance (LADSPA_Handle)

    static LADSPA_Handle lad_instantiate_callback(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        //trace("lad_instantiate_callback");
        axFormatLadspa* desc = (axFormatLadspa*)Descriptor->ImplementationData;
        return desc->lad_instantiate(SampleRate);
      }

    //----------

    // the rest are redirected to the instance(s)

    static void lad_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        //trace("lad_connect_port_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_connect_port(Port,DataLocation);
      }

    //----------

    static void lad_activate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_activate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_activate();
      }

    //----------

    static void lad_run_callback(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        //trace("lad_run_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_run(SampleCount);
      }

    //----------

    //static void lad_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_run_adding(SampleCount);
    //  }

    //----------

    //static void lad_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_set_run_adding_gain(Gain);
    //  }

    //----------

    static void lad_deactivate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_deactivate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_deactivate();
      }

    //----------

    static void lad_cleanup_callback(LADSPA_Handle Instance)
      {
        //trace("lad_cleanup_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_cleanup();
        delete inst; // !!!
      }

    //--------------------------------------------------
    // redirected callback functions
    // (these could be in an instance class)
    //--------------------------------------------------

    // called from static callback function (above)

    // return ptr to instance
    virtual LADSPA_Handle lad_instantiate(unsigned long SampleRate)
      {
        //trace("axFormatLadspa.lad_instantiate");
        axInstance* instance = mBase->createInstance();
        return instance;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  protected:
    axBase*       mBase;
    axDescriptor* mDescriptor;

  protected:

    virtual void* entrypoint(void* ptr)
      {
        //trace("* axFormatLadspa.entrypoint");
        mDescriptor = mBase->getDescriptor();
        axMemset(&ladspadescr,0,sizeof(ladspadescr));
        ladspadescr.UniqueID            = 0;//mUniqueId;
        ladspadescr.Label               = mDescriptor->getName();//(char*)"label";
        ladspadescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        ladspadescr.Name                = mDescriptor->getName();// (char*)"name";
        ladspadescr.Maker               = mDescriptor->getAuthor();//(char*)"maker";
        ladspadescr.Copyright           = mDescriptor->getProduct();//(char*)"copyright";
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

    //--------------------------------------------------
    //
    //--------------------------------------------------

  public:

    axFormatLadspa(axBase* aBase)// : axFormat(aBase)
      {
        //trace("- axFormatLadspa.constructor");
        mBase = aBase;
      }

    virtual ~axFormatLadspa()
      {
        //trace("- axFormatLadspa.destructor");
      }

    virtual char* getFormatName(void)
      {
        return (char*)"ladspa";
      }

};

//----------

typedef axFormatLadspa AX_FORMAT;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

//#ifdef AX_WIN32

//  //---
//
//  // this could (should?) be moved to axPlatform
//  static __thread HINSTANCE gInstance;
//
//  #define _AX_LADSPA_DLLMAIN                                    \
//    __externc BOOL APIENTRY                                     \
//    DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved) \
//    {                                                           \
//      trace("ladspa DllMain()");                                \
//      gInstance = hModule;                                      \
//      return TRUE;                                              \
//    }
//
//  //---

  /*
  yes no need for the __externc {}
   __externc
  {
  */

  //---

//#endif

//----------

//#ifdef AX_LINUX

  #define _AX_LADSPA_DLLMAIN

//#endif

//----------------------------------------------------------------------

#define AX_ENTRYPOINT(_PL,_IF,_FO,_D,_I)                                      \
                                                                              \
_AX_LADSPA_DLLMAIN                                                            \
__externc __dllexport                                                         \
const LADSPA_Descriptor* ladspa_descriptor(unsigned long index)               \
{                                                                             \
  if (index>0) return NULL;                                                   \
  axBaseImpl<_PL,_IF,_FO,_D,_I>* base = new axBaseImpl<_PL,_IF,_FO,_D,_I>();  \
  gGlobalScope.setBase(base);                                                 \
  _FO* format = (_FO*)base->getFormat();                                      \
  LADSPA_Descriptor* descr = (LADSPA_Descriptor*)format->entrypoint(NULL);    \
  return descr;                                                               \
}

//----------

/*
#ifdef AX_WIN32
  } // extern "C"
#endif
*/

//----------------------------------------------------------------------
#endif

