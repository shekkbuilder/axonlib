
#include "dspOsc0.h"
#include "dspRC.h"
#include "dspEnvelope.h"

enum v_enums
{
  v_o1_gain = 0,
  v_o1_vol_att,
  v_o1_vol_dec,
  v_o1_vol_sus,
  v_o1_vol_rel,
  v_o1_tune_oct,
  v_o1_tune_semi,
  v_o1_tune_cent,
  v_o2_gain,
  v_o2_vol_att,
  v_o2_vol_dec,
  v_o2_vol_sus,
  v_o2_vol_rel,
  v_o2_tune_oct,
  v_o2_tune_semi,
  v_o2_tune_cent,
  v_mas_gain,
  v_mas_vol_att,
  v_mas_vol_dec,
  v_mas_vol_sus,
  v_mas_vol_rel
};

class myVoice : public axVoice
{
  private:
    float         o1_gain;
    float         o1_vol_att, o1_vol_dec, o1_vol_sus, o1_vol_rel;
    float         o1_tune_oct, o1_tune_semi, o1_tune_cent;
    float         o2_gain;
    float         o2_vol_att, o2_vol_dec, o2_vol_sus, o2_vol_rel;
    float         o2_tune_oct, o2_tune_semi, o2_tune_cent;
    float         mas_gain;
    float         mas_vol_att, mas_vol_dec, mas_vol_sus, mas_vol_rel;

    float         midi_vel;

    dspOsc0       osc1, osc2;

    dspEnvExpADSR o1_vol_env;
    dspEnvExpADSR o2_vol_env;
    dspEnvExpADSR mas_vol_env;

  public:

    myVoice() : axVoice()
      {
        o1_vol_att = 0;
        o1_vol_dec = 0;
        o1_vol_sus = 1;
        o1_vol_rel = 0;
        o2_vol_att = 0;
        o2_vol_dec = 0;
        o2_vol_sus = 1;
        o2_vol_rel = 0;
        mas_vol_att = 0;
        mas_vol_dec = 0;
        mas_vol_sus = 1;
        mas_vol_rel = 0;
        osc1.setup(osc_Ramp,0,0);
        osc2.setup(osc_Ramp,0,0);
        o1_vol_env.setup();
        o1_vol_env.setScale(32);
        o1_vol_env.setADSR(o1_vol_att,o1_vol_dec,o1_vol_sus,o1_vol_rel);
        o2_vol_env.setup();
        o2_vol_env.setScale(32);
        o2_vol_env.setADSR(o2_vol_att,o2_vol_dec,o2_vol_sus,o2_vol_rel);
        mas_vol_env.setup();
        mas_vol_env.setScale(32);
        mas_vol_env.setADSR(mas_vol_att,mas_vol_dec,mas_vol_sus,mas_vol_rel);
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
        midi_vel = (float)aVel * inv127;
        //venv1.setADSR(att1,dec1,sus1,rel1);
        o1_vol_env.setADSR(o1_vol_att,o1_vol_dec,o1_vol_sus,o1_vol_rel);
        o1_vol_env.noteOn();
        o2_vol_env.setADSR(o2_vol_att,o2_vol_dec,o2_vol_sus,o2_vol_rel);
        o2_vol_env.noteOn();
        mas_vol_env.setADSR(mas_vol_att,mas_vol_dec,mas_vol_sus,mas_vol_rel);
        mas_vol_env.noteOn();
        float tune1 = (o1_tune_oct*12) + o1_tune_semi + o1_tune_cent;
        float tune2 = (o2_tune_oct*12) + o2_tune_semi + o2_tune_cent;
        float fr1 = 440 * powf(2.0,(aNote-69.0+tune1) / 12);
        float fr2 = 440 * powf(2.0,(aNote-69.0+tune2) / 12);
        osc1.setFreq(fr1);
        osc2.setFreq(fr2);
      }

    virtual void noteOff(int aNote, int aVel)
      {
        o1_vol_env.noteOff();
        o2_vol_env.noteOff();
        mas_vol_env.noteOff();
      }

    //virtual void  ctrlChange(int aCtrl, int aVal) {}

    virtual void control(int aIndex, float aValue)
      {
        switch(aIndex)
        {
          case v_o1_gain:      o1_gain      = aValue; break;
          case v_o1_vol_att:   o1_vol_att   = aValue; break;
          case v_o1_vol_dec:   o1_vol_dec   = aValue; break;
          case v_o1_vol_sus:   o1_vol_sus   = aValue; break;
          case v_o1_vol_rel:   o1_vol_rel   = aValue; break;

          case v_o1_tune_oct:   o1_tune_oct   = aValue; break;
          case v_o1_tune_semi:  o1_tune_semi  = aValue; break;
          case v_o1_tune_cent:  o1_tune_cent  = aValue; break;

          case v_o2_gain:      o2_gain      = aValue; break;
          case v_o2_vol_att:   o2_vol_att   = aValue; break;
          case v_o2_vol_dec:   o2_vol_dec   = aValue; break;
          case v_o2_vol_sus:   o2_vol_sus   = aValue; break;
          case v_o2_vol_rel:   o2_vol_rel   = aValue; break;

          case v_o2_tune_oct:   o2_tune_oct   = aValue; break;
          case v_o2_tune_semi:  o2_tune_semi  = aValue; break;
          case v_o2_tune_cent:  o2_tune_cent  = aValue; break;

          case v_mas_gain:      mas_gain      = aValue; break;
          case v_mas_vol_att:   mas_vol_att   = aValue; break;
          case v_mas_vol_dec:   mas_vol_dec   = aValue; break;
          case v_mas_vol_sus:   mas_vol_sus   = aValue; break;
          case v_mas_vol_rel:   mas_vol_rel   = aValue; break;
        }
      }

    virtual void process(float* outs)
      {
        float o1 = osc1.process();
        float o2 = osc2.process();
        float ev1 = o1_vol_env.process();
        float ev2 = o2_vol_env.process();
        o1 *= ev1;
        o2 *= ev2;
        float evm = mas_vol_env.process();
        if (evm<EPSILON) mState=vst_Off;
        float out = (o1+o2) * midi_vel * evm * mas_gain;
        outs[0] = out;
        outs[1] = out;
      }

};
