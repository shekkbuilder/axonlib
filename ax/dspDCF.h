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
 * \file dspDCF.h 
 * \brief DC offset filter.
 */


#ifndef dspDCF_included
#define dspDCF_included

/**
 * \brief DC offset filter.
 *   
 * one pole, one zero DC offset filter from julius orion smith (jos). <br>
 * http://www-ccrma.stanford.edu/~jos/filters/ <br>
 * usage: 
 * \code
 * // include "dspDCF.h"   
 * // create instance
 * dspDCF dcfilter;
 * // setup 
 * float dcout = dcfilter.process(input); // process input sample
 * \endcode 
 *  
 */
class dspDCF
{
  private:
    /*@{*/
    float x1, y1; /**< temp variables */ 
    float R;      /**< filter coefficient */
    /*@}*/

  public:
    /**
     * constructor          
     */
    dspDCF()
    {
      x1 = y1 = 0;
      setup(0.999f);
    }
    /**
     * sets filter coeff. suggested range [0.99 - 1]
     * @param[in] inR float  
     */
    void setup(const float inR)
    {
      R = inR;
    }
    /**
     * process input sample with filter
     * @param[in] in float
     * @return result float
     */
    virtual float process(float in)    
    {
      float x = in;
      float y;
      y = x - x1 + R*y1 + DENORM;
      x1 = x;
      y1 = y;
      return y - DENORM;
    }
};

//----------------------------------------------------------------------
#endif
