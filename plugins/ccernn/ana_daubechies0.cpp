#define AX_PLUGIN     myEffect
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  0
#define AX_WIDTH      286
#define AX_HEIGHT     280
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axEffect.h"
#include "parFloat.h"
#include "parInteger.h"
#include "wdgPanel.h"
#include "wdgScope.h"
#include "../extern/Daubechies.h"

#define BUFFERSIZE 1024

//----------------------------------------------------------------------

class myEffect : public axEffect
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
    int           mIndex;
    float         mScale;
    int           mLength;
    parFloat*     pScale;
    parInteger*   pLength;
    wdgScope*      wScope;
    Daubechies daub;

  public:

    myEffect(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        setupEditor(AX_WIDTH,AX_HEIGHT);
        setupAudio(2,2);
        describe("ana_sigview0","ccernn","product_string",0, AX_MAGIC+1001);
        daub.setSize(BUFFERSIZE);
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
        //appendParameter( pScale  = new parFloat(  this, 0, "scale", "",   1,   0,  4 ) );
        //appendParameter( pLength = new parInteger(this, 1, "length","ms", 250, 10, 1000 ) );
        //processParameters();
        appendParameter( new axParameter(this,0,"dummy") );
      }

    virtual ~myEffect()
      {
        delete[] mBuffer1;
        delete[] mBuffer2;
        delete[] mViewBuffer;
      }

    //----------------------------------------------------------------------
    // plugin
    //----------------------------------------------------------------------

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
    // editor
    //----------------------------------------------------------------------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* panel;
        aEditor->appendWidget( panel = new wdgPanel(this,-1,NULL_RECT,wal_Client));
        panel->appendWidget( wScope = new wdgScope(this,3,axRect( 10,70,266,200),wal_None) );
          wScope->mSize = mLength;
          wScope->mBuffer = mViewBuffer;
        aEditor->doRealign();
      }

    //----------

    virtual void do_IdleEditor(void)
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
        }
        //axEffect::doIdleEditor();
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
