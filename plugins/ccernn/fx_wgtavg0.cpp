#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2
#define AX_WIDTH      300
#define AX_HEIGHT     70

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
#include "wdgValue.h"
#include "wdgSwitch.h"

//----------------------------------------------------------------------

char* flt_txt[] = { (char*)"LP", (char*)"HP" };

//----------

class myPlugin : public axPlugin
{
  public:
    float       z0,z1;
    float       weight;
    int         mode;

    parInteger  *pMode;
    parFloat    *pWeight;

    axEditor    *mEditor;
    wdgSwitch   *wMode;
    wdgValue    *wWeight;


  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("fx_wgtavg0","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);

        appendParameter( pMode   = new parInteger(this, 0, "mode",   "", 0,  0,1, flt_txt ) );
        appendParameter( pWeight = new parFloat(  this, 1, "weight", "", 0.5 ) );
        //pMode->setup(0,0,1,flt_txt);
        //mode   = 0;
        //pMode->setInt(0);
        //pWeight->setValue(0.5);
        processParameters();
        z0=z1=0;
      }

    //virtual ~myPlugin() {}

    //--------------------------------------------------

    // parameters have mOwner = this
    // we override axPlugin.onChange, so we need to call doProcessParameter ourselves
    // (if we need it)
    virtual void onChange(axParameter* aParameter)
      {
        //TODO: better/safer "editor is visible" testing?
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor( "fx_wgtavg_gui", this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
        E->appendWidget( wMode   = new wdgSwitch( E, 0, axRect(10,10,50,20), wal_None/*, pMode*/ ) );
        E->appendWidget( wWeight = new wdgValue(  E, 1, axRect(10,40,50,20), wal_None/*, pWeight*/ ) );
        //wMode->setup("off","on");
        wMode->setup(flt_txt);
        //E->updateWidgetValues(); // connect by wdg/par.mID !!
        E->connect( wMode, pMode );
        E->connect( wWeight, pWeight );
        mEditor = E;
        //TRACE("fx_wgtavg.mEditor = %x\n",(int)mEditor);
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
      }

    virtual void doIdleEditor(void)
      {
        if (mEditor) mEditor->redrawDirty();
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            mode = (int)f;
            break;
          case 1:
            weight = f*f*f;
            break;
        }
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        z0 += ( ( *ins[0] - z0 ) * weight );
        z1 += ( ( *ins[1] - z1 ) * weight );
        switch(mode)
        {
          case 0: // lowpass
            *outs[0] = z0;
            *outs[1] = z1;
            break;
          case 1: // highpass
            *outs[0] = *ins[0] - z0;
            *outs[1] = *ins[1] - z1;
            break;
        }
      }

};

//----------------------------------------------------------------------
#include "axMain.h"