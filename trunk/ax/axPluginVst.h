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

/**
 * @file axPluginVst.h
 * \brief vst plugin implementation
 */

#ifndef axPluginVst_included
#define axPluginVst_included
//----------------------------------------------------------------------
#ifdef AX_PLUGIN_VST
//TODO: make doxygen not include this (ala axBitmapW32, etc)

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

#include "public.sdk/source/vst2.x/audioeffect.cpp"
#include "public.sdk/source/vst2.x/audioeffectx.cpp"

#include "axWindow.h"
//#include "axHostVst.h"

// #include "dispatcher_enums.h"

//----------------------------------------------------------------------

//AX_NUMPROGS & AX_NUMPARAMS should a minimum of 1
#if (AX_NUMPROGS == 0)
	#undef AX_NUMPROGS
	#define AX_NUMPROGS 1
#endif
#if (AX_NUMPARAMS == 0)
	#undef AX_NUMPARAMS
	#define AX_NUMPARAMS 1
#endif

//#define MAX_CHANS 64
#define MAX_MIDI_SEND 1024

//----------------------------------------------------------------------

struct axVstEvents
{
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[MAX_MIDI_SEND];
};

//----------------------------------------------------------------------

//class axPluginImpl :  public AudioEffectX
class axPluginVst :  public AudioEffectX,
                     public axPluginBase
{
  //TODO: privatize/protect things
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
    int           mCurProg;
    float         mPrograms[AX_NUMPROGS][AX_NUMPARAMS];
    char          mProgramNames[AX_NUMPROGS][kVstMaxProgNameLen+1];        // !!!!!


    //----------------------------------------------------------------------

  private:


    //----------------------------------------------------------------------

  public:

    //axPluginVst(audioMasterCallback audioMaster,long numProgs,long numParams)
    //: AudioEffectX(audioMaster,numProgs,numParams)
    axPluginVst(axHost* aHost,long numProgs,long numParams)
    : AudioEffectX( (audioMasterCallback)(int)aHost->getPtr(), numProgs,numParams)
      {
        //trace("axPluginVst constructor. aHost = " << hex << aHost);
        //trace(" aHost->getPtr() = " << hex << aHost->getPtr() );
        mCurProg = 0;
        for (int i=0; i<AX_NUMPROGS; i++)
        {
          strcpy( mProgramNames[i],"Default");
          for (int j=0; j<AX_NUMPARAMS; j++) mPrograms[i][j] = 0;
        }
        mFlags  = 0;
        mWindow = NULL;
        mMidiEventList.numEvents = 0;
        mMidiEventList.reserved  = 0;
        for( int i=0; i<MAX_MIDI_SEND; i++ ) mMidiEventList.events[i] = &mMidiEvents[i];
        mEditorIsOpen = false;
        canProcessReplacing();      // need this for vst sdk 2.4
//        setNumInputs(2);            // defaults to 2 inputs & outputs
//        setNumOutputs(2);           // aka stereo effect
      }

    virtual ~axPluginVst()
      {
        //TRACE("destroying: %s\n",mEffectName.ptr());
        if (mFlags&pfl_HasEditor && mEditorIsOpen) closeEditor();
      }

    //----------------------------------------------------------------------
    //
    // description & info
    //
    //----------------------------------------------------------------------

    virtual void describe(axString aEffect, axString aVendor, axString aProduct, int aVersion, unsigned int aID)
      {
        mEffectName     = aEffect;
        mVendorString   = aVendor;
        mProductString  = aProduct;
        mVendorVersion  = aVersion;
        setUniqueID(aID);
      }

    //----------

    virtual void setupAudio(int aInputs=2, int aOutputs=2, bool aIsSynth=false)
      {
        setNumInputs(aInputs);            // defaults to 2 inputs & outputs
        setNumOutputs(aOutputs);           // aka stereo effect
        isSynth(aIsSynth);
      }

    virtual void setupEditor(int aWidth, int aHeight)
      {
        hasEditor(aWidth,aHeight);
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
        unsigned int ret = 0;
        if (!strcmp(ptr,"sendVstEvents"))        ret=1; // plug-in will send Vst events to Host
        if (!strcmp(ptr,"sendVstMidiEvent"))     ret=1; // plug-in will send MIDI events to Host
        if (!strcmp(ptr,"receiveVstEvents"))     ret=1; // plug-in can receive MIDI events from Host
        if (!strcmp(ptr,"receiveVstMidiEvent"))  ret=1; // plug-in can receive MIDI events from Host
        if (!strcmp(ptr,"receiveVstTimeInfo"))   ret=1; // plug-in can receive Time info from Host
        //if (strcmp(ptr,"offline"))              return 0; // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        //if (strcmp(ptr,"midiProgramNames"))     return 0; // plug-in supports function #getMidiProgramName ()
        //if (strcmp(ptr,"bypass"))               return 0; // plug-in supports function #setBypass ()
        if (!strcmp(ptr,"hasCockosExtensions"))  ret=0xbeef0000;
        //trace("- axPluginVst.canDo: '" << ptr << "' = " << hex << ret);
        return ret;
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

    virtual float getSampleRate(void)
      {
        mSampleRate = updateSampleRate();
        return mSampleRate;
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
    // programs
    //
    //----------------------------------------------------------------------

    //TODO: the programs/banks stuff is too primitive
    // chunks?

    // overloaded by axPlugin
    virtual void setProgram(VstInt32 program)
      {
        //for (int i=0; i<AX_NUMPARAMS; i++) mPrograms[mCurProg][i] = mParameters[i]->doGetValue();
        mCurProg = program;
        //for (int i=0; i<AX_NUMPARAMS; i++) mParameters[i]->doSetValue( mPrograms[mCurProg][i] );
      }

    //----------

    virtual void setProgramName(char* name)
      {
        strcpy( mProgramNames[mCurProg], name );
      }

    //----------

    virtual VstInt32 getProgram(void)
      {
        return mCurProg;
      }

    //----------

    virtual void  getProgramName(char* name)
      {
        strcpy(name,mProgramNames[mCurProg]);
      }

    //----------------------------------------------------------------------
    //
    // midi
    //
    //----------------------------------------------------------------------

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
          sendVstEventsToHost( (VstEvents*)&mMidiEventList );
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

    virtual void updateTimeInfo(void)
      {
        //trace("updateTimeInfo");
        mTimeInfo   = getTimeInfo( kVstPpqPosValid + kVstTempoValid );
        //trace("..ok. mTimeInfo =  " << hex << mTimeInfo );
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

#ifdef AX_PLUGIN_MULTI

    // multi
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

    // multi
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
    // editor
    //
    //----------------------------------------------------------------------

    // [internal]
    void openEditor(void* ptr,long value)
      {
        //#ifdef linux
        //  if (gDP==NULL) gDP = (Display*)value; // gDP = (Display*)value;
        //#endif
        //if( mWindow ) doDestroyEditor();
        //if( mWindow ) TRACE("oops! mWindo is not NULL (axPluginVst::openEditor)\n"); // meaning we could be executing 'inside' it in another thread?????
        axWindow* win = (axWindow*)doCreateEditor(/*mWidth,mHeight*/);
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

    // [internal]
    void closeEditor(void)
      {
        if(mWindow)
        {
          //mWindow->hide();
          //lock
          mWindow = NULL;
          doDestroyEditor();
          //unlock
        }
      }

    //----------

    // [internal]
    void idleEditor(void)
      {
        //if dirtylist not empty, update all widgets in dirtylst
        // clear dirtylist (not delete)
        //lock
        if (mWindow) { doIdleEditor(); }
        //unlock
      }

    //----------

    virtual void hasEditor(int aWidth, int aHeight)
      {
        cEffect.flags |= effFlagsHasEditor;
        mFlags |= pfl_HasEditor;
        mWidth = aWidth;
        mHeight = aHeight;
      }

    //----------------------------------------------------------------------
    //
    // vendor specific
    //
    //----------------------------------------------------------------------

    virtual VstIntPtr vendorSpecific(VstInt32 lArg, VstIntPtr lArg2, void* ptrArg, float floatArg)
      {
        // trace("- vendorSpecific");
        //if( lArg == 0xdeadbef0 && lArg2 >= 1 && lArg2 <= kNumParams )
        //{
        //  ((double *) ptrArg)[0] = 0;
        //  ((double *) ptrArg)[1] = 1469;  // Should be linked to Delay::buffer_size
        //  return 0xbeef;
        //}
        return 0;
      }

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
              // what will happen if we close and re-open the plugin on a different screen/monitor
              // could we get a different Display* (for example, if it has a different bitdepth)?
              // if so, each window needs its own (cached?) Display*
              #ifdef linux
                if (gDP==NULL) gDP = (Display*)value;
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
          case effVendorSpecific:
          //  // v = vendorSpecific (index, value, ptr, opt);
            trace("- effVendorSpecific");
            break;
          default:
            result = AudioEffectX::dispatcher(opcode,index,value,ptr,opt);
        }
        return result;
      }

};

typedef axPluginVst axPluginImpl;

#endif //AX_PLUGIN_VST
//----------------------------------------------------------------------
#endif
