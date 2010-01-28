/*
	******************************************************************************
	lo-fi fx
	(c) 2009, lubomir i. ivanov (neolit123 at gmail)
	using axonlib 0.0.x:
	<http://sites.google.com/site/ccernnaudio/axonlib/>
	******************************************************************************
	NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
	WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
	ANY DIRECT OR INDIRECT,	 SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
	OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
	MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
	TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
	USED WITH ACKNOWLEDGEMENT OF  THE AUTHOR. LAST BUT	 NOT LEAST, BY USING THIS
	PLUG-IN YOU  RELINQUISH  YOUR CLAIM TO SUE IT'S  AUTHOR, AS WELL AS	THE CLAIM
	TO ENTRUST SOMEBODY ELSE WITH DOING SO.
	******************************************************************************
	released under GPL:
	<http://www.gnu.org/licenses/>
	******************************************************************************
*/
// ---------------------------------------------------
// @ define
// ---------------------------------------------------
#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  7
#define AX_WIDTH      128 + 100
#define AX_HEIGHT     20 + AX_NUMPARAMS*(32 + 5)
#define AX_FLAGS      ( AX_EMBEDDED | AX_BUFFERED )
// ---------------------------------------------------
// @ include
// ---------------------------------------------------
#include "axPlugin.h"
#include "axEditor.h"
#include "parFloat.h"
#include "parInteger.h"
#include "wdgLabel.h"
#include "wdgValue.h"
#include "wdgKnob.h"
#include "wdgSwitch.h"
// ---------------------
#include "axBitmap.h"
#include "axSurface.h"
#include "wdgImage.h"
// ---------------------
#include "../extern/lodepng.h"
#include "../extern/lodepng.cpp"
#include "images/pattern0.h"
// ----------------------
// @ static arrays
// ----------------------
char* str_processing[] = { (char*)"Stereo",(char*)"Mono" };
char* str_interpolation[] = { (char*)"Interpolation Off",(char*)"Interpolation On" };

// ---------------------------------------------------
// @ constructor
// ---------------------------------------------------
class myPlugin : public axPlugin
{
	// --------------
	// @ private
	// --------------
  private:
  	axEditor* mEditor;
  	wdgKnob* knb;
  	wdgSwitch* swch;
		// ----------------------
		bool is_gui_initialized;
    axBitmap*       back_bmp;
    axSurface*      back_srf;
    // ----------------------
  	int		gfxd, gfxr, gfxf;
  	int   mono, intp, ftop;
    float srate, ampd, denorm, pifs;
    float g, lim, s, s5, sd;
    float c, c2, c131, f1, k0, k1, k2;
    float m0l, m1l, m2l, m0r, m1r, m2r;
    float h0, h1, n0, n1, j0, j1, s0, s1;
    int r, i;
  // --------------
	// @ public
	// --------------
  public:
    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags) : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    {
    	mEditor = NULL;
    	is_gui_initialized = false;
    	// @ describe fx
      describe("lofi", "liteon", "(c) 2009, lubomir i. ivanov", 0, 0);
      // @ init editor
			hasEditor(AX_WIDTH, AX_HEIGHT);
			// -----------------------
			// @ init variables
			// -----------------------
      srate  = updateSampleRate();
      ampd	 = 1/(6/log(2));
      denorm = 5e-30;
      pifs   = PI/srate;
      m0l = m1l = m2l = m0r = m1r = m2r = 0;
			h0 = h1 = n0 = n1 = j0 = j1 = s0 = s1 = 0;
			i = 0;
      // -----------------------
      // @ append parameters
      // -----------------------
      appendParameter( new parInteger(this,0, "Mode",	"",	0, 0, 1, str_processing ) );
      appendParameter( new parInteger(this,1, "Interpolation", "", 1, 0, 1, str_interpolation) );
      appendParameter( new parInteger(this,2, "S&H amount",	"%", 21, 0, 100 ) );
      appendParameter( new parInteger(this,3, "Depth", "Bits", 10, 4, 16 ) );
      appendParameter( new parFloat(	this,4, "Post Filter", "Scale", 1, 0, 1, 0.01) );
      appendParameter( new parFloat(	this,5, "Pre Amp", "dB", 0, -8, 32, 0.01) );
      appendParameter( new parFloat(	this,6, "Limit", "dB", -0.1, -18, 0, 0.01) );
      // @ process parameters
      processParameters();
    }
    // ------------------------------------
    // @ delete tmp
    // ------------------------------------
    virtual ~myPlugin()
    {
      if(is_gui_initialized)
      {
        delete back_bmp;
        delete back_srf;
      }
    }
    // ------------------------------------
	  // @ on parameter change
		// ------------------------------------
    virtual void onChange(axParameter* aParameter)
    {
      if( mEditor ) mEditor->onChange( aParameter );
      doProcessParameter(aParameter);
    }
    // ------------------------------------
	  // @ create editor
		// ------------------------------------
    //virtual axWindow* doCreateEditor(void)
    virtual void* doCreateEditor(void)
    {
    	// @ new editor
      axEditor* EDIT = new axEditor( "lofi_gui", this, -1, axRect(0,0,AX_WIDTH-1,AX_HEIGHT-1), AX_FLAGS );
      // --------------------------------------
      // @ decode bitmap and attach as convas
      // --------------------------------------
      if(!is_gui_initialized)
      {
        unsigned char*  buffer;
        unsigned char*  image;
        unsigned int    buffersize;
        unsigned int    imagesize;
        LodePNG_Decoder decoder;
        //LodePNG_loadFile(&buffer, &buffersize, filename);
        buffer     =  pattern0;
        buffersize =  1953;
        LodePNG_Decoder_init(&decoder);
        LodePNG_decode(&decoder,&image,&imagesize,buffer,buffersize);
        if (!decoder.error)
        {
          int width = decoder.infoPng.width;
          int height = decoder.infoPng.height;
          //int depth = decoder.infoPng.color.bitDepth;
          //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
          back_bmp = new axBitmap(width,height,image);
          back_bmp->convertRgbaBgra();
          back_bmp->setBackground(128,128,128);
          back_srf = new axSurface(width,height/*,0*/);
          back_srf->mCanvas->drawBitmap(back_bmp, 0,0, 0,0,width,height);
          free(image);
        }
        //free(buffer);
        LodePNG_Decoder_cleanup(&decoder);
        is_gui_initialized=true;
      }
			// ------------------
      // @ append widgets
      // ------------------
      // @ image
      EDIT->appendWidget( new wdgImage( EDIT, 7, axRect(  0,  0, AX_WIDTH,AX_HEIGHT), wal_None, /*NULL,*/ back_srf ) );
      // @ switch
      EDIT->appendWidget( swch = new wdgSwitch( EDIT, 0, axRect( 50, 10, 128, 32), wal_None/*, mParameters[0]*/ ) );
      swch->setup(str_processing);
      EDIT->connect( swch, mParameters[0] );
      EDIT->appendWidget( swch = new wdgSwitch( EDIT, 1, axRect( 50, 10 + 32 + 5, 128, 32), wal_None/*, mParameters[1]*/ ) );
      swch->setup(str_interpolation);
      EDIT->connect( swch, mParameters[1] );
      // @ knobs
      for (int kni=2; kni<7; kni++)
      {
      	EDIT->appendWidget( knb = new wdgKnob( EDIT, kni, axRect( 50, 10 +(kni*(32+5)), 128, 32), wal_None/*, mParameters[kni]*/ ) );
				EDIT->connect( knb, mParameters[kni] );
			}
      mEditor = EDIT;
      return mEditor;
    }
   	// ------------------------------------
    // @ destroy editor
		// ------------------------------------
    virtual void doDestroyEditor(void)
    {
			axEditor* EDIT = mEditor;
			mEditor = NULL;
			delete EDIT;
    }
		// ------------------------------------
    // @ idle editor
		// ------------------------------------
    virtual void doIdleEditor(void)
		{
			if (mEditor) mEditor->redrawDirty();
    }
    // ---------------------------------------------------
    // @ parameter change
		// ---------------------------------------------------
    virtual void doProcessParameter(axParameter* aParameter)
    {
    	// @ get parameter (id) and value (p)
      int  id = aParameter->mID;
      float p = aParameter->getValue();
      // -----------------------
      // @ switch between id's
      // -----------------------
      switch(id)
      {
      	// @ mono or stereo processing
	      case 0:
	      {
	        mono = (int)p;
	        break;
	      }
				// @ enable 2 point linear interpolation
	      case 1:
	      {
	      	intp = (int)p;
	      	break;
	      }
	      // @ exponent for s&h amount 44100 - 342 Hz
	      case 2:
	      {
	      	r = (int)(exp(p*0.0486) - 1);
	      	gfxr = (int)srate/(r + 1);
	        break;
	      }
	      // @ calculate bit depth reduction parameters
	      case 3:
	      {
	      	s  = pow(2,p)*0.5;
					s5 = s*0.5;
					sd = 1/s;
					gfxd = (int)p;
	        break;
	      }
	      // @ 2nd order lp post-filter
	      case 4:
	      {
	      	c = 1/tan(pifs*(f1 = exp(p*6.90775 + 2.99573)));
					k1 = (1+(c2 = c*c) - (c131 = c*1.31))*(k0 = 1/(1 + c2 + c131));
					k2 = 2*(1 - c2)*k0;
					gfxf = (int)f1;
					break;
	      }
	      // @ pre-amp gain
	      case 5:
	      {
	        g = exp(p*ampd);
	        break;
				}
				// @ hard clip saturation
				case 6:
				{
	        lim = exp(p*ampd);
	        break;
				}
			}
    }
    /*
    // ---------------------------------------------------
    // @ process per block
		// ---------------------------------------------------
    virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    {
      return false;
    }
		*/
    // ---------------------------------------------------
    // @ process per sample
		// ---------------------------------------------------
    virtual void doProcessSample(float** ins, float** outs)
		{
			// -------------------
			// @ alias inputs
			// -------------------
      float spl0 = *ins[0];
      float spl1 = *ins[1];

      // -------------------
      // @ process mono
      // -------------------
      if (mono == 1)
      {
        // @ bit reduction
        s0 = (spl0 + spl1)*s5;
  			s0 = (float)(int)s0;
  			s0 *= sd*g;
  			n1 = n0;
  			// @ s&h
  			if ((i += 1) > r)
				{
    			i = 0;
    			h0 = s0;
    			// @ 2 point linear interpolation
    			if (intp == 1) h0 = 0.5*(n1 + (n0 = s0));
  			}
  			// @ post filter
  			spl0 = h0;
  			if (gfxf < 19900)
				{
  				spl0 = (m0l = k0*h0 - k1*m2l - k2*m1l + denorm) + m1l + m1l + m2l + denorm;
  				m2l = m1l;
					m1l = m0l;
				}
  			// @ limit

        //spl0 = spl1 = ((spl0 >? -lim) <? lim);
   			spl0 = spl1 = axMax(-lim,axMin(spl0,lim));

      }
      else
      {
      	// -------------------
        // @ process stereo
        // -------------------
        s0 = s*spl0;
				s1 = s*spl1;
				s0 = (float)(int)s0;
				s1 = (float)(int)s1;
				s0 *= sd*g;
				s1 *= sd*g;
				n1 = n0; j1 = j0;
				if ((i += 1) > r)
				{
	  			i = 0;
	  			h0 = s0;
	  			h1 = s1;
	  			if (intp == 1)
	  			{
						h0=0.5*(n1 + (n0 = s0));
	  				h1=0.5*(j1 + (j0 = s1));
	  			}
  			}
  			spl0 = h0;
  			spl1 = h1;
  			if (gfxf < 19900)
				{
		  		spl0 = (m0l = k0*h0 - k1*m2l - k2*m1l + denorm) + m1l + m1l + m2l + denorm;
		  		m2l = m1l;
					m1l = m0l;
		  		spl1 = (m0r = k0*h1 - k1*m2r - k2*m1r + denorm) + m1r + m1r + m2r + denorm;
		  		m2r = m1r;
					m1r = m0r;
				}
  			//spl0 = (spl0 >? -lim) <? lim;
  			//spl1 = (spl1 >? -lim) <? lim;
   			spl0 = axMax(-lim,axMin(spl0,lim));
   			spl1 = axMax(-lim,axMin(spl1,lim));
      }
      // -------------------
      // @ output
      // -------------------
      *outs[0] = spl0 - denorm;
      *outs[1] = spl1 - denorm;
    }
};
// -------------------
#include "axMain.h"
