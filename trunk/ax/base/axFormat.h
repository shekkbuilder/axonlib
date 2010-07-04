#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------
// 'main' include file

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axAssert.h"

//----------------------------------------------------------------------

// format types
#define ft_Unknown  0
#define ft_Exe      1
#define ft_Vst      2
#define ft_Ladspa   3
//#define ft_Dssi     4
//#define ft_Lv2      5
//#define ft_Au       6

//----------------------------------------------------------------------

class axFormat
{
  public:
    //axFormat() {}
    //virtual ~axFormat() {}
};

//----------

#ifdef AX_FORMAT_EXE
#include "base/axFormatExe.h"
#endif

#ifdef AX_FORMAT_VST
#include "base/axFormatVst.h"
#endif

#ifdef AX_FORMAT_LADSPA
#include "base/axFormatLadspa.h"
#endif

//----------------------------------------------------------------------

// included elsewhere?

//#include "base/axPlatform.h"
//#include "base/axInterface.h"

//----------

#define DEFAULT_DESCRIPTOR axDescriptor
#define DEFAULT_INSTANCE   axInstance

//----------------------------------------------------------------------
#endif
