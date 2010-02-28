/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0 
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See LICENSE_AX for more details.
 *  
 * You should have received a copy of the Axonlib License 
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file dspEnvelope.h
  \brief envelope
*/

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

