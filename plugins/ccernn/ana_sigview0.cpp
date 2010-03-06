//#define AX_DEBUG
//#include "axDebug.h"

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
#include "wdgPanel.h"

//#include "wdgResizer.h"

//----------------------------------------------------------------------

#define MAX_SAMPLE_RATE 192000
#define MAX_BUFFER_SIZE (MAX_SAMPLE_RATE*2)

#define DOUBLEBUFFER

//----------------------------------------------------------------------
//
// scope
//
//----------------------------------------------------------------------

class myScope : public wdgScope
{
  private:
    axBrush* mBackBrush;
  public:
    myScope(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    : wdgScope(aListener,aID,aRect,aAlignment)
      {
        mBackBrush = new axBrush(AX_GREY_DARK);
        setBackgroundBrush(mBackBrush);
      }
    virtual ~myScope()
      {
        delete mBackBrush;
      }
    //virtual void do_PrePaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //    aCanvas->selectBrush(mBackBrush);
    //    aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );
    //  }
    //virtual void do_PostPaint(axCanvas* aCanvas, axRect aRect)
    //  {
    //  }
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

char* str_viewmode[] = { (char*)"left", (char*)"right" };

//----------


class myPlugin : public axPlugin,
                 public axWidgetListener
{
  private:
    int           mIndex;
    float*        mBuffer1;
    float*        mRecBuffer;
    #ifdef DOUBLEBUFFER
    float*        mBuffer2;
    float*        mRec2Buffer;
    float*        mViewBuffer;
    bool          mFilled;
    #endif
    //int           mViewMode;
  protected:
    axEditor*     mEditor;

    float         mScale;
    int           mLength;

    parFloat*     pScale;
    parInteger*   pLength;

    wdgKnob*      wScale;
    wdgKnob*      wLength;

    //wdgSwitches*  wViewMode;
    myScope*      wScope;

  public:

    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        describe("ana_sigview0","ccernn","product_string",0, AX_MAGIC+1001);
        setupAudio(2,2);
        setupEditor(AX_WIDTH,AX_HEIGHT);
        mBuffer1 = new float[MAX_BUFFER_SIZE];
        memset(mBuffer1,0,MAX_BUFFER_SIZE*sizeof(float));
        mRecBuffer = mBuffer1;
        mIndex = 0;
        #ifdef DOUBLEBUFFER
        mBuffer2 = new float[MAX_BUFFER_SIZE];
        memset(mBuffer2,0,MAX_BUFFER_SIZE*sizeof(float));
        mViewBuffer = new float[MAX_BUFFER_SIZE];
        memset(mViewBuffer,0,MAX_BUFFER_SIZE*sizeof(float));
        mRec2Buffer = mBuffer2;
        mFilled = false;
        #endif
        appendParameter( pScale  = new parFloat(  this, 0, "scale", "",   1,   0,  4 ) );
        appendParameter( pLength = new parInteger(this, 1, "length","ms", 250, 10, 1000 ) );
        processParameters();
      }

    //parameters is automatically deleted
    virtual ~myPlugin()
      {
        delete[] mBuffer1;
        #ifdef DOUBLEBUFFER
        delete[] mBuffer2;
        delete[] mViewBuffer;
        #endif
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
            #ifdef DOUBLEBUFFER
            mFilled=false;
            #endif
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

    // we fill the buffer that mRecBuffer is pointing to, until it's full (mLength)
    // then swap the pointers around, so we write to the other buffer

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        mRecBuffer[mIndex*2]   = spl0;
        mRecBuffer[mIndex*2+1] = spl1;
        mIndex += 1;
        if (mIndex>=mLength)
        {
          mIndex=0;
          #ifdef DOUBLEBUFFER
          float* temp = mRec2Buffer;
          mRec2Buffer = mRecBuffer;
          mRecBuffer = temp;
          if( mFilled==false ) mFilled=true;
          #endif
        }
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------------------------------------------------------------------
    // editor
    //----------------------------------------------------------------------

    virtual void* doCreateEditor(void)
      {
        axEditor* EDIT = new axEditor("ana_sigview_editor",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        wdgPanel* panel;
        EDIT->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client));

        panel->appendWidget( wScale     = new wdgKnob(    this,0,axRect( 10,10,128, 32),wal_None/*, pScale */ ) );
        panel->appendWidget( wLength    = new wdgKnob(    this,1,axRect(140,10,128, 32),wal_None/*, pLength*/ ) );
        //panel->appendWidget( wViewMode  = new wdgSwitches(this,2,axRect( 10,50,266, 16),wal_None ) );
        panel->appendWidget( wScope     = new myScope(    this,3,axRect( 10,70,266,200),wal_None ) );
          //wViewMode->setup(2,str_viewmode);
          wScope->mSize = mLength;
          #ifdef DOUBLEBUFFER
          wScope->mBuffer = mViewBuffer;
          #else
          wScope->mBuffer = mRecBuffer;
          #endif
          //wScope->mDrawFlags = wbf_Wave;// | wbf_Slices;
          //wScope->mNumSlices = 8;
        EDIT->connect( wScale, pScale  );
        EDIT->connect( wLength,pLength );
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

    // when mFilled is true, it means that mRec2Buffer points to the buffer that has just been filled
    // copy that to our viewbuffer, and signal 'empty buffer'
    // there is some extra buffering involved in all this, but i think it's safer...

    virtual void doIdleEditor(void)
      {
        if (mEditor)
        {
          #ifdef DOUBLEBUFFER
          if (mFilled)
          {
            memcpy(mViewBuffer,mRec2Buffer,mLength*2*sizeof(float));
            mFilled=false;
            mEditor->appendDirty( wScope );
          }
          #else
          wScope->setScale(mScale);
          mEditor->appendDirty( wScope );
          #endif
          mEditor->redrawDirty();
        }
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
        //switch(aWidget->mID)
        //{
        //  case 2: wScope->mMode = wViewMode->mSelected; break;
        //}
        //mEditor->onRedraw(aWidget);
        mEditor->onChange(aWidget);
      }

    //----------

    virtual void onResize(int dX, int dY)   // delta x,y
      {
        int w = wScope->getRect().w + dX;
        int h = wScope->getRect().h + dY;
        wScope->doResize( w,h );
        mEditor->onResize(wScope,dX,dY);
      }



};

#undef DOUBLEBUFFER


//----------------------------------------------------------------------
#include "axMain.h"
