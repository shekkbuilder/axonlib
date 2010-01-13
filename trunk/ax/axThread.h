#ifndef axThread_included
#define axThread_included
//----------------------------------------------------------------------

//TODO:
//- windows version
//- mutex/lock?

#include <pthread.h>
#include <unistd.h> // sleep

class axThread
{
  private:

    pthread_t mThreadHandle;
    bool      mThreadRunning;
    int       mThreadSleep;

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
      {
        mThreadHandle = 0;
        mThreadRunning = false;
        mThreadSleep = 10;
      }

    //----------

    virtual ~axThread()
      {
        if (mThreadRunning) stopThread();
      }

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

//----------------------------------------------------------------------
#endif
