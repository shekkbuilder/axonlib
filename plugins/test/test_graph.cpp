#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"
//#include "parInteger.h"
#include "wdgPanel.h"
//#include "wdgLabel.h"
//#include "wdgImgKnob2.h"
//#include "images/knob.h"
#include "axModule.h"
#include "axGraph.h"

//----------------------------------------------------------------------
//
// test modules
//
//----------------------------------------------------------------------

class modConst : public axModule
{
  private:
    axPin*  mOut0;
  public:
    modConst(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Const;
        appendOutput( mOut0 = new axPin(this,"out0",mpt_Data,mpr_Const,1) );
        //mOut0->writeValue(1);
      }
    inline void setValue(float aValue) { mOut0->writeValue(aValue); }
    //virtual void doProcess(void)
    //  {
    //  }
};

//----------

class modNoise : public axModule
{
  private:
    axPin*  mOut0;
  public:
    modNoise(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendOutput( mOut0 = new axPin(this,"output",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        mOut0->writeValue( axRandomSigned() );
      }
};

//----------

class modMul : public axModule
{
  private:
    axPin *mIn0, *mIn1;
    axPin *mOut0;
  public:
    modMul(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in0",mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1",mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in0 = mIn0->readValue();
        float in1 = mIn1->readValue();
        mOut0->writeValue( in0*in1 );
      }
};

//----------

class modStereo : public axModule
{
  private:
    axPin *mIn0;
    axPin *mOut0, *mOut1;
  public:
    modStereo(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in",  mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out0",mpt_Data) );
        appendOutput( mOut1 = new axPin(this,"out1",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in = mIn0->readValue();
        mOut0->writeValue( in );
        mOut1->writeValue( in );
      }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axEffect,
                 public axModuleListener
{
  private:
    float         mInput0, mInput1;
    axGraph*      mGraph;
    modConst*     const1;
    modNoise*     noise1;
    modMul*       mul1;
    modStereo*    stereo1;
    float spl0,spl1;

  public:

    //--------------------------------------------------

    //  ________
    // |        |
    // | const1 | --.     ______          _________
    // |________|   '--> |      |        |         | -----> (left out)
    //  ________         | mul1 | -----> | stereo1 |
    // |        |   .--> |______|        |_________| -----> (right out)
    // | noise1 | --'
    // |________|
    //
    // doParameter writes data directly to const1
    // doProcess reads data directly from stereo1

    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        axRandomize(19);
        describe("test_graph","ccernn","ccernn.audio",0001,AX_MAGIC);
        setupAudio(2,2,false);
        setupEditor(640,480);
        mGraph = new axGraph(this,"graph");
          mGraph->appendModule( const1  = new modConst(   this,"const1" ) );
          mGraph->appendModule( noise1  = new modNoise(   this,"noise1" ) );
          mGraph->appendModule( mul1    = new modMul(     this,"mul1"   ) );
          mGraph->appendModule( stereo1 = new modStereo(  this,"stereo" ) );
            mul1->connectDataInput(0,noise1,0);
            mul1->connectDataInput(1,const1,0);
            stereo1->connectDataInput(0,mul1,0);
        mGraph->doCompile();
        appendParameter(new axParameter(this,0,"dummy"));
        processParameters();
      }

    //--------------------------------------------------

    virtual ~myPlugin()
      {
        delete mGraph;
      }

    //--------------------------------------------------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* panel;
        aEditor->appendWidget( panel = new wdgPanel(this,0,NULL_RECT,wal_Client) );
        aEditor->doRealign();
      }

    //--------------------------------------------------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        switch(aParameter->mID)
        {
          case 0: const1->setValue(aParameter->getValue()); break;
        }
      }

    //----------

    virtual void  doProcessSample(float** ins, float** outs)
      {
        spl0 = *ins[0];
        spl1 = *ins[1];
        mGraph->doExecute();
        *outs[0] = stereo1->getOutput(0);
        *outs[1] = stereo1->getOutput(1);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
