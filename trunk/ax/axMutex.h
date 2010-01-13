#ifndef axMutex_included
#define axMutex_included
//----------------------------------------------------------------------

#include <stdint.h> // int32_t

//----------------------------------------------------------------------
// mutex
//----------------------------------------------------------------------

class axMutexBase
{
  public:
    axMutexBase() {}
    virtual ~axMutexBase() {}
    virtual void lock() {}
    virtual bool tryLock(void) { return true; }
    virtual void unlock() {}
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
    virtual bool tryLock(void)  { return (pthread_mutex_trylock(&mMutex)==0); }
    virtual void unlock(void)   { pthread_mutex_unlock(&mMutex); }
};

#endif

//----------------------------------------------------------------------
// atomic
//----------------------------------------------------------------------

/*
// pseudo code:
int cmpxchg(int *addr, int old, int new)
{
  int was = *addr;
  if (was == old) *addr = new;
  return was;
}
*/

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

inline int32_t axLockAdd( volatile int32_t* const v, int32_t a )
{
  __asm__ __volatile__
  (
    "lock; xadd %0, (%1)"
    : "=a" (a)
    : "d" (v), "a" (a)
  );
  return( a );
}

//----------

inline int32_t axLockCmpXchg( volatile int32_t* const v, int32_t newValue, int32_t cmpValue )
{
  __asm__ __volatile__
  (
    "lock; cmpxchg %3, (%1)"
    : "=a" (cmpValue)
    : "d" (v), "a" (cmpValue), "c" (newValue)
  );
  return( cmpValue );
}

//----------------------------------------------------------------------

//inline bool compare_and_swap_32(volatile int *target, int old, int replacement)
//{
//  int before;
//  __asm__ __volatile__("lock; cmpxchgl %1, %2"
//  : "=a"(before)
//  : "q"(replacement), "m"(*target), "0"(old)
//  : "memory");
//  return before == old;
//}
//
//int main()
//{
//  int inc = 0;
//  int cur,next;
//  do
//  {
//    cur = inc;
//    next = cur + 1;
//  } while(!compare_and_swap_32(&inc,cur,next));
//  return 0;
//}

//----------------------------------------------------------------------
#endif
