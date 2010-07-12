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

#ifndef axBasePath_included
#define axBasePath_included

#include "axDefines.h"
#include "axStdlib.h"

//axGetFileName() is needed by trace()
inline const char* axGetFileName(const char* path)
{
  if (!path)
    return "NULL";
  const char *slash, *backslash;
  slash = axStrrchr(path, '/');
  backslash = axStrrchr(path, '\\');
  if (slash)
    return slash + 1;
  else if (backslash)
    return backslash + 1;
  return path;
}

// axGetBasePath
#ifdef AX_WIN32

  const char* axGetBasePath(char* path)
  {
    #if defined AX_FORMAT_LIB || defined AX_FORMAT_EXE

      char filepath[AX_MAX_PATH] = "";
      GetModuleFileName((HINSTANCE__*)gWinInstance, filepath, MAX_PATH);
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

#endif // AX_WIN32

#ifdef AX_LINUX

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

#endif // AX_LINUX

#endif // axBasePath_included