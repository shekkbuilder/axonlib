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

#ifndef axSurface_included
#define axSurface_included
//----------------------------------------------------------------------

#include "axDefines.h"
#include "platform/axContext.h"
#include "base/axSurfaceBase.h"

//----------------------------------------------------------------------

#ifdef AX_LINUX
  #include "platform/linux/axSurfaceLinux.h"
#endif

#ifdef AX_WIN32
  #include "platform/win32/axSurfaceWin32.h"
#endif

//----------------------------------------------------------------------

class axSurface : public axSurfaceImpl
{
  public:
    axSurface(axContext* aContext, int aWidth, int aHeight, int aDepth)
    : axSurfaceImpl(aContext,aWidth,aHeight,aDepth)
      {
      }
    //virtual ~axSurface() {}
};

//----------------------------------------------------------------------
#endif
