#ifndef axVoice_included
#define axVoice_included
//----------------------------------------------------------------------

#include "axArray.h"

#include "axDefines.h"
#include "axMath.h"

/*

- if there is a note on and off for the same note within a block,
  the note will be ignored (cancelled ny note off
  before it has had a chance to be seen)

*/

class axVoiceManager;

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

#define env_offset    0
#define env_attack    1
#define env_decay     2
#define env_sustain   3
#define env_release   4
#define env_finished  5

//----------

class axVoice
{
  public:
    //axVoiceManager* mOwner;
    bool  mActive;
    int   mNote;
    float mVelocity;
    int   mOffset;
    int   mEnvStage;
    float mFreq;
    float iSampleRate;
    float mPhase;
    float mPhaseAdd;
  public:
    axVoice(/*axVoiceManager* aOwner*/)
      {
        //mOwner      = aOwner;
        mActive     = false;
        mNote       = 0;
        mVelocity   = 0;
        mOffset     = 0;
        mEnvStage   = env_offset;
        mFreq       = 440;
        iSampleRate = 1/44100;
        mPhase      = 0;
        mPhaseAdd   = mFreq * iSampleRate;
      }

    //----------

    virtual ~axVoice()
      {
      }

    //----------

    inline void setSampleRate(float aSampleRate) { iSampleRate = 1/aSampleRate; }

    //----------

    virtual void noteOn(int aOffset,int aNote,float aVelocity)
      {
        mActive = true;
        mNote = aNote;
        mVelocity = aVelocity;
        mOffset = aOffset;
        mEnvStage = env_offset;
        mFreq = 440 * pow(2.0,(aNote-69.0) / 12);
        mPhase = 0;
        mPhaseAdd = mFreq * iSampleRate;
        //TRACE("phadd = %f\n",phadd);
      }

    //----------

    virtual void noteOff(int aOffset,int aNote,float aVelocity)
      {
        if (mEnvStage==env_offset) mOffset = 0; // not yet started
        else mOffset = aOffset;
        mEnvStage = env_release;
      }

    //----------

    virtual float process(void)
      {
//        switch(mEnvStage)
//        {
//          case env_offset: mEnvStage = env_attack; /*TRACE("attack\n");*/ break;
//          case env_release: mEnvStage = env_finished; /*TRACE("release\n");*/ break;
//        }
        if (mEnvStage==env_offset)  mEnvStage = env_attack;
        if (mEnvStage==env_release) mEnvStage = env_finished;
        return 0;
      }

    //----------

};

//----------

typedef axArray<axVoice*> axVoices;

//----------------------------------------------------------------------
//
// voice manager
//
//----------------------------------------------------------------------

//#define MAX_VOICES 16

//----------

class axVoiceManager
{
  private:
    //int       mMaxVoices;
    //axVoice mVoices[MAX_VOICES];
    axVoices  mVoices;
    int       mNoteMap[128];
    //float   mSampleRate;

  public:

    axVoiceManager()
      {
        for (int i=0; i<128; i++) mNoteMap[i] = -1;
      }

    //----------

    virtual ~axVoiceManager()
      {
        for (int i=0; i<mVoices.size(); i++) delete mVoices[i];
      }

    //----------

    inline void   setSampleRate(float aSampleRate) { for (int i=0; i<mVoices.size(); i++) mVoices[i]->setSampleRate(aSampleRate); }

    //----------

//    virtual void setupVoices(int aMaxVoices)
//      {
//        for (int i=0; i<aMaxVoices; i++) mVoices[i] = new axVoice(/*this*/);
//        mMaxVoices = aMaxVoices;
//      }
    void appendVoice(axVoice* aVoice)
      {
        mVoices.append(aVoice);
      }

    //----------

    void cleanup(void)
      {
        for (int i=0; i<mVoices.size(); i++)
        {
          if (mVoices[i]->mEnvStage==env_finished)
          {
            //TODO: notemap here or in noteOff?
            mVoices[i]->mActive = false;
          }
        }
      }

    //----------

    int findVoice(void)
      {
        for (int i=0; i<mVoices.size(); i++)
        {
          if (!mVoices[i]->mActive) return i;
        }
        return -1;
      }

    //----------

    void  noteOn(int aOffset, int aNote, float aVelocity)
      {
        //TRACE("axVoiceManager.noteOn( %i,%f)\n",aNote,aVelocity);
        if (mNoteMap[aNote]>=0) return; // already playing
        int voice = findVoice();
        //TRACE("  voice:%i\n",voice);
        if (voice>=0)
        {
          mNoteMap[aNote] = voice;
          mVoices[voice]->noteOn(aOffset,aNote,aVelocity);
        }
      }

    //----------

    void  noteOff(int aOffset, int aNote, float aVelocity)
      {
        //TRACE("axVoiceManager.noteOff(%i,%f)\n",aNote,aVelocity);
        int voice = mNoteMap[aNote];
        if (voice<0) return; // not playing
        //TRACE("  voice:%i\n",voice);
        mNoteMap[aNote] = -1;
        mVoices[voice]->noteOff(aOffset,aNote,aVelocity);
      }

    //----------

    float process(int aChannel=0)
      {
        float out = 0;
        for (int i=0; i<mVoices.size(); i++)
        {
          if (mVoices[i]->mActive)
          {
            if (mVoices[i]->mOffset>0) mVoices[i]->mOffset-=1; // wait
            else out += mVoices[i]->process();
          }
        }
        return out;
      }

    //----------

};

//----------------------------------------------------------------------
#endif
