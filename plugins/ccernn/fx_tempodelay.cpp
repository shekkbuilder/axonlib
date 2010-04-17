//#define AX_DEBUG
//#include "core/axDebug.h"

#include "axPlugin.h"
#include "par/parFloat.h"

//----------------------------------------------------------------------

#define MAX_BEATS             4
#define MIN_BPM               30
#define MAX_SRATE             192000
#define MAX_SECONDS_PER_BEAT  60 / MIN_BPM
#define MAX_SAMPLES_PER_BEAT  MAX_SRATE * MAX_SECONDS_PER_BEAT
#define MAX_DELAY_LENGTH      MAX_BEATS * MAX_SAMPLES_PER_BEAT
//#define MAX_DELAY_LENGTH    1536000

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
  // process
    SPL           mBuffer[MAX_DELAY_LENGTH*2]; // totally 12mb (1.5m*sizeof(float)*2)
    int           mIndex;
    int           mSize;
  //'internal' parameters
    float         m_Delay;
    float         m_Feedback;
    float         m_Dry;
    float         m_Wet;
  //vst parameters
    parFloat*     p_Delay;
    axParameter*  p_Feedback;
    axParameter*  p_Dry;
    axParameter*  p_Wet;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {
        mIndex = 0;
        mSize  = 0;
        describe("fx_tempodelay","ccernn","axonlib example",2,AX_MAGIC+0x1001);
        setupAudio(2,2,false);
        appendParameter( p_Delay    = new parFloat(   this,"delay",   "",0.75, 0.25, MAX_BEATS, 0.25 ) );
        appendParameter( p_Feedback = new axParameter(this,"feedback","",0.75 ) );
        appendParameter( p_Dry      = new axParameter(this,"dry",     "",0.75 ) );
        appendParameter( p_Wet      = new axParameter(this,"wet",     "",0.4  ) );
        setupParameters();
      }

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float val = aParameter->getValue();
        axString name = aParameter->getName();
        //wtrace("doSetParameter(" << index << ") = " << val << "  [" <<  name.ptr() << "]" );
        switch(index)
        {
          // be careful with that index, eugene!
          case 0: m_Delay    = val; break;
          case 1: m_Feedback = val; break;
          case 2: m_Dry      = val; break;
          case 3: m_Wet      = val; break;
        }
      }

    //----------

    //[internal]
    int calcSize(float delay, float tempo, float srate)
      {
        float sec_per_beat = 60 / tempo;
        float spl_per_beat = srate * sec_per_beat;
        return (int)(delay * spl_per_beat);
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        updateTimeInfo(); // get timing info from host
        mSize = calcSize( m_Delay, getTempo(), getSampleRate() );
        while (mIndex>=mSize) mIndex-=mSize;
        return false; // we want doProcessSample
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        int i2 = mIndex*2;
        float in0 = *aInputs[0];
        float in1 = *aInputs[1];
        float dly0 = mBuffer[i2  ];
        float dly1 = mBuffer[i2+1];
        mBuffer[i2  ] = in0 + dly0*m_Feedback;;
        mBuffer[i2+1] = in1 + dly1*m_Feedback;;
        mIndex++;
        if (mIndex>=mSize) mIndex = 0;
        *aOutputs[0] = in0*m_Dry + dly0*m_Wet;
        *aOutputs[1] = in1*m_Dry + dly1*m_Wet;
      }

};

//----------------------------------------------------------------------

#undef MAX_BEATS
#undef MIN_BPM
#undef MAX_SRATE
#undef MAX_SECONDS_PER_BEAT
#undef MAX_SAMPLES_PER_BEAT
#undef MAX_DELAY_LENGTH

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
