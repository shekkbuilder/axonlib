#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"

#include "axSurface.h"
#include "axBitmap.h"
#include "wdgPanel.h"
#include "wdgImgKnob2.h"
#include "axBitmapLoader.h"
#include "../images/knob32.h"

#include "axGraph.h"
#include "axModule.h"
#include "axModule_Std.h"

/*

  should be easy to split into multiple files, for example:
    plugin.cpp
    plugin_graph.h
    plugin_editor.h
*/

//----------------------------------------------------------------------
//
// graph
//
//----------------------------------------------------------------------

//  _________           ______
// |         |-------->|      |
// | audioin |         | mul1 |---.    __________
// |_________|--.  .-->|______|   `-->|          |
//  _________   :  :    ______        | audioout |
// |         |  '----->|      |   .-->|__________|
// | const1  |-----:   | mul2 |---'
// |_________|     '-->|______|
//
// - input samples are delivered to audioin before graph execution
// - outputa samples read from  audioout after graph exec
// - parameter change write to const1 (knob/vst-param)

class myGraph : public axGraph,
                public axModuleListener
{
  private:
    modAudioIn*   audioin;
    modAudioOut*  audioout;
    modConst*     const1; //parameter
    modMul*       mul1;
    modMul*       mul2;

  public:

    myGraph(axModuleListener* aOwner, axString aName)
    : axGraph(aOwner, aName)
      {
        appendModule( audioin  = new modAudioIn(  this,"audio in"  ) );
        appendModule( const1   = new modConst(    this,"param"     ) );
        appendModule( mul1     = new modMul(      this,"mul1"       ) );
        appendModule( mul2     = new modMul(      this,"mul2"       ) );
        appendModule( audioout = new modAudioOut( this,"audio out" ) );
            mul1->connectDataInput( 0,audioin,0 );
            mul1->connectDataInput( 1,const1, 0 );
            mul2->connectDataInput( 0,audioin,1 );
            mul2->connectDataInput( 1,const1, 0 );
        audioout->connectDataInput( 0,mul1,   0 );
        audioout->connectDataInput( 1,mul2,   0 );
      }

    inline void setParam(int aIndex, float aValue)      { if (aIndex==0) const1->setValue(aValue); }
    inline void setInputs(float aLeft, float aRight)    { audioin->setInputs(aLeft,aRight); }
    //inline void getOutputs(float* aLeft, float* aRight) { *aLeft=audioout->getInput(0); *aRight=audioout->getInput(1);}
    inline void getOutputs(float* aLeft, float* aRight) { *aLeft=audioout->readInput(0); *aRight=audioout->readInput(1);}
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor: public axEditor
{
  private:
    wdgPanel*     wPanel;
    wdgImgKnob2*  wKnob;

  public:
    myEditor(axString aWinName, axPlugin* aPlugin, int aID, axRect aRect, int aWinFlags, int aParent)
    : axEditor(aWinName,aPlugin,aID,aRect,aWinFlags, aParent)
      { }

    inline void setup(axSurface* srf)
      {
        appendWidget( wPanel = new wdgPanel(this,-1,NULL_RECT,wal_Client) );
        wPanel->appendWidget( wKnob = new wdgImgKnob2(this,0,axRect(10,10,32,64),wal_None,65,srf) );
        connect( wKnob, getPlugin()->getParamPtr(0) );
        doRealign();
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
    myGraph*      mGraph;
    parFloat*     pKnob;
    bool          mGuiInit;
    axSurface*    srfKnob;

  public:

    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mGuiInit = false;
        describe("test_graph","ccernn","ccernn.audio",0001,AX_MAGIC);
        setupAudio(2,2,false);
        setupEditor(320,240);
          mGraph = new myGraph(this,"graph");
          mGraph->doCompile();
        appendParameter( pKnob = new parFloat(this,0,"gain","",0) );
        processParameters();
      }

    virtual ~myPlugin()
      {
        delete mGraph;
        if (mGuiInit) delete srfKnob;
      }

    // override axEffect's doCreateEditor
    // becuause we want our own editor class, and not the generic axEditor
    virtual void* doCreateEditor(int aParent)
      {
        if (!mGuiInit)
        {
          srfKnob = loadPng((unsigned char*)knob32,knob32_size);
          mGuiInit = true;
        }
        myEditor* ed;
        ed = new myEditor("test_graph_editor",this,-1,axRect(0,0,mWidth,mHeight),AX_FLAGS,aParent);
        ed->setup(srfKnob);
        mEditor = ed;
        return mEditor;
      }

    virtual void doProcessParameter(axParameter* aParameter)
      {
        if (aParameter->mID==0) mGraph->setParam( 0, aParameter->getValue() );
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        mGraph->setInputs( *ins[0], *ins[1] );
        mGraph->doExecute();
        mGraph->getOutputs( outs[0], outs[1] );
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
