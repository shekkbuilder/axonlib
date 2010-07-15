#error deprecated!
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

#ifndef axFormatVst_included
#define axFormatVst_included
//----------------------------------------------------------------------
//TODO: remove temporary docs & info from the vst sdk

//#include "extern/vstsdk24/aeffect.h"
//#include "extern/vstsdk24/aeffectx.h"
#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

#include "platform/axContext.h"
#include "core/axRect.h"
//#include "base/axFormatBase.h"
#include "format/axFormatBase.h"
//#include "gui/axWindow.h"

//----------------------------------------------------------------------

#define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGTHREAD|AX_WIN_EMBEDDED)

//----------------------------------------------------------------------

//#define MAX_CHANS 64
#define MAX_MIDI_SEND 1024

struct axVstEvents
{
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[MAX_MIDI_SEND];
};

//----------------------------------------------------------------------

  //VstStringConstants
  //  kVstMaxProgNameLen    24
  //  kVstMaxParamStrLen    8
  //  kVstMaxVendorStrLen   64
  //  kVstMaxProductStrLen  64
  //  kVstMaxEffectNameLen  32

//----------------------------------------------------------------------

class axFormatVst : public axFormatBase
{
  friend AEffect* main_plugin(audioMasterCallback audioMaster);
  #ifdef linux
    friend int    main(audioMasterCallback audioMaster);
  #endif
  #ifdef WIN32
    friend int    main(int audioMaster, char** empty);
  #endif

  private:
    axHostInfo    mHostInfo;
    audioMasterCallback audioMaster;
    AEffect       aeffect;
    ERect         rect;
    VstTimeInfo*  mTimeInfo;
    axVstEvents   mMidiEventList;
    VstEvent      mMidiEvents[MAX_MIDI_SEND];
    axContext     mContext;
  private:
    char          mEffectName[kVstMaxEffectNameLen];
    char          mVendorString[kVstMaxVendorStrLen];
    char          mProductString[kVstMaxProductStrLen];
    int           mVendorVersion;
  //protected:
    //char          mProgramName[kVstMaxProgNameLen];
    //int           mNumPrograms;
    VstInt32      mCurrentProgram;
    int           mPlayState;
    double        mSamplePos;
    double        mSampleRate;
    double        mBeatPos;
    double        mTempo;
    long          mBlockSize;

  public:

    virtual int     getPlayState(void)      { return mPlayState; }
    virtual double  getSamplePos(void)      { return mSamplePos; }
    virtual double  getSampleRate(void)     { if (mSampleRate==0) updateSampleRate(); return mSampleRate; }
    virtual double  getBeatPos(void)        { return mBeatPos; }
    virtual double  getTempo(void)          { return mTempo; }
    virtual int     getCurrentProgram(void) { return mCurrentProgram; }

  private:

    //----------------------------------------
    // callbacks (host->plugin)
    //----------------------------------------

    // called from vst-host via pointers we set in AEffect

    static VstIntPtr dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        axFormatVst* plug = (axFormatVst*)(ae->object);
        if (opCode==effClose)
        {
          plug->dispatcher(opCode,index,value,ptr,opt);
          delete plug;
          return 1;
        }
        return plug->dispatcher(opCode,index,value,ptr,opt);
      }

    //----------

    static float getParameter_callback(AEffect* ae, VstInt32 index)
      {
        axFormatVst* plug = (axFormatVst*)(ae->object);
        return plug->getParameter(index);
      }

    //----------

    static void setParameter_callback(AEffect* ae, VstInt32 index, float value)
      {
        axFormatVst* plug = (axFormatVst*)(ae->object);
        plug->setParameter(index,value);
      }

    //----------

    static void processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        axFormatVst* plug = (axFormatVst*)(ae->object);
        plug->processReplacing(inputs,outputs,sampleFrames);
      }

    //----------

    static void processDoubleReplacing_callback(AEffect* e, double** inputs, double** outputs, VstInt32 sampleFrames)
      {
        axFormatVst* plug = (axFormatVst*)(e->object);
        plug->processDoubleReplacing(inputs,outputs,sampleFrames);
      }

  protected:

    //----------------------------------------
    // constructor
    //----------------------------------------

    // this is protected, to avoid users creating an axFormatVst themselves
    // rather they should use axFormat, which selects the correct
    // version depending on binary format and os/platform

    axFormatVst(axContext* aContext, int aFormatFlags)
    : axFormatBase(aContext, aFormatFlags)
      {
        mContext = *aContext;
        audioMaster = (audioMasterCallback)aContext->mAudio;
        mPlayState = 0;
        mSamplePos = 0;
        mSampleRate = 0;
        mBeatPos = 0;
        mTempo = 0;
        mBlockSize = 0;

        mCurrentProgram = 0;
        //appendProgram( createDefaultProgram() );
        //setNumPrograms(1);

        axStrncpy(mEffectName,"effect name",kVstMaxEffectNameLen);
        axStrncpy(mVendorString,"vendor string",kVstMaxVendorStrLen);
        axStrncpy(mProductString,"product string",kVstMaxProductStrLen);
        mVendorVersion = 666;
        mEditorOpen = false;
        mEditorRect = axRect(0,0,256,256);
        mMidiEventList.numEvents = 0;
        mMidiEventList.reserved  = 0;
        for( int i=0; i<MAX_MIDI_SEND; i++ ) mMidiEventList.events[i] = &mMidiEvents[i];
        canProcessReplacing();
        memset(&aeffect,0,sizeof(aeffect));
        aeffect.magic                   = kEffectMagic;
        aeffect.object                  = this;
        aeffect.user                    = NULL;
        aeffect.dispatcher              = dispatcher_callback;
        aeffect.setParameter            = setParameter_callback;
        aeffect.getParameter            = getParameter_callback;
        aeffect.processReplacing        = processReplacing_callback;
      //aeffect.processDoubleReplacing  = processDoubleReplacing_callback;
        aeffect.processDoubleReplacing  = NULL;
        aeffect.flags                   = effFlagsCanReplacing;
        aeffect.version                 = 1;
        aeffect.uniqueID                = AX_MAGIC;
        aeffect.numPrograms             = 0;
        aeffect.numParams               = 0;
        aeffect.numInputs               = 2;
        aeffect.numOutputs              = 2;
        aeffect.initialDelay            = 0;
      }

    //----------

    virtual ~axFormatVst()
      {
      }

    //----------------------------------------

  protected:

    inline AEffect* getInstance() { return &aeffect; }

  //private:
  protected:

    //----------------------------------------
    // AEffect flags
    //----------------------------------------

    inline void clear_aeFlags(void)                 { aeffect.flags = 0; }
    inline void clear_aeFlag(int aFlag)             { aeffect.flags &= ~aFlag; }
    inline void set_aeFlag(int aFlag)               { aeffect.flags |= aFlag; }
    inline bool get_aeFlag(int aFlag)               { return (aeffect.flags|aFlag); }
    inline void set_aeFlag(int aFlag, bool aState)  { if (aState) set_aeFlag(aFlag); else clear_aeFlag(aFlag); }

    //----------

    void canProcessReplacing(bool aState=true)  { set_aeFlag(effFlagsCanReplacing,aState); }        // tells that processReplacing() could be used. Mandatory in VST 2.4!
    void canDoubleReplacing(bool aState=true)   { set_aeFlag(effFlagsCanDoubleReplacing,aState); }  // tells that processDoubleReplacing() is implemented.
    void programsAreChunks(bool aState=false)   { set_aeFlag(effFlagsProgramChunks,aState); }              // program data is handled in formatless chunks (using getChunk-setChunks)
    void isSynth(bool aState=false)             { set_aeFlag(effFlagsIsSynth,aState); }
    void hasEditor(bool aState=false)           { set_aeFlag(effFlagsHasEditor,aState); }
    void noSoundInStop(bool aState=true)        { set_aeFlag(effFlagsNoSoundInStop,aState); }

    //----------------------------------------
    // AEffect fields (variables)
    //----------------------------------------

    inline void setUniqueID(int aID)        { aeffect.uniqueID = aID; }         // Must be called to set the plug-ins unique ID!
    inline void setNumInputs(int aNum)      { aeffect.numInputs = aNum; }       // set the number of inputs the plug-in will handle. For a plug-in which could change its IO configuration, this number is the maximun available inputs.
    inline void setNumOutputs(int aNum)     { aeffect.numOutputs = aNum; }      // set the number of outputs the plug-in will handle. For a plug-in which could change its IO configuration, this number is the maximun available ouputs.
    inline void setInitialDelay(int aDelay) { aeffect.initialDelay = aDelay; }  // use to report the plug-in's latency (Group Delay)
    inline void setVersion(int aVer)        { aeffect.version = aVer; }
    inline void setNumPrograms(int aNum)    { aeffect.numPrograms = aNum; }
    inline void setNumParams(int aNum)      { aeffect.numParams = aNum; }

    //----------------------------------------
    //
    // audioMaster (plugin -> host)
    //
    //----------------------------------------

    // returns the host's version (for example 2400 for VST 2.4)
    // [return value]: Host VST version (for example 2400 for VST 2.4)
    // @see AudioEffect::getMasterVersion

    VstInt32 getMasterVersion(void)
      {
        VstInt32 version = 1;
        if (audioMaster) version = (VstInt32)audioMaster(&aeffect,audioMasterVersion,0,0,0,0);
        if (!version)	version = 1; // old
        return version;
      }

    //----------

    // returns current unique identifier when loading shell plug-ins
    // [return value]: current unique identifier on shell plug-in
    // @see AudioEffect::getCurrentUniqueId

    VstInt32 getCurrentUniqueId()
      {
        VstInt32 id = 0;
        if (audioMaster) id = (VstInt32)audioMaster(&aeffect,audioMasterCurrentId,0,0,0,0);
        return id;
      }

    //----------

    // called after a control has changed in the editor and when the associated parameter should be automated
    // [index]: parameter index
    // [opt]: parameter value
    // @see AudioEffect::setParameterAutomated
    //
    // An important thing to notice is that if the user changes a parameter in your editor, which is
    // out of the Host's control if you are not using the default string based interface, you should
    // call setParameterAutomated (). This ensures that the Host is notified of the parameter change, which
    // allows it to record these changes for automation.

    void setParameterAutomated(VstInt32 index, float value)
      {
        //wtrace("  axFormatVst.setParameterAutomated  index: " << index << " value: " << value);
        if (audioMaster) audioMaster(&aeffect,audioMasterAutomate,index,0,0,value);
        //setParameter(index,value);
      }

    //----------

    // Give idle time to Host application,
    // e.g. if plug-in editor is doing mouse tracking in a modal loop.
    // no arguments
    // @see AudioEffect::masterIdle
    // masterIdle

    void masterIdle(void)
      {
        if (audioMaster) audioMaster(&aeffect,audioMasterIdle,0,0,0,0);
      }

    //----------

    // audioMasterGetTime,
    // [value]: request mask
    // [return value]: #VstTimeInfo* or null if not supported
    // @see VstTimeInfoFlags @see AudioEffectX::getTimeInfo

    VstTimeInfo* getTime(VstInt32 filter)
      {
        if (audioMaster)
        {
          VstIntPtr ret = audioMaster(&aeffect,audioMasterGetTime,0,filter,0,0);
          return FromVstPtr<VstTimeInfo> (ret);
        }
        return 0;
      }

    //----------

    //audioMasterProcessEvents,
    // [ptr]: pointer to #VstEvents  @see VstEvents @see AudioEffectX::sendVstEventsToHost
    // Can be called inside processReplacing.
    // param events Fill with VST events
    // return Returns \e true on success

    bool processEvents(VstEvents* events)
      {
        if (audioMaster) return audioMaster(&aeffect,audioMasterProcessEvents,0,0,events,0)==1;
        return false;
      }

    //----------

    //audioMasterIOChanged,
    // [return value]: 1 if supported
    // @see AudioEffectX::ioChanged
    //
    // The Host could call a suspend() (if the plug-in was enabled (in resume() state)) and then ask for
	  // getSpeakerArrangement() and/or check the \e numInputs and \e numOutputs and \e initialDelay and then call a
	  // resume().
	  // return true on success
	  // see also: setSpeakerArrangement(), getSpeakerArrangement()

    bool ioChanged(void)
      {
        if (audioMaster) return (audioMaster(&aeffect,audioMasterIOChanged,0,0,0,0) != 0);
        return false;
      }

    //----------

    //audioMasterSizeWindow,
    // [index]: new width
    // [value]: new height
    // [return value]: 1 if supported  @see AudioEffectX::sizeWindow

    bool sizeWindow(int aWidth, int aHeight)
      {
        if (audioMaster) return audioMaster(&aeffect,audioMasterSizeWindow,aWidth,aHeight,0,0)==1;
        return false;
      }

    //----------

    //audioMasterGetSampleRate,
    // [return value]: current sample rate
    // @see AudioEffectX::updateSampleRate

    //virtual double getSampleRate(void)
    virtual void updateSampleRate(void)
      {
        //mSampleRate = 0;
        if (audioMaster)
        {
          VstIntPtr res = audioMaster(&aeffect, audioMasterGetSampleRate, 0, 0, 0, 0);
          if (res>0) mSampleRate = (float)res;
          //trace(res);
        }
        //return mSampleRate;
      }

    //----------

    //audioMasterGetBlockSize,
    // [return value]: current block size
    // @see AudioEffectX::updateBlockSize

    //audioMasterGetInputLatency,
    // [return value]: input latency in audio samples
    // @see AudioEffectX::getInputLatency

    //audioMasterGetOutputLatency,
    // [return value]: output latency in audio samples
    // @see AudioEffectX::getOutputLatency

    //audioMasterGetCurrentProcessLevel,
    // [return value]: current process level
    // @see VstProcessLevels

    //audioMasterGetAutomationState,
    // [return value]: current automation state
    // @see VstAutomationStates

    //audioMasterOfflineStart,
    // [index]: numNewAudioFiles [value]: numAudioFiles [ptr]: #VstAudioFile*
    // @see AudioEffectX::offlineStart

    //audioMasterOfflineRead,
    // [index]: bool readSource [value]: #VstOfflineOption*
    // @see VstOfflineOption [ptr]: #VstOfflineTask*
    // @see VstOfflineTask
    // @see AudioEffectX::offlineRead

    //audioMasterOfflineWrite,
    // @see audioMasterOfflineRead
    // @see AudioEffectX::offlineRead

    //audioMasterOfflineGetCurrentPass,
    // @see AudioEffectX::offlineGetCurrentPass

    //audioMasterOfflineGetCurrentMetaPass,
    // @see AudioEffectX::offlineGetCurrentMetaPass

    //audioMasterGetVendorString,
    // [ptr]: char buffer for vendor string, limited to #kVstMaxVendorStrLen
    // @see AudioEffectX::getHostVendorString

    //audioMasterGetProductString,
    // [ptr]: char buffer for vendor string, limited to #kVstMaxProductStrLen
    // @see AudioEffectX::getHostProductString

    //audioMasterGetVendorVersion,
    // [return value]: vendor-specific version
    // @see AudioEffectX::getHostVendorVersion

    //audioMasterVendorSpecific,
    // no definition, vendor specific handling
    // @see AudioEffectX::hostVendorSpecific

    //

    //audioMasterCanDo,
    // [ptr]: "can do" string
    // [return value]: 1 for supported

    //audioMasterGetLanguage,
    // [return value]: language code
    // @see VstHostLanguage

    //audioMasterGetDirectory,
    // [return value]: FSSpec on MAC, else char*
    // @see AudioEffectX::getDirectory

    //audioMasterUpdateDisplay,
    // no arguments

    //----------

    //audioMasterBeginEdit,
    // [index]: parameter index
    // @see AudioEffectX::beginEdit
    //
    // tells the Host that if it needs to, it has to record automation data for this control.
    // param index Index of the parameter
    // Returns true on success

    bool beginEdit(VstInt32 index)
      {
        if (audioMaster) return (audioMaster(&aeffect,audioMasterBeginEdit,index,0,0,0)) ? true : false;
        return 0;
      }

    //----------

    //audioMasterEndEdit,
    // [index]: parameter index
    // @see AudioEffectX::endEdit
    //
    // notifies the Host that this control is no longer moved by the mouse.
    // param index Index of the parameter
    // Returns \e true on success

    bool endEdit(VstInt32 index)
    {
      if (audioMaster) return (audioMaster(&aeffect,audioMasterEndEdit,index,0,0,0)) ? true : false;
      return 0;
    }

    //----------

    //audioMasterOpenFileSelector,
    // [ptr]: VstFileSelect*
    // [return value]: 1 if supported
    // @see AudioEffectX::openFileSelector

    //audioMasterCloseFileSelector,
    // [ptr]: VstFileSelect*
    // @see AudioEffectX::closeFileSelector

    //----------------------------------------
    //
    // midi
    //
    //----------------------------------------

    // [internal]
    void sendMidiClear(void)
      {
        mMidiEventList.numEvents = 0;
      }

    //----------

    // [internal]
    // send all midi event in list to host
    // and clear the list
    void sendMidiAll(void)
      {
        //trace("sendMidiAll");
        int num = mMidiEventList.numEvents;
        if( num>0 )
        {
          //sendVstEventsToHost( (VstEvents*)&mMidiEventList );
          axVstEvents* events = &mMidiEventList;
          processEvents( (VstEvents*)events/*(VstEvents*)&mMidiEventList*/ );
          sendMidiClear();
          //mMidiEventList.numEvents = 0;
        }
      }

  //private:
  protected:

    //----------------------------------------
    //
    // callbacks (host -> plugin)
    //
    //----------------------------------------

    // return the value of the parameter with index
    virtual float getParameter(VstInt32 aIndex)
      {
        return mParameters[aIndex]->doGetValue();
      }

    //----------

    //Parameters are the individual parameter settings the user can adjust. A VST Host can automate these
    //parameters. Set parameter \e index to \e value.
    //Index - Index of the parameter to change
    //value - A float value between 0.0 and 1.0 inclusive
    //note: Parameter values, like all VST parameters, are declared as floats with an inclusive range of
    //0.0 to 1.0. How data is presented to the user is merely in the user-interface handling. This is a
    //convention, but still worth regarding. Maybe the VST-Host's automation system depends on this range.

    // called when a parameter changed
    virtual void setParameter(VstInt32 aIndex, float aValue)
      {
        //trace(":::::::::: setParameter");
        axParameter* par = mParameters[aIndex];
        par->doSetValue(aValue,true);
      }

    //----------

    // process 32 bit (single precision) floats (always in a resume state)
    // This process method must be provided. It takes input data, applies its pocessing algorithm, and then puts the
    // result to the output by overwriting the output buffer.
    // inputs       - An array of pointers to the data
    // outputs      - An array of pointers to where the data can be written to
    // sampleFrames - Number of sample frames to process
    // warning! Never call any Mac OS 9 functions (or other functions which call into the OS) inside your
    // audio process function! This will crash the system when your plug-in is run in MP (multiprocessor) mode.
    // If you must call into the OS, you must use MPRemoteCall () (see Apples' documentation), or
    // explicitly use functions which are documented by Apple to be MP safe. On Mac OS X read the system
    // header files to be sure that you only call thread safe functions.

//    #ifdef AX_MULTICHANNEL

      // multi-channel
//    virtual void processReplacing(float** aInputs, float** aOutputs, int aLength)
//      {
//        //sendMidiClear();
//        #ifdef AX_AUTOSYNC
//          updateTimeInfo();
//          if( mPlayState&1 ) doProcessTransport(mPlayState);
//        #endif
//        mBlockSize = sampleFrames;
//        if ( !doProcessBlock(inputs,outputs,sampleFrames) )
//        {
//          // save the pointers
//          // these are pointers to the _start_ of each channel buffer
//          // so we save them in our own variables,
//          // and increase them one by one when calling doProcessSample
//          int i;
//          float*  ins[AX_NUMINPUTS];
//          float* outs[AX_NUMOUTPUTS];
//          for( i=0; i<AX_NUMINPUTS;  i++ ) ins[i]  = inputs[i];
//          for( i=0; i<AX_NUMOUTPUTS; i++ ) outs[i] = outputs[i];
//          while (--sampleFrames >= 0)
//          {
//            doProcessSample(ins,outs);
//            for( i=0; i<AX_NUMINPUTS;  i++ ) ins[i]++;
//            for( i=0; i<AX_NUMOUTPUTS; i++ ) outs[i]++;
//          } //sampleflrames
//        } //process_block
//        doPostProcess(inputs,outputs,sampleFrames);
//        sendMidiAll();
//      }
//    #else

    // stereo
    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        //sendMidiClear();
        #ifdef AX_AUTOSYNC
          updateTimeInfo();
          if( mPlayState&1 ) doProcessTransport(mPlayState);
        #endif
        mBlockSize = sampleFrames;
        if ( !doProcessBlock(inputs,outputs,sampleFrames) )
        {
          float* ins[2];
          float* outs[2];
          ins[0]  = inputs[0];
          ins[1]  = inputs[1];
          outs[0] = outputs[0];
          outs[1] = outputs[1];
          while (--sampleFrames >= 0)
          {
            doProcessSample(ins,outs);
            ins[0]++;   ins[1]++;
            outs[0]++;  outs[1]++;
          } //sampleflrames
        } //process_block
        doPostProcess(inputs,outputs,sampleFrames);
        sendMidiAll();
      }

//    #endif

    //----------

    // process 64 bit (double precision) floats (always in a resume state)
    virtual void processDoubleReplacing(double** aInputs, double** aOutputs, VstInt32 aLength)
      {
        //TODO: need to convert input & output buffers to SPL, and back again
        //doProcessBlock((SPL**)aInputs,(SPL**)aOutputs,aLength);
      }

    //----------------------------------------
    //
    // dispatcher (host -> plugin)
    //
    //----------------------------------------

    virtual VstIntPtr dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        VstIntPtr v = 0;
        switch (opcode)
        {

          // 00
          case effOpen:

            // called when plug-in is initialized
            //trace("axFormatVst.dispatcher :: effOpen");
            doStateChange(fs_Open);
            break;

          // 01
          case effClose:

            // called when plug-in will be released
            //trace("axFormatVst.dispatcher :: effClose");
            doStateChange(fs_Close);
            break;

          // 02
          case effSetProgram:

            // set the current program
            //trace("axFormatVst.dispatcher :: effSetProgram " << (int)value);
            //if (value<numPrograms) setProgram((VstInt32)value);

            doPreProgram(mCurrentProgram);
            //#ifndef AX_NOAUTOSAVE_PROGRAMS
            //saveProgram( getCurrentProgram() );
            //#endif
            mCurrentProgram = (VstInt32)value;
            loadProgram(mCurrentProgram);
//            if (mPrograms.size()>0)
//            {
//              axProgram* prog = mPrograms[mCurrentProgram];
//              int num = mParameters.size();
//              for (int i=0; i<num; i++)
//              {
//                float val = prog->getValue(i);
//                mParameters[i]->doSetValue(val,true);
//              }
              // let plugin know the program have changed
              doSetProgram(mCurrentProgram);
//            }
            break;

          // 03
          case effGetProgram:

            // return the index to the current program
            //trace("axFormatVst.dispatcher :: effGetProgram");
            //v = getProgram();
            v = mCurrentProgram;
            break;

          // 04
          case effSetProgramName:

            // stuff the name field of the current program with name.
            // Limited to kVstMaxProgNameLen.
            //trace("axFormatVst.dispatcher :: effSetProgramName");
            //setProgramName((char*)ptr);
            //strncpy(mProgramName,(char*)ptr,kVstMaxProgNameLen);
            if (mPrograms.size() > 0)
            {
              mPrograms[mCurrentProgram]->setName( (char*)ptr );
            } else *(char*)ptr = '\0';
            break;

          // 05
          case effGetProgramName:

            // stuff name with the name of the current program.
            // Limited to kVstMaxProgNameLen.
            //trace("axFormatVst.dispatcher :: effGetProgramName");
            //getProgramName((char*)ptr);
//            strncpy((char*)ptr,mProgramName,kVstMaxProgNameLen);
            if (mPrograms.size() > 0)
            {
              strncpy( (char*)ptr, mPrograms[mCurrentProgram]->getName().ptr(), kVstMaxProgNameLen );
            } else *(char*)ptr = '\0';
            break;

          // 06
          case effGetParamLabel:

            // Stuff label with the units in which parameter index is displayed
            // (i.e. "sec", "dB", "type", etc...).
            // Limited to kVstMaxParamStrLen.
            //trace("axFormatVst.dispatcher :: effGetParamLabel");
            //getParameterLabel(index,(char*)ptr);
            mParameters[index]->doGetLabel((char*)ptr);
            break;

          // 07
          case effGetParamDisplay:

            // stuff text with a string representation of the value of parameter index.
            // ("0.5", "-3", "PLATE", etc...)
            // Limited to kVstMaxParamStrLen.
            //trace("axFormatVst.dispatcher :: effGetParamDisplay");
            //getParameterDisplay(index,(char*)ptr);
            mParameters[index]->doGetDisplay((char*)ptr);
            break;

          // 08
          case effGetParamName:

            // stuff text with the name of parameter index.
            // ("Time", "Gain", "RoomType", etc...)
            // Limited to kVstMaxParamStrLen.
            //trace("axFormatVst.dispatcher :: effGetParamName");
            //getParameterName(index,(char*)ptr);
            mParameters[index]->doGetName((char*)ptr);
            break;

          // 10
          case effSetSampleRate:

            // called when the sample rate changes (always in a suspend state)
            //trace("axFormatVst.dispatcher :: effSetSampleRate" << opt);
            //setSampleRate(opt);
            mSampleRate = opt;
            doStateChange(fs_Rate);
            break;

          // 11
          case effSetBlockSize:

            // called when the maximun block size changes (always in a suspend state).
            // note that the sampleFrames in process calls could be smaller than this block size, but NOT bigger.
            //trace("axFormatVst.dispatcher :: effSetBlockSize");
            //setBlockSize((VstInt32)value);
            mBlockSize = (VstInt32)value;
            doStateChange(fs_Block);
            break;

          // 12
          case effMainsChanged:

            // suspend: called when plug-in is switched to off
            // resume:  called when plug-in is switched to on
            //trace("axFormatVst.dispatcher :: effMainsChanged");
            //if (!value) suspend(); else resume();
            if (!value) doStateChange(fs_Suspend);
            else doStateChange(fs_Resume);
            break;

          // 13
          case effEditGetRect:

            if (mFormatFlags&ff_HasEditor)
            {
              rect.left     = mEditorRect.x;
              rect.top      = mEditorRect.y;
              rect.right    = mEditorRect.x2()+1;
              rect.bottom   = mEditorRect.y2()+1;
              //trace("effEditGetRect: " << rect.left << "," << rect.top << " : " << rect.right << "," << rect.bottom);
              *(ERect**)ptr = &rect;
              v = 1;
            }
            break;

          // 14
          case effEditOpen:

            //trace("axFormatVst.dispatcher :: effEditOpen");
            if ((mFormatFlags&ff_HasEditor) && !mEditorOpen)
            {
              {
              #ifdef AX_LINUX
                Display* disp = XOpenDisplay(NULL);
                //axAssert(disp!=NULL);
trace("XOpenDisplay = " << (int)disp);
                Window win    = (Window)ptr;
                axContext ctx(disp,win);
              #endif
              #ifdef AX_WIN32
                //HWND win = (HWND)ptr;
                //axContext ctx(win);
                axContext ctx;
                ctx.mInstance     = mContext.mInstance;
                ctx.mWinClassName = mContext.mWinClassName;
                ctx.mWindow       = (HWND)ptr;
              #endif
              mEditorWindow = doOpenEditor(&ctx);
              //mEditorWindow->reparent((int)win);
              mEditorOpen = true;
              v = 1;
              }
            }
            break;

          // 15
          case effEditClose:

            //trace("axFormatVst.dispatcher :: effEditClose");
            if ((mFormatFlags&ff_HasEditor) && mEditorOpen)
            {
              mEditorOpen = false;
              doCloseEditor();
              #ifdef AX_LINUX
                Display* disp = mEditorWindow->mDisplay;//  getDisplay();
                XCloseDisplay(disp);
              #endif
            }
            break;

          // 19
          case effEditIdle:

            //trace("axFormatVst.dispatcher :: effEditIdle");
            if ((mFormatFlags&ff_HasEditor) && mEditorOpen)
            {
              doIdleEditor();
            }
            break;

          // 22
          case DECLARE_VST_DEPRECATED (effIdentify):
            v = CCONST ('N', 'v', 'E', 'f');
            break;

          // 23
          case effGetChunk:

            // VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
            // data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
            // isPreset - true when saving a single program, false for all programs
            // note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
            //  the Host will ask for a block of memory describing the current plug-in state for saving.
            // To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
            // Alternatively, when not using chunk, the Host will simply save all parameter values.

            // host stores plug-in state. Returns the size in bytes of the chunk
            // (plug-in allocates the data array)
            //trace("axFormatVst.dispatcher :: effGetChunk");
            //v = getChunk((void**)ptr, index ? true : false);
            break;


          // 24
          case effSetChunk:

            //VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
            //data - pointer to state data (owned by Host)
            //byteSize - size of state data
            //isPreset - true when restoring a single program, false for all programs

            // host restores plug-in state
            //trace("axFormatVst.dispatcher :: effSetChunk");
            //v = setChunk(ptr, (VstInt32)value, index ? true : false);
            break;

//---

          // 25
          case effProcessEvents:

            {
              //v = processEvents ((VstEvents*)ptr);
              //doProcessEvents();
              //    //TODO: sort?
              //    // if so, stuff all events into a buffer
              //    // a) check last item offset, of later, append after previous
              //    // if before, search from start, and insert (move later)
              //    // b) sort
              //    // c) pre-scan VstEvents array
              VstEvents* ev = (VstEvents*)ptr;
              //sendMidiClear();
              int num = ev->numEvents;
              for (int i=0; i<num; i++)
              {
                VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
                if (event->type==kVstMidiType)
                {
                  doProcessMidi( event->deltaFrames, event->midiData[0], event->midiData[1], event->midiData[2] );
                } //=miditype
              } //numevents
              // sort?
            }
            v = 1;
            break;

          // 26
          case effCanBeAutomated:

            //trace("axFormatVst.dispatcher :: effCanBeAutomated");
            //v = canParameterBeAutomated (index) ? 1 : 0;
            if ( mParameters[index]->getFlags() & pf_Automate ) v = 1;
            break;

          // 27
          case effString2Parameter:

            //trace("axFormatVst.dispatcher :: effString2Parameter");
            //v = string2parameter (index, (char*)ptr) ? 1 : 0;
            break;

          // 29
          case effGetProgramNameIndexed:

            //trace("axFormatVst.dispatcher :: effGetProgramNameIndexed");
            //v = getProgramNameIndexed ((VstInt32)value, index, (char*)ptr) ? 1 : 0;
            //break;
            if (index<mPrograms.size())
            {
              strncpy( (char*)ptr, mPrograms[index]->getName().ptr(), kVstMaxProgNameLen );
              v = 1;
            }
            break;

          // 33
          case effGetInputProperties:

            // struct VstPinProperties
            // {
            //   char label[kVstMaxLabelLen];	          ///< pin name
            //   VstInt32 flags;					              ///< @see VstPinPropertiesFlags
            //   VstInt32 arrangementType;		          ///< @see VstSpeakerArrangementType
            //   char shortLabel[kVstMaxShortLabelLen];	///< short name (recommended: 6 + delimiter)
            //   char future[48];				                ///< reserved for future use
            // };

            // enum VstPinPropertiesFlags
            // {
            // 	kVstPinIsActive   = 1 << 0,		///< pin is active, ignored by Host
            // 	kVstPinIsStereo   = 1 << 1,		///< pin is first of a stereo pair
            // 	kVstPinUseSpeaker = 1 << 2		///< #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
            // };

            // enum VstSpeakerArrangementType
            // {
            // 	kSpeakerArrUserDefined = -2,///< user defined
            // 	kSpeakerArrEmpty = -1,		///< empty arrangement
            // 	kSpeakerArrMono  =  0,		///< M
            // 	kSpeakerArrStereo,			///< L R
            // 	kSpeakerArrStereoSurround,	///< Ls Rs
            // 	...
            // };

            //trace("axFormatVst.dispatcher :: effGetInputProperties");
            //v = getInputProperties (index, (VstPinProperties*)ptr) ? 1 : 0;
            {
            VstPinProperties* pin = (VstPinProperties*)ptr;
            char name[16];
            char num[16];
            name[0] = 0;
            axStrcpy(name,"input ");
            axStrcat(name, axItoa(num,index,3) );
            axStrcpy(pin->label,name);
            pin->flags = 1; // active
            if ((index&1)==0) pin->flags |= 2; // first of stereo pair
            v = 1;
            }
            break;

          // 34
          case effGetOutputProperties:

            //trace("axFormatVst.dispatcher :: effGetOutputProperties");
            //v = getOutputProperties (index, (VstPinProperties*)ptr) ? 1 : 0;
            {
            VstPinProperties* pin = (VstPinProperties*)ptr;
            char name[16];
            char num[16];
            name[0] = 0;
            axStrcpy(name,"output ");
            axStrcat(name, axItoa(num,index,3) );
            axStrcpy(pin->label,name);
            pin->flags = 1; // active
            if ((index&1)==0) pin->flags |= 2; // first of stereo pair
            v = 1;
            }
            break;

          // 35
          case effGetPlugCategory:

            // kPlugCategUnknown = 0,		  ///< Unknown, category not implemented
            // kPlugCategEffect,			    ///< Simple Effect
            // kPlugCategSynth,			      ///< VST Instrument (Synths, samplers,...)
            // kPlugCategAnalysis,			  ///< Scope, Tuner, ...
            // kPlugCategMastering,		    ///< Dynamics, ...
            // kPlugCategSpacializer,		  ///< Panners, ...
            // kPlugCategRoomFx,			    ///< Delays and Reverbs
            // kPlugSurroundFx,			      ///< Dedicated surround processor
            // kPlugCategRestoration,		  ///< Denoiser, ...
            // kPlugCategOfflineProcess,  ///< Offline Process
            // kPlugCategShell,			      ///< Plug-in is container of other plug-ins  @see effShellGetNextPlugin
            // kPlugCategGenerator,		    ///< ToneGenerator, ...

            //trace("axFormatVst.dispatcher :: effGetPlugCategory");
            //v = (VstIntPtr)getPlugCategory ();

            // is the flags set yet?
            // (or do we read them correctly?)

//            if (get_aeFlag(effFlagsIsSynth)) v = kPlugCategSynth;
//            else v = kPlugCategEffect;

            break;

          // 38
          case effOfflineNotify:

            //trace("axFormatVst.dispatcher :: effOfflineNotify");
            //v = offlineNotify ((VstAudioFile*)ptr, (VstInt32)value, index != 0);
            break;

          // 39
          case effOfflinePrepare:

            //trace("axFormatVst.dispatcher :: effOfflinePrepare");
            //v = offlinePrepare ((VstOfflineTask*)ptr, (VstInt32)value);
            break;

          // 40
          case effOfflineRun:

            //trace("axFormatVst.dispatcher :: effOfflineRun");
            //v = offlineRun ((VstOfflineTask*)ptr, (VstInt32)value);
            break;

          // 41
          case effProcessVarIo:

            //trace("axFormatVst.dispatcher :: effProcessVarIo");
            //v = processVariableIo ((VstVariableIo*)ptr) ? 1 : 0;
            break;

          // 42
          case effSetSpeakerArrangement:

            //trace("axFormatVst.dispatcher :: effSetSpeakerArrangement");
            //v = setSpeakerArrangement (FromVstPtr<VstSpeakerArrangement> (value), (VstSpeakerArrangement*)ptr) ? 1 : 0;
            break;

          // 44
          case effSetBypass:

            //trace("axFormatVst.dispatcher :: effSetBypass");
            //v = setBypass (value ? true : false) ? 1 : 0;
            //doStateChange(fs_Bypass);
            break;

          // 45
          case effGetEffectName:

            //trace("axFormatVst.dispatcher :: effGetEffectName");
            //v = getEffectName ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mEffectName);
            v = 1;
            break;

          // 47
          case effGetVendorString:

            //trace("axFormatVst.dispatcher :: effGetVendorString");
            //v = getVendorString ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mVendorString);
            v = 1;
            break;

          // 48
          case effGetProductString:

            //trace("axFormatVst.dispatcher :: effGetVendorString");
            //v = getProductString ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mProductString);
            v = 1;
            break;

          // 49
          case effGetVendorVersion:

            //trace("axFormatVst.dispatcher :: effGetVendorVersion");
            //v = getVendorVersion ();
            v = mVendorVersion;;
            break;

          // 50
          case effVendorSpecific:

            //trace("axFormatVst.dispatcher :: effVendorSpecific");
            //v = vendorSpecific (index, value, ptr, opt);
            break;

// case effVendorSpecific:
//    if (index == effGetParamDisplay && ptr)
//    {
//      if (value>=0 && value<NUM_PARAMS)
//      {
//        sprintf(ptr,"%f",opt);
//        return 0xbeef;
//      }
//    }


          // 51
          case effCanDo:

            {
              //trace("axFormatVst.dispatcher :: effCanDo");
              //v = canDo ((char*)ptr);
              char* p = (char*)ptr;
              //trace("effCanDo: '" << p << "'");
              if (!strcmp(p,"sendVstEvents"))        v=1; // plug-in will send Vst events to Host
              if (!strcmp(p,"sendVstMidiEvent"))     v=1; // plug-in will send MIDI events to Host
              if (!strcmp(p,"receiveVstEvents"))     v=1; // plug-in can receive MIDI events from Host
              if (!strcmp(p,"receiveVstMidiEvent"))  v=1; // plug-in can receive MIDI events from Host
              if (!strcmp(p,"receiveVstTimeInfo"))   v=1; // plug-in can receive Time info from Host
              //if (strcmp(ptr,"offline"))              return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
              //if (strcmp(ptr,"midiProgramNames"))     return 0; // plug-in supports function #getMidiProgramName ()
              //if (strcmp(ptr,"bypass"))               return 0; // plug-in supports function #setBypass ()
              if (!strcmp(p,"hasCockosExtensions"))  v=0xbeef0000;
              trace("effCanDo: '" << p << "' (return: " << hex << v << dec << ")");
            }
            break;

          // 52
          case effGetTailSize:

            //trace("axFormatVst.dispatcher :: effGetTailSize");
            //v = getGetTailSize ();
            break;

          //case 53://effIdle: // deprecated

            // called by: energy xt2
            //  trace("axFormatVst.dispatcher :: effIdle (deprecated)");
            //  //v = getGetTailSize ();
            //  break;

          // 56
          case effGetParameterProperties:

            //struct VstParameterProperties
            //{
            //	float stepFloat;			///< float step
            //	float smallStepFloat;		///< small float step
            //	float largeStepFloat;		///< large float step
            //	char label[kVstMaxLabelLen];///< parameter label
            //	VstInt32 flags;				///< @see VstParameterFlags
            //	VstInt32 minInteger;		///< integer minimum
            //	VstInt32 maxInteger;		///< integer maximum
            //	VstInt32 stepInteger;		///< integer step
            //	VstInt32 largeStepInteger;	///< large integer step
            //	char shortLabel[kVstMaxShortLabelLen];	///< short label, recommended: 6 + delimiter
            //	// The following are for remote controller display purposes.
            //	// Note that the kVstParameterSupportsDisplayIndex flag must be set.
            //	// Host can scan all parameters, and find out in what order
            //	// to display them:
            //	VstInt16 displayIndex;		///< index where this parameter should be displayed (starting with 0)
            //	// Host can also possibly display the parameter group (category), such as...
            //	// ---------------------------
            //	// Osc 1
            //	// Wave  Detune  Octave  Mod
            //	// ---------------------------
            //	// ...if the plug-in supports it (flag #kVstParameterSupportsDisplayCategory)
            //	VstInt16 category;			///< 0: no category, else group index + 1
            //	VstInt16 numParametersInCategory;			///< number of parameters in category
            //	VstInt16 reserved;			///< zero
            //	char categoryLabel[kVstMaxCategLabelLen];	///< category label, e.g. "Osc 1"
            //	char future[16];			///< reserved for future use
            //};
            //
            //enum VstParameterFlags
            //{
            //	kVstParameterIsSwitch				 = 1 << 0,	///< parameter is a switch (on/off)
            //	kVstParameterUsesIntegerMinMax		 = 1 << 1,	///< minInteger, maxInteger valid
            //	kVstParameterUsesFloatStep			 = 1 << 2,	///< stepFloat, smallStepFloat, largeStepFloat valid
            //	kVstParameterUsesIntStep			 = 1 << 3,	///< stepInteger, largeStepInteger valid
            //	kVstParameterSupportsDisplayIndex 	 = 1 << 4,	///< displayIndex valid
            //	kVstParameterSupportsDisplayCategory = 1 << 5,	///< category, etc. valid
            //	kVstParameterCanRamp				 = 1 << 6	///< set if parameter value can ramp up/down
            //};

            //trace("axFormatVst.dispatcher :: effGetParameterProperties");
            //v = getParameterProperties (index, (VstParameterProperties*)ptr) ? 1 : 0;
            break;

          // 58
          case effGetVstVersion:

            //trace("axFormatVst.dispatcher :: effGetVstVersion");
            //v = getVstVersion ();
            break;

// vst 2.1

          // 59
          case effEditKeyDown:

            //trace("axFormatVst.dispatcher :: effEditKeyDown");
            //if (editor)
            //{
            //  VstKeyCode keyCode = {index, (unsigned char)value, (unsigned char)opt};
            //  v = editor->onKeyDown (keyCode) ? 1 : 0;
            //}
            break;

          // 60
          case effEditKeyUp:

            //trace("axFormatVst.dispatcher :: effEditKeyUp");
            //if (editor)
            //{
            //  VstKeyCode keyCode = {index, (unsigned char)value, (unsigned char)opt};
            //  v = editor->onKeyUp (keyCode) ? 1 : 0;
            //}
            break;

          // 61
          case effSetEditKnobMode:

            //trace("axFormatVst.dispatcher :: effSetEditKnobMode");
            //if (editor)
            //  v = editor->setKnobMode ((VstInt32)value) ? 1 : 0;
            break;

          // 62
          case effGetMidiProgramName:

            //trace("axFormatVst.dispatcher :: effGetMidiProgramName");
            //v = getMidiProgramName (index, (MidiProgramName*)ptr);
            break;

          // 63
          case effGetCurrentMidiProgram:

            //trace("axFormatVst.dispatcher :: effGetCurrentMidiProgram");
            //v = getCurrentMidiProgram (index, (MidiProgramName*)ptr);
            break;

          // 64
          case effGetMidiProgramCategory:

            //trace("axFormatVst.dispatcher :: effGetMidiProgramCategory");
            //v = getMidiProgramCategory (index, (MidiProgramCategory*)ptr);
            break;

          // 65
          case effHasMidiProgramsChanged:

            //trace("axFormatVst.dispatcher :: effHasMidiProgramsChanged");
            //v = hasMidiProgramsChanged (index) ? 1 : 0;
            break;

          // 66
          case effGetMidiKeyName:

            //trace("axFormatVst.dispatcher :: effGetMidiKeyName");
            //v = getMidiKeyName (index, (MidiKeyName*)ptr) ? 1 : 0;
            break;

          // 67
          case effBeginSetProgram:

            //trace("axFormatVst.dispatcher :: effBeginSetProgram");
            //v = beginSetProgram () ? 1 : 0;
            break;

          // 68
          case effEndSetProgram:

            //trace("axFormatVst.dispatcher :: effEndSetProgram");
            //v = endSetProgram () ? 1 : 0;
            break;

// vst 2.3

          // 69
          case effGetSpeakerArrangement:

            //trace("axFormatVst.dispatcher :: effGetSpeakerArrangement");
            //v = getSpeakerArrangement (FromVstPtr<VstSpeakerArrangement*> (value), (VstSpeakerArrangement**)ptr) ? 1 : 0;
            break;

          // 70
          case effShellGetNextPlugin:

            //trace("axFormatVst.dispatcher :: effShellGetNextPlugin");
            //v = getNextShellPlugin ((char*)ptr);
            break;

          // 71
          case effStartProcess:

            //trace("axFormatVst.dispatcher :: effStartProcess");
            //v = startProcess ();
            break;

          // 72
          case effStopProcess:

            //trace("axFormatVst.dispatcher :: effStopProcess");
            //v = stopProcess ();
            break;

          // 73
          case effSetTotalSampleToProcess:

            //trace("axFormatVst.dispatcher :: effSetTotalSampleToProcess");
            //v = setTotalSampleToProcess ((VstInt32)value);
            break;

          // 74
          case effSetPanLaw:

            //trace("axFormatVst.dispatcher :: effSetPanLaw");
            //v = setPanLaw ((VstInt32)value, opt) ? 1 : 0;
            break;

          // 75
          case effBeginLoadBank:

            //trace("axFormatVst.dispatcher :: effBeginLoadBank");
            //v = beginLoadBank ((VstPatchChunkInfo*)ptr);
            break;

          // 76
          case effBeginLoadProgram:

            //trace("axFormatVst.dispatcher :: effBeginLoadProgram");
            //v = beginLoadProgram ((VstPatchChunkInfo*)ptr);
            break;

// vst 2.4

          // 77
          case effSetProcessPrecision:

            //trace("axFormatVst.dispatcher ::effSetProcessPrecision");
            //v = setProcessPrecision ((VstInt32)value) ? 1 : 0;
            break;

          // 78
          case effGetNumMidiInputChannels:

            //trace("axFormatVst.dispatcher :: effGetNumMidiInputChannels");
            //v = getNumMidiInputChannels ();
            break;

          // 79
          case effGetNumMidiOutputChannels:

            //trace("axFormatVst.dispatcher :: effGetNumMidiOutputChannels");
            //v = getNumMidiOutputChannels ();
            break;

          //default:
          //  trace("axFormatVst.dispatcher :: unknown dispatch code: " << opcode);
          //  break;

        }
        return v;
      }

  public:

    //--------------------------------------------------
    //
    // implementation of axFormatBase virtual methods
    //
    //--------------------------------------------------

    //virtual axSystemInfo* getSystemInfo(void)
    //  {
    //    return NULL;
    //  }

    //----------

    virtual axHostInfo* getHostInfo(void)
      {
        mHostInfo.name    = "unknown";
        mHostInfo.id      = 0;
        mHostInfo.format  = "vst";
        return &mHostInfo;
      }

    //----------

    virtual void describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        #ifdef AX_DEBUG
          char buf[256];
          //__builtin_sprintf(buf,"%s%s",aName.ptr(),(char*)"_debug");
          //strncpy(mEffectName,buf,kVstMaxEffectNameLen);
          buf[0] = 0;
          axStrcat(buf,aName.ptr());
          axStrcat(buf,(char*)"_debug");
          axStrncpy(mEffectName,buf,kVstMaxEffectNameLen);
        #else
          //strncpy(mEffectName,aName.ptr(),kVstMaxEffectNameLen);
          axStrncpy(mEffectName,aName.ptr(),kVstMaxEffectNameLen);
        #endif
        //strncpy(mVendorString,aVendor.ptr(),kVstMaxVendorStrLen);
        //strncpy(mProductString,aProduct.ptr(),kVstMaxProductStrLen);
        axStrncpy(mVendorString,aVendor.ptr(),kVstMaxVendorStrLen);
        axStrncpy(mProductString,aProduct.ptr(),kVstMaxProductStrLen);
        mVendorVersion = aVersion;
        setUniqueID(aID);
        //trace(buf);
      }

    //----------

    virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      {
        setNumInputs(aInputs);    // defaults to 2 inputs & outputs
        setNumOutputs(aOutputs);  // aka stereo effect
        isSynth(aIsSynth);
      }

    //----------

    virtual void setupEditor(int aWidth, int aHeight)
      {
        //hasEditor(aWidth,aHeight);
        hasEditor(true);
        mEditorRect.set(0,0,aWidth,aHeight);
        mFormatFlags |= ff_HasEditor;
      }

    //----------

    // prepareParameters + transferParameters

    virtual void setupParameters(void)
      {
        prepareParameters();
        transferParameters();
      }

    //----------

    // tells the vst host how many parameters we have
    // needs to be done in the constructor!
    // and initializes parameter-index
    // needed for parameter -> widget mapping if we have an editor

    virtual void prepareParameters(void)
      {
        int num = mParameters.size();
        setNumParams(num); // vst
        for (int i=0; i<num; i++)
        {
          axParameter* par = mParameters[i];
          par->setIndex(i);
          //doSetParameter(par);
        }
      }

    //----------

    // calls doSetParameter for all parameters
    // so that you can fetch them, and setup initial values
    // for your plugin

    virtual void transferParameters(void)
      {
        int num = mParameters.size();
        //setNumParams(num); // vst
        for (int i=0; i<num; i++)
        {
          axParameter* par = mParameters[i];
          //par->setIndex(i);
          doSetParameter(par);
        }
      }

    //----------

    virtual void setupPrograms()
      {
        int num = mPrograms.size();
        if (num>0) { setNumPrograms(num); } // vst
        else
        {
          axProgram* prog = createDefaultProgram();
          appendProgram(prog);
          setNumPrograms(1);
        }
      }

    //----------

    virtual void  notifyParamChanged(axParameter* aParameter)
      {
        //wtrace("axFormatVst.notifyParamChanged");
        int index = aParameter->getIndex();
        float value = aParameter->doGetValue();//getValue();
        //wtrace("  index: " << index << " value: " << value);
        //setParameterAutomated(aParameter->getIndex(),aParameter->getValue());
        setParameterAutomated(index,value);
      }

    //----------

    virtual void  notifyResizeEditor(int aWidth, int aHeight)
      {
        //trace("axFormatVst.notifyResizeEditor: " << aWidth << "," << aHeight);
        //axFormatBase::notifyResizeEditor(aWidth,aHeight); // editor rect
        mEditorRect.w = aWidth;
        mEditorRect.h = aHeight;
        sizeWindow(aWidth, aHeight); // vst
      }

    //----------

    virtual void updateTimeInfo(void)
      {
        //enum VstTimeInfoFlags
        //{
        //	kVstTransportChanged     = 1,		///< indicates that play, cycle or record state has changed
        //	kVstTransportPlaying     = 1 << 1,	///< set if Host sequencer is currently playing
        //	kVstTransportCycleActive = 1 << 2,	///< set if Host sequencer is in cycle mode
        //	kVstTransportRecording   = 1 << 3,	///< set if Host sequencer is in record mode
        //	kVstAutomationWriting    = 1 << 6,	///< set if automation write mode active (record parameter changes)
        //	kVstAutomationReading    = 1 << 7,	///< set if automation read mode active (play parameter changes)

        //	kVstNanosValid           = 1 << 8,	///< VstTimeInfo::nanoSeconds valid
        //	kVstPpqPosValid          = 1 << 9,	///< VstTimeInfo::ppqPos valid
        //	kVstTempoValid           = 1 << 10,	///< VstTimeInfo::tempo valid
        //	kVstBarsValid            = 1 << 11,	///< VstTimeInfo::barStartPos valid
        //	kVstCyclePosValid        = 1 << 12,	///< VstTimeInfo::cycleStartPos and VstTimeInfo::cycleEndPos valid
        //	kVstTimeSigValid         = 1 << 13,	///< VstTimeInfo::timeSigNumerator and VstTimeInfo::timeSigDenominator valid
        //	kVstSmpteValid           = 1 << 14,	///< VstTimeInfo::smpteOffset and VstTimeInfo::smpteFrameRate valid
        //	kVstClockValid           = 1 << 15	///< VstTimeInfo::samplesToNextClock valid
        //};
        //trace("updateTimeInfo");
        mTimeInfo   = getTime( kVstPpqPosValid + kVstTempoValid );
        //trace("..ok. mTimeInfo =  " << hex << mTimeInfo );
        mPlayState  = mTimeInfo->flags & 0xff;
        mSamplePos  = mTimeInfo->samplePos;
        mSampleRate = mTimeInfo->sampleRate;
        mBeatPos    = mTimeInfo->ppqPos;
        mTempo      = mTimeInfo->tempo;
      }

    //----------

    // append midi event to list
    virtual void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        int num = mMidiEventList.numEvents;
        VstMidiEvent* event = (VstMidiEvent*)( mMidiEventList.events[ num ] );
        event->type         = kVstMidiType;
        event->deltaFrames  = offset;
        event->midiData[0]  = msg1;
        event->midiData[1]  = msg2;
        event->midiData[2]  = msg3;
        event->midiData[3]  = 0;
        event->byteSize     = sizeof(VstMidiEvent);
        event->flags        = 0;
        event->noteLength   = 0;
        event->noteOffset   = 0;
        event->detune       = 0;
        mMidiEventList.numEvents+=1;
      }
};

typedef axFormatVst axFormatImpl;

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#ifdef AX_LINUX

  AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
  #define main main_plugin

  #define AX_CONTEXT_INIT(name)                     \
    Display* display = 0;                           \
    Window   window  = 0;                           \
    AX_PTRCAST audio = (AX_PTRCAST)audioMaster;     \
    axContext ctx(display,window,audio);

  /*
  #define AX_CONTEXT_EXIT                           \
    XCloseDisplay(display);
  */

  #ifdef AX_NOGUI

  #define AX_ENTRYPOINT(plugclass)                  \
    AEffect* main(audioMasterCallback audioMaster)  \
    {                                               \
      AX_CONTEXT_INIT(plugclass)                    \
      plugclass* plug = new plugclass(&ctx,ff_None);\
      if (!plug) return 0;                          \
      AEffect* ae = plug->getInstance();            \
      return ae;                                    \
    }

  #else

  #define AX_ENTRYPOINT(plugclass)                  \
    AEffect* main(audioMasterCallback audioMaster)  \
    {                                               \
      XInitThreads();                               \
      AX_CONTEXT_INIT(plugclass)                    \
      plugclass* plug = new plugclass(&ctx,ff_None);\
      if (!plug) return 0;                          \
      AEffect* ae = plug->getInstance();            \
      return ae;                                    \
    }

  #endif //AX_NOGUI

#endif

//----------------------------------------------------------------------

#ifdef AX_WIN32

  //#ifdef __GNUC__
  //  //#define __cdecl __attribute__((cdecl))
  //  //AEffect* main_plugin(audioMasterCallback audioMaster) ;//asm ("main");
  //  // /*__declspec(dllexport)*/ int main_plugin(audioMasterCallback audioMaster);// asm("main");
  //  /*__declspec(dllexport)*/ AEffect* main_plugin(audioMasterCallback audioMaster);// asm("main");
  //  #define main main_plugin
  //#endif

  // this is read only, so it should be safe?
  static HINSTANCE gInstance;

  #define MAKESTRING2(s) #s
  #define MAKESTRING(s) MAKESTRING2(s)
  #define MAKE_NAME(name) MAKESTRING(name) "_window"

  #define AX_CONTEXT_INIT(name)                     \
    HINSTANCE  instance = gInstance;                \
    char*      winname  = (char*)MAKE_NAME(name);   \
    AX_PTRCAST audio    = (AX_PTRCAST)audioMaster;  \
    axContext ctx(instance,winname,audio);

/*
  #define AX_CONTEXT_EXIT
*/

  void register_winclass(HINSTANCE aInstance)
    {
      //trace("register...");
    }

  void unregister_winclass(HINSTANCE aInstance)
    {
      //trace("unregister...");
    }

  #define AX_ENTRYPOINT(plugclass)                                          \
                                                                            \
    extern "C"                                                              \
    BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved) \
    {                                                                       \
      gInstance = hModule;                                                  \
      switch(reason)                                                        \
      {                                                                     \
        case DLL_PROCESS_ATTACH:                                            \
          register_winclass(hModule);                                       \
          break;                                                            \
        case DLL_PROCESS_DETACH:                                            \
          unregister_winclass(hModule);                                     \
          break;                                                            \
      }                                                                     \
      return TRUE;                                                          \
    }                                                                       \
                                                                            \
    int main(int audioMaster, char** empty)                                 \
    {                                                                       \
      AX_CONTEXT_INIT(plugclass)                                            \
      plugclass* plug = new plugclass(&ctx,ff_None);                        \
      if (!plug) return 0;                                                  \
      return (int)plug->getInstance();                                      \
    }

#endif

// who deletes the plugin?

//----------------------------------------------------------------------
#endif