#ifndef axPluginReaper_included
#define axPluginReaper_included
//----------------------------------------------------------------------

#include "axPlugin.h"

class axReaperExtensions
{
  protected:
    // GetPlayPosition() returns the current playback position of the project.
    // This is the time the user is hearing (and the transport shows etc).
    // The value is in seconds.
    double (*GetPlayPosition)() = NULL;
    // GetPlayPosition() returns the current playback decode position of the project.
    // This is the time of the audio block that is being processed by the host.
    // The value is in seconds.
    // This may be behind where your plug-in is processing, due to anticipative processing and/or PDC.
    double (*GetPlayPosition2)() = NULL;
    // GetCursorPosition() returns the current edit cursor position (if any), in seconds.
    double (*GetCursorPosition)() = NULL;
    // GetPlayState() returns an integer value representing the project play state. 1=play, 2=paused, 5=recording, 6=record paused.
    int (*GetPlayState)() = NULL;
    // SetEditCurPos() repositions the edit cursor to "time" (in seconds),
    // optionally moving the view if necessary, and optionally seeking playback (if playing back).
    // This function should ONLY be called from a UI context (i.e. from the editor window, NOT from audio processing etc).
    void (*SetEditCurPos)(double time, bool moveview, bool seekplay) = NULL;
    // GetSetRepeat() is used to change or query the transport "loop/repeat" state.
    // Pass a parameter of -1 to query the repeat state, 0 to clear, 1 to set, and >1 to toggle.
    // The return value is the new value of the repeat state.
    // ONLY use this function to change the repeat state from the UI thread (however you can query safely at any time).
    int (*GetSetRepeat)(int parm) = NULL;
    // GetProjectPath() can be used to query the path that media files are stored in for the current project.
    void (*GetProjectPath)(char *buf, int bufsz) = NULL;
    // These functions control the main transport for the host app. Only call these from the UI thread.
    void (*OnPlayButton)() = NULL;
    void (*OnStopButton)() = NULL;
    void (*OnPauseButton)() = NULL;
    // Returns nonzero if in the main audio thread, or in a thread doing synchronous multiprocessing.
    // In these instances low latency is key.
    // If this is 0, and you are in processReplacing, then you are being called in an anticipative processing thread.
    int (*IsInRealTimeAudio)() = NULL;
    // Returns nonzero if the audio device is open and running.
    int (*Audio_IsRunning)() = NULL;

  public:

    axReaperExtensions(audioMasterCallback audioMaster)
      {
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
      }

    virtual ~axReaperExtensions() {}

};

//----------------------------------------------------------------------
#endif


