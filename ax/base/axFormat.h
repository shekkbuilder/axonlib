#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

#include "base/axBase.h"

//----------------------------------------------------------------------

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

#ifndef AX_WIN_DEFAULT
  #define AX_WIN_DEFAULT 0
#endif

//----------------------------------------------------------------------
#endif
