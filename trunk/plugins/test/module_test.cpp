#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
#include "axModule.h"

//----------------------------------------------------------------------
// gain/gate test modules
//----------------------------------------------------------------------

class modGain : public axModule
{
  private:
    float *in0_, *in1_;
    float *out0_,*out1_;
    float *gain_;
  public:
    modGain(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
    {
      appendPin(new axPin(this,"left in",   mpt_Data, mpd_Input/*,mpr_Dynamic,NULL,0*/));
      appendPin(new axPin(this,"right in",  mpt_Data, mpd_Input));
      appendPin(new axPin(this,"gain",      mpt_Data, mpd_Input));
      appendPin(new axPin(this,"left out",  mpt_Data, mpd_Output));
      appendPin(new axPin(this,"right out", mpt_Data, mpd_Output));
    }
    virtual void doProcess(void)
      {
        mPins[3]->mValue = *mPins[0]->mPtr * *mPins[2]->mPtr;
        mPins[4]->mValue = *mPins[1]->mPtr * *mPins[2]->mPtr;
        }
    virtual void doCompile(void)
      {
        in0_  =  mPins[0]->mPtr;
        in1_  =  mPins[1]->mPtr;
        gain_ =  mPins[2]->mPtr;
        out0_ = &mPins[3]->mValue;
        out1_ = &mPins[4]->mValue;
      }
    virtual void doExecute(void)
      {
        *out0_ = *in0_ * *gain_;
        *out1_ = *in1_ * *gain_;
      }
};

//----------------------------------------------------------------------

class modGate : public axModule
{
  private:
    float *in0_, *in1_;
    float *out0_,*out1_;
    float  gate_;
  public:
    modGate(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
    {
      gate_ = 0;
      appendPin(new axPin(this,"left in",   mpt_Data,   mpd_Input));
      appendPin(new axPin(this,"right in",  mpt_Data,   mpd_Input));
      appendPin(new axPin(this,"gate",      mpt_Signal, mpd_Input));
      appendPin(new axPin(this,"left out",  mpt_Data,   mpd_Output));
      appendPin(new axPin(this,"right out", mpt_Data,   mpd_Output));
    }
    virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0)
      {
        //TRACE("signal: %i, %i, %i\n",aIndex,aType,aNum);
        if (aIndex==2 && aType==sty_gate) { gate_ = (float)aNum; }
      }
    virtual void doProcess(void)
      {
        int muller;
        if (gate_) muller = 1; else muller=0;
        mPins[3]->mValue = *mPins[0]->mPtr * muller;
        mPins[4]->mValue = *mPins[1]->mPtr * muller;
        }
    virtual void doCompile(void)
      {
        in0_  =  mPins[0]->mPtr;
        in1_  =  mPins[1]->mPtr;
        //gate_ =  mPins[2]->mPtr;
        out0_ = &mPins[3]->mValue;
        out1_ = &mPins[4]->mValue;
      }
    virtual void doExecute(void)
      {
        *out0_ = *in0_ * gate_;
        *out1_ = *in1_ * gate_;
      }
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axModuleListener
{
  public:
    float mValue;
    float in0,in1;
    float out0,out1;
    modGain* gain;
    modGate* gate;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("module_test","ccernn","product_string",0,0);
        gain = new modGain(this,"gain");
          gain->connectDirect(0,&in0);
          gain->connectDirect(1,&in1);
          gain->connectDirect(2,&mValue);
          gain->doCompile();
        gate = new modGate(this,"gate");
          gate->connectPin(gain,3,0);
          gate->connectPin(gain,4,1);
          gate->doCompile();
        appendParameter(new parFloat(this,0,"value","",0.75));
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        // parameters are automatically destroyed
        delete gain;
        delete gate;
      }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
        int msg = (msg1 & 0xf0) >> 4;
        if (msg==9)
        {
          if (msg3>0) gate->doSignal(2,sty_gate,1);
          else gate->doSignal(2,sty_gate,0);
        }
        else if (msg==8) gate->doSignal(2,sty_gate,0);
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: mValue = f;  break;
        }
      }

    //--------------------------------------------------

    virtual void doProcessSample(float** ins, float** outs)
      {
        in0 = *ins[0];
        in1 = *ins[1];
        //gain->doProcess();
        //gate->doProcess();
        gain->doExecute();
        gate->doExecute();
        out0 = gate->mPins[3]->mValue;
        out1 = gate->mPins[4]->mValue;
        *outs[0] = out0;
        *outs[1] = out1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
