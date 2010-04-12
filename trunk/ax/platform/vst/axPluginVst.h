#ifndef axPluginVst_included
#define axPluginVst_included
//----------------------------------------------------------------------

//#include "extern/vstsdk24/aeffect.h"
//#include "extern/vstsdk24/aeffectx.h"
#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

#include "platform/axContext.h"
#include "core/axRect.h"
#include "base/axPluginBase.h"
//#include "gui/axWindow.h"

//----------------------------------------------------------------------

#define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGTHREAD|AX_WIN_EMBEDDED)

// plugin states
#define ps_Open     1
#define ps_Close    2
#define ps_Suspend  3
#define ps_Resume   4

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





class axPluginVst : public axPluginBase
{
  friend AEffect* main_plugin(audioMasterCallback audioMaster);// asm ("main");
  friend int main(audioMasterCallback audioMaster);

  private:
    audioMasterCallback audioMaster;
    AEffect       aeffect;
    ERect         rect;
    VstTimeInfo*  mTimeInfo;
    axVstEvents   mMidiEventList;
    VstEvent      mMidiEvents[MAX_MIDI_SEND];
//    axContext ctx;//mContext;
  private:
    char        mEffectName[kVstMaxEffectNameLen];
    char        mVendorString[kVstMaxVendorStrLen];
    char        mProductString[kVstMaxProductStrLen];
    int         mVendorVersion;
  //protected:
    char        mProgramName[kVstMaxProgNameLen];
    VstInt32    mCurrentProgram;
    //float     mSampleRate;
    //VstInt32  mBlockSize;

    int           mPlayState;
    double        mSamplePos;
    double        mSampleRate;
    double        mBeatPos;
    double        mTempo;
    long          mBlockSize;

  private:





    //----------------------------------------
    // callbacks (host->plugin)
    //----------------------------------------

    static VstIntPtr dispatcher_callback(AEffect* ae, VstInt32 opCode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        axPluginVst* plug = (axPluginVst*)(ae->object);
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
        axPluginVst* plug = (axPluginVst*)(ae->object);
        return plug->getParameter(index);
      }

    //----------

    static void setParameter_callback(AEffect* ae, VstInt32 index, float value)
      {
        axPluginVst* plug = (axPluginVst*)(ae->object);
        plug->setParameter(index,value);
      }

    //----------

    static void processReplacing_callback(AEffect* ae, float** inputs, float** outputs, VstInt32 sampleFrames)
      {
        axPluginVst* plug = (axPluginVst*)(ae->object);
        plug->processReplacing(inputs,outputs,sampleFrames);
      }

    //----------

    static void processDoubleReplacing_callback(AEffect* e, double** inputs, double** outputs, VstInt32 sampleFrames)
      {
        axPluginVst* plug = (axPluginVst*)(e->object);
        plug->processDoubleReplacing(inputs,outputs,sampleFrames);
      }

  protected:

    axPluginVst(axContext* aContext, int aPluginFlags)
    : axPluginBase(aContext, aPluginFlags)
      {

        audioMaster = (audioMasterCallback)aContext->mAudio;
        mCurrentProgram = 0;
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

    virtual ~axPluginVst()
      {
      }

    //----------------------------------------

  protected:

    inline AEffect* getInstance() { return &aeffect; }

  private:
  //protected:





    //----------------------------------------
    // AEffect flags
    //----------------------------------------

    inline void clear_aeFlags(void) { aeffect.flags = 0; }
    inline void clear_aeFlag(int aFlag) { aeffect.flags |= aFlag; }
    inline void set_aeFlag(int aFlag) { aeffect.flags |= aFlag; }
    inline void set_aeFlag(int aFlag, bool aState) { if (aState) set_aeFlag(aFlag); else clear_aeFlag(aFlag); }
    //----------
    // tells that processReplacing() could be used. Mandatory in VST 2.4!
    void canProcessReplacing(bool aState=true) { set_aeFlag(effFlagsCanReplacing,aState); }
    // tells that processDoubleReplacing() is implemented.
    void canDoubleReplacing(bool aState=true) { set_aeFlag(effFlagsCanDoubleReplacing,aState); }
    // program data is handled in formatless chunks (using getChunk-setChunks)
    void programsAreChunks(bool aState=true) { set_aeFlag(effFlagsProgramChunks); }
    void isSynth(bool aState=true) { set_aeFlag(effFlagsIsSynth); }
    void hasEditor(bool aState=true) { set_aeFlag(effFlagsHasEditor); }
    void noSoundInStop(bool aState=true) { set_aeFlag(effFlagsNoSoundInStop); }

    //----------------------------------------
    // AEffect fields (variables)
    //----------------------------------------

    // Must be called to set the plug-ins unique ID!
    inline void setUniqueID(int aID) { aeffect.uniqueID=aID; }
    // set the number of inputs the plug-in will handle.
    // For a plug-in which could change its IO configuration, this number is the maximun available inputs.
    inline void setNumInputs(int aNum) { aeffect.numInputs=aNum; }
    // set the number of outputs the plug-in will handle.
    // For a plug-in which could change its IO configuration, this number is the maximun available ouputs.
    inline void setNumOutputs(int aNum) { aeffect.numOutputs=aNum; }
    // use to report the plug-in's latency (Group Delay)
    inline void setInitialDelay(int aDelay) { aeffect.initialDelay=aDelay; }
    inline void setVersion(int aVer) { aeffect.version=aVer; }
    inline void setNumPrograms(int aNum) { aeffect.numPrograms=aNum; }
    inline void setNumParams(int aNum) { aeffect.numParams=aNum; }





    //----------------------------------------
    //
    // audioMaster (plugin -> host)
    //
    //----------------------------------------

    // called after a control has changed in the editor and when the associated parameter should be automated
    // [index]: parameter index
    // [opt]: parameter value
    // @see AudioEffect::setParameterAutomated

    void setParameterAutomated(VstInt32 index, float value)
      {
        //wtrace("  axPluginVst.setParameterAutomated  index: " << index << " value: " << value);
        audioMaster(&aeffect,audioMasterAutomate,index,0,0,value);
        //setParameter(index,value);
      }

    //----------

    // returns the host's version (for example 2400 for VST 2.4)
    // [return value]: Host VST version (for example 2400 for VST 2.4)
    // @see AudioEffect::getMasterVersion

    VstInt32 getMasterVersion(void)
      {
        VstInt32 version = 1;
        version = (VstInt32)audioMaster(&aeffect,audioMasterVersion,0,0,0,0);
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
        if (audioMaster)
          id = (VstInt32)audioMaster(&aeffect,audioMasterCurrentId,0,0,0,0);
        return id;
      }

    //----------

    // give idle time to Host application
    // no arguments
    // @see AudioEffect::masterIdle
    // masterIdle

    //
    // audioMasterGetTime,
    // [return value]: #VstTimeInfo* or null if not supported [value]: request mask  @see VstTimeInfoFlags @see AudioEffectX::getTimeInfo

    VstTimeInfo* getTime(VstInt32 filter)
      {
        if (audioMaster)
        {
          VstIntPtr ret = audioMaster (&aeffect,audioMasterGetTime,0,filter,0,0);
          return FromVstPtr<VstTimeInfo> (ret);
        }
        return 0;
      }

    //audioMasterProcessEvents,
    // [ptr]: pointer to #VstEvents  @see VstEvents @see AudioEffectX::sendVstEventsToHost
    // Can be called inside processReplacing.
    // param events Fill with VST events
    // return Returns \e true on success

    bool processEvents(VstEvents* events)
      {
        if (audioMaster) return audioMaster(&aeffect,audioMasterProcessEvents,0,0,events,0)==1;
        return 0;
      }

    //audioMasterIOChanged,			            // [return value]: 1 if supported  @see AudioEffectX::ioChanged
    //audioMasterSizeWindow,			          // [index]: new width [value]: new height [return value]: 1 if supported  @see AudioEffectX::sizeWindow
    //audioMasterGetSampleRate,		          // [return value]: current sample rate  @see AudioEffectX::updateSampleRate
    //audioMasterGetBlockSize,		          // [return value]: current block size  @see AudioEffectX::updateBlockSize
    //audioMasterGetInputLatency,		        // [return value]: input latency in audio samples  @see AudioEffectX::getInputLatency
    //audioMasterGetOutputLatency,	        // [return value]: output latency in audio samples  @see AudioEffectX::getOutputLatency
    //audioMasterGetCurrentProcessLevel,	  // [return value]: current process level  @see VstProcessLevels
    //audioMasterGetAutomationState,		    // [return value]: current automation state  @see VstAutomationStates
    //audioMasterOfflineStart,			        // [index]: numNewAudioFiles [value]: numAudioFiles [ptr]: #VstAudioFile*  @see AudioEffectX::offlineStart
    //audioMasterOfflineRead,				        // [index]: bool readSource [value]: #VstOfflineOption* @see VstOfflineOption [ptr]: #VstOfflineTask*  @see VstOfflineTask @see AudioEffectX::offlineRead
    //audioMasterOfflineWrite,			        // @see audioMasterOfflineRead @see AudioEffectX::offlineRead
    //audioMasterOfflineGetCurrentPass,	    // @see AudioEffectX::offlineGetCurrentPass
    //audioMasterOfflineGetCurrentMetaPass, // @see AudioEffectX::offlineGetCurrentMetaPass
    //audioMasterGetVendorString,			      // [ptr]: char buffer for vendor string, limited to #kVstMaxVendorStrLen  @see AudioEffectX::getHostVendorString
    //audioMasterGetProductString,		      // [ptr]: char buffer for vendor string, limited to #kVstMaxProductStrLen  @see AudioEffectX::getHostProductString
    //audioMasterGetVendorVersion,		      // [return value]: vendor-specific version  @see AudioEffectX::getHostVendorVersion
    //audioMasterVendorSpecific,			      // no definition, vendor specific handling  @see AudioEffectX::hostVendorSpecific
    //audioMasterCanDo,					            // [ptr]: "can do" string [return value]: 1 for supported
    //audioMasterGetLanguage,				        // [return value]: language code  @see VstHostLanguage
    //audioMasterGetDirectory,			        // [return value]: FSSpec on MAC, else char*  @see AudioEffectX::getDirectory
    //audioMasterUpdateDisplay,			        // no arguments
    //audioMasterBeginEdit,                 // [index]: parameter index  @see AudioEffectX::beginEdit
    //audioMasterEndEdit,                   // [index]: parameter index  @see AudioEffectX::endEdit
    //audioMasterOpenFileSelector,		      // [ptr]: VstFileSelect* [return value]: 1 if supported  @see AudioEffectX::openFileSelector
    //audioMasterCloseFileSelector,		      // [ptr]: VstFileSelect*  @see AudioEffectX::closeFileSelector





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
    void sendMidiAll(void)
      {
        int num = mMidiEventList.numEvents;
        if( num>0 )
        {
          //sendVstEventsToHost( (VstEvents*)&mMidiEventList );
          processEvents( (VstEvents*)&mMidiEventList );
          sendMidiClear();
          //mMidiEventList.numEvents = 0;
        }
      }

    //----------

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





    //----------------------------------------
    //
    // callbacks (host -> plugin)
    //
    //----------------------------------------

    // return the value of the parameter with index
    virtual float getParameter(VstInt32 aIndex)
      {
        //return 0;
        return mParameters[aIndex]->doGetValue();
      }

    //----------

    // called when a parameter changed
    virtual void setParameter(VstInt32 aIndex, float aValue)
      {
        axParameter* par = mParameters[aIndex];
        par->doSetValue(aValue);
      }

    //----------

//    // process 32 bit (single precision) floats (always in a resume state)
//    virtual void processReplacing(float** aInputs, float** aOutputs, VstInt32 aLength)
//      {
//        doProcessBlock((SPL**)aInputs,(SPL**)aOutputs,aLength);
//      }

#ifdef AX_MULTICHANNEL

    // multi
    virtual void processReplacing(float** aInputs, float** aOutputs, int aLength)
      {
        //sendMidiClear();
        #ifdef AX_AUTOSYNC
          updateTimeInfo();
          if( mPlayState&1 ) doProcessTransport(mPlayState);
        #endif
        mBlockSize = sampleFrames;
        if ( !doProcessBlock(inputs,outputs,sampleFrames) )
        {
          int i;
          float*  ins[AX_NUMINPUTS];
          float* outs[AX_NUMOUTPUTS];
          for( i=0; i<AX_NUMINPUTS;  i++ ) ins[i]  = inputs[i];
          for( i=0; i<AX_NUMOUTPUTS; i++ ) outs[i] = outputs[i];
          while (--sampleFrames >= 0)
          {
            doProcessSample(ins,outs);
            for( i=0; i<AX_NUMINPUTS;  i++ ) ins[i]++;
            for( i=0; i<AX_NUMOUTPUTS; i++ ) outs[i]++;
          } //sampleflrames
        } //process_block
        doPostProcess(inputs,outputs,sampleFrames);
        sendMidiAll();
      }

#else

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
          ins[0]=inputs[0];   ins[1]=inputs[1];
          outs[0]=outputs[0]; outs[1]=outputs[1];
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

#endif


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

    //  00  effOpen
    //  01  effClose
    //  02  effSetProgram
    //  03  effGetProgram
    //  04  effSetProgramName
    //  05  effGetProgramName
    //  06  effGetParamLabel
    //  07  effGetParamDisplay
    //  08  effGetParamName
    //  09  DEPRECATED (effGetVu)
    //  10  effSetSampleRate
    //  11  effSetBlockSize
    //  12  effMainsChanged
    //  13  effEditGetRect
    //  14  effEditOpen
    //  15  effEditClose
    //  16  DEPRECATED (effEditDraw)
    //  17  DEPRECATED (effEditMouse)
    //  18  DEPRECATED (effEditKey)
    //  19  effEditIdle
    //  20  DEPRECATED (effEditTop)
    //  21  DEPRECATED (effEditSleep)
    //  22  DEPRECATED (effIdentify)
    //  23  effGetChunk,
    //  24  effSetChunk,
    //  //
    //  25  effProcessEvents
    //  26  effCanBeAutomated
    //  27  effString2Parameter
    //  28  DEPRECATED (effGetNumProgramCategories)
    //  29  effGetProgramNameIndexed
    //  30  DEPRECATED (effCopyProgram)
    //  31  DEPRECATED (effConnectInput)
    //  32  DEPRECATED (effConnectOutput)
    //  33  effGetInputProperties
    //  34  effGetOutputProperties
    //  35  effGetPlugCategory
    //  36  DEPRECATED (effGetCurrentPosition)
    //  37  DEPRECATED (effGetDestinationBuffer)
    //  38  effOfflineNotify
    //  39  effOfflinePrepare
    //  40  effOfflineRun
    //  41  effProcessVarIo
    //  42  effSetSpeakerArrangement
    //  43  DEPRECATED (effSetBlockSizeAndSampleRate)
    //  44  effSetBypass
    //  45  effGetEffectName
    //  46  DEPRECATED (effGetErrorText)
    //  47  effGetVendorString
    //  48  effGetProductString
    //  49  effGetVendorVersion
    //  50  effVendorSpecific
    //  51  effCanDo
    //  52  effGetTailSize
    //  53  DEPRECATED (effIdle)
    //  54  DEPRECATED (effGetIcon)
    //  55  DEPRECATED (effSetViewPosition)
    //  56  effGetParameterProperties
    //  57  DEPRECATED (effKeysRequired)
    //  58  effGetVstVersion
    //  // VST_2_1_EXTENSIONS
    //  59  effEditKeyDown
    //  60  effEditKeyUp
    //  61  effSetEditKnobMode
    //  62  effGetMidiProgramName
    //  63  effGetCurrentMidiProgram
    //  64  effGetMidiProgramCategory
    //  65  effHasMidiProgramsChanged
    //  66  effGetMidiKeyName
    //  67  effBeginSetProgram
    //  68  effEndSetProgram
    //  // VST_2_3_EXTENSIONS
    //  69  effGetSpeakerArrangement
    //  70  effShellGetNextPlugin
    //  71  effStartProcess
    //  72  effStopProcess
    //  73  effSetTotalSampleToProcess
    //  74  effSetPanLaw
    //  75  effBeginLoadBank
    //  76  effBeginLoadProgram
    //  // VST_2_4_EXTENSIONS
    //  77  effSetProcessPrecision
    //  78  effGetNumMidiInputChannels
    //  79  effGetNumMidiOutputChannels

    virtual VstIntPtr dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        VstIntPtr v = 0;
        switch (opcode)
        {

// --- plugin state ---

          // called when plug-in is initialized
          case effOpen:
            //trace("axPluginVst.dispatcher :: effOpen");
            doStateChange(ps_Open);
            break;

          // called when plug-in will be released
          case effClose:
            //trace("axPluginVst.dispatcher :: effClose");
            doStateChange(ps_Close);
            break;

          // suspend: called when plug-in is switched to off
          // resume:  called when plug-in is switched to on
          case effMainsChanged:
            //trace("axPluginVst.dispatcher :: effMainsChanged");
            //if (!value) suspend(); else resume();
            if (!value) doStateChange(ps_Suspend);
            else doStateChange(ps_Resume);
            break;

// --- audio state ---

          // called when the sample rate changes (always in a suspend state)
          case effSetSampleRate:
            //trace("axPluginVst.dispatcher :: effSetSampleRate");
            //setSampleRate(opt);
            mSampleRate = opt;
            break;

          // called when the maximun block size changes (always in a suspend state).
          // note that the sampleFrames in process calls could be smaller than this block size, but NOT bigger.
          case effSetBlockSize:
            //trace("axPluginVst.dispatcher :: effSetBlockSize");
            //setBlockSize((VstInt32)value);
            mBlockSize = (VstInt32)value;
            break;

          // --- programs ---

          // set the current program
          case effSetProgram:
            //trace("axPluginVst.dispatcher :: effSetProgram");
            //if (value<numPrograms) setProgram((VstInt32)value);
            mCurrentProgram = (VstInt32)value;
            doSetProgram(mCurrentProgram);
            break;

          // return the index to the current program
          case effGetProgram:
            //trace("axPluginVst.dispatcher :: effGetProgram");
            //v = getProgram();
            v = mCurrentProgram;
            break;

          // stuff the name field of the current program with name.
          // Limited to kVstMaxProgNameLen.
          case effSetProgramName:
            //trace("axPluginVst.dispatcher :: effSetProgramName");
            //setProgramName((char*)ptr);
            strncpy(mProgramName,(char*)ptr,kVstMaxProgNameLen);
            break;

          // stuff name with the name of the current program.
          // Limited to kVstMaxProgNameLen.
          case effGetProgramName:
            //trace("axPluginVst.dispatcher :: effGetProgramName");
            //getProgramName((char*)ptr);
            strncpy((char*)ptr,mProgramName,kVstMaxProgNameLen);
            break;

// --- parameters ---

          // Stuff label with the units in which parameter index is displayed
          // (i.e. "sec", "dB", "type", etc...).
          // Limited to kVstMaxParamStrLen.
          case effGetParamLabel:
            //trace("axPluginVst.dispatcher :: effGetParamLabel");
            //getParameterLabel(index,(char*)ptr);
            mParameters[index]->doGetLabel((char*)ptr);
            break;

          // stuff text with a string representation of the value of parameter index.
          // ("0.5", "-3", "PLATE", etc...)
          // Limited to kVstMaxParamStrLen.
          case effGetParamDisplay:
            //trace("axPluginVst.dispatcher :: effGetParamDisplay");
            //getParameterDisplay(index,(char*)ptr);
            mParameters[index]->doGetDisplay((char*)ptr);
            break;

          // stuff text with the name of parameter index.
          // ("Time", "Gain", "RoomType", etc...)
          // Limited to kVstMaxParamStrLen.
          case effGetParamName:
            //trace("axPluginVst.dispatcher :: effGetParamName");
            //getParameterName(index,(char*)ptr);
            mParameters[index]->doGetName((char*)ptr);
            break;

// --- editor ---

          case effEditGetRect:
            //trace("axPluginVst.dispatcher :: effEditGetRect");
            if (mPlugFlags&pf_HasEditor)
            {
              rect.left     = mEditorRect.x;
              rect.top      = mEditorRect.y;
              rect.right    = mEditorRect.x2();
              rect.bottom   = mEditorRect.y2();
              *(ERect**)ptr = &rect;
              v = 1;
            }
            //doSetupEditor();
            break;

          case effEditOpen:
            //trace("axPluginVst.dispatcher :: effEditOpen");
            if ((mPlugFlags&pf_HasEditor) && !mEditorOpen)
            {
              {
              #ifdef AX_LINUX
                Display* disp = XOpenDisplay(NULL);
                Window win    = (Window)ptr;
                axContext ctx(disp,win);
              #endif
              #ifdef AX_WIN32
                HWND win = (HWND)ptr;
                axContext ctx(win);
              #endif
              mEditorWindow = doOpenEditor(&ctx);
              mEditorOpen = true;
              v = 1;
              }
            }
            break;

          case effEditClose:
            //trace("axPluginVst.dispatcher :: effEditClose");
            if ((mPlugFlags&pf_HasEditor) && mEditorOpen)
            {
              mEditorOpen = false;
              doCloseEditor();
              #ifdef AX_LINUX
                Display* disp = mEditorWindow->mDisplay;//  getDisplay();
                XCloseDisplay(disp);
              #endif
            }
            break;

          case effEditIdle:
            //trace("axPluginVst.dispatcher :: effEditIdle");
            if ((mPlugFlags&pf_HasEditor) && mEditorOpen)
            {
              doIdleEditor();
            }
            break;

// --- load/save ---

          // host stores plug-in state. Returns the size in bytes of the chunk
          // (plug-in allocates the data array)
          case effGetChunk:
            //trace("axPluginVst.dispatcher :: effGetChunk");
            //v = getChunk((void**)ptr, index ? true : false);
            break;

          // host restores plug-in state
          case effSetChunk:
            //trace("axPluginVst.dispatcher :: effSetChunk");
            //v = setChunk(ptr, (VstInt32)value, index ? true : false);
            break;

//

// --- vst events ---

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

// --- parameters and programs ---

          case effCanBeAutomated:
            //trace("axPluginVst.dispatcher :: effCanBeAutomated");
            //v = canParameterBeAutomated (index) ? 1 : 0;
            if ( mParameters[index]->getFlags() & pf_Automate ) v = 1;
            break;
          case effString2Parameter:
            //trace("axPluginVst.dispatcher :: effString2Parameter");
            //v = string2parameter (index, (char*)ptr) ? 1 : 0;
            break;
          case effGetProgramNameIndexed:
            //trace("axPluginVst.dispatcher :: effGetProgramNameIndexed");
            //v = getProgramNameIndexed ((VstInt32)value, index, (char*)ptr) ? 1 : 0;
            break;

//--- connections, configuration ---

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

          case effGetInputProperties:
            //trace("axPluginVst.dispatcher :: effGetInputProperties");
            //v = getInputProperties (index, (VstPinProperties*)ptr) ? 1 : 0;
            break;
          case effGetOutputProperties:
            //trace("axPluginVst.dispatcher :: effGetOutputProperties");
            //v = getOutputProperties (index, (VstPinProperties*)ptr) ? 1 : 0;
            break;

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

          case effGetPlugCategory:
            //trace("axPluginVst.dispatcher :: effGetPlugCategory");
            //v = (VstIntPtr)getPlugCategory ();
            break;

//--- offline ---

          case effOfflineNotify:
            //trace("axPluginVst.dispatcher :: effOfflineNotify");
            //v = offlineNotify ((VstAudioFile*)ptr, (VstInt32)value, index != 0);
            break;
          case effOfflinePrepare:
            //trace("axPluginVst.dispatcher :: effOfflinePrepare");
            //v = offlinePrepare ((VstOfflineTask*)ptr, (VstInt32)value);
            break;
          case effOfflineRun:
            //trace("axPluginVst.dispatcher :: effOfflineRun");
            //v = offlineRun ((VstOfflineTask*)ptr, (VstInt32)value);
            break;

//--- others ---

          case effSetSpeakerArrangement:
            //trace("axPluginVst.dispatcher :: effSetSpeakerArrangement");
            //v = setSpeakerArrangement (FromVstPtr<VstSpeakerArrangement> (value), (VstSpeakerArrangement*)ptr) ? 1 : 0;
            break;
          case effProcessVarIo:
            //trace("axPluginVst.dispatcher :: effProcessVarIo");
            //v = processVariableIo ((VstVariableIo*)ptr) ? 1 : 0;
            break;
          case effSetBypass:
            //trace("axPluginVst.dispatcher :: effSetBypass");
            //v = setBypass (value ? true : false) ? 1 : 0;
            break;
          case effGetEffectName:
            //trace("axPluginVst.dispatcher :: effGetEffectName");
            //v = getEffectName ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mEffectName);
            v = 1;
            break;
          case effGetVendorString:
            //trace("axPluginVst.dispatcher :: effGetVendorString");
            //v = getVendorString ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mVendorString);
            v = 1;
            break;
          case effGetProductString:
            //trace("axPluginVst.dispatcher :: effGetVendorString");
            //v = getProductString ((char*)ptr) ? 1 : 0;
            strcpy((char*)ptr,mProductString);
            v = 1;
            break;
          case effGetVendorVersion:
            //trace("axPluginVst.dispatcher :: effGetVendorVersion");
            //v = getVendorVersion ();
            v = mVendorVersion;;
            break;
          case effVendorSpecific:
            //trace("axPluginVst.dispatcher :: effVendorSpecific");
            //v = vendorSpecific (index, value, ptr, opt);
            break;

          case effCanDo:
            {
            //trace("axPluginVst.dispatcher :: effCanDo");
            //v = canDo ((char*)ptr);
            char* p = (char*)ptr;
            wtrace("effCanDo: '" << p << "'");
            if (!strcmp(p,"sendVstEvents"))        v=1; // plug-in will send Vst events to Host
            if (!strcmp(p,"sendVstMidiEvent"))     v=1; // plug-in will send MIDI events to Host
            if (!strcmp(p,"receiveVstEvents"))     v=1; // plug-in can receive MIDI events from Host
            if (!strcmp(p,"receiveVstMidiEvent"))  v=1; // plug-in can receive MIDI events from Host
            if (!strcmp(p,"receiveVstTimeInfo"))   v=1; // plug-in can receive Time info from Host
            //if (strcmp(ptr,"offline"))              return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
            //if (strcmp(ptr,"midiProgramNames"))     return 0; // plug-in supports function #getMidiProgramName ()
            //if (strcmp(ptr,"bypass"))               return 0; // plug-in supports function #setBypass ()
            if (!strcmp(p,"hasCockosExtensions"))  v=0xbeef0000;
            //trace("- axPluginVst.canDo: '" << ptr << "' = " << hex << ret);
            }
            break;

          case effGetTailSize:
            //trace("axPluginVst.dispatcher :: effGetTailSize");
            //v = getGetTailSize ();
            break;

          // called by: energy xt2

          //case 53://effIdle:
          //  trace("axPluginVst.dispatcher :: effIdle (deprecated)");
          //  //v = getGetTailSize ();
          //  break;

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

          case effGetParameterProperties:
            //trace("axPluginVst.dispatcher :: effGetParameterProperties");
            //v = getParameterProperties (index, (VstParameterProperties*)ptr) ? 1 : 0;
            break;
          case effGetVstVersion:
            //trace("axPluginVst.dispatcher :: effGetVstVersion");
            //v = getVstVersion ();
            break;

// vst 2.1

          case effEditKeyDown:
            //trace("axPluginVst.dispatcher :: effEditKeyDown");
            //if (editor)
            //{
            //  VstKeyCode keyCode = {index, (unsigned char)value, (unsigned char)opt};
            //  v = editor->onKeyDown (keyCode) ? 1 : 0;
            //}
            break;
          case effEditKeyUp:
            //trace("axPluginVst.dispatcher :: effEditKeyUp");
            //if (editor)
            //{
            //  VstKeyCode keyCode = {index, (unsigned char)value, (unsigned char)opt};
            //  v = editor->onKeyUp (keyCode) ? 1 : 0;
            //}
            break;
          case effSetEditKnobMode:
            //trace("axPluginVst.dispatcher :: effSetEditKnobMode");
            //if (editor)
            //  v = editor->setKnobMode ((VstInt32)value) ? 1 : 0;
            break;
          case effGetMidiProgramName:
            //trace("axPluginVst.dispatcher :: effGetMidiProgramName");
            //v = getMidiProgramName (index, (MidiProgramName*)ptr);
            break;
          case effGetCurrentMidiProgram:
            //trace("axPluginVst.dispatcher :: effGetCurrentMidiProgram");
            //v = getCurrentMidiProgram (index, (MidiProgramName*)ptr);
            break;
          case effGetMidiProgramCategory:
            //trace("axPluginVst.dispatcher :: effGetMidiProgramCategory");
            //v = getMidiProgramCategory (index, (MidiProgramCategory*)ptr);
            break;
          case effHasMidiProgramsChanged:
            //trace("axPluginVst.dispatcher :: effHasMidiProgramsChanged");
            //v = hasMidiProgramsChanged (index) ? 1 : 0;
            break;
          case effGetMidiKeyName:
            //trace("axPluginVst.dispatcher :: effGetMidiKeyName");
            //v = getMidiKeyName (index, (MidiKeyName*)ptr) ? 1 : 0;
            break;
          case effBeginSetProgram:
            //trace("axPluginVst.dispatcher :: effBeginSetProgram");
            //v = beginSetProgram () ? 1 : 0;
            break;
          case effEndSetProgram:
            //trace("axPluginVst.dispatcher :: effEndSetProgram");
            //v = endSetProgram () ? 1 : 0;
            break;

// vst 2.3

          case effGetSpeakerArrangement:
            //trace("axPluginVst.dispatcher :: effGetSpeakerArrangement");
            //v = getSpeakerArrangement (FromVstPtr<VstSpeakerArrangement*> (value), (VstSpeakerArrangement**)ptr) ? 1 : 0;
            break;
          case effSetTotalSampleToProcess:
            //trace("axPluginVst.dispatcher :: effSetTotalSampleToProcess");
            //v = setTotalSampleToProcess ((VstInt32)value);
            break;
          case effShellGetNextPlugin:
            //trace("axPluginVst.dispatcher :: effShellGetNextPlugin");
            //v = getNextShellPlugin ((char*)ptr);
            break;
          case effStartProcess:
            //trace("axPluginVst.dispatcher :: effStartProcess");
            //v = startProcess ();
            break;
          case effStopProcess:
            //trace("axPluginVst.dispatcher :: effStopProcess");
            //v = stopProcess ();
            break;
          case effSetPanLaw:
            //trace("axPluginVst.dispatcher :: effSetPanLaw");
            //v = setPanLaw ((VstInt32)value, opt) ? 1 : 0;
            break;
          case effBeginLoadBank:
            //trace("axPluginVst.dispatcher :: effBeginLoadBank");
            //v = beginLoadBank ((VstPatchChunkInfo*)ptr);
            break;
          case effBeginLoadProgram:
            //trace("axPluginVst.dispatcher :: effBeginLoadProgram");
            //v = beginLoadProgram ((VstPatchChunkInfo*)ptr);
            break;

// vst 2.4

          case effSetProcessPrecision:
            //trace("axPluginVst.dispatcher ::effSetProcessPrecision");
            //v = setProcessPrecision ((VstInt32)value) ? 1 : 0;
            break;
          case effGetNumMidiInputChannels:
            //trace("axPluginVst.dispatcher :: effGetNumMidiInputChannels");
            //v = getNumMidiInputChannels ();
            break;
          case effGetNumMidiOutputChannels:
            //trace("axPluginVst.dispatcher :: effGetNumMidiOutputChannels");
            //v = getNumMidiOutputChannels ();
            break;
          //default:
          //  trace("axPluginVst.dispatcher :: unknown dispatch code: " << opcode);
          //  break;

        }
        return v;
      }

  public:





    //--------------------------------------------------
    //
    // implementation of axPluginBase virtual methods
    //
    //--------------------------------------------------

    virtual void describe(axString aName, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        #ifdef AX_DEBUG
          char buf[256];
          sprintf(buf,"%s%s",aName.ptr(),(char*)"_debug");
          strncpy(mEffectName,buf,kVstMaxEffectNameLen);
        #else
          strncpy(mEffectName,aName.ptr(),kVstMaxEffectNameLen);
        #endif
        strncpy(mVendorString,aVendor.ptr(),kVstMaxVendorStrLen);
        strncpy(mProductString,aProduct.ptr(),kVstMaxProductStrLen);
        mVendorVersion = aVersion;
        setUniqueID(aID);
      }

    //----------

    virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      {
        setNumInputs(aInputs);            // defaults to 2 inputs & outputs
        setNumOutputs(aOutputs);           // aka stereo effect
        isSynth(aIsSynth);
      }

    //----------

    virtual void setupEditor(int aWidth, int aHeight)
      {
        //hasEditor(aWidth,aHeight);
        hasEditor(true);
        mEditorRect.set(0,0,aWidth,aHeight);
        mPlugFlags |= pf_HasEditor;
      }

    //----------

    virtual void setupParameters(void)
      {
        int num = mParameters.size();
        setNumParams(num); // vst
        for (int i=0; i<num; i++)
        {
          doSetParameter(mParameters[i]);
        }
      }

    //----------

    virtual void  notifyParamChanged(axParameter* aParameter)
      {
        //wtrace("axPluginVst.notifyParamChanged");
        int index = aParameter->getIndex();
        float value = aParameter->getValue();
        //wtrace("  index: " << index << " value: " << value);
        //setParameterAutomated(aParameter->getIndex(),aParameter->getValue());
        setParameterAutomated(index,value);
      }

    //virtual void  notifyResizeEditor(int aWidth, int aHeight)
    //  {
    //    //sizeWindow()
    //  }

    //enum VstTimeInfoFlags
    //{
    ////-------------------------------------------------------------------------------------------------------
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
    ////-------------------------------------------------------------------------------------------------------
    //};
    //

    virtual void updateTimeInfo(void)
      {
        //trace("updateTimeInfo");
        mTimeInfo   = getTime( kVstPpqPosValid + kVstTempoValid );
        //trace("..ok. mTimeInfo =  " << hex << mTimeInfo );
        mPlayState  = mTimeInfo->flags & 0xff;
        mSamplePos  = mTimeInfo->samplePos;
        mSampleRate = mTimeInfo->sampleRate;
        mBeatPos    = mTimeInfo->ppqPos;
        mTempo      = mTimeInfo->tempo;
      }

    //--------------------------------------------------

};

typedef axPluginVst axPluginImpl;





//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

#ifdef AX_LINUX

  #define __cdecl
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

  #define AX_ENTRYPOINT(plugclass)                  \
    AEffect* main(audioMasterCallback audioMaster)  \
    {                                               \
      XInitThreads();                               \
      AX_CONTEXT_INIT(plugclass)                    \
      plugclass* plug = new plugclass(&ctx);        \
      if (!plug) return 0;                          \
      AEffect* ae = plug->getInstance();            \
      return ae;                                    \
    }

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  // this is read only, so it should be safe?
  static HINSTANCE gInstance;

  #define MAKESTRING2(s) #s
  #define MAKESTRING(s) MAKESTRING2(s)
  #define MAKE_NAME(name) MAKESTRING(name) "_window"

  #define AX_CONTEXT_INIT(name)                             \
    HINSTANCE instance  = gInstance;                        \
    char*       winname = (char*)MAKE_NAME(name);           \
    AX_PTRCAST audio = (AX_PTRCAST)audioMaster;     \
    axContext ctx(instance,winname,audio);

/*
  #define AX_CONTEXT_EXIT
*/

//hInstance = hInstDll;
//switch(fdwReason) {
//    case DLL_PROCESS_ATTACH:
//        initMyLibraries();
//        if(initLibrariesFailed) {
//            return FALSE;
//        }
//        break;
//    case DLL_PROCESS_DETACH:
//        deinitializeLibaries();
//        break;

  #define AX_ENTRYPOINT(plugclass)                                          \
    BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved) \
    {                                                                       \
      gInstance = hModule;                                                  \
      return TRUE;                                                          \
    }                                                                       \
    int main(audioMasterCallback audioMaster)                               \
    {                                                                       \
      AX_CONTEXT_INIT(plugclass)                                            \
      plugclass* plug = new plugclass(&ctx);                                \
      if (!plug) return 0;                                                  \
      return (int)plug->getInstance();                                      \
    }

#endif

//----------------------------------------------------------------------
#endif
