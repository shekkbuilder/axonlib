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

#ifndef axDetectWine_included
#define axDetectWine_included

#include "axDefines.h" // sets winver etc..
#include "windows.h"

// detect wine
int axDetectWine(void)
{
  unsigned int ret = 0;
  HINSTANCE hLib = LoadLibrary(TEXT("ntdll.dll"));
  if (hLib)
  {
    FARPROC pAddr = GetProcAddress(hLib, "get_wine_version");          
    if (pAddr)
      ret = 1;
    if (!FreeLibrary(hLib))
      ret = -2;
  }
  else
    ret = -1;
  return ret;
}

#endif // axDetectWine_included
