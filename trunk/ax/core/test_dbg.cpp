/*

  g++ test_dbg.cpp -s -O3 -pedantic -Wno-long-long -Wall

*/

//#define AX_FORMAT_EXE // !
#define AX_DEBUG
#define AX_DEBUG_LOG "./test.log"
#define AX_DEBUG_LOG_APPEND // append will append to the log file

#include "axDefines.h"
#include "axDebug.h"
#include "axDebugConsole.h"

#include "axRand.h"

///////////////////////////////
// test code

#define _TEST_CODE_PORTION \
  \
  axDebugConsole dbg_c; \
  axRand r; \
  \
  trace(" \
  \n this test will output a random value 5000 \
  \n times in the console. \
  \n on win32 will show a msgbox at the end. \
  \n the values should be logged in './test.log' \
  \n press enter (twice on win32) to continue."); \
  \
  std::cin.get(); \
  \
  short some_n = (unsigned short)r.randInt(); \
  \
  short i = 5000; \
  while (i--) \
    trace(some_n);

///////////////////////////////

#ifdef AX_WIN32

  #include "windows.h"
  
  int APIENTRY WinMain( HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine,
                        int nCmdShow)
  {
    _TEST_CODE_PORTION;
    
    return 0;
  }
  
  class Application
  {
    public:
      Application() { }      
      ~Application()
      {
        MessageBox(NULL,(char*)"test done", "msg",
                    MB_TOPMOST | MB_OK | MB_ICONINFORMATION);
      }
  };
  Application __winapitest;

#endif

#ifdef AX_LINUX
  
  int main(void)    
  {
    // axDebugConsole also checks for AX_WIN32 and return blank class
    // in this case
    _TEST_CODE_PORTION;
    
    trace("test done");
    return 0;
  }

#endif



///////////////////////////////
