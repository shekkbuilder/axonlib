#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------


//TODO: proper ladspa sdk
#include "../extern/ladspa.h"

// this might be overkill?
// inputs + outputs + parameters
#define MAX_LADSPA_PORTS    256

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorLadspa : public axDescriptor
{
  public:
    axDescriptorLadspa(axBase* aBase)// : axDescriptor(aBase)
      { /*trace("  axDescriptorLadspa.constructor");*/ }
    virtual ~axDescriptorLadspa()
      { /*trace("  axDescriptorLadspa.destructor");*/ }
};

typedef axDescriptorLadspa AX_DESCRIPTOR;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstanceLadspa : public axInstance
{
  protected:
    axBase*       mBase;
    axDescriptor* mDescriptor;
    int           mNumInputs;
    int           mNumOutputs;
    int           mNumParams;
    float*        mInputs[MAX_LADSPA_PORTS];
    float*        mOutputs[MAX_LADSPA_PORTS];
    LADSPA_Data*  mParamPtr[MAX_LADSPA_PORTS];
    LADSPA_Data   mParamPrev[MAX_LADSPA_PORTS];
    int mBlockSize;

  public:

    axInstanceLadspa(axBase* aBase) /*: axInstance(aBase)*/
      {
        //trace("axInstanceLadspa.constructor")
        mBase       = aBase;
        mDescriptor = mBase->getDescriptor();
        mNumInputs  = mDescriptor->getNumInputs();
        mNumOutputs = mDescriptor->getNumOutputs();
        mNumParams  = mDescriptor->getNumParams();
      }


    virtual ~axInstanceLadspa()
      {
        //trace("axInstanceLadspa.destructor");
      }

    // callbacks

    virtual void lad_connect_port(unsigned long Port, LADSPA_Data* DataLocation)
      {
        //trace("axFormatLadspa.lad_connect_port");
        unsigned int io = mNumInputs + mNumOutputs;
        if (Port<io) // audio in/out
        {
          //TODO: don't hardcode ports!!!
          switch (Port)
          {
            case 0: mInputs[0]  = DataLocation; break;
            case 1: mInputs[1]  = DataLocation; break;
            case 2: mOutputs[0] = DataLocation; break;
            case 3: mOutputs[1] = DataLocation; break;
          }
        }
        else // parameter
        {
          int po = Port - io;
          mParamPtr[po] = DataLocation;
        }
      }

    virtual void lad_activate(void)
      {
        //trace("axFormatLadspa.lad_activate");
        doStateChange(is_Resume);
      }

    virtual void lad_run(unsigned long SampleCount)
      {
        //trace("axFormatLadspa.lad_run");
        int io  = mNumInputs + mNumOutputs;
        int par = mNumParams;//mParameters.size();
        for (int i=0; i<par; i++)
        {
          float val = *mParamPtr[i];
          if (val!=mParamPrev[i])
          {
//            mParameters[i]->doSetValue(val,true);
            mParamPrev[i] = val;
          }
        }
        mBlockSize = SampleCount;
        bool swallowed = doProcessBlock(mInputs,mOutputs,mBlockSize);
        if ( !swallowed )
        {
          float* ins[2];
          float* outs[2];
          ins[0]  = mInputs[0];
          ins[1]  = mInputs[1];
          outs[0] = mOutputs[0];
          outs[1] = mOutputs[1];
          //trace(SampleCount);
          int num = SampleCount;
          while (--num >= 0)
          {
            doProcessSample(ins,outs);
            ins[0]++;   ins[1]++;
            outs[0]++;  outs[1]++;
          } //SampleCount
        } //process_block
        doPostProcess(mInputs,mOutputs,mBlockSize);
      }

    //virtual void lad_run_adding(unsigned long SampleCount) {}
    //virtual void lad_set_run_adding_gain(LADSPA_Data Gain) {}

    virtual void lad_deactivate(void)
      {
        //trace("axFormatLadspa.lad_deactivate");
        doStateChange(is_Suspend);
      }

    virtual void lad_cleanup(void)
      {
        //trace("axFormatLadspa.lad_cleanup");*/
        doStateChange(is_Close);
      }

    //

    virtual void appendParameter(axParameter* aParameter) {}
    virtual void setupParameters(void) {}
    virtual void updateTimeInfo();

};

typedef axInstanceLadspa AX_INSTANCE;

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

class axFormatLadspa : public axFormat
{

  friend const LADSPA_Descriptor* ladspa_descriptor(unsigned long index);

  private:
    LADSPA_Descriptor ladspadescr;

  private:

    //--------------------------------------------------
    // static callback functions, host -> plugin
    //--------------------------------------------------

    // called by host to instantiate a plugin..
    // return the ptr to the instance (LADSPA_Handle)

    static LADSPA_Handle lad_instantiate_callback(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        //trace("lad_instantiate_callback");
        axFormatLadspa* desc = (axFormatLadspa*)Descriptor->ImplementationData;
        return desc->lad_instantiate(SampleRate);
      }

    //----------

    // the rest are redirected to the instance(s)

    static void lad_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        //trace("lad_connect_port_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_connect_port(Port,DataLocation);
      }

    //----------

    static void lad_activate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_activate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_activate();
        //
        //
      }

    //----------

    static void lad_run_callback(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        //trace("lad_run_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_run(SampleCount);
        //
        //
      }

    //----------

    //static void lad_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_run_adding(SampleCount);
    //  }

    //----------

    //static void lad_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_set_run_adding_gain(Gain);
    //  }

    //----------

    static void lad_deactivate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_deactivate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_deactivate();
        //
        //
      }

    //----------

    static void lad_cleanup_callback(LADSPA_Handle Instance)
      {
        //trace("lad_cleanup_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_cleanup();
        delete inst; // !!!
        //
        //
      }

    //--------------------------------------------------
    // redirected callback functions
    // (these could be in an instance class)
    //--------------------------------------------------

    // called from static callback function (above)

    // return ptr to instance
    virtual LADSPA_Handle lad_instantiate(unsigned long SampleRate)
      {
        //trace("axFormatLadspa.lad_instantiate");
        axInstance* instance = mBase->createInstance();
        //instance->mSampleRate = SampleRate;
        instance->doStateChange(is_Open);
        return instance;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  protected:
    axBase*               mBase;
    axDescriptor*         mDescriptor;
    char*                 mPortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor mPortDesc[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  mPortHint[MAX_LADSPA_PORTS];

  protected:

    virtual void* entrypoint(void* ptr)
      {
        //trace("* axFormatLadspa.entrypoint");
        mDescriptor = mBase->getDescriptor();
        int i;
        int index=0;
        for (i=0; i<mDescriptor->getNumInputs();  i++) { mPortNames[index++] = mDescriptor->getInputName(i);  }
        for (i=0; i<mDescriptor->getNumOutputs(); i++) { mPortNames[index++] = mDescriptor->getOutputName(i); }
        for (i=0; i<mDescriptor->getNumParams();  i++) { mPortNames[index++] = mDescriptor->getParamName(i);  }

        axMemset(&ladspadescr,0,sizeof(ladspadescr));
        ladspadescr.UniqueID            = 0;//mUniqueId;
        ladspadescr.Label               = mDescriptor->getName();//(char*)"label";
        ladspadescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        ladspadescr.Name                = mDescriptor->getName();// (char*)"name";
        ladspadescr.Maker               = mDescriptor->getAuthor();//(char*)"maker";
        ladspadescr.Copyright           = mDescriptor->getProduct();//(char*)"copyright";
        ladspadescr.PortCount           = mDescriptor->getNumInputs() + mDescriptor->getNumOutputs() + mDescriptor->getNumParams();
        ladspadescr.PortDescriptors     = mPortDesc;
        ladspadescr.PortNames           = mPortNames;//g_stereo_ports;
        ladspadescr.PortRangeHints      = mPortHint;
        ladspadescr.ImplementationData  = this;
        ladspadescr.instantiate         = lad_instantiate_callback;
        ladspadescr.connect_port        = lad_connect_port_callback;
        ladspadescr.activate            = lad_activate_callback;
        ladspadescr.run                 = lad_run_callback;
        ladspadescr.run_adding          = NULL;//lad_run_adding_callback;            // ala process, optional
        ladspadescr.set_run_adding_gain = NULL;//lad_set_run_adding_gain_callback;   // if above
        ladspadescr.deactivate          = lad_deactivate_callback;
        ladspadescr.cleanup             = lad_cleanup_callback;
        return (void*)&ladspadescr;
        //TODO: fix
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  public:

    axFormatLadspa(axBase* aBase)// : axFormat(aBase)
      {
        //trace("- axFormatLadspa.constructor");
        mBase = aBase;
      }

    virtual ~axFormatLadspa()
      {
        //trace("- axFormatLadspa.destructor");
      }

    virtual char* getFormatName(void)
      {
        return (char*)"ladspa";
      }

};

//----------

typedef axFormatLadspa AX_FORMAT;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------
#define AX_ENTRYPOINT(_PL,_IF,_FO,_D,_I)                                      \
                                                                              \
__externc __dllexport                                                         \
const LADSPA_Descriptor* ladspa_descriptor(unsigned long index)               \
{                                                                             \
  if (index>0) return NULL;                                                   \
  _AX_DEBUG_SETUP                                                             \
  axBaseImpl<_PL,_IF,_FO,_D,_I>* base = new axBaseImpl<_PL,_IF,_FO,_D,_I>();  \
  gGlobalScope.setBase(base);                                                 \
  _FO* format = (_FO*)base->getFormat();                                      \
  LADSPA_Descriptor* descr = (LADSPA_Descriptor*)format->entrypoint(NULL);    \
  return descr;                                                               \
}

//----------------------------------------------------------------------
#endif
