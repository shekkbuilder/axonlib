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
    virtual void doProcessState(int aState) // overwride default method
    {
      #if defined(AX_DEBUG) // check for win32 and ax_debug
        switch(aState)
        {        
          case pst_Open:                                // - on plugin open
            axCPUID();                                  // call cpuid
            #ifdef WIN32            
              axDstdCreate();                           // create console on win32
            #endif            
            // ----------
            // check definitions and print
            trace("### axCPUID() check:");
            if (__AX_SSE3__)     trace("__AX_SSE3__");
            if (__AX_SSSE3__)    trace("__AX_SSSE3__");
            if (__AX_FPU__)      trace("__AX_FPU__");
            if (__AX_CMOV__)     trace("__AX_CMOV__");
            if (__AX_SSE__)      trace("__AX_SSE__");
            if (__AX_SSE2__)     trace("__AX_SSE2__");
            if (__AX_SSE4A__)    trace("__AX_SSE4A__");
            if (__AX_SSE5__)     trace("__AX_SSE5__");
            if (__AX_MMX__)      trace("__AX_MMX__");
            if (__AX_MMXEXT__)   trace("__AX_MMXEXT__");
            if (__AX_3DNOW__)    trace("__AX_3DNOW__");            
            if (__AX_3DNOWEXT__) trace("__AX_3DNOWEXT__");
            trace("### ---------------\n"); 
          break;
          case pst_Close:                          // - on plugin close
            #ifdef WIN32
              axDstdDestroy();                       // destroy console on win32
            #endif            
          break;
        }
      #endif
    }
};

#include "axMain.h"
