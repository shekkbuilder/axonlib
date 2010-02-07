#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
//#define AX_AUTOSYNC
#define AX_WIDTH      150
#define AX_HEIGHT     190
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axMath.h"
#include "axPlugin.h"
#include "parFloat.h"
#include "axEditor.h"
#include "wdgKnob.h"
#include "wdgVuMeter.h"
#include "wdgPanel.h"

#include "parInteger.h"
#include "wdgImgSwitch.h"
#include "axSurface.h"
#include "axBitmapLoader.h"
#include "images/testbutton.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  public:
    bool        editor_initialized;
    axEditor*   mEditor;
    wdgVuMeter* wMeter;
    float       left,right;
    wdgKnob*    wSpeed;
    parFloat*   pSpeed;
    float       speed;

    parInteger*   psw;
    wdgImgSwitch* sw;
    axSurface*    swSrf;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        editor_initialized = false;
        mEditor = NULL;
        describe("ana_vu0","ccernn","product_string",0,0);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        left = 0;
        right = 0;
        appendParameter( pSpeed = new parFloat(  this,0,"speed","", 20, 1,50 ) );
        appendParameter( psw    = new parInteger(this,1,"psw",  "", 0,  0,1  ) );
        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //--------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        if(!editor_initialized)
        {
          unsigned char* buffer = testbutton;
          unsigned int   size = 10935;
          swSrf = loadPng(buffer,size);
          editor_initialized = true;
        }
        axEditor* EDIT = new axEditor("ana_vu_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        wdgPanel* panel;
        EDIT->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client));

        panel->appendWidget( wSpeed = new wdgKnob(     this,0,axRect(10, 10, 128,32 ), wal_None  ));
        panel->appendWidget( sw     = new wdgImgSwitch(this,1,axRect(100,100,30, 30 ), wal_None, 0, swSrf ));
        panel->appendWidget( wMeter = new wdgVuMeter(  this,2,axRect(10, 50, 10, 128), wal_None  ));
        EDIT->connect( wSpeed, pSpeed );
        EDIT->connect( sw, psw );
        left = 0;
        right = 0;
        EDIT->doRealign();
        mEditor = EDIT;
        return mEditor;
      }

    //----------

    virtual void doDestroyEditor(void)
      {
        axEditor* EDIT = mEditor;
        mEditor = NULL;
        delete EDIT;
      }

    //----------

    virtual void doIdleEditor(void)
      {
        if (mEditor)
        {
          float val = (left + right) * 0.5;
          wMeter->mLevel = val;
          mEditor->appendDirty(wMeter);
          mEditor->redrawDirty();
        }
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: speed = 1/(f*f*f); break;
          //case 1: TRACE("...\n"); break;
        }
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
          float l = fabs(spl0);
          if (l>left) left=l;
          else left  += ( 0 - left  ) * speed;
          float r = fabs(spl1);
          if (r>right) right=l;
          else right  += ( 0 - right  ) * speed;
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if (mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------

    virtual void onChange(axWidget* aWidget)
      {
        if (mEditor) mEditor->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
