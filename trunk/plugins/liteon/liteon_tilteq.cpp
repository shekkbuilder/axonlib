/*
  ******************************************************************************
  tilt equalizer
  (c) 2009, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://sites.google.com/site/ccernnaudio/axonlib/>
  ******************************************************************************
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/
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
    float amp, pi, sr3;
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
        float srate  = updateSampleRate();
        float j = axSqrtf(2);
        amp    = 6/log(2);        
        pi     = 3.141592;
        sr3    = 3*srate;
        a0 = b1 = 0;
        lp_out = lp_out_r = 0;
        appendParameter( new parInteger(this,0, "processing",       "",       0,   0,  1, str_processing) );
        appendParameter( new parFloat(  this,1, "center frequency", "scale",  50,  0,  100, 0.05   ) );
        appendParameter( new parFloat(  this,2, "tilt (low/high)",  "db",     0,  -6,  6,   0.05   ) );
        appendParameter( new parFloat(  this,3, "output gain",      "db",     0,  -25, 25,  0.05   ) );       
        processParameters();
      }

    //----------
    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        wdebug("id = ", id, false);
        wdebug(", value = ", f);
        switch(id)
        {
          case 0: // slider1
            mono = (int)f;
            break;
          case 1: // slider2        
          {            
            // debug test           
            //----
            float sx = 16+f*1.20103;
            float f0 = floor(exp(sx*log(1.059))*8.17742);
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

    
    // debug here? 
    virtual void doProcessState(int aState)
    {
      #if defined(AX_DEBUG) && defined(WIN32)
        switch(aState)
        {
          case pst_Resume:
            axDwinCreate();
          break;
          case pst_Suspend:
            axDwinDestroy();
          break;
        }      
      #endif
    }
    
    //--------------------------------------------------
    virtual void doProcessSample(float** ins, float** outs)
      {
        // wdebug() here = freeze
        float spl0 = *ins[0]+DENORM;
        float spl1 = *ins[1]+DENORM;
        if (mono==1)
        {
          //process mono
          float input = (spl0+spl1)/2;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0=spl1=output*outgain;
        }
        else
        {
          //process stereo
          float input = spl0;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0 = output*outgain;
          float input_r = spl1;
          lp_out_r = a0*input_r + b1*lp_out_r;
          float output_r = input_r + lgain*lp_out_r + hgain*(input_r - lp_out_r);
          spl1 = output_r*outgain;
        }        
        // ---------
        *outs[0] = spl0-DENORM;
        *outs[1] = spl1-DENORM;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
