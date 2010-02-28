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
  \file dspSync.h
  \brief sync
*/

#ifndef dspSync_included
#define dspSync_included
//----------------------------------------------------------------------

#include "dspModule.h"

/**
 * \brief desc
 *
 * long desc
 *
 */
class dspSync : public dspModule
{
  public:
    void setBeats(float aBeats) {}      // setup, number of beats
    bool update(void) { return false; } // sync now?
    int  next(void) { return 0; }       // number of samples till next sync point
};

//----------------------------------------------------------------------
#endif
