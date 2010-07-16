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

#if 0

#ifndef axDebug_included
#define axDebug_included

///////////////////////////////////////////////////////////////////////////////
// axDebug.h
//
//
//
//   disabled / is not being used !!!
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "axDefines.h"

// pragma message
#define _pmsg(x)  _Pragma (#x)
#define pmsg(x)   _pmsg(message (#x))

//----------

// set macros depending on which flags are enabled
#ifdef AX_DEBUG

  #include <iostream>
  #include <fstream>

  #include        "axBasePath.h"
  
  // AX_DEBUG_CONSOLE
  // ------------------------------------------------------
  #if defined AX_WIN32 && defined AX_DEBUG_CONSOLE
    #include        "axDebugConsole.h"    
    #define         _AX_D_CON_CHECK if (axCon.init)
    axDebugConsole  axCon;
  #else  
    #define         _AX_D_CON_CHECK
  #endif  
  
    
  
  // AX_DEBUG_LOG
  // ------------------------------------------------------
  #ifdef AX_DEBUG_LOG
    #include    "axDebugLog.h"
    #define     _AX_D_LOG_CHECK if (axCout.init)
    axDebugLog  axCout;
    // _AX_COUT_SETUP
    // should be called before "new axBaseImpl<...>" in WIN32 entry points
    #define _AX_COUT_SETUP axCout.setup();
  
    // trace macros
    #define trace(x) \
    _AX_D_LOG_CHECK \
      _AX_D_CON_CHECK \
        axCout  << "["  << axGetFileName(__FILE__) << ":" <<  __LINE__ \
                << "] " << x << std::endl
    #define _trace(x) \
      _AX_D_LOG_CHECK \
        _AX_D_CON_CHECK \
          axCout << x << std::endl

  #else // AX_DEBUG_LOG
  // ------------------------------------------------------
    #define axCout std::cout    
    
    // trace macros
    #define trace(x) \
      _AX_D_CON_CHECK \
      axCout  << "["  << axGetFileName(__FILE__) << ":" <<  __LINE__ \
              << "] " << x << std::endl
    #define _trace(x) \
      _AX_D_CON_CHECK \
      axCout << x << std::endl

  #endif
  // -------------------------------------------
  
#else // AX_DEBUG

  #define trace(x)
  #define _trace(x)

#endif

// include assert
#include "axAssert.h"

#endif // axDebug_included

#endif // 0
