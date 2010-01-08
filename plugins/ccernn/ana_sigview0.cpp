#define AX_DEBUG
#include "axDebug.h"

#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2

#define AX_WIDTH      286
#define AX_HEIGHT     280
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

#include "axEditor.h"
#include "wdgKnob.h"
#include "wdgSwitches.h"
#include "wdgScope.h"

//#include "wdgResizer.h"

//----------------------------------------------------------------------

#define MAX_SAMPLE_RATE 192000
#define MAX_BUFFER_SIZE (MAX_SAMPLE_RATE*2)

char* str_viewmode[] = { (char*)"left", (char*)"right" };

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
    wdgKnob*      wScale;
    wdgKnob*      wLength;
    wdgSwitches*  wViewMode;
    wdgScope*      wScope;
    //wdgResizer*   wSizer;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("ana_sigview0","ccernn","product_string",0, AX_MAGIC+1001);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        mBuffer1 = new float[MAX_BUFFER_SIZE];
        mBuffer2 = new float[MAX_BUFFER_SIZE];
        mViewBuffer = new float[MAX_BUFFER_SIZE];
        memset(mBuffer1,0,MAX_BUFFER_SIZE*sizeof(float));
        memset(mBuffer2,0,MAX_BUFFER_SIZE*sizeof(float));
        memset(mViewBuffer,0,MAX_BUFFER_SIZE*sizeof(float));
        mIndex = 0;
        mRecBuffer = mBuffer1;
        mRec2Buffer = mBuffer2;
        mFilled = false;

        appendParameter( pScale  = new parFloat(  this, 0, "scale", "",   1,   0,  4 ) );
        appendParameter( pLength = new parInteger(this, 1, "length","ms", 250, 10, 1000 ) );
        processParameters();
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

    virtual void doProcessState(int aState)
      {
        //TRACE("DoProcessState: %i\n",aState);
        //mIndex = 0;
      }

    //----------

    virtual void doProcessTransport(int aState)
      {
        //TRACE("doProcessTransport: %i\n",aState);
        //mIndex = 0;
      }

    //----------

    #define ms (updateSampleRate()*0.001)

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id  = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            mScale = f;
            break;
          case 1:
            mLength = f*ms;
            mIndex=0;
            mFilled=false;
            if (mEditor) wScope->mSize = mLength;
            break;
        }
      }

    #undef ms

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

          assert(mIndex>=0);
          assert(mIndex<mLength);

        mRecBuffer[mIndex*2]   = spl0;
        mRecBuffer[mIndex*2+1] = spl1;
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

    // we need this one because we have to notify the editor
    // that something has changed.
    virtual void onChange(axParameter* aParameter)
      {
        if (mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //----------

    // this one is called when any widget changes its internal value
    // check if it's the left/right selector widget, and if so,
    // 'do the right thing'
    virtual void onChange(axWidget* aWidget)
      {
        switch(aWidget->mID)
        {
          case 2: wScope->mMode = wViewMode->mSelected; break;
        }
        mEditor->onRedraw(aWidget);
      }

    //----------

    virtual void onResize(int dX, int dY)   // delta x,y
      {
        int w = wScope->mRect.w + dX;
        int h = wScope->mRect.h + dY;
        wScope->doResize( w,h );
        mEditor->onResize(dX,dY);
      }

    //----------------------------------------------------------------------
    // editor
    //----------------------------------------------------------------------

    // can this one die because of some threading issues? (ala close editor) think,think..
    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("ana_sigview_editor",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        E->appendWidget( wScale     = new wdgKnob(    E,   0,axRect( 10,10,128, 32),wal_None/*, pScale */ ) );
        E->appendWidget( wLength    = new wdgKnob(    E,   1,axRect(140,10,128, 32),wal_None/*, pLength*/ ) );
        E->appendWidget( wViewMode  = new wdgSwitches(this,2,axRect( 10,50,266, 16),wal_None ) );
        E->appendWidget( wScope     = new wdgScope(   this,3,axRect( 10,70,266,200),wal_None ) );
          wViewMode->setup(2,str_viewmode);
          wScope->mSize = mLength;
          wScope->mBuffer = mViewBuffer;
          wScope->mDrawFlags = wbf_Wave | wbf_Slices;
          wScope->mWaveColor = AX_YELLOW;
          wScope->mNumSlices = 8;
          wScope->mSlicesColor = AX_WHITE;

        //E->appendWidget( wSizer = new wdgResizer( this,99,axRect(276,270,10,10),wal_None ) );

        E->connect( wScale, pScale  );
        E->connect( wLength,pLength );
        // could mEditor be in use from another thread? gui? audio? setParameter?
        // it shouldn't (we are creating the editor), but, ...
        mEditor = E;
        return E;
      }

    //----------

    // see below (idle)
    virtual void doDestroyEditor(void)
      {
        axEditor* E = mEditor;
        mEditor = NULL;
        delete E;
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
            memcpy(mViewBuffer,mRec2Buffer,mLength*2*sizeof(float));
            mFilled=false;
            mEditor->appendDirty( wScope );
          }
          mEditor->redrawDirty();
        }
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
