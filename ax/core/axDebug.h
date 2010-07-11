/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axDebug_included
#define axDebug_included

///////////////////////////////////////////////////////////////////////////////
// axDebug.h
///////////////////////////////////////////////////////////////////////////////

#include "axDefines.h"

//----------

//#include "axStdlib.h"

// ccernn:
// this created a curcular-inclusion (if AX_DEBUG_MEM or something defined)
// axStdlib includes axMalloc, and axMalloc again, includes axDebug..
// a simple solution (for this single case) is to have this internal
// function inline:

inline char* ax__Strrchr(const char* s, int c)
{
  char* p = NULL;
  while (*s++) if (*s == c) p = (char*) s;
  return p;
}

//----------

//axGetFileName() is needed by trace()
inline const char* axGetFileName(const char* path)
{
  if (!path)
    return "NULL";
  const char *slash, *backslash;
  slash = ax__Strrchr(path, '/');
  backslash = ax__Strrchr(path, '\\');
  if (slash)
    return slash + 1;
  else if (backslash)
    return backslash + 1;
  return path;
}

// set macros depending on which flags are enabled
#ifdef AX_DEBUG
  #include <iostream>
  #include <fstream>

  // the axDebugLog.h below needs _trace,
  // but that is not defined until AFTER the axDebugLog.h has been included
  //
  // also some problems with axGetBasePath (redefinition)

  #ifdef AX_DEBUG_LOG
    #include "axDebugLog.h"
    axDebugLog axCout;
  #else
    #define axCout std::cout
  #endif

  #define trace(x) \
    axCout  <<  "["  << axGetFileName(__FILE__) << ":" <<  __LINE__ << "] " \
            << x << std::endl
  #define _trace(x) \
    axCout  <<  x << std::endl

#else

  #define trace(x)
  #define _trace(x)

#endif

#include "axAssert.h"

#endif // axDebug_included
