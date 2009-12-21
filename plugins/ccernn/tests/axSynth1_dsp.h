#ifndef axSynth1_dsp_included
#define axSynth1_dsp_included
//----------------------------------------------------------------------

class CWgtAvg
{
  protected:
    float mValue;
    float mTarget;
    float mWeight;
  public:
    CWgtAvg(float aValue=0, float aTarget=0, float aWeight=1)
      {
        mValue = aValue;
        mTarget = aTarget;
        mWeight = aWeight;
      }
    float process(void)
      { // envelope
        mValue += ( mTarget-mValue) * mWeight;
        return mValue;
      }
    float process(float aTarget)
      { // filter
        mValue += ( aTarget-mValue) * mWeight;
        return mValue;
      }
    float output(void) { return mValue; }
};

//----------------------------------------------------------------------

class COsc
{
  private:
    int   mType; // off,const,noise,saw,isaw,squ,sin
    float mPhase;
    float mPhaseAdd;
    float mValue;
  public:
    COsc(int aType)
      {
        mType = aType;
        mPhase = 0;
        mPhaseAdd = 0;
        mValue = 0;
      }
    float process(void)
      {
        while( mPhase>=1 ) mPhase -= 1;
        while( mPhase<0 ) mPhase += 1;
        switch( mType )
        {
          case 0: mValue=0; break;
          case 1: mValue=1; break;
          case 2: mValue=axRandomSigned(); break;
          case 3: mValue=(1-mPhase)*2-1; break;
          case 4: mValue=mPhase*2-1; break;
          case 5: mValue=(mPhase>0.5)?1:-1; break;
          case 6: mValue=sin(mPhase*PI2); break;
        }
        mPhase += mPhaseAdd;
        return mValue;
      }
    void resetPhase(void)
      {
        mPhase=0;
      }
    void setSpeed(float aAdder)
      {
        mPhaseAdd=aAdder;
      }
};

//----------------------------------------------------------------------

class CSVF
{
};

//----------------------------------------------------------------------

class CProgram
{
};

//----------------------------------------------------------------------

class CVoice
{
  public:
    int getState(void) { return 0; }
    float getAmpl(void) { return 0; }
};

typedef axArray<CVoice*> CVoices;

//----------------------------------------------------------------------

class CVoiceManager
{
  protected:
    int     mMaxVoices;
    int     mNewNoteMode;
    CVoices mFreeVoices;
    CVoices mPlayingVoices;
  public:
    void setMaxVoices(int num) {}
    void setNewNoteMode(int mode) {}
    CVoice* noteOn(float note, float vel) { return NULL; }
    void noteOff(float note, float vel) {}
    void allNotesOff(void) {}
};

//----------------------------------------------------------------------
#endif
