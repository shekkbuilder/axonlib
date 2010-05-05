// -------------------------------
#ifndef axMemory_included
#define axMemory_included


// /////////////////////////////////////////////////////////////////////////////


// ### could be now deprecated


// /////////////////////////////////////////////////////////////////////////////

// --------------------
#if defined (AX_DEBUG) && defined (AX_DEBUG_MEMORY)

#include "axDefines.h"  // u32
#include <iostream>     // cout
//#include <fstream>
using namespace std;

// hmmm. in linux, if i don't have the following line, the compiler complaints about not finding
// axGetFileName, but if i _do_ have it, it says "new" needs another argument (string?)
#include "core/axUtils.h"

#ifndef ulong
  #define ulong unsigned long
#endif
#ifndef culong
  #define culong const unsigned long
#endif

// counter
ulong axTotalAllocMem = 0;

// override new
// -----------------
void* operator new(unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] new, ";
  if (void* ptr = ::operator new(size, std::nothrow))
  {
    axTotalAllocMem += size;
    cout << static_cast<void*>(ptr) << ", " << size << ", "
    << axTotalAllocMem << "\n";
    return ptr;
  }
  throw "new failed!";
}

void* operator new[](unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] new[], ";
  if (void* ptr = ::operator new(size, std::nothrow))
  {
    axTotalAllocMem += size;
    cout << static_cast<void*>(ptr) << ", " << size << ", "
    << axTotalAllocMem << "\n";
    return ptr;
  }
  throw "new[] failed!";
}

// override delete
// -----------------
// ### seems not to work !

void operator delete(void* ptr, unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] delete[], ";
  axTotalAllocMem -= size;
  cout << static_cast<void*>(ptr) << ", " << size << ", "
  << axTotalAllocMem << "\n";
  if (ptr == NULL) throw "delete failed!";
  return ::operator delete[](ptr);
}

void operator delete[](void* ptr, unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] delete[], ";
  axTotalAllocMem -= size;
  cout << static_cast<void*>(ptr) << ", " << size << ", "
  << axTotalAllocMem << "\n";
  if (ptr == NULL) throw "delete[] failed!";
  return ::operator delete[](ptr);
}

// override stdlib functions
// ----------------------------
void* axMalloc(unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] malloc(), ";
  if (void* ptr = ::operator new(size, std::nothrow))
  {
    axTotalAllocMem += size;
    cout << static_cast<void*>(ptr) << ", " << size << ", "
    << axTotalAllocMem << "\n";
    return ptr;
  }
  throw "malloc() failed!";
}

// ######### not called ?
void* axCalloc(unsigned int size, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] calloc(), ";
  if (void* ptr = ::operator new(size, std::nothrow))
  {
    axTotalAllocMem += size;
    cout << static_cast<void*>(ptr) << ", " << size << ", "
    << axTotalAllocMem << "\n";
    return ptr;
  }
  throw "calloc() failed!";
}

// ######### not called ?
void axFree(void* ptr, const char* file, culong line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] free(), ";
  ulong size = sizeof(ptr);
  axTotalAllocMem -= size;
  cout << static_cast<void*>(ptr) << ", " << size << ", "
  << axTotalAllocMem << "\n";
  if (ptr == NULL) throw "free() failed!";
  free(ptr);
}

//void* operator realloc(void* ptr, unsigned int size, const char* file, culong line)

// define all macros here
// --------------------------
#define new new(__FILE__, __LINE__)
// #define delete delete(__FILE__, __LINE__) // ###### <-- causes errors..hm
#define malloc(size) axMalloc(size, __FILE__, __LINE__)
#define free(ptr) axFree(ptr, __FILE__, __LINE__)

#endif
// --------------------

#endif
// -------------------------------
