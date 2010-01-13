#include <stdio.h>
#include "axThread.h"
#include "axMutex.h"

volatile int test; // need volatile?
axMutex mutex;
volatile int lock;

//----------------------------------------------------------------------

class myThread1 : public axThread
{
  public:
    myThread1() : axThread() {}
    virtual void doThreadFunc(void)
    {
      for (int i=0; i<1000000; i++)
      {
        // (a)
        //mutex.lock();
        //test++;
        //mutex.unlock();
        // (b)
        int qwe=0;
        while (qwe==0) { qwe = axLockCmpXchg(&lock,1,0); }
        test++;
        while (qwe==1) { qwe = axLockCmpXchg(&lock,0,1); }
      }
    }
};

//----------

class myThread2 : public axThread
{
  public:
    myThread2() : axThread() {}
    virtual void doThreadFunc(void)
    {
      for (int i=0; i<1000000; i++)
      {
        // (a)
        //mutex.lock();
        //test++;
        //mutex.unlock();
        // (b)
        //if (mutex.tryLock())
        //{
        //  test++;
        //  mutex.unlock();
        //}
        // (c)
        int qwe=0;
        while (qwe==0) { qwe = axLockCmpXchg(&lock,1,0); }
        test++;
        while (qwe==1) { qwe = axLockCmpXchg(&lock,0,1); }        
      }
   }
};

//----------

myThread1 thread1;
myThread2 thread2;

//----------------------------------------------------------------------

int main(void)
{ 
  test = 0;
  lock = 0;
  thread1.startThread(-1);
  thread2.startThread(-1);
  thread1.stopThread();
  thread2.stopThread();
  printf("test = %i\n",test);
  return 0;
}

