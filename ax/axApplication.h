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
  \file axApplication.h
  \brief standalone application

  [can we have more detailed file descriptions here? and where will it be found in the generated docs?]

*/

#ifndef axApplication_included
#define axApplication_included
//----------------------------------------------------------------------
#include "axGlobals.h"

/// standalone application base
/**
  inherit from this one, and override the main() method if you ant to create a standalone application.
  \todo command line argument parsing
*/

class axApplication
{
  public:
    axApplication() {}
    virtual ~axApplication() {}
    virtual void main(void) {}
};


//----------------------------------------------------------------------
#endif
