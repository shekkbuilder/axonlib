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
  \file axMain.h
  \brief main application/plugin entry point.

  tries to take care of how he different platforms work with loadable libraries (dll/so),
  and hide the differences between a plugin and a standalone binary
*/

//----------------------------------------------------------------------

#ifndef axMain_included
#define axMain_included
//----------------------------------------------------------------------

// TODO:
// - win32: window class?

#ifndef AX_NUMPROGS
  #define AX_NUMPROGS 1
#endif

#ifndef AX_NUMPARAMS
  #define AX_NUMPARAMS 0
#endif

#ifndef AX_FLAGS
  #define AX_FLAGS 0
#endif

//----------------------------------------------------------------------
//
// application
//
//----------------------------------------------------------------------

#ifdef AX_APPLICATION

  int main(void)
  {
    AX_APPLICATION* app = new AX_APPLICATION();
    //TODO: command line arguments
    app->main();
    return 0;
  }

#endif // AX_APPLICATION

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

#include "axDebug.h"

#ifdef AX_PLUGIN

#include "axHost.h"

  #ifdef WIN32

    /// DllMain
    /**
      called when windows load the dll
      saves the HINSTANCE variable, so we have it accessible
    */

    BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved)
    {
      gInstance = hModule;
      return TRUE;
    }

    /// main entry point
    /**
      called by the host when the dll/so is initially loaded.
      note that the dll/so will only be loaded once, and all instances of your plugin will share access to any global data
    */

    int main(audioMasterCallback audioMaster)
    {
      //AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      axHost* host = new axHost((void*)audioMaster);
      AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return (int)plugin->getAeffect();
    }

  #endif //Win32

  //----------

  #ifdef linux

    #define __cdecl

    AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
    #define main main_plugin

    /// main entry point
    /**
      called by the host when the dll/so is initially loaded.
      note that the dll/so will only be loaded once, and all instances of your plugin will share access to any global data.
      (a little #define trickery is going on in the linux version, because of the calling convention used in the vst sdk)
    */

    AEffect* main(audioMasterCallback audioMaster)
    {
      //AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      axHost* host = new axHost((void*)(int)audioMaster);
      AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return plugin->getAeffect();
    }

  #endif //linux

#endif //AX_PLUGIN

//----------------------------------------------------------------------
#endif
