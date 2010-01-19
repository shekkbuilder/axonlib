#ifndef dspDCF_included
#define dspDCF_included
//----------------------------------------------------------------------

class dspDCF
{
  private:
    float x1, y1;
    float R;

  public:
    dspDCF()
      {
        x1 = y1 = 0;
        setup(0.999f); // init hp coeff
      }

    void setup(const float inR)
      {
        R = inR;
      }
    
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
