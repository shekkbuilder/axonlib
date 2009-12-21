#define AX_PLUGIN     myPlugin
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  1
#define AX_WIDTH      320
#define AX_HEIGHT     240

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "parInteger.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    int         trans;
    parInteger  *pTrans;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("midi_transpose0","ccernn","product_string",0,0);
        appendParameter( pTrans = new parInteger( this,0,"mode", "", 0, -24,24 ) );
      }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int id = aParameter->mID;
        float f = aParameter->getValue();
        TRACE("DoProcessParameter (%i) %f\n",id,f);
        switch(id)
        {
          case 0:
            trans = (int)f;
            break;
        }
      }

    //----------

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //TRACE("DoProcessMidi (%i) - %i,%i,%i\n",ofs,msg1,msg2,msg3);
        if( msg1==144 || msg1==128) { msg2 += trans; }
        sendMidi(ofs,msg1,msg2,msg3);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
