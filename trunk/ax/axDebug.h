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
 * @file axDebug.h
 * \brief debugger methods
 *
 * if AX_DEBUG is defined, 'assert.h' and 'iostream' are included for debugging purposes. <br><br>
 * assert() examples:
 * \code
 * assert(var);   // if 'var' is false the compiler will trow an error and abort
 * assert(num>5); // same as above but if 'num' is not greater than 5
 * \endcode
 * warn() examples:
 * \code
 * warn("message"); // show a warning but does not break
 * \endcode
 * trace() examples:
 * \code
 * trace(var);              // trace a variable
 * trace("message");        // trace a message
 * trace(var << "message"); // combined
 * \endcode
 */

#ifndef axDebug_included
#define axDebug_included
//----------------------------------------------------------------------

// assert() gives an error (not declared) if AX_DEBUG is not defined
#include <assert.h>

#ifdef AX_DEBUG
	//#include <assert.h>
	#include <iostream>
	using namespace std;
	#define warn(x) { printf("WARN | LINE: %i | %s\n", __LINE__, x); }
	#define trace(x) { cout << "TRC | LINE: " << __LINE__ << " | " << x << endl; cout.flush(); }
#else
  #define NDEBUG
  #define trace(x) ((void)0) // asserts.h uses this method to return 'void'
  #define warn(x) ((void)0)
#endif

//----------------------------------------------------------------------
#endif
