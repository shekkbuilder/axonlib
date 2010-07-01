#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axAssert.h"
#include "core/axUtils.h"
#include "base/axContext.h"
#include "par/axParameter.h"
//#include "gui/axWindow.h"

//----------------------------------------------------------------------

//// format types
#define ft_Unknown  0
#define ft_Exe      1
#define ft_Vst      2
#define ft_Ladspa   3
#define ft_Dssi     4
#define ft_Lv2      5
#define ft_Au       6

#define DEFAULT_DESCRIPTOR axDescriptor

//----------------------------------------------------------------------

class axFormat;
class axPlatform;
class axDescriptor;
class axInstance;
class axInterface;

//----------

class axFormat
{
  public:
    axFormat() {}
    virtual axFormat*     getFormat(void)       { return this; }
    virtual axPlatform*   getPlatform(void)     { return NULL; }
    virtual axDescriptor* getDescriptor(void)   { return NULL; }
    virtual axInstance*   getInstance(void)     { return NULL; }
    virtual axInterface*  getInterface(void)    { return NULL; }
    virtual int           getType(void)         { return ft_Unknown; }
    virtual char*         getName(void)         { return (char*)"noname"; } // for window class registration
};

//----------

#include "axPlatform.h"
#include "axInterface.h"

//----------

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
