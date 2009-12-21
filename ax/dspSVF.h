#ifndef dspSVF_included
#define dspSVF_included
//----------------------------------------------------------------------

#define svf_mode_none 0
#define svf_mode_lp   1
#define svf_mode_hp   2
#define svf_mode_bp   3
#define svf_mode_n    4

class dspSVF
{
  private:
    float z1,z2;
  public:
    int   mMode;
    float mFreq;
    float mBW;

  public:

    dspSVF()
      {
        z1 =z2 = 0;
        setup(0,1,1);
      }

    void setup(int m, float f, float b)
      {
        mMode  = m;
        mFreq  = f;
        mBW    = b;
      }

    void resume(void)
      {
        z1 = z2 = 0;
      }

    float process(float in)
      {
        float out = 0;
        if( mMode==0 ) out=in;
        else
        //if( mMode > 0 )
        {
          float L = z2 + mFreq * z1;
          float H = in - L - mBW   * z1;
          float B = mFreq * H + z1;
          float N = H + L;
          z1 = B;
          z2 = L;
          switch(mMode)
          {
            case 1: out = L; break;
            case 2: out = H; break;
            case 3: out = B; break;
            case 4: out = N; break;
          } //switch
        } //mMode
        return out;
      } //process

};

//----------------------------------------------------------------------
#endif
