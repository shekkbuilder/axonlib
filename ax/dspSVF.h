#ifndef dspSVF_included
#define dspSVF_included
//----------------------------------------------------------------------

//#include "dspModule.h"

#define svf_mode_none 0
#define svf_mode_lp   1
#define svf_mode_hp   2
#define svf_mode_bp   3
#define svf_mode_n    4

class dspSVF
{
  private:
    float z1_L,z2_L;
  public:
    int   mMode;
    float mFreq;
    float mBW;

  public:

    dspSVF()
      {
        z1_L =z2_L = 0;
        setup(0,1,1);
      }

    void setup(int m, float f, float b)
      {
        mMode  = m;
        mFreq  = f;
        mBW    = b;
      }

    //void resume(void)
    //  {
    //    z1 = z2 = 0;
    //  }

    virtual float process(float** ins, float** outs)
    //float process(float in)
      {
        float in_L = *ins[0];
        float out_L = 0;
        if( mMode==0 ) out_L = in_L;
        else
        {
          float L = z2_L + mFreq * z1_L;
          float H = in_L - L - mBW   * z1_L;
          float B = mFreq * H + z1_L;
          float N = H + L;
          z1_L = B;
          z2_L = L;
          switch(mMode)
          {
            case 1: out_L = L; break;
            case 2: out_L = H; break;
            case 3: out_L = B; break;
            case 4: out_L = N; break;
          } //switch
        } //mMode
        return out_L;
      } //process

};

//----------------------------------------------------------------------
#endif
