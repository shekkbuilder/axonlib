#ifndef axReaperExt_included
#define axReaperExt_included
//----------------------------------------------------------------------

// http://reaper.fm/sdk/plugin/plugin.php
// http://reaper.fm/sdk/vst/vst_ext.php

//#include "pluginterfaces/vst2.x/aeffect.h"
//#include "pluginterfaces/vst2.x/aeffectx.h"

class axReaperExt
{
  private:
    // Reaper VST extensions
    double  (*mGetPlayPosition)();
    double  (*mGetPlayPosition2)();
    double  (*mGetCursorPosition)();
    int     (*mGetPlayState)();
    void    (*mSetEditCurPos)(double time, bool moveview, bool seekplay);
    int     (*mGetSetRepeat)(int parm);
    void    (*mGetProjectPath)(char *buf, int bufsz);
    void    (*mOnPlayButton)();
    void    (*mOnStopButton)();
    void    (*mOnPauseButton)();
    int     (*mIsInRealTimeAudio)();
    int     (*mAudio_IsRunning)();
    // Reaper SDK
    bool    (*mAddCustomizableMenu)(const char* menuidstr, const char* menuname, const char* kbdsecname, bool addtomainmenu);
    int     (*mShowMessageBox)(const char* msg, const char* title, int type);

  public:

    axReaperExt(audioMasterCallback audioMaster)
      {
        if (audioMaster)
        {
          // the following generates (in release build...)
          // warning: dereferencing type-punned pointer will break strict-aliasing rules
          // Reaper VST extensions
          *(long*)&mGetPlayPosition     = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayPosition",0.0);
          *(long*)&mGetPlayPosition2    = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayPosition2",0.0);
          *(long*)&mGetCursorPosition   = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetCursorPosition",0.0);
          *(long*)&mGetPlayState        = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetPlayState",0.0);
          *(long*)&mSetEditCurPos       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"SetEditCurPos",0.0);
          *(long*)&mGetSetRepeat        = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetSetRepeat",0.0);
          *(long*)&mGetProjectPath      = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetProjectPath",0.0);
          *(long*)&mOnPlayButton        = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnPlayButton",0.0);
          *(long*)&mOnStopButton        = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnStopButton",0.0);
          *(long*)&mOnPauseButton       = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"OnPauseButton",0.0);
          *(long*)&mIsInRealTimeAudio   = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"IsInRealTimeAudio",0.0);
          *(long*)&mAudio_IsRunning     = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"Audio_IsRunning",0.0);
          // Reaper SDK
          *(long*)&mAddCustomizableMenu = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"AddCustomizableMenu",0.0);
          *(long*)&mShowMessageBox      = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"ShowMessageBox",0.0);
        }
        else
        {
          // set all to NULL, for a bnit more safe usage:
          // if (mFunc) mFunc();
        }
      }

    //----------

    //~axReaperExt()
    //  {
    //  }

    //--------------------------------------------------

    double  GetPlayPosition(void) { if (mGetPlayPosition) return mGetPlayPosition(); else return 0; }
    double  GetPlayPosition2(void) { if (mGetPlayPosition2) return mGetPlayPosition2(); else return 0; }
    double  GetCursorPosition(void) { if (mGetCursorPosition) return mGetCursorPosition(); else return 0; }
    int     GetPlayState(void) { if (mGetPlayState) return mGetPlayState(); else return 0; }
    void    SetEditCurPos(double time, bool moveview, bool seekplay) { if (mSetEditCurPos) mSetEditCurPos(time,moveview,seekplay); }
    int     GetSetRepeat(int parm) { if (mGetSetRepeat) return mGetSetRepeat(parm); else return 0; }
    void    GetProjectPath(char* buf, int bufs) { if (buf) buf[0]=0; if (mGetProjectPath) mGetProjectPath(buf,bufs); }
    void    OnPlayButton(void) { if (mOnPlayButton) mOnPlayButton(); }
    void    OnStopButton(void) { if (mOnStopButton) mOnStopButton(); }
    void    OnPauseButton(void) { if (mOnPauseButton) mOnPauseButton(); }
    int     IsInRealTimeAudio(void) { if (mIsInRealTimeAudio) return mIsInRealTimeAudio(); else return 0; }
    int     Audio_IsRunning(void) { if (mAudio_IsRunning) return mAudio_IsRunning(); else return 0; }

    //--------------------------------------------------

    bool    AddCustomizableMenu(const char* menuidstr, const char* menuname, const char* kbdsecname, bool addtomainmenu) { if (mAddCustomizableMenu) return mAddCustomizableMenu(menuidstr,menuname,kbdsecname,addtomainmenu); else return false; }
    //type: 0=OK, 2=OKCANCEL(1?), 2=ABORTRETRYIGNORE, 3=YESNOCANCEL, 4=YESNO, 5=RETRYCANCEL
    //ret: 1=OK, 2=CANCEL, 3=ABORT, 4=RETRY, 5=IGNORE, 6=YES, 7=NO
    int     ShowMessageBox(const char* msg, const char* title, int type) { if (mShowMessageBox) return mShowMessageBox(msg,title,type); else return 0; }

};

//----------------------------------------------------------------------
#endif
