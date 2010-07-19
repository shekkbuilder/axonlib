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
BOOL axDetectWine(void)
{
  BOOL ret = 0;
  HINSTANCE hLib = LoadLibrary(TEXT("ntdll.dll"));
  if (hLib)
  {
    ret = GetProcAddress(hLib, "wine_get_version") != NULL ||
				  GetProcAddress(hLib, "wine_nt_to_unix_file_name") != NULL;
    BOOL unload = FreeLibrary(hLib);
    if (!unload)
      printf("wine detect: # cannot release ntdll.dll");
  }
  else
    printf("wine detect: # cannot load ntdll.dll"); 
  // give it a try now. here is a refference:
  // http://wave-notify.googlecode.com/svn-history/r270/wave-notify/branches/stable/Wine.cpp
  printf("wine detect: %i\n",ret); // returns 0 in wine/ubuntu !!  
  return ret;
}

#endif // axDetectWine_included
