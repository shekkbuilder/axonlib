
#define AX_DEBUG
#include "axDebug.h"

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  0

#define AX_WIDTH      286
#define AX_HEIGHT     280
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
//#include "wdgKnob.h"
//#include "wdgSwitches.h"
#include "wdgPanel.h"
#include "wdgScope.h"

//#include "wdgResizer.h"

#include "../extern/Daubechies.h"

//----------------------------------------------------------------------

#define BUFFERSIZE 1024

//----------------------------------------------------------------------

class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
    float*        mBuffer1;
    float*        mBuffer2;
    float*        mRecBuffer;
    float*        mRec2Buffer;
    float*        mViewBuffer;
    bool          mFilled;
    int           mViewMode;
  protected:
    axEditor*     mEditor;
    int           mIndex;
    float         mScale;
    int           mLength;
    parFloat*     pScale;
    parInteger*   pLength;
//    wdgKnob*      wScale;
//    wdgKnob*      wLength;
//    wdgSwitches*  wViewMode;
    wdgScope*      wScope;
    //wdgResizer*   wSizer;

    Daubechies daub;

  public:

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        daub.setSize(BUFFERSIZE);
        mEditor = NULL;
        describe("ana_sigview0","ccernn","product_string",0, AX_MAGIC+1001);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        mBuffer1 = new float[BUFFERSIZE];
        mBuffer2 = new float[BUFFERSIZE];
        mViewBuffer = new float[BUFFERSIZE];
        memset(mBuffer1,0,BUFFERSIZE*sizeof(float));
        memset(mBuffer2,0,BUFFERSIZE*sizeof(float));
        memset(mViewBuffer,0,BUFFERSIZE*sizeof(float));
        mIndex = 0;
        mRecBuffer = mBuffer1;
        mRec2Buffer = mBuffer2;
        mFilled = false;

        mScale = 1;
        mLength = BUFFERSIZE;


//        appendParameter( pScale  = new parFloat(  this, 0, "scale", "",   1,   0,  4 ) );
//        appendParameter( pLength = new parInteger(this, 1, "length","ms", 250, 10, 1000 ) );
//        processParameters();
      }

    //parameters is automatically deleted
    virtual ~myPlugin()
      {
        delete[] mBuffer1;
        delete[] mBuffer2;
        delete[] mViewBuffer;
      }

    //----------------------------------------------------------------------
    // plugin
    //----------------------------------------------------------------------

//    virtual void doProcessState(int aState)
//      {
//        //TRACE("DoProcessState: %i\n",aState);
//        //mIndex = 0;
//      }

    //----------

//    virtual void doProcessTransport(int aState)
//      {
//        //TRACE("doProcessTransport: %i\n",aState);
//        //mIndex = 0;
//      }

    //----------

//    #define ms (updateSampleRate()*0.001)
//
//    virtual void doProcessParameter(axParameter* aParameter)
//      {
//        int id  = aParameter->mID;
//        float f = aParameter->getValue();
//        switch(id)
//        {
//          case 0:
//            mScale = f;
//            break;
//          case 1:
//            mLength = f*ms;
//            mIndex=0;
//            mFilled=false;
//            if (mEditor) wScope->mSize = mLength;
//            break;
//        }
//      }
//
//    #undef ms

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
        float spl0 = *ins[0];
        float spl1 = *ins[1];

          //assert(mIndex>=0);
          //assert(mIndex<mLength);

        mRecBuffer[mIndex]   = (spl0+spl1) * 0.5;
        mIndex += 1;
        if (mIndex>=mLength)
        {
          mIndex=0;
          // switch buffers
          float* temp = mRecBuffer;
          mRecBuffer = mRec2Buffer;
          mRec2Buffer = temp;
          //signal redraw
          if( mFilled==false ) mFilled=true;
        }

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------------------------------------------------------------------
    // listeners
    //----------------------------------------------------------------------

//    // we need this one because we have to notify the editor
//    // that something has changed.
//    virtual void onChange(axParameter* aParameter)
//      {
//        if (mEditor) mEditor->onChange(aParameter);
//        doProcessParameter(aParameter);
//      }

    //----------

//    // this one is called when any widget changes its internal value
//    // check if it's the left/right selector widget, and if so,
//    // 'do the right thing'
//    virtual void onChange(axWidget* aWidget)
//      {
//        switch(aWidget->mID)
//        {
//          case 2: wScope->mMode = wViewMode->mSelected; break;
//        }
//        mEditor->onRedraw(aWidget);
//      }

    //----------

//    virtual void onResize(int dX, int dY)   // delta x,y
//      {
//        int w = wScope->mRect.w + dX;
//        int h = wScope->mRect.h + dY;
//        wScope->doResize( w,h );
//        mEditor->onResize(dX,dY);
//      }

    //----------------------------------------------------------------------
    // editor
    //----------------------------------------------------------------------

    // can this one die because of some threading issues? (ala close editor) think,think..
    virtual void* doCreateEditor(void)
      {
        axEditor* EDIT = new axEditor("ana_sigview_editor",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        wdgPanel* panel;
        EDIT->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client));

        panel->appendWidget( wScope = new wdgScope(this,3,axRect( 10,70,266,200),wal_None) );
          //wViewMode->setup(2,str_viewmode);
          wScope->mSize = mLength;
          wScope->mBuffer = mViewBuffer;
          wScope->mDrawFlags = wbf_Wave;// | wbf_Slices;
          //wScope->mWaveColor = AX_YELLOW;
          //wScope->mNumSlices = 8;
          //wScope->mSlicesColor = AX_WHITE;

        //E->appendWidget( wSizer = new wdgResizer( this,99,axRect(276,270,10,10),wal_None ) );

//        E->connect( wScale, pScale  );
//        E->connect( wLength,pLength );
        // could mEditor be in use from another thread? gui? audio? setParameter?
        // it shouldn't (we are creating the editor), but, ...
        EDIT->doRealign();
        mEditor = EDIT;
        return EDIT;
      }

    //----------

    // see below (idle)
    virtual void doDestroyEditor(void)
      {
        axEditor* EDIT = mEditor;
        mEditor = NULL;
        delete EDIT;
      }

    //----------

    // let's say this has just been called from the gui thread to update the interface
    // immediately after this, you close the editor (doDestroyEditor above)
    // it would be no good to delete the mEditor class while we're still drawing
    // so, some locking is probablynecessary
    virtual void doIdleEditor(void)
      {
        if (mEditor)
        {
          if (mFilled)
          {
            memcpy(mViewBuffer,mRec2Buffer,mLength*sizeof(float));
            daub.daubTrans( mViewBuffer, BUFFERSIZE );
            mFilled=false;
            mEditor->appendDirty( wScope );
          }
          mEditor->redrawDirty();
        }
      }

    //----------------------------------------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //    if (mEditor) mEditor->onChange(aParameter);
    //    doProcessParameter(aParameter);
    //  }

    //----------

    //virtual void onChange(axWidget* aWidget)
    //  {
    //    if (mEditor) mEditor->onChange(aWidget);
    //  }


};

//----------------------------------------------------------------------
#include "axMain.h"
