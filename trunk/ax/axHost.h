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
 * \file axHost.h
 * \brief host class
*/ 

#ifndef axHost_included
#define axHost_included
//----------------------------------------------------------------------

/*
  this class should define what a plugin can expect from a host,
  to abstract away the vst host specifics.
  - host detection
  - ? system info? (msb/lsb, cputype, display-format..
      use this for a standalone plugin wrapper too?
  - host extensions (especially reaper)
  - -> host: resize window
*/

class axHostBase
{
  public:
    axHostBase(void* ptr) {}
    virtual ~axHostBase() {}
    virtual void* getPtr(void) { return NULL; }
};

#ifdef AX_PLUGIN_VST
  #include "axHostVst.h"
#endif


class axHost : public axHostImpl
{
  public:
    axHost(void* ptr) : axHostImpl(ptr) {}
    virtual ~axHost() {}
};

//----------------------------------------------------------------------
#endif
