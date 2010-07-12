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

// set macros depending on which flags are enabled
#ifdef AX_DEBUG

  #include <iostream>
  #include <fstream>

  // the axDebugLog.h below needs _trace,
  // but that is not defined until AFTER the axDebugLog.h has been included
  //
  // also some problems with axGetBasePath (redefinition)

  #ifdef AX_DEBUG_LOG
    //define axCout mDebugLog
    
    #ifndef ___TMP_NO_GLOBAL
      #define axCout gGlobalScope.mDebugLog
    #else
      #include "axDebugLog.h"
      axDebugLog axCout;
    #endif
        
  #else
    #define axCout std::cout
  #endif

  #define trace(x) \
    axCout  <<  "["  << axGetFileName(__FILE__) << ":" <<  __LINE__ << "] " \
            << x << std::endl
  #define _trace(x) \
    axCout  <<  x << std::endl

#else // AX_DEBUG

  #define trace(x)
  #define _trace(x)

#endif

#include "axAssert.h"

#endif // axDebug_included
