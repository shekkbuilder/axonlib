#ifndef dspBuffer_included
#define dspBuffer_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------

class dspBuffer
{
  private:

    float*  mBuffer;
    int     mIndex;
    int     mLength;

  public:

    dspBuffer()
      {
        mBuffer = NULL;
        mIndex = 0;
        mLength = 0;
      }

    virtual ~dspBuffer()
      {
        destroy();
      }

    //----------

    inline float* getBuffer(void) { return mBuffer; }
    inline int getLength(void) { return mLength; }

    inline void setIndex(int aIndex) { mIndex = aIndex; } // toto: error-check, range-check, ...
    inline int getIndex(void) { return mIndex; }

    void setup(float* aBuffer, int aLength)
      {
        mBuffer = aBuffer;
        mLength = aLength;
      }

    //----------

    void create(int aSize)
      {
        mBuffer = new float[aSize*2];
        mLength = aSize;
        mIndex = 0;
      }

    void destroy(void)
      {
        if (mBuffer) delete[] mBuffer;
        mBuffer = NULL;
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        mBuffer[mIndex*2]   = spl0;
        mBuffer[mIndex*2+1] = spl1;
        mIndex += 1;
        if (mIndex>=mLength) mIndex=0;
        //*outs[0] = spl0;
        //*outs[1] = spl1;
      }
};

//----------------------------------------------------------------------

/*
class dspDoubleBuffer
{
  private:

    float*  mRecBuffer;
    float*  mPlayBuffer;
    int     mIndex;
    int     mLength;
    bool    mFilled;

  public:

    dspDoubleBuffer()
      {
        mRecBuffer = NULL;
        mPlayBuffer = NULL;
        mIndex = 0;
        mLength = 0;
        mFilled = false;
      }

    virtual ~dspDoubleBuffer()
      {
        destroy();
      }

    void create(int aSize)
      {
        mRecBuffer = new float[aSize*2];
        mPlayBuffer = new float[aSize*2];
      }

    void destroy(void)
      {
        if (mRecBuffer) delete[] mRecBuffer;
        if (mPlayBuffer) delete[] mPlayBuffer;
        mRecBuffer = NULL;
        mPlayBuffer = NULL;
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        mRecBuffer[mIndex*2]   = spl0;
        mRecBuffer[mIndex*2+1] = spl1;
        mIndex += 1;
        if (mIndex>=mLength)
        {
          mIndex=0;
          float* temp = mRecBuffer;
          mRecBuffer = mPlayBuffer;
          mPlayBuffer = temp;
          mFilled=true;
        }
        *outs[0] = spl0;
        *outs[1] = spl1;
      }
};
*/

//----------------------------------------------------------------------
#endif
