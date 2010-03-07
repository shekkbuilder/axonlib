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

/**
  \file axHostVst.h
  \brief host vst class
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
        // ISO C++ forbids casting between pointer-to-function and pointer-to-object
        //audioMaster = (audioMasterCallback)ptr;
        audioMaster = (audioMasterCallback)(int)ptr;
      }
    virtual ~axHostVst() {}
    // ISO C++ forbids casting between pointer-to-function and pointer-to-object
    //virtual int getMaster(void) { return (int)audioMaster; }
    virtual void* getPtr(void) { return (void*)(int)audioMaster; }


};

typedef axHostVst axHostImpl;

//----------------------------------------------------------------------
#endif



