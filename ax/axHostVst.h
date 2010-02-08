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
  \file axHostVst.h
*/

#ifndef axHostVst_included
#define axHostVst_included
//----------------------------------------------------------------------

#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"

// vst-specific implementation of axHost

class axHostVst : public axHostBase
{
  private:
    audioMasterCallback audioMaster;
  public:
    axHostVst(void* ptr)
    : axHostBase(ptr)
      {
        audioMaster = (audioMasterCallback)ptr;
      }
    virtual ~axHostVst() {}
};

typedef axHostVst axHostImpl;

//----------------------------------------------------------------------
#endif



