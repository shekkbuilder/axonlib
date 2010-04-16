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

// NOTE: requires some debugging

/**
 * @file dspPRNG0.h 
 * \brief pseudo-random number generator
 */

#ifndef dspPRNG_included
#define dspPRNG_included

/**
 * \brief pseudo-random number generator
 * 
 * prng with non-uniform distribution <br>
 * has a negative probability-density graph (less values near zero) <br> 
 * usage example:
 * \code
 * // include "dspPRNG.h"
 * // ..   
 * // create instance
 * dspPRNG prng;
 * // process 
 * float value = prng.process(); // value = random[-1, +1]
 * \endcode    
 */
class dspPRNG
{
  private:
  /** variables */
    float y, x;

  public:
    /** constructor */
    dspPRNG()
    {
      reset();
    }
    
    /**
     * reset the prng
     */
    virtual void reset(void)
    {
      axRandomize();              // randomize seed for rand()
      x = axRandom(999.f);        // return random seed for PRNG0 (0..999.f)
      y = 1.f;                    // set initial y to 1
    }
    
    /**
     * return a random float number [-1, +1]
     * @return float
     */
    inline float process(void)
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
