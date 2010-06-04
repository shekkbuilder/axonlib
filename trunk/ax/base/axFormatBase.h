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

#ifndef axFormatBase_included
#define axFormatBase_included
//----------------------------------------------------------------------

#include "platform/axContext.h"
#include "core/axRect.h"
#include "axParameter.h"
#include "gui/axWindow.h"

//----------------------------------------------------------------------

// TODO: the 'p' was there because it was called axPlugin
//       change to 'f' for format
//       this can propagate through a few classes,
//       but if not done, it will be confusing..

// format flags
#define ff_None       0
#define ff_HasEditor  1

//#define AX_PLUG_DEFAULT ( pf_HasEditor )

//----------

// format states (doStateChange)
#define fs_Open     1
#define fs_Close    2
#define fs_Suspend  3
#define fs_Resume   4

// format transport (doTransportChange)
#define ft_Changed      1   // indicates that play, cycle or record state has changed
#define ft_Playing      2   // set if Host sequencer is currently playing
#define ft_CycleActive  4   // set if Host sequencer is in cycle mode
#define ft_Recording    8   // set if Host sequencer is in record mode
#define ft_AutoWriting  64  // set if automation write mode active (record parameter changes)
#define ft_AutoReading  128 // set if automation read mode active (play parameter changes)

//----------------------------------------------------------------------

struct axSystemInfo
{
  axString  os;       // win32, linux, ..
  int       cpuid;    // cpu caps, bits
  char*     cpuidstr; // cpu caps, string format
  bool      bigend;   // big endian? else little endian
};

//----------

struct axHostInfo
{
  axString name;
  int      id;
  axString format;
};

//----------------------------------------------------------------------

class axFormatBase// : public axParameterListener
{
  friend class axEditor;
  protected:
    int           mFormatFlags;
    axParameters  mParameters;
    bool          mEditorOpen;
    axRect        mEditorRect;
    axWindow*     mEditorWindow;

  protected:

    axFormatBase(axContext* aContext, int aFormatFlags)
      {
        mFormatFlags    = aFormatFlags;//0;
        mEditorOpen   = false;
        mEditorRect   = axRect(0,0,256,256);
        mEditorWindow = NULL;
        }

    virtual ~axFormatBase()
      {
      }

  public:

    inline axRect   getEditorRect(void) { return mEditorRect; }
    inline bool     isEditorOpen(void)  { return mEditorOpen; }

    virtual int     getPlayState(void)  { return 0; }
    virtual double  getSamplePos(void)  { return 0; }
    virtual double  getSampleRate(void) { return 0; }
    virtual double  getBeatPos(void)    { return 0; }
    virtual double  getTempo(void)      { return 0; }

    virtual axSystemInfo* getSystemInfo(void) { return NULL; }
    virtual axHostInfo*   getHostInfo(void)   { return NULL; }

    //--------------------------------------------------

    // call this to descript your plugin.
    // most of these are directly related to similar vst features
    virtual void  describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID) {}

    // setup audio. an effect would normally have 2 inputs and 2 outputs,
    // while a synth often has 0 inputs and 2 outputs (and the 'isSynth' flag set
    virtual void  setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false) {}

    // this both tells the framework that you have an editor, and its (initial) size
    // (doOpenEditor,doCloseEditor,coIdleEditor will be called)
    virtual void  setupEditor(int aWidth, int aHeight) {}

    // after appending your parameters (appendParameter), you should call
    // this, to tell the framework how many parameters you have,
    // and to distribute the parameter values.
    // (doSetParameter will be called for each parameter)
    virtual void  setupParameters(void) {}

    // when tweaking a widget, we need to tell the host that the value has changed,
    // so that it can automate it, or redraw something...
    virtual void  notifyParamChanged(axParameter* aParameter) {}

    // (not used yet, not finished..)
    // call this to tell the host you want to resize the editor
    // note that not all hosts support resizeable editors
    virtual void  notifyResizeEditor(int aWidth, int aHeight) {}

    // call this to get updated info about project tempo,
    // current time (in beats), samplerate, etc...
    virtual void  updateTimeInfo(void) {}

    //
    virtual void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

  protected:

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    // this will be called when the plugins is suspend/resume, and open/close
    virtual void doStateChange(int aState) {}

    // transport state has changed
    virtual void doTransportChange(int aState) {}

    // set new program
    virtual void doSetProgram(int aProgram) {}

    // parameter calue has changed, grab new value here
    virtual void __hot doSetParameter(axParameter* aParameter) {}

    // not used
    //virtual bool doProcessEvents(void) {return false;}

    // this will be called for each incoming midi message, before doProcessBlock
    virtual void __hot doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}

    // process a block of samples. return true if you did all needed work,
    // or false if doProcessSample should be called for each sample
    virtual bool __hot doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize) {return false;}

    // process one sample.
    // only called if you returned true from doProcessBlock
    virtual void __hot doProcessSample(SPL** aInputs, SPL** aOutputs) {}

    // called after current block is finished, do any eventual post-processing here
    virtual void __hot doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize) {}

    // create and open your editor
    virtual axWindow* doOpenEditor(axContext* aContext) { return NULL; }

    // close and destroy the editor
    virtual void doCloseEditor(void) {}

    // called regularly when the editor is open
    // host decides rate (some hosts is configure-able)
    virtual void doIdleEditor(void) {}

};

//----------------------------------------------------------------------
#endif
