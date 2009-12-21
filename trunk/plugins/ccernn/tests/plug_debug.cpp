#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  2
#define AX_WIDTH      320
#define AX_HEIGHT     200
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_AUTOSYNC

#define AX_DEBUG
#include "axDebug.h"

#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "wdgKnob.h"

#include "axBitmap.h"
#include "axSurface.h"
#include "wdgImgKnob.h"
#include "wdgImage.h"

#include "extern/lodepng.h"
#include "extern/lodepng.cpp"
#include "../axonlib/images/knob1.h"

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  private:
    bool is_gui_initialized;
    axBitmap*       knob_bmp;
    axSurface*      knob_srf;
    axBitmap*       back_bmp;
    axSurface*      back_srf;

  public:
    axEditor  *mEditor;
    wdgKnob   *mKnob1;
    wdgImgKnob   *mKnob2;
    parFloat  *mParam1,*mParam2;
    float      mValue1,mValue2;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags )
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        mEditor = NULL;
        is_gui_initialized = false;
        describe("plug_debug","ccernn","product_string",0,666);
        hasEditor(AX_WIDTH,AX_HEIGHT);
        appendParameter( mParam1 = new parFloat(this,0,"mParam1","",0,-1,1,.1) );
        appendParameter( mParam2 = new parFloat(this,1,"mParam2","",0,-1,1,.1) );
        processParameters();
        mValue1 = 0;
      }

    //----------

    virtual ~myPlugin()
      {
        if(is_gui_initialized)
        {
          delete knob_bmp;
          delete knob_srf;
        }
      }

    //--------------------------------------------------

    virtual void onChange(axParameter* aParameter)
      {
        if(mEditor) mEditor->onChange(aParameter);
        doProcessParameter(aParameter);
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    // can this one die because of some threading issues? (ala close editor) think,think..

    virtual axWindow* doCreateEditor(void)
      {
        axEditor* E = new axEditor("plug_debug_window",this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
        if(!is_gui_initialized)
        {
          unsigned char*  buffer;
          unsigned char*  image;
          unsigned int    buffersize;
          unsigned int    imagesize;
          LodePNG_Decoder decoder;
          //LodePNG_loadFile(&buffer, &buffersize, filename);
          buffer     =  knob1;
          buffersize =  20697;
          LodePNG_Decoder_init(&decoder);
          LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
          if (!decoder.error)
          {
            int width = decoder.infoPng.width;
            int height = decoder.infoPng.height;
            //int depth = decoder.infoPng.color.bitDepth;
            //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
            knob_bmp = new axBitmap(width,height,image);
            knob_bmp->convertRgbaBgra();
            knob_bmp->setBackground(128,128,128);
            knob_srf = new axSurface(width,height,0);
            knob_srf->mCanvas->drawBitmap(knob_bmp, 0,0, 0,0,width,height);
            free(image);
          }
          //free(buffer);
          LodePNG_Decoder_cleanup(&decoder);
          is_gui_initialized=true;
        }
        E->appendWidget( mKnob1 = new wdgKnob(   E,0,axRect(10,10,128,32),wal_None,mParam1) );
        E->appendWidget( mKnob2 = new wdgImgKnob(E,1,axRect(10,50,48,48),wal_None,mParam2,64,knob_srf) );
        E->connect( mKnob1, mParam1 );
        E->connect( mKnob2, mParam2 );
        // could mEditor be in use from another thread? gui? audio? setParameter?
        // it shouldn't (we are creating the editor), but, ...
        mEditor = E;
        return mEditor;
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
        if(mEditor) mEditor->redrawDirty();
      }

    //--------------------------------------------------
    // plugin
    //--------------------------------------------------

    //virtual void doProcessState(int aState) {}
    //virtual void doProcessTransport(int aState) {}
    //virtual void doProcessMidi(int ofs, char msg1, char msg2, char msg3) {}

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float val = aParameter->getValue();
        //lock
        switch(id)
        {
          case 0: mValue1 = val;  break;
          case 1: mValue2 = val;  break;
        }
        //unlock
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    float beatsize = (60/mTempo) * mSampleRate;
    //    return false;
    //  }

    //----------

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
