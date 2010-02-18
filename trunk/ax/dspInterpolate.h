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

#include "dspRBJ.h"

// http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/
// TODO: test

/**
 * \brief interpolation methods    
 */
class dspInterpolate
{
  private:
    float factor;
    bool filter_enabled;
    dspRBJ f0;

  public:
    /**
     * constructor          
     */
    dspInterpolate()
    {
      setup();
    }
    /**
     * set oversampling factor
     * @param[in] _factor unsigned int factor (1x....Nx)
     * @param[in] _srate unsigned int sample rate (hz)
     * @param[in] _filter_enalbed bool enable filter (true / false)     
     * @return void
     */
    
    void setup(const unsigned int _factor=4, const float _srate=44100,
    const bool _filter_enabled=true)
    {
      factor = _factor;
      if (factor < 1) factor = 1;
      filter_enabled = _filter_enabled;
      if (_filter_enabled)
      {
        // filter.setup ( lp, srate, freq, q, gain, param interpolation )
        f0.setup(1, _factor*_srate,_factor*_srate*0.499f, 0.5f, 1.f, false);
      }
    }
    /**
     * process input
     * @param[in] *cb_function(float) float - pointer to function (callback)        
     * @param[in] in float - input value       
     * @return float - output value
     */
    virtual float process(float (*cb_function)(float), const float in)
    {
      float y, t_y, x = 0;
      float t_in = in, out;
      float invf = 1/factor;
      // --
      for (unsigned int i=0; i<factor; i++)
      {
        // linear interp
        y = t_in + x*(in - t_in);
        
        // process (call back to function)
        y = (*cb_function)(y);
        
        // filter
        t_y = y;
        if (filter_enabled) t_y = f0.process(y);
        
        // discard samples
        if (x == 0) out = t_y;
        x += invf;
      }      
      return out;
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
