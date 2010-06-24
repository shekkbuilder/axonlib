#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

//#include "axEditor.h"

//----------------------------------------------------------------------

// format types
#define ft_Unknown  0
#define ft_Exe      1
#define ft_Vst      2
#define ft_Ladspa   3
#define ft_Dssi     4
#define ft_Lv2      5
#define ft_Au       6

//----------------------------------------------------------------------

#ifdef AX_FORMAT_EXE
#include "axFormatExe.h"
#endif

#ifdef AX_FORMAT_VST
#include "axFormatVst.h"
#endif

#ifdef AX_FORMAT_LADSPA
#include "axFormatLadspa.h"
#endif

//----------------------------------------------------------------------
#endif

/*

template<class _D, class _I, class _In, class _P>
class axFormat
{
  public:
    axFormat() {}
    virtual ~axFormat() {}
    // axDescriptor* getDescriptor()
    // axInstance*   getInstance()
    // axInterface*  getInterface()
    // axPlatform*   getPlatform()
}:

*/
