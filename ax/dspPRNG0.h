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

// NOTE: requires some debugging

/**
 * @file dspPRNG0.h 
 * \brief pseudo-random number generator
 */

#ifndef dspPRNG0_included
#define dspPRNG0_included

/**
 * \brief pseudo-random number generator
 * 
 * prng with non-uniform distribution <br>
 * has a negative probability-density graph (less values near zero) <br> 
 * usage example:
 * \code
 * // include "dspPRNG0.h"
 * // ..   
 * // create instance
 * dspPRNG0 prng;
 * // process 
 * float value = prng.process(); // value = random[-1, +1]
 * \endcode    
 */
class dspPRNG0
{
  private:
  /** variables */
    float y, x;

  public:
    /** constructor */
    dspPRNG0()
    {
      reset();
    }
    
    /**
     * reset the prng
     * @param[in] void
     * @return void
     */
    virtual void reset(void)
    {
      axRandomize();              // randomize seed for rand()
      x = axRandom(999.f);        // return random seed for PRNG0 (0..999.f)
      y = 1.f;                    // set initial y to 1
    }
    
    /**
     * return a random float number [-1, +1]
     * @param[in] void
     * @return float
     */
    virtual float process(void)
    {
      __asm__ __volatile__("":::);
      __asm__
      (
        // y = sinf((x += 1)*y);
        "fld1;"
        "faddp;"
        "fxch;"
        "fmul %%st(1);"
        "fsin;"
        : "=t" (y), "=u" (x) : "0" (x), "1" (y)
      );
      return y;
    }    
};

//----------------------------------------------------------------------
#endif
