#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   0
#define AX_NUMPARAMS  4
//#define AX_AUTOSYNC
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
#include "parInteger.h"

//----------------------------------------------------------------------

char* str_processing[] = { (char*)"stereo",(char*)"mono" };

//-----

class myPlugin : public axPlugin
{
  public:
    float amp, denorm, pi, sr3;
    float outgain, lgain, hgain;
    int   mono;
    float input,  output,  lp_out;
    float input_r,output_r,lp_out_r;
    float a0, b1;

  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
        describe("tilteq","liteon","product_string",0,0);
//@init
//amp = 6/log(2);
//denorm = 10^-30;
//pi = 22/7;
//sr3 = 3*srate;
        float srate  = updateSampleRate();
        amp    = 6/log(2);
        denorm = 0.00000000001;//10^-30;
        pi     = 22/7;
        sr3    = 3*srate;
        a0 = b1 = 0;
        lp_out = lp_out_r = 0;
//slider1:0<0,1,1{Stereo,Mono}>Processing
//slider2:50<0,100,0.05>Center Frequency (Scale)
//slider3:0<-6,6,0.05>Tilt (Low / High) (dB)
//slider4:0<-25,25,0.05>Output Gain (dB)
        appendParameter( new parInteger(this,0, "processing",       "",       0,   0,  1, str_processing) );
        appendParameter( new parFloat(  this,1, "center frequency", "scale",  50,  0,  100, 0.05   ) );
        appendParameter( new parFloat(  this,2, "tilt (low/high)",  "db",     0,  -6,  6,   0.05   ) );
        appendParameter( new parFloat(  this,3, "output gain",      "db",     0,  -25, 25,  0.05   ) );
        processParameters();
      }

    //----------

//@slider
// mono    = slider1;
// gain    = slider3;
// outgain = exp(slider4/amp);
////conditition
//gfactor = 4;
//gain > 0 ? (
//  g1 = -gfactor*gain;
//  g2 = gain;
//) : (
//  g1 = -gain;
//  g2 = gfactor*gain;
//);
////two separate gains
//lgain = exp(g1/amp)-1;
//hgain = exp(g2/amp)-1;
////f0
//sx = 16+slider2*1.20103;
//f0 = floor(exp(sx*log(1.059))*8.17742);
////filter
//omega = 2*pi*f0;
//n = 1/(sr3 + omega);
//a0 = 2*omega*n;
//b1 = (sr3 - omega)*n;

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: // slider1
            mono = (int)f;
            break;
          case 1: // slider2
          {
            float sx = 16+f*1.20103;
            float f0 = floor(exp(sx*log(1.059))*8.17742);
            TRACE("f0 : %f\n",f0);
            float omega = 2*pi*f0;
            float n = 1/(sr3 + omega);
            a0 = 2*omega*n;
            b1 = (sr3 - omega)*n;
            break;
          }
          case 2: // slider3
          {
            float gain = f;
            float gfactor = 4;
            float g1, g2;
            if (gain>0)
            {
              g1 = -gfactor*gain;
              g2 = gain;
            }
            else
            {
              g1 = -gain;
              g2 = gfactor*gain;
            }
            lgain = exp(g1/amp)-1;
            hgain = exp(g2/amp)-1;
            break;
          }
          case 3: // slider4
            outgain = exp(f/amp);
            break;
        }
      }

    //--------------------------------------------------

//@sample
//mono == 1 ? (
////process mono
//input = (spl0+spl1)/2;
//lp_out = a0*input + b1*lp_out;
//output = input + lgain*lp_out + hgain*(input - lp_out);
//spl0=spl1=output*outgain+denorm;
//) : (
////process stereo
//input = spl0;
//lp_out = a0*input + b1*lp_out;
//output = input + lgain*lp_out + hgain*(input - lp_out);
//spl0=output*outgain+denorm;
//input_r = spl1;
//lp_out_r = a0*input_r + b1*lp_out_r;
//output_r = input_r + lgain*lp_out_r + hgain*(input_r - lp_out_r);
//spl1=output_r*outgain+denorm;
//);

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        if (mono==1)
        {
          //process mono
          float input = (spl0+spl1)/2;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0=spl1=output*outgain+denorm;
        }
        else
        {
          //process stereo
          float input = spl0;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0 = output*outgain+denorm;
          float input_r = spl1;
          lp_out_r = a0*input_r + b1*lp_out_r;
          float output_r = input_r + lgain*lp_out_r + hgain*(input_r - lp_out_r);
          spl1 = output_r*outgain+denorm;
        }
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
