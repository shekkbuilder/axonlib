#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  5

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "dspSVF.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

char* str_mode[] =
{
  (char*)"bypass",
  (char*)"clip",
  (char*)"fold",
  (char*)"wrap+",
  (char*)"wrap",
  (char*)"wrap-"
};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    parFloat    *pPre;
    parInteger  *pMode;
    parFloat    *pThr;
    parFloat    *pFlt;
    parFloat    *pPost;
    int   mode;
    float thr,pre,post;
    float flt0, flt0_d;
    float flt1, flt1_d;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_dist0","ccernn","product_string",0,2);
        appendParameter( pPre  = new parFloat(    this,0,"pre gain", "", 1, 0,2 ) );
        appendParameter( pMode = new parInteger(  this,1,"mode",     "", 0, 0,5, str_mode ) );
        appendParameter( pThr  = new parFloat(    this,2,"threshold","", 1 ) );
        appendParameter( pFlt  = new parFloat(    this,3,"filter",   "", 1 ) );
        appendParameter( pPost = new parFloat(    this,4,"post gain","", 1, 0,2 ) );
        pre  = 1;
        mode = 0;
        thr  = 1;
        flt0 = flt0_d = 1;
        flt1 = flt1_d = 1;
        post = 1;
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: pre    = (f*f);   break;
          case 1: mode   = (int)f;  break;
          case 2: thr    = (f*f);   break;
          case 3: flt0_d = (f*f*f);
                  flt1_d = (f*f*f); break;
          case 4: post   = (f*f);   break;
        }
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float in0 = *ins[0] * pre;
        float in1 = *ins[1] * pre;
        float out0 = in0;
        float out1 = in1;
        float over,under;
        switch(mode)
        {
          case 1: // clip
            out0 = axMin(thr,axMax(-thr,in0));  // l
            out1 = axMin(thr,axMax(-thr,in1));  // r
            break;
          case 2: // fold
            if( in0> thr ) { over =in0-thr; out0= thr-over;  }  // l+
            if( in0<-thr ) { under=in0+thr; out0=-thr-under; }  // l+
            if( in1> thr ) { over =in1-thr; out1= thr-over;  }  // r+
            if( in1<-thr ) { under=in1+thr; out1=-thr-under; }  // r-
            break;
          case 3: // wrap+
            if( in0> thr ) { over =in0-thr; out0=-thr+over;  }
            if( in0<-thr ) { under=in0+thr; out0= thr+under; }
            if( in1> thr ) { over =in1-thr; out1=-thr+over;  }
            if( in1<-thr ) { under=in1+thr; out1= thr+under; }
            break;
          case 4: // wrap (flat)
            if( in0> thr ) { out0=-thr; }
            if( in0<-thr ) { out0= thr; }
            if( in1> thr ) { out1=-thr; }
            if( in1<-thr ) { out1= thr; }
            break;
          case 5: // wrap-
            if( in0> thr ) { over =in0-thr; out0=-thr-over;  }
            if( in0<-thr ) { under=in0+thr; out0= thr-under; }
            if( in1> thr ) { over =in1-thr; out1=-thr-over;  }
            if( in1<-thr ) { under=in1+thr; out1= thr-under; }
            break;
        }
        flt0 += (out0-flt0)*flt0_d;
        flt1 += (out1-flt1)*flt1_d;
        *outs[0] = flt0 * post;
        *outs[1] = flt1 * post;

      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
