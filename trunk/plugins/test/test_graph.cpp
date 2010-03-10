#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

//#define AX_DEBUG
//#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"
#include "wdgPanel.h"

#include "axGraph.h"
#include "axModule.h"
#include "axModule_Std.h"

//----------------------------------------------------------------------
//
// graph
//
//----------------------------------------------------------------------

//  _________
// |         |-----.
// | audioin |     .    ______         __________
// |_________|--   '-->|      |   .-->|          |
//  _________          | mul1 |---|   | audioout |
// |         |     .-->|______|   `-->|__________|
// | param1  |-----'
// |_________|
//

class myGraph : public axGraph,
                public axModuleListener
{
  private:
    modAudioIn*   audioin;
    modAudioOut*  audioout;
    modConst*     const1;
    modMul*       mul1;
  public:
    myGraph(axModuleListener* aOwner, axString aName)
    : axGraph(aOwner, aName)
      {
        appendModule( audioin  = new modAudioIn(  this,"audio in"  ) );
        appendModule( const1   = new modConst(    this,"param"     ) );
        appendModule( mul1     = new modMul(      this,"mul"       ) );
        appendModule( audioout = new modAudioOut( this,"audio out" ) );
            mul1->connectDataInput( 0,audioin,0 );
            mul1->connectDataInput( 1,const1, 0 );
        audioout->connectDataInput( 0,mul1,   0 );
        audioout->connectDataInput( 1,mul1,   0 );
      }
    inline void setParam(int aIndex, float aValue)      { if (aIndex==0) const1->setValue(aValue); }
    inline void setInputs(float aLeft, float aRight)    { audioin->setInputs(aLeft,aRight); }
    inline void getOutputs(float* aLeft, float* aRight) { *aLeft=audioout->getInput(0); *aRight=audioout->getInput(1);}
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
    myGraph*      mGraph;
  public:
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        axRandomize(19);
        describe("test_graph","ccernn","ccernn.audio",0001,AX_MAGIC);
        setupAudio(2,2,false);
        setupEditor(320,240);
          mGraph = new myGraph(this,"graph");
          mGraph->doCompile();
        appendParameter(new axParameter(this,0,"param"));
        processParameters();
      }
    virtual ~myPlugin() { delete mGraph; }
    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* panel;
        aEditor->appendWidget( panel = new wdgPanel(this,0,NULL_RECT,wal_Client) );
        // nothing yet...
        aEditor->doRealign();
      }
    virtual void doProcessParameter(axParameter* aParameter) { if (aParameter->mID==0) mGraph->setParam( 0, aParameter->getValue() ); }
    virtual void doProcessSample(float** ins, float** outs)
      {
        mGraph->setInputs( *ins[0], *ins[1] );
        mGraph->doExecute();
        mGraph->getOutputs( outs[0], outs[1] );
      }
};

//----------------------------------------------------------------------
#include "axMain.h"
