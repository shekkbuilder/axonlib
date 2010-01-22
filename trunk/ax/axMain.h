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
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */
 

#ifndef axMain_included
#define axMain_included
//----------------------------------------------------------------------

// TODO:
// - win32: window class?

#ifndef AX_NUMPROGS
  #define AX_NUMPROGS 0
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
    app->main();
    return 0;
  }

#endif // AX_APPLICATION

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

#ifdef AX_PLUGIN

//#include "axHostVst.h"

  #ifdef WIN32

    BOOL APIENTRY DllMain(HINSTANCE hModule,DWORD reason,LPVOID lpReserved)
    {
      gInstance = hModule;
      return TRUE;
    }
    //----------
    int main(audioMasterCallback audioMaster)
    {
      AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      //axHostVst* host = new axHostVst(audioMaster);
      //AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return (int)plugin->getAeffect();
    }

  #endif //Win32

  //----------

  #ifdef linux

    AEffect* main_plugin(audioMasterCallback audioMaster) asm ("main");
    #define main main_plugin
    //----------
    AEffect* main(audioMasterCallback audioMaster)
    {
      AX_PLUGIN* plugin = new AX_PLUGIN(audioMaster,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      //axHostVst* host = new axHostVst(audioMaster);
      //AX_PLUGIN* plugin = new AX_PLUGIN(host,AX_NUMPROGS,AX_NUMPARAMS,AX_FLAGS);
      if (!plugin) return 0;
      return plugin->getAeffect();
    }

  #endif //linux

#endif //AX_PLUGIN

//----------------------------------------------------------------------
#endif
