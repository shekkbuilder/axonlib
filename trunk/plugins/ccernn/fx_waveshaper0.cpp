#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   16
#define AX_NUMPARAMS  1

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

//char* str_mode[] =
//{
//  (char*)"bypass",
//  (char*)"clip",
//  (char*)"fold",
//  (char*)"wrap+",
//  (char*)"wrap",
//  (char*)"wrap-"
//};

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    parFloat* pAmount;
    float     mAmount, k;
    //int       mMode;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_waveshaper0","ccernn","product_string",0,2);
        appendParameter( new parFloat(  this,0,"amount","", 0, -1, 1  ) );
        //appendParameter( new parInteger(this,1,"mode",  "", 0, 0, 7  ) );
        processParameters();
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            mAmount = f;
            k = 2 * mAmount/(1-mAmount);
            break;
//          case 1:
//            mMode = (int)f;
//            break;
        }
      }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

//        switch(mMode)
//        {
//          case 0:
            spl0 = (1+k)*spl0 / (1+k*fabs(spl0));
            spl1 = (1+k)*spl1 / (1+k*fabs(spl1));
//            break;
//          case 1:
//            spl0 = 2  * pow(spl0,2) - 1;
//            spl1 = 2  * pow(spl1,2) - 1;
//            break;
//          case 2:
//            spl0 = 4  * pow(spl0,3) - 3  * spl0;
//            spl1 = 4  * pow(spl1,3) - 3  * spl1;
//            break;
//          case 3:
//            spl0 = 8  * pow(spl0,4) - 8  * pow(spl0,2) + 1;
//            spl1 = 8  * pow(spl1,4) - 8  * pow(spl1,2) + 1;
//            break;
//          case 4:
//            spl0 = 16 * pow(spl0,5) - 20 * pow(spl0,3) + 5*spl0;
//            spl1 = 16 * pow(spl1,5) - 20 * pow(spl1,3) + 5*spl1;
//            break;
//          case 5:
//            spl0 = cos(3*spl0);
//            spl1 = cos(3*spl1);
//            break;
//          case 6:
//            spl0 = (spl0 + pow(spl0,3) ) / 2;
//            spl1 = (spl1 + pow(spl1,3) ) / 2;
//            break;
//          case 7:
//            spl0 = spl0 + 3*spl0 + 5*spl0 + 7*spl0 + 9*spl0;
//            spl1 = spl1 + 3*spl1 + 5*spl1 + 7*spl1 + 9*spl1;
//            break;
//        }

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------

};

//----------------------------------------------------------------------
#include "axMain.h"
