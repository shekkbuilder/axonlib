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

#ifndef axDebugLog_included
#define axDebugLog_included

///////////////////////////////////////////////////////////////////////////////
// axDebugLog.h
//
//
//
//   disabled / is not being used !!!
//
//
//
///////////////////////////////////////////////////////////////////////////////

#if defined AX_DEBUG

#if defined AX_DEBUG_LOG

#include <iostream>
#include <fstream>

#include "axDefines.h"
#include "axStdlib.h"
#include "axMutex.h" // linux: undefined reference to `pthread_mutex_trylock'
#include "axBasePath.h" // axGetBasePath is now there

// check for debug log append
#ifndef AX_DEBUG_LOG_APPEND
  #define AX_DEBUG_LOG_APPEND
#else
  #undef AX_DEBUG_LOG_APPEND
  #define AX_DEBUG_LOG_APPEND | std::fstream::app
#endif

// tee buffer class. based on code from:
// http://wordaligned.org/articles/cpp-streambufs
class axTeebuf : public std::streambuf
{
  private:

    std::streambuf* sb1;
    std::streambuf* sb2;

    virtual int overflow(int c)
    {
      if (c == EOF)
        return !EOF;
      else
      {
        int const r1 = sb1->sputc(c);
        int const r2 = sb2->sputc(c);
        return r1 == EOF || r2 == EOF ? EOF : c;
      }
    }

    virtual int sync()
    {
      int const r1 = sb1->pubsync();
      int const r2 = sb2->pubsync();
      return (r1 == 0 && r2 == 0) ? 0 : -1;
    }

  public:

   axTeebuf(std::streambuf* sb1, std::streambuf* sb2)
    : sb1(sb1), sb2(sb2) {}

  ~axTeebuf() {}
};

/*
// blackhole stream
class axBlackhole : public std::ostream
{
  public:
    axBlackhole () :
      std::ostream (NULL) {}
    ~axBlackhole () {}
};
*/

// axDebugLog is an ostream object
class axDebugLog : public std::ostream
{
  private:

    std::fstream        filestream;    
    axTeebuf            tbuf;
    
    //axBlackhole       blackhole;
    //axMutex           mtx;

  public:
    bool init;
    
    axDebugLog() :
      std::ostream(&tbuf), tbuf(std::cerr.rdbuf(), filestream.rdbuf())
    {
      init = 0;
    }

    ~axDebugLog()
    {
      // mtx.unlock();
      
      // do not close fstream with this destructor or log data will be lost !
      // leave it to the OS
      //axfstream.close();
    }

    //void setup(void)
    void setup(void)
    {
      init = 1;
      char filepath[AX_MAX_PATH] = "";
      axGetBasePath(filepath);
      axStrcat(filepath, AX_DEBUG_LOG);
      // std::cout << "dbg_log_filepath: " << filepath << std::endl;
      // mtx.lock();
      filestream.open(AX_DEBUG_LOG, std::fstream::out AX_DEBUG_LOG_APPEND);
    }
};

#endif // AX_DEBUG_LOG 

#endif // AX_DEBUG

#endif // axDebugLog_included

#endif // 0