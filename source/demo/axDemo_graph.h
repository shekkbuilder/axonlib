#ifndef axDemo_graph_included
#define axDemo_graph_included
//----------------------------------------------------------------------

#include "audio/axModule.h"
#include "audio/axGraph.h"

//----------------------------------------------------------------------

class axDemo_graph : public axGraph
{
  public:
    axDemo_graph(axString aName)
    : axGraph(aName)
      {
      }
    ~axDemo_graph()
      {
      }
};

//----------------------------------------------------------------------
#endif


// some old examples (from pre.r151)





//#define AX_PLUGIN     myPlugin
//#define AX_NUMPROGS   1
//#define AX_NUMPARAMS  1
//#define AX_DEBUG
//#include "axDebug.h"
//#include "axFormat.h"
//#include "parFloat.h"
//#include "axModule.h"
//
////----------------------------------------------------------------------
//// gain/gate test modules
////----------------------------------------------------------------------
//
//class modGain : public axModule
//{
//  private:
//    float *in0_, *in1_;
//    float *out0_,*out1_;
//    float *gain_;
//  public:
//    modGain(axModuleListener* aOwner, axString aName)
//    : axModule(aOwner,aName)
//    {
//      appendPin(new axPin(this,"left in",   mpt_Data, mpd_Input/*,mpr_Dynamic,NULL,0*/));
//      appendPin(new axPin(this,"right in",  mpt_Data, mpd_Input));
//      appendPin(new axPin(this,"gain",      mpt_Data, mpd_Input));
//      appendPin(new axPin(this,"left out",  mpt_Data, mpd_Output));
//      appendPin(new axPin(this,"right out", mpt_Data, mpd_Output));
//    }
//    virtual void doProcess(void)
//      {
//        mPins[3]->mValue = *mPins[0]->mPtr * *mPins[2]->mPtr;
//        mPins[4]->mValue = *mPins[1]->mPtr * *mPins[2]->mPtr;
//        }
//    virtual void doCompile(void)
//      {
//        in0_  =  mPins[0]->mPtr;
//        in1_  =  mPins[1]->mPtr;
//        gain_ =  mPins[2]->mPtr;
//        out0_ = &mPins[3]->mValue;
//        out1_ = &mPins[4]->mValue;
//      }
//    virtual void doExecute(void)
//      {
//        *out0_ = *in0_ * *gain_;
//        *out1_ = *in1_ * *gain_;
//      }
//};
//
////----------------------------------------------------------------------
//
//class modGate : public axModule
//{
//  private:
//    float *in0_, *in1_;
//    float *out0_,*out1_;
//    float  gate_;
//  public:
//    modGate(axModuleListener* aOwner, axString aName)
//    : axModule(aOwner,aName)
//    {
//      gate_ = 0;
//      appendPin(new axPin(this,"left in",   mpt_Data,   mpd_Input));
//      appendPin(new axPin(this,"right in",  mpt_Data,   mpd_Input));
//      appendPin(new axPin(this,"gate",      mpt_Signal, mpd_Input));
//      appendPin(new axPin(this,"left out",  mpt_Data,   mpd_Output));
//      appendPin(new axPin(this,"right out", mpt_Data,   mpd_Output));
//    }
//    virtual void doSignal(int aIndex, int aType, int aNum=0, float aVal=0)
//      {
//        //TRACE("signal: %i, %i, %i\n",aIndex,aType,aNum);
//        if (aIndex==2 && aType==sty_gate) { gate_ = (float)aNum; }
//      }
//    virtual void doProcess(void)
//      {
//        int muller;
//        if (gate_) muller = 1; else muller=0;
//        mPins[3]->mValue = *mPins[0]->mPtr * muller;
//        mPins[4]->mValue = *mPins[1]->mPtr * muller;
//        }
//    virtual void doCompile(void)
//      {
//        in0_  =  mPins[0]->mPtr;
//        in1_  =  mPins[1]->mPtr;
//        //gate_ =  mPins[2]->mPtr;
//        out0_ = &mPins[3]->mValue;
//        out1_ = &mPins[4]->mValue;
//      }
//    virtual void doExecute(void)
//      {
//        *out0_ = *in0_ * gate_;
//        *out1_ = *in1_ * gate_;
//      }
//};
//
////----------------------------------------------------------------------
//
//class myPlugin : public axFormat,
//                 public axModuleListener
//{
//  public:
//    float mValue;
//    float in0,in1;
//    float out0,out1;
//    modGain* gain;
//    modGate* gate;
//
//  public:
//
//    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
//    : axFormat(audioMaster,aNumProgs,aNumParams,aPlugFlags)
//      {
//        describe("module_test","ccernn","product_string",0,0);
//        gain = new modGain(this,"gain");
//          gain->connectDirect(0,&in0);
//          gain->connectDirect(1,&in1);
//          gain->connectDirect(2,&mValue);
//          gain->doCompile();
//        gate = new modGate(this,"gate");
//          gate->connectPin(gain,3,0);
//          gate->connectPin(gain,4,1);
//          gate->doCompile();
//        appendParameter(new parFloat(this,0,"value","",0.75));
//        processParameters();
//      }
//
//    //----------
//
//    virtual ~myPlugin()
//      {
//        // parameters are automatically destroyed
//        delete gain;
//        delete gate;
//      }
//
//    //----------
//
//    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
//      {
//        //TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
//        int msg = (msg1 & 0xf0) >> 4;
//        if (msg==9)
//        {
//          if (msg3>0) gate->doSignal(2,sty_gate,1);
//          else gate->doSignal(2,sty_gate,0);
//        }
//        else if (msg==8) gate->doSignal(2,sty_gate,0);
//      }
//
//    //----------
//
//    virtual void doProcessParameter(axParameter* aParameter)
//      {
//        int  id = aParameter->mID;
//        float f = aParameter->getValue();
//        switch(id)
//        {
//          case 0: mValue = f;  break;
//        }
//      }
//
//    //--------------------------------------------------
//
//    virtual void doProcessSample(float** ins, float** outs)
//      {
//        in0 = *ins[0];
//        in1 = *ins[1];
//        //gain->doProcess();
//        //gate->doProcess();
//        gain->doExecute();
//        gate->doExecute();
//        out0 = gate->mPins[3]->mValue;
//        out1 = gate->mPins[4]->mValue;
//        *outs[0] = out0;
//        *outs[1] = out1;
//      }
//
//};
//
////----------------------------------------------------------------------
//#include "axMain.h"




// another old thing....





//#define AX_PLUGIN     myPlugin
//#define AX_NUMPROGS   1
//#define AX_NUMPARAMS  1
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//
//#define AX_DEBUG
//#include "axDebug.h"
//
//#include "axEffect.h"
//#include "parFloat.h"
//
//#include "axSurface.h"
//#include "axBitmap.h"
//#include "wdgPanel.h"
//#include "wdgImgKnob2.h"
//#include "axBitmapLoader.h"
//#include "../images/knob32.h"
//
//#include "axGraph.h"
//#include "axModule.h"
//#include "axModule_Std.h"
//
///*
//
//  should be easy to split into multiple files, for example:
//    plugin.cpp
//    plugin_graph.h
//    plugin_editor.h
//*/
//
////----------------------------------------------------------------------
////
//// graph
////
////----------------------------------------------------------------------
//
////  _________           ______
//// |         |-------->|      |
//// | audioin |         | mul1 |---.    __________
//// |_________|--.  .-->|______|   `-->|          |
////  _________   :  :    ______        | audioout |
//// |         |  '----->|      |   .-->|__________|
//// | const1  |-----:   | mul2 |---'
//// |_________|     '-->|______|
////
//// - input samples are delivered to audioin before graph execution
//// - outputa samples read from  audioout after graph exec
//// - parameter change write to const1 (knob/vst-param)
//
//class myGraph : public axGraph,
//                public axModuleListener
//{
//  private:
//    modAudioIn*   audioin;
//    modAudioOut*  audioout;
//    modConst*     const1; //parameter
//    modMul*       mul1;
//    modMul*       mul2;
//
//  public:
//
//    myGraph(axModuleListener* aOwner, axString aName)
//    : axGraph(aOwner, aName)
//      {
//        appendModule( audioin  = new modAudioIn(  this,"audio in"  ) );
//        appendModule( const1   = new modConst(    this,"param"     ) );
//        appendModule( mul1     = new modMul(      this,"mul1"       ) );
//        appendModule( mul2     = new modMul(      this,"mul2"       ) );
//        appendModule( audioout = new modAudioOut( this,"audio out" ) );
//            mul1->connectDataInput( 0,audioin,0 );
//            mul1->connectDataInput( 1,const1, 0 );
//            mul2->connectDataInput( 0,audioin,1 );
//            mul2->connectDataInput( 1,const1, 0 );
//        audioout->connectDataInput( 0,mul1,   0 );
//        audioout->connectDataInput( 1,mul2,   0 );
//      }
//
//    inline void setParam(int aIndex, float aValue)      { if (aIndex==0) const1->setValue(aValue); }
//    inline void setInputs(float aLeft, float aRight)    { audioin->setInputs(aLeft,aRight); }
//  //inline void getOutputs(float* aLeft, float* aRight) { *aLeft=audioout->getInput(0); *aRight=audioout->getInput(1);}
//    inline void getOutputs(float* aLeft, float* aRight) { *aLeft=audioout->readInput(0); *aRight=audioout->readInput(1);}
//};
//
////----------------------------------------------------------------------
////
//// editor
////
////----------------------------------------------------------------------
//
//class myEditor: public axEditor
//{
//  private:
//    wdgPanel*     wPanel;
//    wdgImgKnob2*  wKnob;
//
//  public:
//    myEditor(axString aWinName, axFormat* aPlugin, int aID, axRect aRect, int aWinFlags, int aParent)
//    : axEditor(aWinName,aPlugin,aID,aRect,aWinFlags, aParent)
//      { }
//
//    inline void setup(axSurface* srf)
//      {
//        appendWidget( wPanel = new wdgPanel(this,-1,NULL_RECT,wal_Client) );
//        wPanel->appendWidget( wKnob = new wdgImgKnob2(this,0,axRect(10,10,32,64),wal_None,65,srf) );
//        connect( wKnob, getPlugin()->getParamPtr(0) );
//        doRealign();
//      }
//};
//
////----------------------------------------------------------------------
////
//// plugin
////
////----------------------------------------------------------------------
//
//class myPlugin : public axEffect,
//                 public axModuleListener
//{
//  private:
//    myGraph*      mGraph;
//    parFloat*     pKnob;
//    bool          mGuiInit;
//    axSurface*    srfKnob;
//
//  public:
//
//    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
//    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
//      {
//        mGuiInit = false;
//        describe("test_graph","ccernn","ccernn.audio",0001,AX_MAGIC);
//        setupAudio(2,2,false);
//        setupEditor(320,240);
//          mGraph = new myGraph(this,"graph");
//          mGraph->doCompile();
//        appendParameter( pKnob = new parFloat(this,0,"gain","",0) );
//        processParameters();
//      }
//
//    virtual ~myPlugin()
//      {
//        delete mGraph;
//        if (mGuiInit) delete srfKnob;
//      }
//
//    virtual void* doCreateEditor(int aParent)
//      {
//        if (!mGuiInit)
//        {
//          srfKnob = loadPng((unsigned char*)knob32,knob32_size);
//          mGuiInit = true;
//        }
//        myEditor* ed;
//        ed = new myEditor("test_graph_editor",this,-1,axRect(0,0,mWidth,mHeight),AX_FLAGS,aParent);
//        ed->setup(srfKnob);
//        mEditor = ed;
//        return mEditor;
//      }
//
//    virtual void doProcessParameter(axParameter* aParameter)
//      {
//        if (aParameter->mID==0) mGraph->setParam( 0, aParameter->getValue() );
//      }
//
//    virtual void doProcessSample(float** ins, float** outs)
//      {
//        mGraph->setInputs( *ins[0], *ins[1] );
//        mGraph->doExecute();
//        mGraph->getOutputs( outs[0], outs[1] );
//      }
//
//};
//
////----------------------------------------------------------------------
//#include "axMain.h"
