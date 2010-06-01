#ifndef axThread_included
#define axThread_included
//----------------------------------------------------------------------

#include "axDefines.h"

//----------------------------------------------------------------------

class axThreadBase
{
  protected:
    bool  mThreadRunning;
    int   mThreadSleep;
  public:
    axThreadBase()                      { mThreadRunning=false; mThreadSleep=-1; }
    virtual ~axThreadBase()             {}
    virtual void startThread(int ms=-1) {}
    virtual void stopThread(void)       {}
    //
    virtual void doThreadFunc(void)     {}
    //
};

//----------------------------------------------------------------------
#ifdef AX_LINUX
//----------------------------------------------------------------------

#include <pthread.h>
#include <unistd.h> // sleep

class axThread : public axThreadBase
{
  private:
    pthread_t mThreadHandle;
    //bool      mThreadRunning;
    //int       mThreadSleep;

  private:

    static void* threadProc(void* data)
      {
        axThread* thr = (axThread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              usleep(thr->mThreadSleep*1000); //ms*1000;
            }
          } else thr->doThreadFunc();
        }
        return NULL;
      }

  public:

    axThread()
    : axThreadBase()
      {
        mThreadHandle = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~axThread()
      {
        if (mThreadRunning) stopThread();
      }

    //virtual void doThreadFunc(void) {}

    virtual void startThread(int ms=-1)
      {
        mThreadSleep = ms;
        mThreadRunning = true;
        pthread_create(&mThreadHandle,NULL,&threadProc,this);
      }

    virtual void stopThread(void)
      {
        mThreadRunning = false;
        void* ret;
        pthread_join(mThreadHandle,&ret);
      }

};

#endif

//----------------------------------------------------------------------
#ifdef AX_WIN32
//----------------------------------------------------------------------

#include <windows.h>
//#include <unistd.h> // sleep

class axThread : public axThreadBase
{
  private:
    HANDLE  mThreadHandle;
    DWORD   mThreadID;
      //bool  mThreadRunning;
      //int   mThreadSleep;

  private:

    static DWORD WINAPI threadProc(LPVOID data)
      {
        axThread* thr = (axThread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              Sleep(thr->mThreadSleep);
            }
          } else thr->doThreadFunc();
        }
        return NULL;
      }

  public:

    axThread() //: axThreadBase()
      {
        mThreadHandle = 0;
        mThreadID = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~axThread()
      {
        if (mThreadRunning) stopThread();
      }

    virtual void startThread(int ms=-1)
      {
        mThreadSleep   = ms;
        mThreadRunning = true;
        mThreadHandle = CreateThread(NULL,0,&threadProc,(LPVOID)this,NULL,&mThreadID);
      }

    virtual void stopThread(void)
      {
        void* ret;
        mThreadRunning = false;
        DWORD waiter = WaitForSingleObject(mThreadHandle,INFINITE);
        CloseHandle(mThreadHandle);
      }

};

#endif
//----------------------------------------------------------------------
#endif
