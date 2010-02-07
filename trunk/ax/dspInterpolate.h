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


/*
desc:n times oversampler
slider1:1<1,64,1>oversample (x)
slider2:1<0,1,1{OFF,ON}>Antialiasing filter
@slider
factor = slider1;
filter_coef = exp(-2*$pi*(0.5/factor) );
filter = slider2;
@sample
//spl0_ = spl0_tmp; //store last input sample
spl0_tmp = spl0;
x = 0;

loop(factor,
  //interp
  y = spl0_tmp + x*(spl0-spl0_tmp);
  
  //proces
  //y = y*0.8;
  
  // filter here
  //filter ? tmp = y + filter_coef*(tmp-y) : tmp = y;
  m1=m0;
  filter ? tmp=0.5*(m1+m0=y) : tmp=y;
  
  //decimator
  x==0 ? spl0 = tmp; 
  x += 1/factor;
);

spl1 = spl0;

*/