/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "format/axFormatBase.h"
#include "../extern/ladspa.h"

//----------------------------------------------------------------------

// this might be overkill?
// inputs + outputs + parameters
#define MAX_LADSPA_PORTS 256

//----------------------------------------------------------------------

class axFormatLadspa : public axFormatBase
{
  private:
    LADSPA_Descriptor     mDescriptor;
    int                   mNumPorts;
    char*                 mPortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor mPortDesc[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  mPortHint[MAX_LADSPA_PORTS];
  private:
    float*                mInputs[MAX_LADSPA_PORTS];
    float*                mOutputs[MAX_LADSPA_PORTS];
    LADSPA_Data*          mParamPtr[MAX_LADSPA_PORTS];
    LADSPA_Data           mParamPrev[MAX_LADSPA_PORTS];
  private:
  //int                   mUniqueId;
    char                  mLabel[64];
    char                  mName[64];
    char                  mMaker[64];
    char                  mCopyright[64];
    int                   mNumInputs;
    int                   mNumOutputs;
    unsigned long         mSampleRate;
    unsigned long         mBlockSize;

  private:

    //--------------------------------------------------
    // host callback functions
    //--------------------------------------------------

    // instantiates a plugin. A handle is returned indicating the new
    // plugin instance. This function must return NULL if instantiation
    // fails.
    // Note that instance initialisation should generally occur in
    // activate() rather than here. */

    //static LADSPA_Handle l_instantiate(const struct _LADSPA_Descriptor * Descriptor, unsigned long SampleRate)
    static LADSPA_Handle lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Descriptor->ImplementationData;
        return ladspa->instantiate(SampleRate);
      }

    // connects a port on an instantiated plugin to a memory location at
    // which a block of data for the port will be read/written. The data
    // location is expected to be an array of LADSPA_Data for audio
    // ports or a single LADSPA_Data value for control ports.
    // Memory issues will be managed by the host. The plugin must
    // read/write the data at these locations every time run() or
    // run_adding() is called and the data present at the time of this
    // connection call should not be considered meaningful.

    // connect_port() may be called more than once for a plugin instance
    // to allow the host to change the buffers that the plugin is
    // reading or writing. These calls may be made before or after
    // activate() or deactivate() calls.

    // connect_port() must be called at least once for each port before
    // run() or run_adding() is called. When working with blocks of
    // LADSPA_Data the plugin should pay careful attention to the block
    // size passed to the run function as the block allocated may only
    // just be large enough to contain the block of samples.

    // Plugin writers should be aware that the host may elect to use the
    // same buffer for more than one port and even use the same buffer
    // for both input and output (see LADSPA_PROPERTY_INPLACE_BROKEN).
    // However, overlapped buffers or use of a single buffer for both
    // audio and control data may result in unexpected behaviour.

    static void lad_connect_port(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->connect_port(Port,DataLocation);
      }

    // initialises a plugin instance and activates it for use.
    // This is separated from instantiate() to aid real-time support and
    // so that hosts can reinitialise a plugin instance by calling
    // deactivate() and then activate(). In this case the plugin
    // instance must reset all state information dependent on the
    // history of the plugin instance except for any data locations
    // provided by connect_port() and any gain set by
    // set_run_adding_gain(). If there is nothing for activate() to do
    // then the plugin writer may provide a NULL rather than an empty
    // function.

    // When present, hosts must call this function once before run() (or
    // run_adding()) is called for the first time. This call should be
    // made as close to the run() call as possible and indicates to
    // real-time plugins that they are now live. Plugins should not rely
    // on a prompt call to run() after activate(). activate() may not be
    // called again unless deactivate() is called first. Note that
    // connect_port() may be called before or after a call to
    // activate().

    static void lad_activate(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->activate();
      }

  /* This method is a function pointer that runs an instance of a
     plugin for a block. Two parameters are required: the first is a
     handle to the particular instance to be run and the second
     indicates the block size (in samples) for which the plugin
     instance may run.

     Note that if an activate() function exists then it must be called
     before run() or run_adding(). If deactivate() is called for a
     plugin instance then the plugin instance may not be reused until
     activate() has been called again.

     If the plugin has the property LADSPA_PROPERTY_HARD_RT_CAPABLE
     then there are various things that the plugin should not do
     within the run() or run_adding() functions (see above). */

    /*

    we are not getting told when a parameter changes, so, to get our
    doSetParameter calls, we have to check manually if any of them
    has changed, and if so, call the doSetParameter, and then remember
    this new value, so we can compare it again the next round

    */

    static void lad_run(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->run(SampleCount);
      }

  /* This method is a function pointer that runs an instance of a
     plugin for a block. This has identical behaviour to run() except
     in the way data is output from the plugin. When run() is used,
     values are written directly to the memory areas associated with
     the output ports. However when run_adding() is called, values
     must be added to the values already present in the memory
     areas. Furthermore, output values written must be scaled by the
     current gain set by set_run_adding_gain() (see below) before
     addition.

     run_adding() is optional. When it is not provided by a plugin,
     this function pointer must be set to NULL. When it is provided,
     the function set_run_adding_gain() must be provided also. */

    //static void lad_run_adding(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
    //    ladspa->run_adding(SampleCount);
    //  }

  /* This method is a function pointer that sets the output gain for
     use when run_adding() is called (see above). If this function is
     never called the gain is assumed to default to 1. Gain
     information should be retained when activate() or deactivate()
     are called.

     This function should be provided by the plugin if and only if the
     run_adding() function is provided. When it is absent this
     function pointer must be set to NULL. */

    //static void lad_set_run_adding_gain(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
    //    ladspa->set_run_adding_gain(Gain);
    //  }

  /* This is the counterpart to activate() (see above). If there is
     nothing for deactivate() to do then the plugin writer may provide
     a NULL rather than an empty function.

     Hosts must deactivate all activated units after they have been
     run() (or run_adding()) for the last time. This call should be
     made as close to the last run() call as possible and indicates to
     real-time plugins that they are no longer live. Plugins should
     not rely on prompt deactivation. Note that connect_port() may be
     called before or after a call to deactivate().

     Deactivation is not similar to pausing as the plugin instance
     will be reinitialised when activate() is called to reuse it. */

    static void lad_deactivate(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->deactivate();
      }

  /* Once an instance of a plugin has been finished with it can be
     deleted using the following function. The instance handle passed
     ceases to be valid after this call.

     If activate() was called for a plugin instance then a
     corresponding call to deactivate() must be made before cleanup()
     is called. */

    static void lad_cleanup(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->cleanup();
        delete ladspa; // !!!
      }

  //--------------------------------------------------
  //
  //--------------------------------------------------

  public:

    axFormatLadspa(axContext* aContext, int aFormatFlags=ff_None)
    : axFormatBase(aContext,aFormatFlags)
      {
        //trace("axFormatLadspa::constructor");
        mDescriptor.UniqueID            = 0;//mUniqueId;
        mDescriptor.Label               = mLabel;//"label";
        mDescriptor.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        mDescriptor.Name                = mName;//"name";
        mDescriptor.Maker               = mMaker;//"maker";
        mDescriptor.Copyright           = mCopyright;//"copyright";
        mDescriptor.PortCount           = 0;//mNumPorts;
        mDescriptor.PortDescriptors     = NULL;//mPortDescr;
        mDescriptor.PortNames           = NULL;//mPortNames;
        mDescriptor.PortRangeHints      = NULL;//mPortRange;
        mDescriptor.ImplementationData  = this;
        mDescriptor.instantiate         = lad_instantiate;
        mDescriptor.connect_port        = lad_connect_port;
        mDescriptor.activate            = lad_activate;
        mDescriptor.run                 = lad_run;                  // ala processReplacing
        mDescriptor.run_adding          = NULL;//lad_run_adding;           // ala process, optional
        mDescriptor.set_run_adding_gain = NULL;//lad_set_run_adding_gain;  // if above
        mDescriptor.deactivate          = lad_deactivate;
        mDescriptor.cleanup             = lad_cleanup;

        axMemset( mParamPrev, 0, sizeof(mParamPrev));

      }

    //----------

    virtual ~axFormatLadspa()
      {
        for (int i=0; i<mNumPorts; i++) axFree( mPortNames[i] );
      }

    //--------------------------------------------------

    inline LADSPA_Descriptor* getDescriptor(void)
      {
        trace("getDescriptor");
        return &mDescriptor;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual LADSPA_Handle instantiate(unsigned long SampleRate)
      {
        trace("instantiate");
        mSampleRate = SampleRate;
        doStateChange(fs_Open);
        return this;
      }

    //----------

    // this is being called every block (i think) by jost!
    // probably to allow modular/dynamic connections

    virtual void connect_port(unsigned long Port, LADSPA_Data* DataLocation)
      {
        //trace("connect_port");
        unsigned int io = mNumInputs + mNumOutputs;
        if (Port<io) // audio in/out
        {
          //TODO: don't hardcode ports!!!
          switch (Port)
          {
            case 0: mInputs[0]  = DataLocation; break;
            case 1: mInputs[1]  = DataLocation; break;
            case 2: mOutputs[0] = DataLocation; break;
            case 3: mOutputs[1] = DataLocation; break;
          }
        }
        else // parameter
        {
          int po = Port - io;
          mParamPtr[po] = DataLocation;
        }
        //trace("connect_port finished");
      }

    //----------

    virtual void activate(void)
      {
        trace("activate");
        doStateChange(fs_Resume);
      }

    //----------

    //TODO: copy from axFormatVst
    //TODO: don't hardcode num in/out (see vst/multi)

    virtual void run(unsigned long SampleCount)
      {
        //trace("run");
        //chack parameters for changes
        int io  = mNumInputs + mNumOutputs;
        int par = mParameters.size();
        for (int i=0; i<par; i++)
        {
          float val = *mParamPtr[i];
          if (val!=mParamPrev[i])
          {
            mParameters[i]->doSetValue(val,true);
            mParamPrev[i] = val;
          }
        }

        mBlockSize = SampleCount;

        if ( !doProcessBlock(mInputs,mOutputs,mBlockSize) )
        {
          float* ins[2];
          float* outs[2];
          ins[0]  = mInputs[0];
          ins[1]  = mInputs[1];
          outs[0] = mOutputs[0];
          outs[1] = mOutputs[1];
          trace(SampleCount);
          int num = SampleCount;
          while (--num >= 0)
          {
            doProcessSample(ins,outs);
            ins[0]++;   ins[1]++;
            outs[0]++;  outs[1]++;
          } //SampleCount
        } //process_block
        doPostProcess(mInputs,mOutputs,mBlockSize);

      }

    //----------

    //virtual void run_adding(unsigned long SampleCount)
    //  {
    //    //trace("run_adding");
    //  }

    //----------

    //virtual void set_run_adding_gain(LADSPA_Data Gain)
    //  {
    //    //trace("run_adding_gain");
    //  }

    //----------

    virtual void deactivate(void)
      {
        trace("deactivate");
        doStateChange(fs_Suspend);

      }

    //----------

    virtual void cleanup(void)
      {
        trace("cleanup");
        doStateChange(fs_Close);
      }

    //--------------------------------------------------
    // axFormatBase
    //--------------------------------------------------

    virtual void describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        trace("describe");
        axStrncpy(mName,aProduct.ptr(),64);
        axStrncpy(mLabel,aName.ptr(),64);
        axStrncpy(mMaker,aVendor.ptr(),64);
        axStrcpy( mCopyright,(char*)"copyright?");
        mDescriptor.UniqueID = aID;
      }

    //----------

    virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      {
        trace("setupAudio");
        mNumInputs  = aInputs;
        mNumOutputs = aOutputs;
      }

    //----------

    //virtual void setupEditor(int aWidth, int aHeight) {}

    //----------

    virtual void setupParameters(void)
      {
        trace("setupParameters");
        prepareParameters();
        //transferParameters();
      }

    //----------

    virtual void prepareParameters(void)
      {
        trace("prepareParameters");
        int io = mNumInputs + mNumOutputs;
        int par = mParameters.size();
        mNumPorts = io+par;
        trace("mNumPorts: "<<mNumPorts);

        char temp[16];
        int po = 0;

        for (int i=0; i<mNumInputs; i++)
        {
          mPortNames[po] = (char*)axMalloc(16);
          axStrcpy( mPortNames[po],"input ");
          axStrcat( mPortNames[po], axItoa(temp,i) );
          mPortDesc[po]                 = LADSPA_PORT_AUDIO
                                        | LADSPA_PORT_INPUT;
          mPortHint[po].HintDescriptor  = LADSPA_HINT_DEFAULT_NONE;
          mPortHint[po].LowerBound      = 0;
          mPortHint[po].UpperBound      = 1;
          po++;
        }
        for (int i=0; i<mNumOutputs; i++)
        {
          mPortNames[po] = (char*)axMalloc(16);
          axStrcpy( mPortNames[po],"output ");
          axStrcat(mPortNames[po], axItoa(temp,i) );
          mPortDesc[po]                 = LADSPA_PORT_AUDIO
                                        | LADSPA_PORT_OUTPUT;
          mPortHint[po].HintDescriptor  = LADSPA_HINT_DEFAULT_NONE;
          mPortHint[po].LowerBound      = 0;
          mPortHint[po].UpperBound      = 1;
          po++;
        }
        for (int i=0; i<par; i++)
        {
          mPortNames[po] = (char*)axMalloc(16);
          axStrcpy( mPortNames[po], mParameters[i]->getName().ptr() );
          mPortDesc[po]                 = LADSPA_PORT_CONTROL
                                        | LADSPA_PORT_INPUT;
          mPortHint[po].HintDescriptor  = LADSPA_HINT_BOUNDED_BELOW
                                        | LADSPA_HINT_BOUNDED_ABOVE
                                        | LADSPA_HINT_DEFAULT_MINIMUM;
          mPortHint[po].LowerBound      = 0;
          mPortHint[po].UpperBound      = 1;
          po++;
        }
        mDescriptor.PortCount       = mNumPorts;
        mDescriptor.PortDescriptors = mPortDesc;
        mDescriptor.PortNames       = (const char * const *)mPortNames;
        mDescriptor.PortRangeHints  = mPortHint;
        trace("prepareParameters finished");
      }

    //----------

    // this can not be run in the constructor!
    // connect_port must have been called, so we
    // have the pointers...

    virtual void transferParameters(void)
      {
        trace("transferParameters");
        int par = mParameters.size();
        for (int i=0; i<par; i++)
        {
          float val = mParameters[i]->getValue(); //doGetValue();
          *mParamPtr[i] = val;
        }
      }

    //----------

    //virtual void notifyParamChanged(axParameter* aParameter) {}
    //virtual void notifyResizeEditor(int aWidth, int aHeight) {}
    //virtual void updateTimeInfo(void) {}
    //virtual void updateSampleRate(void) {}
    //virtual void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

};

typedef axFormatLadspa axFormatImpl;

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

  #define AX_CONTEXT_INIT(name) \
    axContext ctx(0,0,0);

  //#define AX_CONTEXT_EXIT
  
    #ifdef AX_WIN32          
 
      static __thread HINSTANCE gInstance;
      #define __AXDLLMAIN                                             \
        extern "C"                                                    \
        BOOL APIENTRY                                                 \
        DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved)   \
        {                                                             \
          gInstance = hModule;                                        \
          return TRUE;                                                \
        }
    #else
      #define __AXDLLMAIN      
    #endif
    
  #define AX_ENTRYPOINT(plugclass)                                    \
    __AXDLLMAIN                                                       \
                                                                      \
    __dllexport                                                       \
    const LADSPA_Descriptor* ladspa_descriptor (unsigned long Index)  \
    {                                                                 \
      AX_CONTEXT_INIT(plugclass)                                      \
      plugclass* plug = new plugclass(&ctx);                          \
      LADSPA_Descriptor* descriptor = plug->getDescriptor();          \
      return descriptor;                                              \
    }

/*
  #define MAKESTRING2(s) #s
  #define MAKESTRING(s) MAKESTRING2(s)
  #define MAKE_NAME(name) MAKESTRING(name) "_window"

  #define AX_CONTEXT_INIT(name)                     \
    HINSTANCE  instance = gInstance;                \
    char*      winname  = (char*)MAKE_NAME(name);   \
    AX_PTRCAST audio    = (AX_PTRCAST)audioMaster;  \
    axContext ctx(instance,winname,audio);
    */

//----------------------------------------------------------------------
#endif
