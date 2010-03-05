#define AX_PLUGIN     myEffect
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  4
#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)

#define AX_DEBUG
#include "axDebug.h"

#include "axEffect.h"
#include "parFloat.h"
#include "wdgPanel.h"
#include "wdgKnob.h"

//----------------------------------------------------------------------

//#define MIN_BPM             30.0f
//#define MAX_SRATE           192000
//#define MAX_NUMBEATS        4
//#define MAX_SecondsPerBeat  ( 60.0f / MIN_BPM )
//#define MAX_SamplesPerSec   ( MAX_SRATE * 2 )
//#define MAX_BUFFER_SIZE     (int)( MAX_NUMBEATS * MAX_SecondsPerBeat * MAX_SamplesPerSec  )
#define MAX_BUFFER_SIZE       4096

class myEffect : public axEffect
{
  private:
    float buf[MAX_BUFFER_SIZE*2];
    int pos, size;
    float frac;
    float z0, z00, z01;
    float dly,fbk,wet,dry;
  private:
    parFloat *pDly, *pFbk, *pWet, *pDry;
    wdgKnob  *wDly, *wFbk, *wWet, *wDry;

  public:

//slider1:  100 <  0,4096,0.01 > delay (samples)
//slider2:  0.9 <  0,1,   0.001> feedback
//slider3: -6   <-60,0,   0.1  > volume (db)
//slider4: -6   <-60,0,   0.1  > dry (db)

    myEffect(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axEffect(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("fx_fracdelay0","ccernn","product_string",0,0);
        setupAudio(2,2);
        setupEditor(150,180);
        appendParameter( pDly = new parFloat( this,0,"dly", "",   100,  0,    MAX_BUFFER_SIZE ) );
        appendParameter( pFbk = new parFloat( this,1,"fbk",  "",  0.9 ) );
        appendParameter( pWet = new parFloat( this,2,"wet", "db",-6,   -60,   0 ) );
        appendParameter( pDry = new parFloat( this,3,"dry", "db",-6,   -60,   0 ) );
        processParameters();

//@init
//  buf  = 0;
//  pos  = 0;
//  size = 0;
//  frac = 0;
//  z00  = 0;
//  z01  = 0;

        memset(buf,0,sizeof(buf));
        pos = 0;
        size = 0;
        frac = 0;
        z00 = 0;
        z01 = 0;

      }

    //----------

//@slider
//  dly  = slider1;
//  fbk = slider2;
//  wet  = 2^(slider3/6);
//  dry  = 2^(slider4/6);
//  size = floor(dly);
//  frac = dly - size;

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            dly = f;
            size = axFloor(dly);
            frac = dly-(float)size;
            //trace("size " << size << "frac " << frac);
            // bug/crash if size==0
            //while (pos>=size) pos-=size;
            if (pos>=size) pos=0;
            break;
          case 1: fbk = f;        break;
          case 2: wet = axPowf(2,f/6);  break;
          case 3: dry = axPowf(2,f/6);  break;
        }
      }

//@block
//  while
//  (
//    midirecv(ts,msg1,msg23) ? (
//      m = msg1 & 240;
//      (m == 9*16) ? (
//        note = msg23 & 127;
//        freq = 440 * pow(2.0,(note-69.0)/12.0);
//        vel  = (msg23/256) | 0;
//        //wet  = vel/127;
//        dly  = srate / freq;
//        //slider1 = dly;
//        //slider3 = wet;
//        //sliderchange(slider1+slider3);
//        size = floor(dly);
//        frac = dly - size;
//        pos  = 0;
//        z0   = 0;
//      );
//      //(m == 13*16) ? (
//      //  note = msg23 & 127;
//      //  vel  = (msg23/256) | 0;
//      //  fbk = (note/127) - 0.001;
//      //);
//      midisend(ts,msg1,msg23);
//    );
//  );

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        float srate = getSampleRate();
        int msg = msg1 & 240;
        if (msg==9*16)
        {
          int note = msg2;
          float freq = 440 * axPowf(2.0,(note-69.0)/12.0);
          dly  = srate / freq;
          //trace("note: " << note << " freq " << freq << " dly " << dly);
          size = axFloor(dly);
          frac = dly - (float)size;
          pos  = 0;
          z0   = 0;
        }
        sendMidi(ofs,msg1,msg2,msg3);
      }

    //----------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo();
    //    return false;
    //  }

    //----------

//@sample
//  pos2 = pos*2;
//  in0 = spl0;
//  in1 = spl1;
//  d00 = buf[pos2];
//  d01 = buf[pos2+1];
//  dly<1 ? (
//    i00 = (in0*(1-frac)) + (d00*frac);
//    i01 = (in1*(1-frac)) + (d01*frac);
//  ) : (
//    i00 = (d00*(1-frac)) + (z00*frac);
//    i01 = (d01*(1-frac)) + (z01*frac);
//  );
//  buf[pos2]   = in0 + (fbk*i00);
//  buf[pos2+1] = in1 + (fbk*i01);
//  z00 = d00;
//  z01 = d01;
//  pos += 1;
//  pos>=size ? pos=0; //-=size
//  spl0 = (in0*dry) + (i00*wet);
//  spl1 = (in1*dry) + (i01*wet);

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

        int pos2 = pos*2;
        float in0 = spl0;
        float in1 = spl1;
        float d00 = buf[pos2];
        float d01 = buf[pos2+1];
        float i00,i01;
        if (dly<1)
        {
          i00 = (in0*(1-frac)) + (d00*frac);
          i01 = (in1*(1-frac)) + (d01*frac);
        } else {
          i00 = (d00*(1-frac)) + (z00*frac);
          i01 = (d01*(1-frac)) + (z01*frac);
        }
        buf[pos2]   = in0 + (fbk*i00);
        buf[pos2+1] = in1 + (fbk*i01);
        z00 = d00;
        z01 = d01;
        pos += 1;
        if (pos>=size) pos=0; //-=size
        spl0 = (in0*dry) + (i00*wet);
        spl1 = (in1*dry) + (i01*wet);

        *outs[0] = spl0;
        *outs[1] = spl1;
      }

    //----------

    virtual void do_SetupEditor(axEditor* aEditor)
      {
        wdgPanel* P;
        aEditor->appendWidget( P = new wdgPanel(this, -1, NULL_RECT,wal_Client) );
        P->appendWidget( wDly = new wdgKnob( this, -1, axRect( 10, 10, 128,32 ), wal_None ) );
        P->appendWidget( wFbk = new wdgKnob( this, -1, axRect( 10, 50, 128,32 ), wal_None ) );
        P->appendWidget( wWet = new wdgKnob( this, -1, axRect( 10, 90, 128,32 ), wal_None ) );
        P->appendWidget( wDry = new wdgKnob( this, -1, axRect( 10,130, 128,32 ), wal_None ) );
        aEditor->doRealign();
        aEditor->connect( wDly, pDly );
        aEditor->connect( wFbk, pFbk);
        aEditor->connect( wWet, pWet );
        aEditor->connect( wDry, pDry );
        wDly->setSensitivity(0.005,0.001);
      }

};

//----------------------------------------------------------------------
#include "axMain.h"


