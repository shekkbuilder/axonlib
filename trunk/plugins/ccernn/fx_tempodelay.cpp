//#define AX_DEBUG
//#include "core/axDebug.h"

#include "axPlugin.h"
#include "par/parFloat.h"

#define MAX_BEATS             4
#define MIN_BPM               30
#define MAX_SRATE             192000
#define MAX_SECONDS_PER_BEAT  60 / MIN_BPM
#define MAX_SAMPLES_PER_BEAT  MAX_SRATE * MAX_SECONDS_PER_BEAT
#define MAX_DELAY_LENGTH      MAX_BEATS * MAX_SAMPLES_PER_BEAT

class myPlugin : public axPlugin
{
  private:
    SPL           mBuffer[MAX_DELAY_LENGTH*2];
    int           mIndex;
    int           mSize;

  //axParameter*  p_Delay;
    parFloat*     p_Delay;
    axParameter*  p_Feedback;
    axParameter*  p_Dry;
    axParameter*  p_Wet;

    float         m_Delay;
    float         m_Feedback;
    float         m_Dry;
    float         m_Wet;

  public:

    myPlugin(axContext* aContext)
    : axPlugin(aContext, pf_None)
      {

        //wtrace("MAX_BEATS: " << MAX_BEATS);
        //wtrace("MIN_BPM: " << MIN_BPM);
        //wtrace("MAX_SRATE: " << MAX_SRATE);
        //wtrace("MAX_SECONDS_PER_BEAT: " << MAX_SECONDS_PER_BEAT);
        //wtrace("MAX_SAMPLES_PER_BEAT: " << MAX_SAMPLES_PER_BEAT);
        //wtrace("MAX_DELAY_LENGTH: " << MAX_DELAY_LENGTH);

        mIndex = 0;
        mSize = 0;

        m_Delay    = 0.75;
        m_Feedback = 0.75;
        m_Dry      = 0.75;
        m_Wet      = 0.4;
        describe("fx_tempodelay","ccernn","axonlib example",0,AX_MAGIC+0x1001);
        setupAudio(2,2,false);
        //appendParameter( p_Delay    = new axParameter(this,"delay",   "",m_Delay) );
        appendParameter( p_Delay    = new parFloat(   this,"delay",   "",m_Delay, 0.25, 4, 0.25 ) );
        appendParameter( p_Feedback = new axParameter(this,"feedback","",m_Feedback) );
        appendParameter( p_Dry      = new axParameter(this,"dry",     "",m_Dry) );
        appendParameter( p_Wet      = new axParameter(this,"wet",     "",m_Wet) );
        setupParameters();

      }

    virtual void doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float val = aParameter->getValue();
        axString name = aParameter->getName();
        //wtrace("doSetParameter(" << index << ") = " << val << " : " <<  name.ptr() );
        switch(index)
        {
          case 0: m_Delay = val;    break;
          case 1: m_Feedback = val; break;
          case 2: m_Dry = val;      break;
          case 3: m_Wet = val;      break;
        }
      }

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
    {
        updateTimeInfo();
        float srate             = getSampleRate();
        float bpm               = getTempo();
        float seconds_per_beat  = 60 / bpm;                   //wtrace("seconds_per_beat: " << seconds_per_beat);
        float samples_per_beat  = srate * seconds_per_beat;   //wtrace("samples_per_beat: " << samples_per_beat);
        mSize                   = m_Delay * samples_per_beat; //wtrace("delay_samples: " << delay_samples);
        return false;
    }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        int i2 = mIndex*2;
        float in0  = *aInputs[0];
        float in1  = *aInputs[1];
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
AX_ENTRYPOINT(myPlugin)
