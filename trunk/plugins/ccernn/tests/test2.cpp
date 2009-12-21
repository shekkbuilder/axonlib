#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  4
#define AX_WIDTH      320
#define AX_HEIGHT     240

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"

#include "parFloat.h"
#include "parInteger.h"

#include "wdgValuePar.h"
#include "wdgValue.h"

#include "wdgLabel.h"

//----------------------------------------------------------------------

//TODO: widget <-> parameters via mUser ptr

// should we protect this in some way?
// we only read from it (runtime)
char* str_filter[] =
{
  (char*)"off",
  (char*)"lowpass",
  (char*)"highpass",
  (char*)"bandpass",
  (char*)"notch"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    char buf[257];
  public:
    axEditor*   mEditor;
    parFloat    *pFloat1;
    parFloat    *pFloat2;
    parInteger  *pInt1;
    parInteger  *pInt2;

    wdgValuePar *w1,*w2,*w3,*w4;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("test2","ccernn","product_string",0,0);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter( pFloat1 = new parFloat(  this,0,"float1", "",  1     ) );
        appendParameter( pFloat2 = new parFloat(  this,1,"float1", "", -0.5, -2,5, 0.5 ) );
        appendParameter( pInt1   = new parInteger(this,2,"int1",   "",  24,   15,  31  ) );
        appendParameter( pInt2   = new parInteger(this,3,"int2",   "",  1,    0,   4,  str_filter ) );

        buf[0] = '\0';
        // reaper vst extensions
        void (*GetProjectPath)(char *buf, int bufsz);
        *(long*)&GetProjectPath = audioMaster(NULL,0xdeadbeef,0xdeadf00d,0,(char*)"GetProjectPath",0.0);
        if( GetProjectPath ) GetProjectPath(buf,256);
        TRACE("Project Path: %s\n",buf);

        //hostCanDo
        int res;
        res = canHostDo((char*)"sendVstEvents");                  TRACE("canHostDo sendVstEvents                  = %i\n",res);
        res = canHostDo((char*)"sendVstMidiEvent");               TRACE("canHostDo sendVstMidiEvent               = %i\n",res);
        res = canHostDo((char*)"sendVstTimeInfo");                TRACE("canHostDo sendVstTimeInfo                = %i\n",res);
        res = canHostDo((char*)"receiveVstEvents");               TRACE("canHostDo receiveVstEvents               = %i\n",res);
        res = canHostDo((char*)"receiveVstMidiEvent");            TRACE("canHostDo receiveVstMidiEvent            = %i\n",res);
        res = canHostDo((char*)"reportConnectionChanges");        TRACE("canHostDo reportConnectionChanges        = %i\n",res);
        res = canHostDo((char*)"acceptIOChanges");                TRACE("canHostDo acceptIOChanges                = %i\n",res);
        res = canHostDo((char*)"sizeWindow");                     TRACE("canHostDo sizeWindow                     = %i\n",res);
        res = canHostDo((char*)"offline");                        TRACE("canHostDo offline                        = %i\n",res);
        res = canHostDo((char*)"openFileSelector");               TRACE("canHostDo openFileSelector               = %i\n",res);
        res = canHostDo((char*)"closeFileSelector");              TRACE("canHostDo closeFileSelector              = %i\n",res);
        res = canHostDo((char*)"startStopProcess");               TRACE("canHostDo startStopProcess               = %i\n",res);
        res = canHostDo((char*)"shellCategory");                  TRACE("canHostDo shellCategory                  = %i\n",res);
        res = canHostDo((char*)"sendVstMidiEventFlagIsRealtime"); TRACE("canHostDo sendVstMidiEventFlagIsRealtime = %i\n",res);

      }

    //parameters is automatically deleted
    //virtual ~myPlugin() {}

    virtual void onChange(axParameter* aParameter)
      {
        if( mEditor ) mEditor->onChange( aParameter );
        doProcessParameter(aParameter);
      }
      // or we could set parameter.owner to Editor when opening editor

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* ED = new axEditor( this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
        ED->setFlag( wfl_Vertical );
        ED->setLayout( 10,10,5,5 );
        ED->appendWidget( w1 = new wdgValuePar( ED,0, axRect(0,0,80,20), wal_Stacked, pFloat1 ) );
        ED->appendWidget( w2 = new wdgValuePar( ED,1, axRect(0,0,80,20), wal_Stacked, pFloat2 ) );
        ED->appendWidget( w3 = new wdgValuePar( ED,2, axRect(0,0,80,20), wal_Stacked, pInt1 ) );
        ED->appendWidget( w4 = new wdgValuePar( ED,3, axRect(0,0,80,20), wal_Stacked, pInt2 ) );

        // append a few extra widgets for testing
        ED->appendWidget(      new wdgLabel   ( ED,-1,axRect(0,0,80,20), wal_Stacked, "label" ) );

        w1->doSetValue( pFloat1->doGetValue() );
        w2->doSetValue( pFloat2->doGetValue() );
        w3->doSetValue( pInt1->doGetValue() );
        w4->doSetValue( pInt2->doGetValue() );
        ED->doRealign();

        sizeWindow(100,100);

        mEditor = ED;
        return mEditor;
      }

    //----------

    //TODO: lock? what if someone else is using mEditor
    virtual void doDestroyEditor(void)
      {
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
      }

    //----------

    //virtual void doIdleEditor(void)
    //  {
    //    //TRACE("doIdleEditor\n");
    //  }

    //----------

    //virtual void doProcessState(int aState)
    //  {
    //    //TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessTransport(int aState)
    //  {
    //    //TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessParameter(axParameter* aParameter)
    //  {
    //    int id = aParameter->mID;
    //    //float f = aParameter->getValue();
    //    switch(id)
    //    {
    //      case 0:
    //        break;
    //    }
    //  }

    //----------

    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3)
    //  {
    //    TRACE("DoProcessMidi: %i - %i,%i,%i\n",osg,msg1,msg2,msg3);
    //  }

    //----------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        *outs[0] = *ins[0];
        *outs[1] = *ins[1];
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
