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

#ifndef axDebugLog_included
#define axDebugLog_included

///////////////////////////////////////////////////////////////////////////////
// axDebugLog.h
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include "axDefines.h"
#include "axStdlib.h"
#include "axMutex.h" // linux: undefined reference to `pthread_mutex_trylock'


//////////////////////////////////////////  temp stuff

// the below is disturbing !!!

#define gWinInstance NULL

//axGetBasePath() is needed by class axDebugLog
#ifdef AX_WIN32
#ifndef axPlatformWin32_included

  const char* axGetBasePath(char* path)
  {
    #if defined AX_FORMAT_LIB || defined AX_FORMAT_EXE
      char filepath[AX_MAX_PATH] = "";
      GetModuleFileName(gWinInstance, filepath, MAX_PATH);
      const char* slash = axStrrchr(filepath, '\\');
      if (slash)
        axStrncpy(path, filepath, (slash + 1) - (char*)filepath);
      else
        axStrcat(path, (char*)".\\");
    #else
      axStrcat(path, (char*)".\\");
    #endif
    return path;
  }

#endif
#endif

//////////////////////////////////////////

#ifdef AX_LINUX
#ifndef axPlatformLinux_included

const char* axGetBasePath(char* path)
{
  #ifdef AX_FORMAT_LIB

    char filepath[AX_MAX_PATH] = "";
    Dl_info dli;
    dladdr(__func__, &dli);
    const char* slash = axStrrchr(dli.dli_fname, '/');
    if (slash)
	    axStrncpy(path, dli.dli_fname, (slash + 1) - (char*)dli.dli_fname);
    else
      axStrcat(path, (char*)"./");

  #elif defined AX_FORMAT_EXE

    char filepath[AX_MAX_PATH] = "";
    if (readlink("/proc/self/exe", filepath, AX_MAX_PATH))
    {
      const char* slash = axStrrchr(filepath, '/');
      if (slash)
		    axStrncpy(path, filepath, (slash + 1) - (char*)filepath);
      else
        axStrcat(path, (char*)"./");
    }

  #else
    axStrcat(path, (char*)"./");
  #endif
  return path;
}

#endif
#endif


////////////////////////////////////////// end of temp stuff

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

// axDebugLog is an ostream object
class axDebugLog : public std::ostream
{
  private:

    std::fstream    axfstream;
    axTeebuf        tbuf;
    // axMutex         mtx;

  public:

    axDebugLog() :
      std::ostream(&tbuf), tbuf(std::cout.rdbuf(), axfstream.rdbuf())
    {
      char filepath[AX_MAX_PATH] = "";
      axGetBasePath(filepath);
      axStrcat(filepath, AX_DEBUG_LOG);
      std::cout << "dbg_log_filepath = " << filepath << std::endl;
      // mtx.lock();
      axfstream.open(AX_DEBUG_LOG, std::fstream::out AX_DEBUG_LOG_APPEND);
    }

    ~axDebugLog()
    {
      // mtx.unlock();
      axfstream.close();
    }
};

#endif // axDebugLog_included
