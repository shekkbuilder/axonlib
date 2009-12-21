#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
#define AX_WIDTH      640
#define AX_HEIGHT     480
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_AUTOSYNC

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"

#include "axEditor.h"
#include "wdgTabs.h"
#include "wdgSwitches.h"
#include "wdgKnob.h"
#include "wdgKnobPar.h"

char* str_tabs[] =
{
  (char*)"page1",
  (char*)"page2",
  (char*)"page3"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    axEditor*     mEditor;
    wdgTabs*      mTabs;
    axContainer*  tab1;
    axContainer*  tab2;
    axContainer*  tab3;
    //wdgSwitches*  mSwitches;
    float value;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("ana_info0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter(new parFloat(this,0,"value","",0));
        processParameters();
      }

    //----------

    //virtual ~myPlugin()
    //  {
    //  }

    //----------

    //if has editor
    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor(this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);

        E->appendWidget(new wdgKnobPar(E,0,axRect(10,10,100,32),wal_Stacked,mParameters[0]));
        E->appendWidget(  mTabs = new wdgTabs(       E,-1, axRect(10,50,500,300), wal_None ) );

        mTabs->appendPage( tab1 = new axContainer( mTabs,-1, axRect(0,0,500,280), wal_None ) );
        mTabs->appendPage( tab2 = new axContainer( mTabs,-1, axRect(0,0,500,280), wal_None ) );
        mTabs->appendPage( tab3 = new axContainer( mTabs,-1, axRect(0,0,500,280), wal_None ) );

        mTabs->setup(3,str_tabs);
        tab1->setBackground( true, AX_RED_DARK );
        tab2->setBackground( true, AX_GREEN_DARK );
        tab3->setBackground( true, AX_BLUE_DARK );
        mTabs->setPage(0,false);

        tab2->setLayout(10,10,10,10);
        tab2->setFlag(wfl_Vertical);
        tab2->appendWidget(new wdgKnob(tab2,-1,axRect(0,0,128,32),wal_Stacked));
        tab2->appendWidget(new wdgKnob(tab2,-1,axRect(0,0,128,32),wal_Stacked));
        tab2->appendWidget(new wdgKnob(tab2,-1,axRect(0,0,128,32),wal_Stacked));
        tab2->appendWidget(new wdgKnob(tab2,-1,axRect(0,0,128,32),wal_Stacked));
        tab2->appendWidget(new wdgKnob(tab2,-1,axRect(0,0,128,32),wal_Stacked));
        tab2->doRealign();

        E->updateWidgetValues();
        mEditor = E;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* tempeditor = mEditor;
        mEditor = NULL;
        delete tempeditor;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        mEditor->redrawDirty();
      }

    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: value = f;  break;
        }
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
