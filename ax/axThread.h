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
 * @file
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */

#ifndef axThread_included
#define axThread_included
//----------------------------------------------------------------------

//TODO:
//- windows version
//- mutex/lock?

class axThreadBase
{
  protected:
    bool      mThreadRunning;
    int       mThreadSleep;
  public:
    axThreadBase()                    { mThreadRunning=false; mThreadSleep=10;  }
    virtual ~axThreadBase()           { if (mThreadRunning) stopThread();  }
    virtual void doThreadFunc(void)   {}
    virtual void startThread(int ms)  {}
    virtual void stopThread(void)     {}
};

//----------------------------------------------------------------------

#ifdef linux

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
            while (thr->mThreadRunning) //==true)
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
        //mThreadSleep = 10;
      }

    //----------

    //virtual ~axThread()
    //  {
    //    if (mThreadRunning) stopThread();
    //  }

    //--------------------------------------------------

    // overload this one...
    virtual void doThreadFunc(void) {}

    //--------------------------------------------------

    virtual void startThread(int ms)
      {
        mThreadSleep   = ms;
        mThreadRunning = true;
        /*int result =*/ pthread_create(&mThreadHandle,NULL,&threadProc,this);
      }

    //--------------------------------------------------

    virtual void stopThread(void)
      {
        void* ret;
        mThreadRunning = false;
        /*int result =*/ pthread_join(mThreadHandle,&ret);
      }

};

#endif

//----------------------------------------------------------------------

#ifdef WIN32

#include <windows.h>
//#include <unistd.h> // sleep

class axThread : public axThreadBase
{
  private:

    //pthread_t mThreadHandle;
    HANDLE    mThreadHandle;
    DWORD     mThreadID;
    //bool      mThreadRunning;
    //int       mThreadSleep;

  private:

    //static void* threadProc(void* data)
    static DWORD WINAPI threadProc(LPVOID data)
      {
        //axThread* thr = (axThread*)data;
        axThread* thr = (axThread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning) //==true)
            {
              thr->doThreadFunc();
              Sleep(thr->mThreadSleep); //ms*1000;
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
        mThreadID = 0;
        //mThreadRunning = false;
        //mThreadSleep = 10;
      }

    //----------

    //virtual ~axThread()
    //  {
    //    if (mThreadRunning) stopThread();
    //  }

    //--------------------------------------------------

    // overload this one...
    virtual void doThreadFunc(void) {}

    //--------------------------------------------------

    virtual void startThread(int ms)
      {
        mThreadSleep   = ms;
        mThreadRunning = true;
        // /*int result =*/ pthread_create(&mThreadHandle,NULL,&threadProc,this);
      	mThreadHandle = CreateThread(NULL,0,&threadProc,(LPVOID)this,NULL,&mThreadID);
      }

    //--------------------------------------------------

    virtual void stopThread(void)
      {
        void* ret;
        mThreadRunning = false;
        // /*int result =*/ pthread_join(mThreadHandle,&ret);
        //DWORD waiter = WaitForMultipleObjects(MAX_THREADS,hThreads,TRUE,INFINITE);
        DWORD waiter = WaitForSingleObject(mThreadHandle,INFINITE);
        CloseHandle(mThreadHandle);
      }

};


#endif

//----------------------------------------------------------------------
#endif










