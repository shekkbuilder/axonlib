/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

//hardcoded, until we can be sure it is setup everywhere else (plugins)
#define AX_PLUGIN_VST

//TODO: dirty parameters?
//TODO: move vst-specific overloaded funcs to axPluginVst

/**
  \file axPlugin.h
  \brief main plugin class
*/

#ifndef axPlugin_included
#define axPlugin_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "axString.h"
#include "axParameter.h"

//----------

// plugin flags
#define pfl_HasEditor 1

// state
#define pst_Open    1
#define pst_Close   2
#define pst_Suspend 3
#define pst_Resume  4

// transport
#define ptr_changed       1
#define ptr_playing       2
#define ptr_cycle         4
#define ptr_recording     8
#define ptr_autowriting   64
#define ptr_auto reading  128


//AX_NUMPROGS should a minimum of 1 ?,
//otherwise zero-length arrays in axPluginVst.h
#if (AX_NUMPROGS == 0)
	#undef AX_NUMPROGS
	#define AX_NUMPROGS 1
#endif

//----------------------------------------------------------------------

/// main plugin base
/**
  if you are coding plugins, this is the major class to look at. just override the methods you need, and you're almost ready to go.
*/

class axPluginBase
{
  public:

    /// setup plugin information
    /**
      descript your plugin to the host. do this once only, in your plugin constructor.
      \param aEffect  the name of your plugin
      \param aVendor  the author, plugin-writer
      \param aProduct product string (haven't seen this being used by any host...)
      \param aVersion plugin version number
      \param aID      plugin unique id (for vst)
    */
    virtual void describe(axString aEffect, axString aVendor, axString aProduct, int aVersion, unsigned int aID) {}

    /// gui?
    /**
      call this in your plugin constructor if your plugin has a gui.
      \param aWidth the initial width of the editor
      \param aHeight the initial height of the editor
    */
    virtual void hasEditor(int aWidth, int aHeight) {}
    /// send midi out
    /**
      call this to send midi out of the plugin.
      note that midi input 'swallows' the midi events, so you need to send them out again if you need or want midi thru.
      call this in doProcessBlock or doProcessSample. events are cached and sent all at once to the host after audio block processing.
    */
    virtual void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

    /// get host sync and tme info
    /**
      update the internal time, sync, .. variables.
      if you #define AX_AUTOSYNC somewhere, this will be called automatically just before every audio block
      (and the doProcessTransport is called if needed).
    */
    virtual void updateTimeInfo(void) {}

    //----------

    /// plugin state has changed
    /**
      \param aState one of pst_Open, pst_Close, pst_Suspend, pst_Resume
    */
    virtual void  doProcessState(int aState) {}

    /// host transport has changed
    /**
      if you have #defined AX_AUTOSYNC, this will be called before every audio block if transport state has changed.
    */
    virtual void  doProcessTransport(int aState) {}

    /// midi input
    /**
      all midi input is processed at the start of each audio block. for every midi event, this will be called
      \param ofs the events offset into the upcoming audio block (doProcessBlock), in samples
      \param msg1 midi message 1
      \param msg2 midi message 2
      \param msg3 midi message 3
    */
    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

    /// parameter has changed
    /**
      called every time a parameter has changed, either from tweaking a widget, or from automation from the host.
      note that this can be called anytime, even when you're in the middle of doProcessBlock, or somewhere inbetween
      doProcessSample calls. so, yu might need soee caching, or mutexing, depending on what you do with the variables.
      pointer juggling might be dangerous,
      \param aParameter the parameter that has changed
    */
    virtual void  doProcessParameter(axParameter* aParameter) {}

    /// process audio buffer
    /**
      process the given audio buffer.
      \param inputs the input channels (usually 0 or 2)
      \param outputs the outputs channels (usually 2)
      \param sampleFrames number of samples to process
      \return return true if you processed the block, and no other processing is necessary.
              return false if doProcessSample should be called for every sample in the block
    */
    virtual bool  doProcessBlock(float** inputs, float** outputs, long sampleFrames) { return false; }

    /// process one sample
    /**
      called for every sample if doProcessBlock returned false.
      if you don't override this method, the default is to just copy the input to the output
      \param ins read your sample to process from here
      \param outs and write them here
    */
    virtual void  doProcessSample(float** ins, float** outs) { *outs[0]=*ins[0]; *outs[1]=*ins[1]; }

    /// post process buffer
    /**
      called after all other processing is finished. your chance to do any post-processing, if necessary.
      called with the same parameters as doProcessBlock.
      \param inputs the input channels
      \param outputs the outputs channels now with processed samples
      \param sampleFrames number of samples
    */
    virtual void  doPostProcess(float** inputs, float** outputs, long sampleFrames) {}

    /// create editor
    /**
      if you called hasEditor in your constructor, this will be called th create the editor, before the plugin window is shown.
      \return pointer to window (cast to void*)
    */
    virtual void* doCreateEditor() { return NULL; }

    /// destroy editor
    /**
      destroy the editor
    */
    virtual void  doDestroyEditor(void) {}

    /// idle editor
    /**
      this is called 20-30 times per second, depending on the host.
      (if you have not #define AX_NODIRTYWIDGETS, you should redraw the dirty-widgets list here)
    */
    virtual void  doIdleEditor(void) {}
};

//----------------------------------------------------------------------

#ifdef AX_PLUGIN_VST
  #include "axPluginVst.h"
#endif

//----------------------------------------------------------------------

/// axPlugin
/**
*/

class axPlugin  : public axPluginImpl,
                  public axParameterListener
{
  //public:
  protected:
  //axMutex       mMutex_params;
    int           mPlugFlags;
    axParameters  mParameters;
    axString      mName;

  public:

    /// constructor
    /**
      calls axInitialize(aPlugFlags)
      \param audioMaster audioMaster (vst)
      \param numProgs number of programs
      \param numParams number of parameters
      \param aPlugFlags calls axInitialize with these flags
    */
    axPlugin(audioMasterCallback audioMaster,long numProgs,long numParams, int aPlugFlags=0)
    : axPluginImpl(audioMaster,numProgs,numParams)
      {
        mPlugFlags = aPlugFlags;
        axInitialize(mPlugFlags);   // os/platform specific initialization
// move to vst
//        canProcessReplacing();      // need this for vst sdk 2.4
//        setNumInputs(2);            // defaults to 2 inputs & outputs
//        setNumOutputs(2);           // aka stereo effect
        mParameters.clear();
        mName = "?";
      }

    //----------

    /// destructor
    /**
      parameters are automaticlly deleted. axCleanup is called.
    */
    virtual ~axPlugin()
      {
        deleteParameters();
        axCleanup(mPlugFlags);
        //TRACE("axPlugin destructor ok\n");
      }

    //--------------------------------------------------
    // programs vs params
    //--------------------------------------------------

    /// select a program
    /**
      switches the current program (set of parameters).
      copies the current parameters into the program memory, and updates all parameters from the new program.
      \param program  program to switch to
    */

    virtual void setProgram(VstInt32 program)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
        mCurProg = program;
        for (int i=0; i<AX_NUMPARAMS; i++) mParameters[i]->doSetValue( mPrograms[mCurProg][i] );
      }

    /// save program
    /**
      saves all parameters to program memory
    */

    virtual void saveProgram(void)
      {
        for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
      }

    /// duplicate program
    /**
      duplicates the parameters for the first program [0] to all other programs
      for simple initializing to a blank state
    */

    virtual void duplicatePrograms(void)
      {
        for (int i=1; i<AX_NUMPROGS; i++)
        {
          strcpy( mProgramNames[i],mProgramNames[0]);
          for (int j=0; j<AX_NUMPARAMS; j++) mPrograms[i][j] = mPrograms[0][j];
        }

      }

    //--------------------------------------------------
    // plugin handler
    //--------------------------------------------------

    /// add parameter
    /**
      append parameter to the end of the parameter-list.if listener is NULL, it is set to 'this'
      \param aParameter the parameter to append
    */

    void appendParameter( axParameter* aParameter )
      {
        if (aParameter->getListener()==NULL) aParameter->setListener(this);
        mParameters.append( aParameter );
      }

    //----------

    /// delete parameters
    /**
      deletes all parameters (if any)
      will automatically be called from the destructor
    */

    void deleteParameters(void)
      {
        for( int i=0; i<mParameters.size(); i++) delete mParameters[i];
        mParameters.clear();
      }

    //----------

    /// process parameters
    /**
      process all parameters (ex. for initial setup). will call doProcessParameter for all parameters.
      do this after you have created and appended all parameters, to feed-forward the parameter values
      to your internal variables.
    */

    void processParameters(void)
      {
        for( int i=0; i<mParameters.size(); i++ ) doProcessParameter( mParameters[i] );
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    /// get parameter name
    /**
      get name of parameter, "lowpass", "size", ..
      \param index index of parameter
      \param text pointer to (pre-allocated) buffer to copy string to
    */

    virtual void getParameterName(VstInt32 index, char* text)
      {
        mParameters[index]->doGetName(text);
      }

    //----------

    /// get parameter label
    /**
      get label of parameter, "db", "ms", ..
      \param index index of parameter
      \param label pointer to (pre-allocated) buffer to copy string to
    */


    virtual void getParameterLabel(VstInt32 index, char* label)
      {
        mParameters[index]->doGetLabel(label);
      }

    //----------

    /// get parameter display
    /**
      get text representaion of internal value, for displauy on gui...
      \param index index of parameter
      \param text pointer to (pre-allocated) buffer to copy string to
    */

    virtual void getParameterDisplay(VstInt32 index, char* text)
      {
        mParameters[index]->doGetDisplay(text);
      }

    //----------

    /// set parameter value
    /**
      set parameter, value = 0..1
      \param index index of parameter
      \param value value (0..1) to set
    */


    virtual void setParameter(VstInt32 index, float value)
      {
        mParameters[index]->doSetValue(value);
      }

    //----------

    /// get parameter value
    /**
      get parameter value (0..1)
      \param index index of parameter
      \return parameter value (0..1)
    */


    virtual float getParameter(VstInt32 index)
      {
        return mParameters[index]->doGetValue();
      }

    //--------------------------------------------------
    // parameter listener
    //--------------------------------------------------

    /// onChange handler
    /**
      called when parameter has changed. default action is to just call doProcessParameter.
      if you have a gui/editor, you need to override this one, to redraw the affected widgets.
      \param aParameter the parameter that has changed
    */

    virtual void onChange(axParameter* aParameter)
      {
        doProcessParameter(aParameter);
      }

    //----------------------------------------
    // widget listener
    //----------------------------------------

};

//----------------------------------------------------------------------
#endif
