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

#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------
//TODO
#include "axCanvas.h"
#define DEF_PENWIDTH 0

class axPenBase
{
  public:
    axPenBase(axColor aColor, int aSize, int aStyle) {}
    virtual ~axPenBase() {}
};

// in canvas:
// pen = canvas.createPen(AX_WHITE,1,0);
// canvas.usePen(pen);
// canvas.deletePen(pen);
//
//

//----------------------------------------------------------------------

#ifdef linux

class axPen : public axPenBase
{
};

#endif

//----------------------------------------------------------------------
#endif


