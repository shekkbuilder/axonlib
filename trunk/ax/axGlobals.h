#ifndef axGlobals_included
#define axGlobals_included
//----------------------------------------------------------------------

/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * \brief global things
 */

//----------------------------------------------------------------------

#define AX_EMBEDDED   1 ///< use embedded/reparented windows (plugins)
#define AX_WINDOWED   2 ///< use normal window (standalone
#define AX_BUFFERED   4 ///< double buffering (background surface)

static char base_path[1024];

//----------------------------------------------------------------------

#ifdef linux

  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  //#include <X11/cursorfont.h>
  #include <pthread.h>

  Display* gDP = NULL; ///< global Display* (x11)

    /**
     * returns platform as text string. ("linux" or "win32")
     *
     * @param[out] result char*
     */

  char* axPlatform(void)
    {
      return (char*)"linux";
    }

    /**
     * returns basepath. for plugins, this is the directory of the .dll file,
     * for standalone programs, it's the executable directory
     * [not implemented for linux]
     *
     * @param[out] directory char* string
     */

  char* axBasePath(void)
    {
      return base_path; // !!!
    }

    /**
     * initialize the library
     * this is important
     * do this as one of the first things at startup
     *
     * @param[in] aFlag int a combination of AX_EMBEDDED, AX_WINDOWED, AX_BUFFERED
     */

  void  axInitialize(int aFlags)
    {
      XInitThreads();
      if (aFlags&AX_WINDOWED) if(!gDP) gDP=XOpenDisplay(NULL);
    }

    /**
     * clean up after the library (freeing memory, etc)
     *
     * @param[out] aFlags int same flags as the ones used in AxInitialize
     */

  void  axCleanup(int aFlags)
    {
      if (aFlags&AX_WINDOWED) if (gDP) XCloseDisplay(gDP);
    }

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifdef WIN32

  #include <windows.h>

  HINSTANCE gInstance; ///< global HINSTANCE (win32)

    /**
     * returns platform as text string. ("linux" or "win32")
     *
     * @param[out] result char*
     */

  char* axPlatform(void)
    {
      return (char*)"win32";
    }

    /**
     * returns basepath. for plugins, this is the directory of the .dll file,
     * for standalone programs, it's the executable directory
     * [not implemented for linux]
     *
     * @param[out] directory char* string
     */

  char* axBasePath(void)
    {
      GetModuleFileName(gInstance,base_path,1024);
      return base_path;
    }

    /**
     * initialize the library
     * this is important
     * do this as one of the first things at startup
     *
     * @param[in] aFlag int a combination of AX_EMBEDDED, AX_WINDOWED, AX_BUFFERED
     */

  void  axInitialize(int aFlags)
    {
      gInstance = (HINSTANCE)GetModuleHandle(NULL);
    }

    /**
     * clean up after the library (freeing memory, etc)
     *
     * @param[out] aFlags int same flags as the ones used in AxInitialize
     */

  void  axCleanup(int aFlags)
    {
    }

#endif

//----------------------------------------------------------------------
#endif
