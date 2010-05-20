#ifndef axMutex_included
#define axMutex_included
//----------------------------------------------------------------------

#include "axDefines.h"

class axMutexBase
{
  public:
    axMutexBase()               {}
    virtual ~axMutexBase()      {}
    virtual void lock(void)     {}
    virtual bool tryLock(void)  {return false;}
    virtual void unlock(void)   {}
};

//----------------------------------------------------------------------
#ifdef AX_LINUX
//----------------------------------------------------------------------

#include <pthread.h>

class axMutexImpl : public axMutexBase
{
  private:
    pthread_mutex_t mMutex;// = PTHREAD_MUTEX_INITIALIZER;
  public:
    axMutexImpl() : axMutexBase() { pthread_mutex_init(&mMutex,NULL); }
    virtual ~axMutexImpl()        { pthread_mutex_destroy(&mMutex); }
    virtual void lock(void)       { pthread_mutex_lock(&mMutex); }
    virtual bool tryLock(void)    { return (pthread_mutex_trylock(&mMutex)==0); }
    virtual void unlock(void)     { pthread_mutex_unlock(&mMutex); }
};

#endif
//----------------------------------------------------------------------
#ifdef AX_WIN32
//----------------------------------------------------------------------

#define WINVER 0x0501
#include <windows.h>

class axMutexImpl : public axMutexBase
{
  private:
    CRITICAL_SECTION  CriticalSection;
  public:
    axMutexImpl() : axMutexBase() { InitializeCriticalSection(&CriticalSection); }
    virtual ~axMutexImpl()        { DeleteCriticalSection(&CriticalSection); }
    virtual void lock(void)       { EnterCriticalSection(&CriticalSection); }
    virtual bool tryLock(void)    { return TryEnterCriticalSection( &CriticalSection ); }
    virtual void unlock(void)     { LeaveCriticalSection(&CriticalSection); }
};

#endif
//----------------------------------------------------------------------

typedef axMutexImpl axMutex;

//----------------------------------------------------------------------
#endif
