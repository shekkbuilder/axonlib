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
 * @file dspInterpolate.h
 * \brief interpolation methods
 */

#ifndef dspInterpolate_included
#define dspInterpolate_included

/**
 * \brief interpolation methods
 *   
 */
 
// TODO:   http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/ 

 
class dspInterpolate
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
    dspInterpolate()
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
     * @param[out] result float
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
