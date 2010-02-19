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
 * \file axProgram.h
 * \brief program class 
*/ 

#ifndef axProgram_included
#define axProgram_included
//----------------------------------------------------------------------

class axProgram
{
  public:
    //axArray<axParameter*> mParameters;
    axParameters mParameters;
};

//----------

class axBank
{
  public:
    axArray<axProgram*> mPrograms;
};

//----------------------------------------------------------------------
#endif
