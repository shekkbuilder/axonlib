#ifndef axPluginVst_included
#define axPluginVst_included
//----------------------------------------------------------------------
// implements the methods in axPlugin

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"
#include "public.sdk/source/vst2.x/audioeffect.cpp"
#include "public.sdk/source/vst2.x/audioeffectx.cpp"
#include "axWindow.h"

// #include "dispatcher_enums.h"

//----------------------------------------------------------------------

//#define MAX_CHANS 64
#define MAX_MIDI_SEND 1024

// plugin flags
#define pfl_HasEditor 1

// state
#define pst_Open    1
#define pst_Close   2
#define pst_Suspend 3
#define pst_Resume  4

//----------------------------------------------------------------------

//struct VstEvents
//{
//	VstInt32 numEvents;		///< number of Events in array
//	VstIntPtr reserved;		///< zero (Reserved for future use)
//	VstEvent* events[2];	///< event pointer array, variable size
//};
struct axVstEvents
{
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[MAX_MIDI_SEND];
};

//----------------------------------------------------------------------

class axPluginImpl :  public AudioEffectX
{
  public:
  //private:
    bool          mEditorIsOpen; // axEditor reads this
  public:
    int           mFlags;
    int           mWidth;
    int           mHeight;
    axWindow*     mWindow;
    axString      mEffectName;
    axString      mVendorString;
    axString      mProductString;
    int           mVendorVersion;
  public:
    ERect         rect;
    VstTimeInfo*  mTimeInfo;
    int           mPlayState;
    double        mSamplePos;
    double        mSampleRate;
    double        mBeatPos;
    double        mTempo;
    long          mBlockSize;
    axVstEvents   mMidiEventList;
    VstEvent      mMidiEvents[MAX_MIDI_SEND];
  public:
    char          mProgramNames[AX_NUMPROGS][kVstMaxProgNameLen+1];        // !!!!!
    int           mCurProg;
    float         mPrograms[AX_NUMPROGS][AX_NUMPARAMS];

  public:

    //----------------------------------------------------------------------

    // #ifdef AX_AUTOSYNC
    //  updateTimeInfo();
    //  if( mPlayState&1 ) doProcessTransport(mPlayState);

    //void describe(axString aEffect, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
    //void hasEditor(int aWidth, int aHeight/*, bool aState=true*/)
    //void sendMidi_clear(void)
    //void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //void sendMidi_all(void)
    //void updateTimeInfo(void)

    virtual axWindow* doCreateEditor(void) { return NULL; }
    virtual void      doDestroyEditor(void) {}
    virtual void      doIdleEditor(void) {}
    virtual void      doProcessState(int aState) {}
    virtual void      doProcessTransport(int aState) {}
    virtual void      doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3) {}
    virtual bool      doProcessBlock(float** inputs, float** outputs, long sampleFrames) { return false; }
    virtual void      doProcessSample(float** ins, float** outs) { *outs[0]=*ins[0]; *outs[1]=*ins[1]; }
    virtual void      doPostProcess(float** inputs, float** outputs, long sampleFrames) {}

    //----------------------------------------------------------------------

    axPluginImpl(audioMasterCallback audioMaster,long numProgs,long numParams)
    : AudioEffectX(audioMaster,numProgs,numParams)
      {

        mCurProg = 0;
        for (int i=0; i<AX_NUMPROGS; i++)
        {
          strcpy( mProgramNames[i],"Default");
          for (int j=0; j<AX_NUMPARAMS; j++) mPrograms[i][j] = 0;
        }
        mFlags  = 0;
        mWidth  = 256;
        mHeight = 256;
        mWindow = NULL;
        mMidiEventList.numEvents = 0;
        mMidiEventList.reserved  = 0;
        for( int i=0; i<MAX_MIDI_SEND; i++ ) mMidiEventList.events[i] = &mMidiEvents[i];
        mEditorIsOpen = false;
      }

    virtual ~axPluginImpl()
      {
        //TRACE("destroying: %s\n",mEffectName.ptr());
        if (mFlags&pfl_HasEditor && mEditorIsOpen) closeEditor();
      }

    //----------------------------------------------------------------------
    //
    // description & info
    //
    //----------------------------------------------------------------------

    void describe(axString aEffect, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        mEffectName     = aEffect;
        mVendorString   = aVendor;
        mProductString  = aProduct;
        mVendorVersion  = aVersion;
        setUniqueID(aID);
      }

    //----------------------------------------
    // AudioEffectX
    //----------------------------------------

    //host:
    //  bool AudioEffectX::getHostVendorString (char* text)
    //  bool AudioEffectX::getHostProductString (char* text)
    //  VstInt32 AudioEffectX::getHostVendorVersion ()
    //  VstInt32 AudioEffectX::canHostDo (char* text)
    //  VstIntPtr AudioEffectX::hostVendorSpecific (VstInt32 lArg1, VstIntPtr lArg2, void* ptrArg, float floatArg)

    virtual bool getEffectName(char* name)
      {
        strcpy(name,mEffectName.ptr());
        return true;
      }

    //----------

    virtual bool getVendorString(char* text)
      {
        strcpy(text,mVendorString.ptr());
        return true;
      }

    //----------

    virtual bool getProductString(char* text)
      {
        strcpy(text,mProductString.ptr());
        return true;
      }

    //----------

    virtual VstInt32 getVendorVersion()
      {
        return mVendorVersion;
      }

    //----------

    virtual VstInt32 canDo(char *ptr)
      {
        if (strcmp(ptr,"sendVstEvents"))        return 1; // plug-in will send Vst events to Host
        if (strcmp(ptr,"sendVstMidiEvent"))     return 1; // plug-in will send MIDI events to Host
        if (strcmp(ptr,"receiveVstEvents"))     return 1; // plug-in can receive MIDI events from Host
        if (strcmp(ptr,"receiveVstMidiEvent"))  return 1; // plug-in can receive MIDI events from Host
        if (strcmp(ptr,"receiveVstTimeInfo"))   return 1; // plug-in can receive Time info from Host
        //if (strcmp(ptr,"offline"))              return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        //if (strcmp(ptr,"midiProgramNames"))     return 0; // plug-in supports function #getMidiProgramName ()
        //if (strcmp(ptr,"bypass"))               return 0; // plug-in supports function #setBypass ()
        if (strcmp(ptr,"hasCockosExtensions"))  return 0xbeef0000;
        return 0;
      }

    //----------------------------------------------------------------------
    //
    // state
    //
    //----------------------------------------------------------------------

    // Called when plug-in is initialized
    virtual void open()
      {
        doProcessState(pst_Open);
      }

    // Called when plug-in will be released
    virtual void close()
      {
        doProcessState(pst_Close);
      }

    // Called when plug-in is switched to off
    virtual void suspend()
      {
        doProcessState(pst_Suspend);
      }

    // Called when plug-in is switched to on
    virtual void resume()
      {
        doProcessState(pst_Resume);
      }

    //----------

    virtual void setSampleRate(float sampleRate)
      {
        this->sampleRate = sampleRate;
        AudioEffectX::setSampleRate(sampleRate);
        mSampleRate = sampleRate;
        //doSetSamplerate(mSampleRate);
      }

    //----------

    virtual void setBlockSize(VstInt32 blockSize) // max block size
      {
        //this->blockSize = blockSize;
        AudioEffectX::setBlockSize(blockSize);
        mBlockSize = blockSize;
      }

    //----------------------------------------------------------------------
    //
    // parameters
    //
    //----------------------------------------------------------------------
    // (handled in axPlugin)

    //    virtual void getParameterName(VstInt32 index, char* text)
    //    virtual void getParameterLabel(VstInt32 index, char* label)
    //    virtual void getParameterDisplay(VstInt32 index, char* text)
    //    virtual void setParameter(VstInt32 index, float value)
    //    virtual float getParameter(VstInt32 index)

    // host->plugin
    //virtual bool canParameterBeAutomated (VstInt32 index) { return true; }
    // plug->host
    //VstInt32 AudioEffectX::getAutomationState ()
    //virtual void setParameterAutomated(VstInt32 index, float value) {}

    //----------------------------------------------------------------------
    //
    // programs
    //
    //----------------------------------------------------------------------
    // AudioEffectX
    //TODO:

    virtual void setProgramName(char* name)
      {
        strcpy( mProgramNames[mCurProg], name );
      }

    virtual void  getProgramName(char* name)
      {
        strcpy(name,mProgramNames[mCurProg]);
      }

    virtual VstInt32 getProgram(void)
      {
        return mCurProg;
      }

    virtual void setProgram(VstInt32 program)
      {
        //for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
        mCurProg = program;
        //for (int i=0; i<AX_NUMPARAMS; i++) mParameters[i]->doSetValue( mPrograms[mCurProg][i] );
      }

    //----------------------------------------------------------------------
    //
    // editor
    //
    //----------------------------------------------------------------------

    // plug->host
    //bool AudioEffectX::sizeWindow(VstInt32 width, VstInt32 height)

    void hasEditor(int aWidth, int aHeight/*, bool aState=true*/)
      {
        cEffect.flags |= effFlagsHasEditor;
        mFlags |= pfl_HasEditor;
        mWidth = aWidth;
        mHeight = aHeight;
      }

    //----------

    void openEditor(void* ptr,long value)
      {
        //#ifdef linux
        //  if (gDP==NULL) gDP = (Display*)value; // gDP = (Display*)value;
        //#endif
        //if( mWindow ) doDestroyEditor();
        //if( mWindow ) TRACE("oops! mWindo is not NULL (axPluginVst::openEditor)\n"); // meaning we could be executing 'inside' it in another thread?????
        axWindow* win = doCreateEditor();
        if (win)
        {
          win->reparent((int)ptr);
          win->show();
          //lock
          mWindow = win;
          //unlock
        }
      }

    //----------

    void closeEditor(void)
      {
        if(mWindow)
        {
          mWindow->hide();
          //lock
          mWindow = NULL;
          doDestroyEditor();
          //unlock
        }
      }

    //----------

    void idleEditor(void)
      {
        //if dirtylist not empty, update all widgets in dirtylst
        // clear dirtylist (not delete)
        //lock
        if (mWindow) { doIdleEditor(); }
        //unlock
      }

    //----------------------------------------------------------------------
    //
    // midi
    //
    //----------------------------------------------------------------------

    void sendMidiClear(void)
      {
        mMidiEventList.numEvents = 0;
      }

    //----------

    void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
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

    //----------

    void sendMidiAll(void)
      {
        int num = mMidiEventList.numEvents;
        if( num>0 )
        {
          sendVstEventsToHost( (VstEvents*)&mMidiEventList );
          sendMidiClear();
          //mMidiEventList.numEvents = 0;
        }
      }

    //----------------------------------------
    // AudioEffectX
    //----------------------------------------

    //TODO: sort?
    // if so, stuff all events into a buffer
    // a) check last item offset, of later, append after previous
    // if before, search from start, and insert (move later)
    // b) sort
    // c) pre-scan VstEvents array

    virtual VstInt32 processEvents(VstEvents* ev)
      {
        //sendMidiClear();
        int num = ev->numEvents;
        for ( int i=0; i<num; i++)
        {
          VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
          if (event->type==kVstMidiType)
          {
            doProcessMidi( event->deltaFrames, event->midiData[0], event->midiData[1], event->midiData[2] );
          } //=miditype
        } //numevents
        // sort?
        return true;
      }

    //----------------------------------------------------------------------
    //
    // processing
    //
    //----------------------------------------------------------------------

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

    void updateTimeInfo(void)
      {
        mTimeInfo   = getTimeInfo( kVstPpqPosValid + kVstTempoValid );
        mPlayState  = mTimeInfo->flags & 0xff;
        mSamplePos  = mTimeInfo->samplePos;
        mSampleRate = mTimeInfo->sampleRate;
        mBeatPos    = mTimeInfo->ppqPos;
        mTempo      = mTimeInfo->tempo;
      }

    //----------

    //virtual bool process_block_double(double** inputs, double** outputs, long sampleFrames)
    //  {
    //    return false;
    //  }

    //virtual void process_sample_double(double** ins, double** outs)
    //  {
    //    *outs[0] = *ins[0];
    //    *outs[1] = *ins[1];
    //  }

    //----------------------------------------
    // AudioEffectX
    //----------------------------------------

    // deprecated???
    virtual void process(float** inputs, float** outputs, long sampleFrames)
      {
        processReplacing(inputs,outputs,sampleFrames);
      }

    //----------

    //    // multi
    //    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
    //      {
    //        mBlockSize = sampleFrames;
    //        if ( !process_block(inputs,outputs,sampleFrames) )
    //        {
    //          int i;
    //          float*  ins[NUM_INPUTS];
    //          float* outs[NUM_OUTPUTS];
    //          for( i=0; i<NUM_INPUTS;  i++ ) ins[i]  = inputs[i];
    //          for( i=0; i<NUM_OUTPUTS; i++ ) outs[i] = outputs[i];
    //          while (--sampleFrames >= 0)
    //          {
    //            process_sample(ins,outs);
    //            for( i=0; i<NUM_INPUTS;  i++ ) ins[i]++;
    //            for( i=0; i<NUM_OUTPUTS; i++ ) outs[i]++;
    //          } //sampleflrames
    //        } //process_block
    //      }

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
          ins[0] = inputs[0];
          ins[1] = inputs[1];
          outs[0] = outputs[0];
          outs[1] = outputs[1];
          while (--sampleFrames >= 0)
          {
            doProcessSample(ins,outs);
            ins[0]++;
            ins[1]++;
            outs[0]++;
            outs[1]++;
          } //sampleflrames
        } //process_block
        doPostProcess(inputs,outputs,sampleFrames);
        sendMidiAll();
      }

    //----------

    //    // multi
    //    virtual void  processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
    //      {
    //        mBlockSize = sampleFrames;
    //        if ( !process_block_double(inputs,outputs,sampleFrames) )
    //        {
    //          int i;
    //          double*  ins[NUM_INPUTS];
    //          double* outs[NUM_OUTPUTS];
    //          for( i=0; i<NUM_INPUTS;  i++ ) ins[i]  = inputs[i];
    //          for( i=0; i<NUM_OUTPUTS; i++ ) outs[i] = outputs[i];
    //          while (--sampleFrames >= 0)
    //          {
    //            process_sample_double(ins,outs);
    //            for( i=0; i<NUM_INPUTS;  i++ ) ins[i]++;
    //            for( i=0; i<NUM_OUTPUTS; i++ ) outs[i]++;
    //          } //sampleflrames
    //        } //process_block
    //      }

    //    // stereo
    //    virtual void processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
    //      {
    //        mBlockSize = sampleFrames;
    //        if ( !process_block_double(inputs,outputs,sampleFrames) )
    //        {
    //          double* ins[2];
    //          double* outs[2];
    //          ins[0] = inputs[0];
    //          ins[1] = inputs[1];
    //          outs[0] = outputs[0];
    //          outs[1] = outputs[1];
    //          while (--sampleFrames >= 0)
    //          {
    //            process_sample_double(ins,outs);
    //            ins[0]++;
    //            ins[1]++;
    //            outs[0]++;
    //            outs[1]++;
    //          } //sampleflrames
    //        } //process_block
    //      }

    //----------------------------------------------------------------------
    //
    // dispatcher
    //
    //----------------------------------------------------------------------

    virtual VstIntPtr dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
      {
        //TRACE("dispatcher: opcode:%i, index:%i, value:%i, ptr:%x, float:%f\n",opcode,index,value,(int)ptr,opt);
        //TRACE("dispatcher: opcode:%s, index:%i, value:%i, ptr:%x, float:%f\n", dispatch_enums[opcode],index,value,(int)ptr,opt);
        int result = 0;
        switch (opcode)
        {
          case effEditGetRect:
            //TRACE("effEditGetRect\n");
            if (mFlags&pfl_HasEditor)
            {
              rect.left     = 0;
              rect.top      = 0;
              rect.right    = mWidth;
              rect.bottom   = mHeight;
              *(ERect**)ptr = &rect;
              result = 1;
            }
            break;
          case effEditOpen:
            //TRACE("effEditOpen. ptr=%08x\n",(int)ptr);
            if (mFlags&pfl_HasEditor)
            {
              #ifdef linux
                if (gDP==NULL) gDP = (Display*)value;
                //TRACE("- value=%08x, gDP=%08x\n",(int)value,(int)gDP);
              #endif
              openEditor(ptr,value);
              result = 1;
              mEditorIsOpen = true;
            }
            break;
          case effEditClose:
            //TRACE("effEditClose\n");
            if (mFlags&pfl_HasEditor && mEditorIsOpen);
            {
              mEditorIsOpen = false;
              closeEditor();
            }
            break;
          case effEditIdle:
            //TRACE("effEditIdle\n");
            if (mFlags&pfl_HasEditor && mEditorIsOpen);
            {
              idleEditor();
            }
            break;
          default:
            result = AudioEffectX::dispatcher(opcode,index,value,ptr,opt);
        }
        return result;
      }

};

//----------------------------------------------------------------------
#endif
