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
 * @file dspBiquad.h
 * \brief biquad filters based on rbj's filter cookbook
 */

#ifndef dspRBJ_included
#define dspRBJ_included

/**
 * \brief biquad filters based on rbj's filter cookbook
 * 
 * quick variation of 'biquid.c' by: <br>
 * Tom St Denis, http://tomstdenis.home.dhs.org
 * <br><br>
 * original filters by: <br>
 * Robert Bristow-Johnson, rbj@audioimagination.com <br>
 *
 */
class dspRBJ
{
  private:
  /** coeficients and temp variables */
    float a0, a1, a2, a3, a4, b0, b1, b2, x1, x2, y1, y2;
    
  /** filter types */
  enum
  {
    NOF,    /** no filter */
    LPF,    /** low pass filter */
    HPF,    /** gigh pass filter */
    BPF,    /** band pass filter */
    NOTCH,  /** notch filter */
    PEQ,    /** peaking band */
    LSH,    /** low shelf filter */
    HSH     /** high shelf filter */
  };

  public:
    /**
     * constructor
     */
    dspDCF()
    {
      setup();
    }
    
    /**
     * setup filter
     * @param[in] type unsigned int - filter type. see enum (default=0)
     * @param[in] srate float - sample rate
     * @param[in] freq float - frequency [0.f - 1.f]             
     * @param[in] q float - q factor (default = 0.5f)
     * @param[in] gain float - gain in decibels
     * @return void           
     */
    void setup(const unsigned int type=0, const float srate=44100, const float freq=0.5f,
    const float q=0.5f, const float gain=1.f)
    {
      // tmp calculations (uses axMath.h)
      const float A, omega, sn, cs, alpha, beta;
      // ---           
      A = axPowf(10.f, gain*0.025f);  // 10^(g/40)
      omega = PI2 * freq / srate;
      sn = axSinf(omega);
      cs = axCosf(omega);
      alpha = sn * axSinhf(LN2 * 0.5f * bandwidth * omega / sn);
      beta = axSqrtf(A + A);
      
      // calculate coeff
      switch (type)
      {
        case LPF:
          b0 = (1 - cs) * 0.5f;
          b1 = 1 - cs;
          b2 = (1 - cs) * 0.5f;
          a0 = 1 + alpha;
          a1 = -2 * cs;
          a2 = 1 - alpha;
          break;
        case HPF:
          b0 = (1 + cs) * 0.5f;
          b1 = -(1 + cs);
          b2 = (1 + cs) * 0.5f;
          a0 = 1 + alpha;
          a1 = -2 * cs;
          a2 = 1 - alpha;
          break;
        case BPF:
          b0 = alpha;
          b1 = 0;
          b2 = -alpha;
          a0 = 1 + alpha;
          a1 = -2 * cs;
          a2 = 1 - alpha;
          break;
        case NOTCH:
          b0 = 1;
          b1 = -2 * cs;
          b2 = 1;
          a0 = 1 + alpha;
          a1 = -2 * cs;
          a2 = 1 - alpha;
          break;
        case PEQ:
          b0 = 1 + (alpha * A);
          b1 = -2 * cs;
          b2 = 1 - (alpha * A);
          a0 = 1 + (alpha / A);
          a1 = -2 * cs;
          a2 = 1 - (alpha / A);
          break;
        case LSH:
          b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
          b1 = 2 * A * ((A - 1) - (A + 1) * cs);
          b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
          a0 = (A + 1) + (A - 1) * cs + beta * sn;
          a1 = -2 * ((A - 1) + (A + 1) * cs);
          a2 = (A + 1) + (A - 1) * cs - beta * sn;
          break;
        case HSH:
          b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
          b1 = -2 * A * ((A - 1) + (A + 1) * cs);
          b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
          a0 = (A + 1) - (A - 1) * cs + beta * sn;
          a1 = 2 * ((A - 1) - (A + 1) * cs);
          a2 = (A + 1) - (A - 1) * cs - beta * sn;
          break;
        case NOF:
          b0 = b1 = b2 = a0 = a1 = a2 = 1.f;
          break;
      }
  
      // precompute
      const float inv_a0 = 1/a0;
      a0 = b0 * inv_a0;
      a1 = b1 * inv_a0;
      a2 = b2 * inv_a0;
      a3 = a1 * inv_a0;
      a4 = a2 * inv_a0;
  
      // zero initial samples
      x1 = x2 = 0.f;
      y1 = y2 = 0.f;    
    }
    
    /**
     * process input sample with filter
     * @param[in] in float
     * @param[out] result float
     */
    virtual float process(const float in)
    {
      // direct form i
      float result;
      result = a0*in + a1*x1 + a2*x2 - a3*y1 - a4*y2 + DENORM;
      x2 = x1;
      x1 = in;
      y2 = y1;
      y1 = result;
      return result - DENORM;
    }
};

//----------------------------------------------------------------------
#endif
