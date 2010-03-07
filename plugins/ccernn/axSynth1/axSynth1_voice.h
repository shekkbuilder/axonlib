
#include "dspOsc0.h"
#include "dspRC.h"
#include "dspEnvelope.h"

#define v_gain      0
#define v_tuneoct   1
#define v_tunesemi  2
#define v_tunecent  3
#define v_attack    4
#define v_decay     5
#define v_sustain   6
#define v_release   7

class myVoice : public axVoice
{
  private:
    float         gain,tuneoct,tunesemi,tunecent,vel;
    float         attack,decay,sustain,release;
    dspOsc0       osc1,osc2;
  //dspRC         att1,
  //              rel1;
    dspEnvExpADSR env1;
  public:

    myVoice() : axVoice()
      {
        attack  = 0;
        decay   = 0;
        sustain = 1;
        release = 0;
        osc1.setup(osc_Ramp,0,0);
        osc2.setup(osc_Ramp,0,0);
        //osc2.setup(osc_Squ,0,0);
        //att1.setup(0,1,0);
        //rel1.setup(0,0,0);
        //env1.setScale(200);
        env1.setup();
        env1.setScale(32);
        env1.setADSR(attack,decay,sustain,release);
      }

    virtual ~myVoice()
      {
      }

    virtual void setSampleRate(float aRate)
      {
        osc1.setSampleRate(aRate);
        osc2.setSampleRate(aRate);
        axVoice::setSampleRate(aRate);
      }

    virtual void noteOn(int aNote, int aVel)
      {
        vel = (float)aVel * inv127;
        //att1.setValue(0);         // start from 0
        //att1.setWeight(attack);   // fade up
        //rel1.setValue(1);         // initially, set to full on
        //rel1.setWeight(0);        // fade speed = 0 (until note off)
        env1.setADSR(attack,decay,sustain,release);
        env1.noteOn();
        float detune = (tuneoct*12) + tunesemi + tunecent;
        float fr1 = 440 * powf(2.0,(aNote-69.0) / 12);
        float fr2 = 440 * powf(2.0,(aNote-69.0+detune) / 12);
        osc1.setFreq(fr1);
        osc2.setFreq(fr2);
      }

    virtual void noteOff(int aNote, int aVel)
      {
        //rel1.setValue( att1.getValue() ); // start from current amplitude
        //rel1.setWeight(release);          // and let it fade down
        env1.noteOff();
      }

    //virtual void  ctrlChange(int aCtrl, int aVal) {}

    virtual void control(int aIndex, float aValue)
      {
        switch(aIndex)
        {
          case v_gain:      gain      = aValue; break;
          case v_attack:    attack    = aValue; break;
          case v_decay:     decay     = aValue; break;
          case v_sustain:   sustain   = aValue; break;
          case v_release:   release   = aValue; break;
          case v_tuneoct:   tuneoct   = aValue; break;
          case v_tunesemi:  tunesemi  = aValue; break;
          case v_tunecent:  tunecent  = aValue; break;
        }
      }

    virtual void process(float* outs)
      {
        float out1 = osc1.process();
        float out2 = osc2.process();
        //float a = att1.process();
        //float r = rel1.process();
        float ee = env1.process();
        if (ee<EPSILON) mState=vst_Off;
        float out = (out1+out2) * vel * ee * gain;
        //outs[0] = out1*vel*n * gain;
        //outs[1] = out2*vel*n * gain;
        outs[0] = out;
        outs[1] = out;
      }

};
