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

#ifndef axConfig_included
#define axConfig_included
//----------------------------------------------------------------------

/**
 * \file axConfig.h
 * \brief configuration 
*/ 

#ifdef WIN32
  #define AX_WIN32
#endif

#ifdef linux
  #define AX_LINUX
#endif

// plugin settings

#define AX_PLUGIN_VST
//#define AX_PLUGIN_MULTI

// gui settings

//#define AX_PAINTERS
//#define AX_NODIRTYWIDGETS

//----------------------------------------------------------------------

#endif
