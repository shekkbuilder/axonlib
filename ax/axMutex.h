#ifndef axMutex_included
#define axMutex_included
//----------------------------------------------------------------------

/*

bool trylock(void)
  {
    //@tryagain
      mov  eax,1
      xchg eax,[mutex]
      cmp eax,0
      jz @canlock
      ret 0
      //sleep
      //jmp @tryagain
    @canlock
      ret 1
  }

*/

//----------------------------------------------------------------------

class axMutexBase
{
  public:
    axMutexBase() {}
    virtual ~axMutexBase() {} //=0
    virtual void lock() {} //=0
    virtual bool tryLock(void) { return true; } //=0
    virtual void unlock() {} //=0
};

//----------------------------------------------------------------------
#ifdef WIN32
#include <windows.h>

class axMutex : public axMutexBase
{
  private:
    CRITICAL_SECTION  CriticalSection;
  public:
    axMutex() : axMutexBase()   { InitializeCriticalSection(&CriticalSection); }
    virtual ~axMutex()          { DeleteCriticalSection(&CriticalSection); }
    virtual void lock(void)     { EnterCriticalSection(&CriticalSection); }
    virtual bool tryLock(void)  { return TryEnterCriticalSection( &CriticalSection ); }
    virtual void unlock(void)   { LeaveCriticalSection(&CriticalSection); }
};

#endif
//----------------------------------------------------------------------
#ifdef linux
#include <pthread.h>

class axMutex : public axMutexBase
{
  private:
    pthread_mutex_t mMutex;// = PTHREAD_MUTEX_INITIALIZER;
  public:
    axMutex() : axMutexBase()   { pthread_mutex_init(&mMutex,NULL); }
    virtual ~axMutex()          { pthread_mutex_destroy(&mMutex); }
    virtual void lock(void)     { pthread_mutex_lock(&mMutex); }
    virtual bool tryLock(void)  { return pthread_mutex_trylock(&mMutex); }
    virtual void unlock(void)   { pthread_mutex_unlock(&mMutex); }
};

#endif

//----------------------------------------------------------------------
#endif
