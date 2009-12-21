#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  0
#define AX_WIDTH      400
#define AX_HEIGHT     300
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_AUTOSYNC
#define AX_DEBUG

#include "axDebug.h"
#include "axPlugin.h"
#include "axReaperExtensions.h"
#include "axEditor.h"
#include "parFloat.h"
#include "parInteger.h"
#include "wdgLabel.h"
#include "wdgKnobPar.h"
#include "wdgKnobSmallPar.h"
//#include "axSynth1_dsp.h"
#include "axSynth1_gui.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    axReaperExtensions* reaper;
    //float irate;
    //float scale;
  public:
    axEditor  *mEditor;
    wdgMultiPage* tabs;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        axRandomize(418);
        describe("axSynth1","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        setNumInputs(2);
        setNumOutputs(2);
        isSynth();
        float sr = updateSampleRate();
        TRACE("  ? mSampleRate: %f, updateSampleRate() = %f\n",mSampleRate,sr);
        //irate = 1/mSampleRate;
        //scale = mSampleRate / 44100;

        float f;
        int i;
        char buf[257];

        getHostVendorString(buf);
        TRACE("  ? getHostVendorString() : '%s'\n",buf );
        i = getHostVendorVersion();
        TRACE("  ? getHostVendorVersion() : %i\n",i );
        getHostProductString(buf);
        TRACE("  ? getHostProductString() : '%s'\n",buf );
        i = getHostLanguage();
        TRACE("  ? getHostLanguage() : %i\n",i );

        // reaper vst extensions
        buf[0] = '\0';
        reaper = new axReaperExtensions(audioMaster);
        if( reaper->GetPlayPosition )   { f = reaper->GetPlayPosition();    TRACE("  ? reaper.GetPlayPosition() : %f\n",f); }
        if( reaper->GetPlayPosition2 )  { f = reaper->GetPlayPosition2();   TRACE("  ? reaper.GetPlayPosition2() : %f\n",f); }
        if( reaper->GetCursorPosition ) { f = reaper->GetCursorPosition();  TRACE("  ? reaper.GetCursorPosition() : %f\n",f); }
        if( reaper->GetPlayState )      { i = reaper->GetPlayState();       TRACE("  ? reaper.GetPlayState() : %i\n",i); }
        if( reaper->GetSetRepeat )      { i = reaper->GetSetRepeat(-1);     TRACE("  ? reaper.GetSetRepeat() : %i\n",i); }
        if( reaper->GetProjectPath )    { reaper->GetProjectPath(buf,256);  TRACE("  ? reaper.GetProjectPath() : '%s'\n",buf); }
        if( reaper->IsInRealTimeAudio ) { i = reaper->IsInRealTimeAudio();  TRACE("  ? reaper.IsInRealtimeAudio() : %i\n",i); }
        if( reaper->Audio_IsRunning )   { i = reaper->Audio_IsRunning();    TRACE("  ? reaper.Audio_IsRunning() : %i\n",i); }

        //hostCanDo
        int res;
        res = canHostDo((char*)"sendVstEvents");                  TRACE("  ? canHostDo sendVstEvents                  = %i\n",res);
        res = canHostDo((char*)"sendVstMidiEvent");               TRACE("  ? canHostDo sendVstMidiEvent               = %i\n",res);
        res = canHostDo((char*)"sendVstTimeInfo");                TRACE("  ? canHostDo sendVstTimeInfo                = %i\n",res);
        res = canHostDo((char*)"receiveVstEvents");               TRACE("  ? canHostDo receiveVstEvents               = %i\n",res);
        res = canHostDo((char*)"receiveVstMidiEvent");            TRACE("  ? canHostDo receiveVstMidiEvent            = %i\n",res);
        res = canHostDo((char*)"reportConnectionChanges");        TRACE("  ? canHostDo reportConnectionChanges        = %i\n",res);
        res = canHostDo((char*)"acceptIOChanges");                TRACE("  ? canHostDo acceptIOChanges                = %i\n",res);
        res = canHostDo((char*)"sizeWindow");                     TRACE("  ? canHostDo sizeWindow                     = %i\n",res);
        res = canHostDo((char*)"offline");                        TRACE("  ? canHostDo offline                        = %i\n",res);
        res = canHostDo((char*)"openFileSelector");               TRACE("  ? canHostDo openFileSelector               = %i\n",res);
        res = canHostDo((char*)"closeFileSelector");              TRACE("  ? canHostDo closeFileSelector              = %i\n",res);
        res = canHostDo((char*)"startStopProcess");               TRACE("  ? canHostDo startStopProcess               = %i\n",res);
        res = canHostDo((char*)"shellCategory");                  TRACE("  ? canHostDo shellCategory                  = %i\n",res);
        res = canHostDo((char*)"sendVstMidiEventFlagIsRealtime"); TRACE("  ? canHostDo sendVstMidiEventFlagIsRealtime = %i\n",res);
      }

    //--------------------------------------------------

    virtual ~myPlugin()
      {
        delete reaper;
        //if( mEditor ) delete mEditor;
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        TRACE("> doCreateEditor\n" );
        axEditor* E = new axEditor(    this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
        E->setLayout(10,30,0,0);
        //E->setFlag(wfl_Vertical);
        E->appendWidget( tabs = new wdgMultiPage( E,0,NULL_RECT,wal_Client ) );
        tabs->appendWidget( new myTab1( tabs,0,NULL_RECT,wal_Client ) );
        tabs->setPage(0);
        E->doRealign();
        mEditor = E;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        TRACE("> doDestroyEditor\n" );
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if( mEditor ) mEditor->redrawDirty();
      }

    //----------

    virtual void doProcessState(int aState)
      {
        TRACE("> doProcessState: %i (",aState );
        switch(aState)
        {
          case pst_Open:    TRACE("pst_Open)\n"); break;
          case pst_Close:   TRACE("pst_Close)\n"); break;
          case pst_Suspend: TRACE("pst_Suspend)\n"); break;
          case pst_Resume:  TRACE("pst_Resume)\n"); break;
          default: TRACE("?)\n");
        }
      }

    //----------

    virtual void doProcessTransport(int aState)
      {
        TRACE("> doProcessTransport: %i ( ",aState);
        if( aState&1 )   TRACE("TransportChanged ");
        if( aState&2 )   TRACE("TransportPlaying ");
        if( aState&4 )   TRACE("TransportCycleActive ");
        if( aState&8 )   TRACE("TransportRecording ");
        if( aState&64 )  TRACE("AutomationWriting ");
        if( aState&128 ) TRACE("AutomationReading ");
        TRACE(")\n");
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        TRACE("> doProcessParameter(%i) = %f\n",aParameter->mID,aParameter->getValue());
      }

    //----------

    // called before doProcessBlock, once for eeac incoming midi message
    // send messages with sendMidi(offset,msg1,msg2,msg3)
    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        TRACE("> doProcessMidi ofs:%i msg:%02x,%02x,%02x\n",ofs,msg1,msg2,msg3);
//        if( msg1==9*16 )
//        {
//          int _note = msg2;
//          if( midinote<0 || midinote==_note )
//          {
//            float _vel = (float)msg3 * inv127;/// 127;
//            float _freq   = 440 * pow(2.0,(_note-69.0) / 12);
//            EVENTS[ofs*2]     = _vel;
//            EVENTS[(ofs*2)+1] = _freq;
//          }
//        }
      }

    //----------

    // return:
    // - true to indicate that you have finished processing the samples
    // - false (default) if you want doProcessSample calls
    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
      {
        //updateTimeInfo(); // auto
        return false;
      }

    //----------

    // only called if doProcessBlock returned false (default)
    // default = copy input to output
    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"

