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
  \file axMutex.h
  \brief mutex/critical sections
*/

#ifndef axMutex_included
#define axMutex_included
//----------------------------------------------------------------------

#include <stdint.h> // int32_t

//----------------------------------------------------------------------
// mutex
//----------------------------------------------------------------------

/// mutex base

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

#ifdef linux

  #include <pthread.h>
  /// linux, pthread_mutex
  class axMutexX11 : public axMutexBase
  {
    private:
      pthread_mutex_t mMutex;// = PTHREAD_MUTEX_INITIALIZER;
    public:
      axMutexX11() : axMutexBase()  { pthread_mutex_init(&mMutex,NULL); }
      virtual ~axMutexX11()         { pthread_mutex_destroy(&mMutex); }
      virtual void lock(void)       { pthread_mutex_lock(&mMutex); }
      virtual bool tryLock(void)    { return (pthread_mutex_trylock(&mMutex)==0); }
      virtual void unlock(void)     { pthread_mutex_unlock(&mMutex); }
  };
  typedef axMutexX11 axMutexImpl;

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  #include <windows.h>
  /// win32, critical sections
  class axMutexW32 : public axMutexBase
  {
    private:
      CRITICAL_SECTION  CriticalSection;
    public:
      axMutexW32() : axMutexBase()  { InitializeCriticalSection(&CriticalSection); }
      virtual ~axMutexW32()         { DeleteCriticalSection(&CriticalSection); }
      virtual void lock(void)       { EnterCriticalSection(&CriticalSection); }
      virtual bool tryLock(void)    { return TryEnterCriticalSection( &CriticalSection ); }
      virtual void unlock(void)     { LeaveCriticalSection(&CriticalSection); }
  };
  typedef axMutexW32 axMutexImpl;

#endif

//----------------------------------------------------------------------

class axMutex : public axMutexImpl
{
  public:
    axMutex() : axMutexImpl() {}
};

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

//inline int32_t axLockAdd( volatile int32_t* const v, int32_t a )
//{
//  __asm__ __volatile__
//  (
//    "lock; xadd %0, (%1)"
//    : "=a" (a)
//    : "d" (v), "a" (a)
//  );
//  return( a );
//}

//----------

//inline int32_t axLockCmpXchg( volatile int32_t* const v, int32_t newValue, int32_t cmpValue )
//{
//  __asm__ __volatile__
//  (
//    "lock; cmpxchg %3, (%1)"
//    : "=a" (cmpValue)
//    : "d" (v), "a" (cmpValue), "c" (newValue)
//  );
//  return( cmpValue );
//}

//----------------------------------------------------------------------

//////inline bool compare_and_swap_32(volatile int *target, int old, int replacement)
//////{
//////  int before;
//////  __asm__ __volatile__("lock; cmpxchgl %1, %2"
//////  : "=a"(before)
//////  : "q"(replacement), "m"(*target), "0"(old)
//////  : "memory");
//////  return before == old;
//////}
//////
//////int main()
//////{
//////  int inc = 0;
//////  int cur,next;
//////  do
//////  {
//////    cur = inc;
//////    next = cur + 1;
//////  } while(!compare_and_swap_32(&inc,cur,next));
//////  return 0;
//////}

//----------------------------------------------------------------------
#endif
