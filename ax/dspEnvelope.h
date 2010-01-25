//#ifndef dspEnvelope_included
//#define dspEnvelope_included
////----------------------------------------------------------------------
//
//#define ENV_CTRL_ATTACK   0
//#define ENV_CTRL_DECAY    1
//#define ENV_CTRL_SUSTAIN  2
//#define ENV_CTRL_RELEASE  3
//
//#define ENV_STAGE_ATTACK   0
//#define ENV_STAGE_DECAY    1
//#define ENV_STAGE_SUSTAIN  2
//#define ENV_STAGE_RELEASE  3
//
////----------------------------------------------------------------------
//
//struct _envstage
//{
//  int   mLength;
//  float mRate;
//};
//
////----------
//
//class dspEnvelope : public axModule
//{
//  private:
//    int       mNumStages;
//    _envstage mStages[4];
//    int       mCurStage;
//    float     mValue;
//
//  public:
//
//    virtual void  noteOn(int aOffset, int aNote, float aVelocity)
//      {
//        mStage = ENV_STAGE_ATTACK;
//        mValue = 0;
//      }
//
//    virtual void  noteOff(int aOffset, int aNote, float aVelocity)
//      {
//        mStage = ENV_STAGE_RELEASE;
//      }
//
//    virtual void  control(int aNum, float aValue) {}
//
//    virtual float process(void)
//      {
//        float out = mValue;
//        switch(mStage)
//        {
//          case ENV_STAGE_ATTACK:
//            if (mACount>0)
//            {
//              mValue += mARate;
//              mACount--;
//            } else mStage = ENV_STAGE_DECAY;
//            break;
//          case ENV_STAGE_DECAY:
//            if (mDCount>0)
//            {
//              mValue += mDRate;
//              mDCount--;
//            } else mStage = ENV_STAGE_DECAY;
//            break;
//          case ENV_STAGE_RELEASE:
//            break;
//        }
//        return out;
//      }
//};
//
//
////----------------------------------------------------------------------
//#endif
//
