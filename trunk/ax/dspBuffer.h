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
 * @file dspBuffer.h
 * \brief dsp buffer
 */

#ifndef dspBuffer_included
#define dspBuffer_included
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
