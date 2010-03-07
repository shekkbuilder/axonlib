/*
  ******************************************************************************
  test cpu for features
  (c) 2009, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://axonlib.googlecode.com/>
  ******************************************************************************
*/
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1

#define AX_DEBUG      // add debug
#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"

class myPlugin : public axPlugin
{
  public:
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost, aNumProgs, aNumParams, aPlugFlags)
    {
      describe("cpuidtest","liteon","cpuidtest", 0, 0x6c74);
      setupAudio(0, 0);
      appendParameter( new parInteger(this,0, "null", "", 0, 0, 1, 0) );
    }
    virtual void doProcessState(int aState) // override default method
    {
      #if defined(AX_DEBUG) // check for win32 and ax_debug
        switch(aState)
        {        
          case pst_Open:                                // - on plugin open
            axCPUID();                                  // call cpuid
            #if defined(WIN32) && defined(AX_DEBUG) 
              axDstdCreate();                           // create console (win32 only)
              axDwinCreate();                           // create debug window (win32 only)
            #endif            
            // ----------
            // check definitions and print
            trace("### axCPUID() check:"); wdebug("### axCPUID() check:", "", __LINE__);
            if (__AX_SSE3__)     { trace("__AX_SSE3__");     wdebug("__AX_SSE3__", ""); }
            if (__AX_SSSE3__)    { trace("__AX_SSSE3__");    wdebug("__AX_SSSE3__", ""); }
            if (__AX_FPU__)      { trace("__AX_FPU__");      wdebug("__AX_FPU__", ""); }
            if (__AX_CMOV__)     { trace("__AX_CMOV__");     wdebug("__AX_CMOV__", ""); }
            if (__AX_SSE__)      { trace("__AX_SSE__");      wdebug("__AX_SSE__", ""); }
            if (__AX_SSE2__)     { trace("__AX_SSE2__");     wdebug("__AX_SSE2__", ""); }
            if (__AX_SSE4A__)    { trace("__AX_SSE4A__");    wdebug("__AX_SSE4A__", ""); }
            if (__AX_SSE5__)     { trace("__AX_SSE5__");     wdebug("__AX_SSE5__", ""); }
            if (__AX_MMX__)      { trace("__AX_MMX__");      wdebug("__AX_MMX__", ""); }
            if (__AX_MMXEXT__)   { trace("__AX_MMXEXT__");   wdebug("__AX_MMXEXT__", ""); }
            if (__AX_3DNOW__)    { trace("__AX_3DNOW__");    wdebug("__AX_3DNOW__", ""); }           
            if (__AX_3DNOWEXT__) { trace("__AX_3DNOWEXT__"); wdebug("__AX_3DNOWEXT__", ""); }
            trace("### ---------------\n"); 
          break;
          case pst_Close:                          // - on plugin close
            #if defined(WIN32) && defined(AX_DEBUG)
              axDstdDestroy();                       // destroy console (win32 only)
              axDwinDestroy();                       // destroy debug window (win32 only)
            #endif            
          break;
        }
      #endif
    }
};

#include "axMain.h"