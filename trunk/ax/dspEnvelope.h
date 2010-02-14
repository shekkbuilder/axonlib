#ifndef dspEnvelope_included
#define dspEnvelope_included
//----------------------------------------------------------------------

#define env_off       0
#define env_offset    1
#define env_attack    2
#define env_decay     3
#define env_sustain   4
#define env_release   5
#define env_finished  0xffff

//----------

struct envstage
{
  int   mLength;
  float mRate;
};

//----------

class dspEnvelope //: public axModule
{
  private:
    bool mGate;
    int mStage;
  public:

    dspEnvelope() { mStage=env_off; }
    virtual ~dspEnvelope() {}

    //----------

    inline void setStage(int aStage)
      {
        mStage = aStage;
      }

    inline int getStage(void)
      {
        return mStage;
      }

    //----------

    virtual void noteOn(void)
      {
        mGate = true;
        mStage = env_attack;
        }

    virtual void noteOff(void)
      {
        mGate = false;
        mStage = env_finished;//env_release;
      }

    virtual float process(void)
      {
        if (mGate) return 1;
        else return 0;
      }
};

//----------------------------------------------------------------------
#endif

