#define AGAIN_NO_GUI
//#define AGAIN_GUI

//----------------------------------------

// without gui: 31 lines
#ifdef AGAIN_NO_GUI

//#define AX_DEBUG
//#include "axDebug.h"
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  1
#include "axPlugin.h"
#include "parFloat.h"

class myPlugin : public axPlugin
{
  private:
    float gain;
    float buf[1024];
  public:
    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("again","ccernn","no_product_string",0001,0x666);
        appendParameter(new parFloat(this,0,"gain","",1,0,2,0.1));
        processParameters();
      }
    virtual void doProcessParameter(axParameter* aParameter)
      {
        if (aParameter->mID==0) gain = aParameter->getValue();
      }
    //virtual bool doProcessBlock(float** ins, float** outs, int sampleFrames)
    //  {
    //    return false;
    //  }
    virtual void doProcessSample(float** ins, float** outs)
      {
        *outs[0] = *ins[0] * gain;
        *outs[1] = *ins[1] * gain;
      }
};
#include "axMain.h"

#endif

//----------------------------------------

// with gui: 79 lines
#ifdef AGAIN_GUI

//#define AX_DEBUG
//#include "axDebug.h"
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  1
// next 3: gui
#define AX_WIDTH      52
#define AX_HEIGHT     72
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#include "axPlugin.h"
#include "parFloat.h"
#include "axEditor.h"
#include "wdgImgKnob.h"
#include "wdgLabel.h"
#include "axBitmapLoader.h"
#include "images/knob2.h"
class myPlugin : public axPlugin
{
  private:
    float       gain;
    axEditor    *mEditor;
    bool        is_gui_initialized;
    axSurface   *srfKnob;
    wdgImgKnob  *wGain;
  public:
    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        srfKnob = NULL;
        is_gui_initialized = false;
        hasEditor(AX_WIDTH,AX_HEIGHT);
        describe("again_gui","ccernnb","no_product_string",0001,0x666);
        appendParameter(new parFloat(this,0,"gain","",1,0,2,0.1));
        processParameters();
      }
    virtual ~myPlugin()
      {
        if (srfKnob) delete srfKnob;
      }
    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }
    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("again_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        if(!is_gui_initialized)
        {
          srfKnob = loadPng( knob2, 15255 );
          is_gui_initialized=true;
        }
        E->appendWidget(new wdgImgKnob(E, 0,axRect(10,30,32,32),wal_None,65,srfKnob));
        E->appendWidget(new wdgLabel(  E,-1,axRect(10,10,32,16),wal_None,"gain",AX_GREY_LIGHT,tal_Center));
        for (int i=0; i<AX_NUMPARAMS; i++) E->connect(E->mWidgets[i],mParameters[i]);
        mEditor = E;
        return mEditor;
      }
    virtual void doDestroyEditor(void)
      {
        axEditor* tempeditor = mEditor;
        mEditor = NULL;
        delete tempeditor;
      }
    virtual void doIdleEditor(void)
      {
        mEditor->redrawDirty();
      }
    virtual void doProcessParameter(axParameter* aParameter)
      {
        if (aParameter->mID==0) gain = aParameter->getValue();
      }
    virtual void doProcessSample(float** ins, float** outs)
      {
        *outs[0] = *ins[0] * gain;
        *outs[1] = *ins[1] * gain;
      }
};
#include "axMain.h"

#endif
