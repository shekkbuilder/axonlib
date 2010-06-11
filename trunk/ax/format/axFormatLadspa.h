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
/*

*/
//----------------------------------------------------------------------

#include "format/axFormatBase.h"
#include "../extern/ladspa.h"

#ifndef NULL
  #define NULL 0
#endif

//struct LADSPA_PortRangeHint
//{
//  LADSPA_PortRangeHintDescriptor HintDescriptor;
//  LADSPA_Data LowerBound;
//  LADSPA_Data UpperBound;
//};

#define MAX_LADSPA_PORTS 256

class axFormatLadspa : public axFormatBase
{
  private:
    LADSPA_Descriptor     mDescriptor;
    int                   mNumPorts;
    char*                 mPortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor mPortDescr[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  mPortRange[MAX_LADSPA_PORTS];

  //--------------------------------------------------
  private:
  //--------------------------------------------------

    // host callback functions

    //static LADSPA_Handle l_instantiate(const struct _LADSPA_Descriptor * Descriptor, unsigned long SampleRate)
    static LADSPA_Handle lad_instantiate(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Descriptor->ImplementationData;
        return ladspa->instantiate(SampleRate);
      }

    static void lad_connect_port(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->connect_port(Port,DataLocation);
      }

    static void lad_activate(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->activate();
      }

    static void lad_run(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->run(SampleCount);
      }

    static void lad_run_adding(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->run_adding(SampleCount);
      }

    static void lad_set_run_adding_gain(LADSPA_Handle Instance, LADSPA_Data Gain)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->set_run_adding_gain(Gain);
      }

    static void lad_deactivate(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->deactivate();
      }

    static void lad_cleanup(LADSPA_Handle Instance)
      {
        axFormatLadspa* ladspa = (axFormatLadspa*)Instance;
        ladspa->cleanup();
      }

  //--------------------------------------------------
  public:
  //--------------------------------------------------

    axFormatLadspa(axContext* aContext, int aFormatFlags=ff_None)
    : axFormatBase(aContext,aFormatFlags)
      {
        mDescriptor.UniqueID            = 0;
        mDescriptor.Label               = "label";
        mDescriptor.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        mDescriptor.Name                = "name";
        mDescriptor.Maker               = "maker";
        mDescriptor.Copyright           = "copyright";
        mDescriptor.PortCount           = mNumPorts;
        mDescriptor.PortDescriptors     = mPortDescr;
        mDescriptor.PortNames           = mPortNames;
        mDescriptor.PortRangeHints      = mPortRange;
        mDescriptor.ImplementationData  = this;
        mDescriptor.instantiate         = lad_instantiate;
        mDescriptor.connect_port        = lad_connect_port;
        mDescriptor.activate            = lad_activate;
        mDescriptor.run                 = lad_run;
        mDescriptor.run_adding          = lad_run_adding;
        mDescriptor.set_run_adding_gain = lad_set_run_adding_gain;
        mDescriptor.deactivate          = lad_deactivate;
        mDescriptor.cleanup             = lad_cleanup;
      }

    //----------

    virtual ~axFormatLadspa()
      {
      }

    //--------------------------------------------------

    inline LADSPA_Descriptor* getDescriptor(void)
      {
        return &mDescriptor;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    // instantiates a plugin. A handle is returned indicating the new
    // plugin instance. This function must return NULL if instantiation
    // fails.
    // Note that instance initialisation should generally occur in
    // activate() rather than here. */

    virtual LADSPA_Handle instantiate(unsigned long SampleRate)
      {
        trace("instantiate");
        return this;
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

    virtual void connect_port(unsigned long Port, LADSPA_Data * DataLocation)
      {
        trace("connect_port");
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

    virtual void activate(void)
      {
        trace("activate");
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

    virtual void run(unsigned long SampleCount)
      {
        //trace("run");
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

    virtual void run_adding(unsigned long SampleCount)
      {
        //trace("run_adding");
      }

  /* This method is a function pointer that sets the output gain for
     use when run_adding() is called (see above). If this function is
     never called the gain is assumed to default to 1. Gain
     information should be retained when activate() or deactivate()
     are called.

     This function should be provided by the plugin if and only if the
     run_adding() function is provided. When it is absent this
     function pointer must be set to NULL. */

    virtual void set_run_adding_gain(LADSPA_Data Gain)
      {
        //trace("run_adding_gain");
      }

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

    virtual void deactivate(void)
      {
        trace("deactivate");
      }

  /* Once an instance of a plugin has been finished with it can be
     deleted using the following function. The instance handle passed
     ceases to be valid after this call.

     If activate() was called for a plugin instance then a
     corresponding call to deactivate() must be made before cleanup()
     is called. */

    virtual void cleanup(void)
      {
        trace("cleanup");
      }

};

//----------------------------------------------------------------------

// host calls this to get a LADSPA_Descriptor for our plugin...

const LADSPA_Descriptor* ladspa_descriptor(unsigned long Index)
{
  trace("ladspa_descriptor");
  axFormatLadspa* plugin = new axFormatLadspa(/*axContext* aContext*/);
  LADSPA_Descriptor* descriptor = plugin->getDescriptor();
  return descriptor;
}

//----------------------------------------------------------------------
#endif
