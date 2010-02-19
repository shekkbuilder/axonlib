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

/**
 * \file axReaperExt.h
 * \brief provides access to reaper extentions 
*/ 
 
#ifndef axPluginReaperExt_included
#define axPluginReaperExt_included
//----------------------------------------------------------------------

#include "axPlugin.h"



//#include "reaper_plugin.h"
//#include "reaper_plugin_functions.h"

/*

#define REASAMPLE_SIZE 8
typedef double ReaSample;
//#define REAPER_PLUGIN_DLL_EXPORT __declspec(dllexport)
//#define REAPER_PLUGIN_HINSTANCE HINSTANCE
//#include <pthread.h>
//#ifdef _MSC_VER
//  #define INT64 __int64
//  #define INT64_CONSTANT(x) (x##i64)
//#else
//  #define INT64 long long
//  #define INT64_CONSTANT(x) (x##LL)
//#endif

typedef struct reaper_plugin_info_t
{
  int   caller_version;
  HWND  hwnd_main;
  int   (*Register)(const char *name, void *infostruct);
  void* (*GetFunc)(const char *name);
} reaper_plugin_info_t;

//class ProjectStateContext
//{
//public:
//  virtual ~ProjectStateContext(){};
//  virtual void AddLine(const char *fmt, ...)=0;
//  virtual int GetLine(char *buf, int buflen)=0;
//};
typedef void ProjectStateContext;

typedef struct
{
  int frame_offset;
  int size;
  unsigned char midi_message[4];
} MIDI_event_t;

//class MIDI_eventlist
//{
//public:
//  virtual void AddItem(MIDI_event_t *evt)=0;
//  virtual MIDI_event_t *EnumItems(int *bpos)=0;
//  virtual void DeleteItem(int bpos)=0;
//  virtual int GetSize()=0;
//  virtual void Empty()=0;
//};
typedef void MIDI_eventlist;

//----------------------------------------------------------------------
// PCM source API
//----------------------------------------------------------------------

typedef struct
{
  double time_s;
  double samplerate;
  int nch;
  int length;
  ReaSample *samples;
  int samples_out;
  MIDI_eventlist *midi_events;
  double approximate_playback_latency;
  double absolute_time_s;
  double force_bpm;
} PCM_source_transfer_t;

typedef struct
{
  double start_time;
  double peakrate;
  int numpeak_points;
  int nchpeaks;
  ReaSample *peaks;
  int peaks_out;
  enum { PEAKTRANSFER_PEAKS_MODE=0, PEAKTRANSFER_WAVEFORM_MODE=1, PEAKTRANSFER_MIDI_MODE=2 };
  int output_mode;
  double absolute_time_s;
  ReaSample *peaks_minvals;
  int peaks_minvals_used;
  double samplerate;
  int *exp[30];
} PCM_source_peaktransfer_t;

typedef struct
{
  double global_time;
  double global_item_time;
  double srate;
  int length;
  int overwritemode;
  MIDI_eventlist *events;
  double item_playrate;
  double latency;
  unsigned int *overwrite_actives;
} midi_realtime_write_struct_t;

//class PCM_source
//{
//  public:
//    virtual ~PCM_source() { }
//    virtual PCM_source *Duplicate()=0;
//    virtual bool IsAvailable()=0;
//    virtual void SetAvailable(bool avail) { }
//    virtual const char *GetType()=0;
//    virtual const char *GetFileName() { return NULL; };
//    virtual bool SetFileName(const char *newfn)=0;
//    virtual PCM_source *GetSource() { return NULL; }
//    virtual void SetSource(PCM_source *src) { }
//    virtual int GetNumChannels()=0;
//    virtual double GetSampleRate()=0;
//    virtual double GetLength()=0;
//    virtual double GetLengthBeats() { return -1.0; }
//    virtual int GetBitsPerSample() { return 0; }
//    virtual double GetPreferredPosition() { return -1.0; }
//    virtual int PropertiesWindow(HWND hwndParent)=0;
//    virtual void GetSamples(PCM_source_transfer_t *block)=0;
//    virtual void GetPeakInfo(PCM_source_peaktransfer_t *block)=0;
//    virtual void SaveState(ProjectStateContext *ctx)=0;
//    virtual int LoadState(char *firstline, ProjectStateContext *ctx)=0;
//    virtual void Peaks_Clear(bool deleteFile)=0;
//    virtual int PeaksBuild_Begin()=0;
//    virtual int PeaksBuild_Run()=0;
//    virtual void PeaksBuild_Finish()=0;
//    virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void PCM_source;

typedef struct
{
  int m_id;
  double m_time, m_endtime;
  bool m_isregion;
  char *m_name;
  char resvd[128];
} REAPER_cue;

typedef struct
{
  PCM_source* m_sliceSrc;
  double m_beatSnapOffset;
  char resvd[128];
} REAPER_slice;

typedef struct
{
  double draw_start_time;
  int draw_start_y;
  double pixels_per_second;
  int width, height;
  int mouse_x, mouse_y;
  void *extraParms[8];
} REAPER_inline_positioninfo;

typedef struct
{
  PCM_source *(*CreateFromType)(const char *type, int priority);
  PCM_source *(*CreateFromFile)(const char *filename, int priority);
  const char *(*EnumFileExtensions)(int i, char **descptr);
} pcmsrc_register_t;

//class ISimpleMediaDecoder
//{
//public:
//  virtual ~ISimpleMediaDecoder() { }
//  virtual ISimpleMediaDecoder *Duplicate()=0;
//  virtual void Open(const char *filename, int diskreadmode, int diskreadbs, int diskreadnb)=0;
//  virtual void Close(bool fullClose)=0;
//  virtual const char *GetFileName()=0;
//  virtual const char *GetType()=0;
//  virtual void GetInfoString(char *buf, int buflen, char *title, int titlelen)=0;
//  virtual bool IsOpen()=0;
//  virtual int GetNumChannels()=0;
//  virtual int GetBitsPerSample()=0;
//  virtual double GetSampleRate()=0;
//  virtual INT64 GetLength()=0;
//  virtual INT64 GetPosition()=0;
//  virtual void SetPosition(INT64 pos)=0;
//  virtual int ReadSamples(ReaSample *buf, int length)=0;
//  virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void ISimpleMediaDecoder;

typedef struct
{
  bool doquant;
  char movemode;
  char sizemode;
  char quantstrength;
  double quantamt;
  char swingamt;
  char range_min;
  char range_max;
} midi_quantize_mode_t;

//class PCM_sink
//{
//  public:
//    PCM_sink() { m_st=0.0; }
//    virtual ~PCM_sink() { }
//    virtual void GetOutputInfoString(char *buf, int buflen)=0;
//    virtual double GetStartTime() { return m_st; }
//    virtual void SetStartTime(double st) { m_st=st; }
//    virtual const char *GetFileName()=0;
//    virtual int GetNumChannels()=0;
//    virtual double GetLength()=0;
//    virtual INT64 GetFileSize()=0;
//    virtual void WriteMIDI(MIDI_eventlist *events, int len, double samplerate)=0;
//    virtual void WriteDoubles(ReaSample **samples, int len, int nch, int offset, int spacing)=0;
//    virtual bool WantMIDI() { return 0; }
//    virtual int GetLastSecondPeaks(int sz, ReaSample *buf) { return 0; }
//    virtual void GetPeakInfo(PCM_source_peaktransfer_t *block) { }
//    virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//  private:
//    double m_st;
//};
typedef void PCM_sink;

typedef struct
{
  unsigned int (*GetFmt)(char **desc);
  const char *(*GetExtension)(const void *cfg, int cfg_l);
  HWND (*ShowConfig)(const void *cfg, int cfg_l, HWND parent);
  PCM_sink *(*CreateSink)(const char *filename, void *cfg, int cfg_l, int nch, int srate, bool buildpeaks);
} pcmsink_register_t;

typedef struct
{
  pcmsink_register_t sink;
  int (*Extended)(int call, void* parm1, void* parm2, void* parm3);
  char expand[256];
} pcmsink_register_ext_t;

//class REAPER_Resample_Interface
//{
//public:
//  virtual ~REAPER_Resample_Interface(){}
//  virtual void SetRates(double rate_in, double rate_out)=0;
//  virtual void Reset()=0;
//  virtual double GetCurrentLatency()=0;
//  virtual int ResamplePrepare(int out_samples, int nch, ReaSample **inbuffer)=0;
//  virtual int ResampleOut(ReaSample *out, int nsamples_in, int nsamples_out, int nch)=0;
//  virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void REAPER_Resample_Interface;

//class IReaperPitchShift
//{
//  public:
//    virtual ~IReaperPitchShift() { };
//    virtual void set_srate(double srate)=0;
//    virtual void set_nch(int nch)=0;
//    virtual void set_shift(double shift)=0;
//    virtual void set_formant_shift(double shift)=0;
//    virtual void set_tempo(double tempo)=0;
//    virtual void Reset()=0;
//    virtual ReaSample *GetBuffer(int size)=0;
//    virtual void BufferDone(int input_filled)=0;
//    virtual void FlushSamples()=0;
//    virtual bool IsReset()=0;
//    virtual int GetSamples(int requested_output, ReaSample *buffer)=0;
//    virtual void SetQualityParameter(int parm)=0;
//};
typedef void IReaperPitchShift;

//class REAPER_PeakGet_Interface
//{
//public:
//  virtual ~REAPER_PeakGet_Interface() { }
//  virtual double GetMaxPeakRes()=0;
//  virtual void GetPeakInfo(PCM_source_peaktransfer_t *block)=0;
//  virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void REAPER_PeakGet_Interface;

//class REAPER_PeakBuild_Interface
//{
//public:
//  virtual ~REAPER_PeakBuild_Interface() { }
//  virtual void ProcessSamples(ReaSample **samples, int len, int nch, int offs, int spread)=0;
//  virtual int Run()=0;
//  virtual int GetLastSecondPeaks(int sz, ReaSample *buf)=0;
//  virtual void GetPeakInfo(PCM_source_peaktransfer_t *block)=0;
//  virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void REAPER_PeakBuild_Interface;

typedef struct accelerator_register_t
{
  int (*translateAccel)(MSG *msg, accelerator_register_t *ctx);
  bool isLocal;
  void *user;
} accelerator_register_t;

typedef struct
{
  ACCEL accel;
  const char *desc;
} gaccel_register_t;

typedef struct
{
  const char* action_desc;
  const char* action_help;
} action_help_t;

typedef struct
{
  int (*editFile)(const char *filename, HWND parent, int trackidx);
  const char *(*wouldHandle)(const char *filename);
} editor_register_t;

typedef struct
{
  bool (*WantProjectFile)(const char *fn);
  const char *(*EnumFileExtensions)(int i, char **descptr);
  int (*LoadProject)(const char *fn, ProjectStateContext *genstate);
} project_import_register_t;

typedef struct project_config_extension_t
{
  bool (*ProcessExtensionLine)(const char *line, ProjectStateContext *ctx, bool isUndo, struct project_config_extension_t *reg);
  void (*SaveExtensionConfig)(ProjectStateContext *ctx, bool isUndo, struct project_config_extension_t *reg);
  void (*BeginLoadProjectState)(bool isUndo, struct project_config_extension_t *reg);
  void *userData;
} project_config_extension_t;

typedef struct audio_hook_register_t
{
  void (*OnAudioBuffer)(bool isPost, int len, double srate, struct audio_hook_register_t *reg);
  void *userdata1;
  void *userdata2;
  int input_nch, output_nch;
  ReaSample *(*GetBuffer)(bool isOutput, int idx);
} audio_hook_register_t;

struct KbdAccel;
struct CommandAction;

typedef struct
{
  DWORD cmd;
  const char *text;
} KbdCmd;

typedef struct
{
  int key;
  int cmd;
  int flags;
} KbdKeyBindingInfo;

typedef struct
{
  int uniqueID;
  const char *name;
  KbdCmd *action_list;
  int action_list_cnt;
  KbdKeyBindingInfo *def_keys;
  int def_keys_cnt;
  bool (*onAction)(int cmd, int val, int valhw, int relmode, HWND hwnd);
  #ifdef _WDL_PTRLIST_H_
    WDL_PtrList<struct KbdAccel> *accels;
    WDL_PtrList<struct CommandAction> *recent_cmds;
  #else
    void *accels;
    void *recent_cmds;
  #endif
  void *extended_data[32];
} KbdSectionInfo;

typedef struct
{
  #ifdef _WIN32
    CRITICAL_SECTION cs;
  #else
    pthread_mutex_t mutex;
  #endif
  PCM_source *src;
  int m_out_chan;
  double curpos;
  bool loop;
  double volume;
  double peakvol[2];
  void *preview_track;
} preview_register_t;

//----------

typedef LRESULT (*screensetCallbackFunc)(int action, char *id, void *param, int param2);

//class midi_Output
//{
//public:
//  virtual ~midi_Output() {}
//  virtual void BeginBlock() { }
//  virtual void EndBlock(int length, double srate, double curtempo) { }
//  virtual void SendMsg(MIDI_event_t *msg, int frame_offset)=0;
//  virtual void Send(unsigned char status, unsigned char d1, unsigned char d2, int frame_offset)=0;
//};
typedef void midi_Output;

//class midi_Input
//{
//public:
//  virtual ~midi_Input() {}
//  virtual void start()=0;
//  virtual void stop()=0;
//  virtual void SwapBufs(unsigned int timestamp)=0;
//  virtual void RunPreNoteTracking(int isAccum) { }
//  virtual MIDI_eventlist *GetReadBuf()=0;
//  virtual void SwapBufsPrecise(unsigned int coarsetimestamp, double precisetimestamp)
//  {
//    SwapBufs(coarsetimestamp);  // default impl is for backward compatibility
//  }
//};
typedef void midi_Input;

class MediaTrack;
class MediaItem;
class MediaItem_Take;

//class IReaperControlSurface
//{
//  public:
//    IReaperControlSurface() { }
//    virtual ~IReaperControlSurface() { }
//    virtual const char *GetTypeString()=0;
//    virtual const char *GetDescString()=0;
//    virtual const char *GetConfigString()=0;
//    virtual void CloseNoReset() { }
//    virtual void Run() { }
//    virtual void SetTrackListChange() { }
//    virtual void SetSurfaceVolume(MediaTrack *trackid, double volume) { }
//    virtual void SetSurfacePan(MediaTrack *trackid, double pan) { }
//    virtual void SetSurfaceMute(MediaTrack *trackid, bool mute) { }
//    virtual void SetSurfaceSelected(MediaTrack *trackid, bool selected) { }
//    virtual void SetSurfaceSolo(MediaTrack *trackid, bool solo) { }
//    virtual void SetSurfaceRecArm(MediaTrack *trackid, bool recarm) { }
//    virtual void SetPlayState(bool play, bool pause, bool rec) { }
//    virtual void SetRepeatState(bool rep) { }
//    virtual void SetTrackTitle(MediaTrack *trackid, const char *title) { }
//    virtual bool GetTouchState(MediaTrack *trackid, int isPan) { return false; }
//    virtual void SetAutoMode(int mode) { }
//    virtual void ResetCachedVolPanStates() { }
//    virtual void OnTrackSelection(MediaTrack *trackid) { }
//    virtual bool IsKeyDown(int key) { return false; }
//    virtual int Extended(int call, void *parm1, void *parm2, void *parm3) { return 0; }
//};
typedef void IReaperControlSurface;

typedef struct
{
  const char *type_string;
  const char *desc_string;
  IReaperControlSurface *(*create)(const char *type_string, const char *configString, int *errStats);
  HWND (*ShowConfig)(const char *type_string, HWND parent, const char *initConfigString);
} reaper_csurf_reg_t;

*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axReaperExt
{

  public:

    // Reaper VST extensions
    double  (*GetPlayPosition)();
    double  (*GetPlayPosition2)();
    double  (*GetCursorPosition)();
    int     (*GetPlayState)();
    void    (*SetEditCurPos)(double time, bool moveview, bool seekplay);
    int     (*GetSetRepeat)(int parm);
    void    (*GetProjectPath)(char *buf, int bufsz);
    void    (*OnPlayButton)();
    void    (*OnStopButton)();
    void    (*OnPauseButton)();
    int     (*IsInRealTimeAudio)();
    int     (*Audio_IsRunning)();
    // Reaper SDK

  public:

    axReaperExt(audioMasterCallback audioMaster)
      {
        // Reaper VST extensions
        *(long*)&GetPlayPosition    = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayPosition",0.0);
        *(long*)&GetPlayPosition2   = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayPosition2",0.0);
        *(long*)&GetCursorPosition  = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetCursorPosition",0.0);
        *(long*)&GetPlayState       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayState",0.0);
        *(long*)&SetEditCurPos      = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"SetEditCurPos",0.0);
        *(long*)&GetSetRepeat       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetSetRepeat",0.0);
        *(long*)&GetProjectPath     = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetProjectPath",0.0);
        *(long*)&OnPlayButton       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnPlayButton",0.0);
        *(long*)&OnStopButton       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnStopButton",0.0);
        *(long*)&OnPauseButton      = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnPauseButton",0.0);
        *(long*)&IsInRealTimeAudio  = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"IsInRealTimeAudio",0.0);
        *(long*)&Audio_IsRunning    = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"Audio_IsRunning",0.0);
        // Reaper SDK
      }

    virtual ~axReaperExt() {}

};

//----------------------------------------------------------------------
#endif


