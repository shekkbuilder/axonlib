// -------------------------------
#ifndef axMemory_included
#define axMemory_included

// --------------------
#if defined(AX_DEBUG) && defined(AX_DEBUG_MEMORY)

#ifndef cu32
  #define cu32 const u32
#endif

// counter
u32 axTotalAllocMem = 0;

// override new
// -----------------
void* operator new(size_t size, const char* file, cu32 line)
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

void* operator new[](size_t size, const char* file, cu32 line)
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

void operator delete(void* ptr, size_t size, const char* file, cu32 line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] delete[], ";
  axTotalAllocMem -= size;
  cout << static_cast<void*>(ptr) << ", " << size << ", "
  << axTotalAllocMem << "\n";  
  if (ptr == NULL) throw "delete failed!";  
  return ::operator delete[](ptr);
}

void operator delete[](void* ptr, size_t size, const char* file, cu32 line)
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
void* axMalloc(size_t size, const char* file, cu32 line)
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
void* axCalloc(size_t size, const char* file, cu32 line)
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
void axFree(void* ptr, const char* file, cu32 line)
{
  cout << "[" << axGetFileName(file) << "|" << line << "] free(), ";
  u32 size = sizeof(ptr);
  axTotalAllocMem -= size;
  cout << static_cast<void*>(ptr) << ", " << size << ", "
  << axTotalAllocMem << "\n";
  if (ptr == NULL) throw "free() failed!";
  free(ptr);
}

//void* operator realloc(void* ptr, size_t size, const char* file, cu32 line)

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