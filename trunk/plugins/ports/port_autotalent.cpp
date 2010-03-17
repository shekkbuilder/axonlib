#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
//#define AX_NUMPARAMS  19
#define AX_NUMPARAMS  17
//#define AX_AUTOSYNC
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
//#define AX_DEBUG
//#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

//#include "../../extern/mayer_fft.h"
#include "../../extern/mayer_fft.c"

//#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171


//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    //axEditor* mEditor;
    //float mValue;
  private:

    //fft_vars*     fmembvars;    // member variables for fft routine
    int     nfft;       // size of FFT
    int     numfreqs;   // number of frequencies represented (nfft/2 + 1)
    float*  fft_data;   // array for writing/reading to/from FFT function

    unsigned long fs;           // Sample rate
    unsigned long cbsize;       // size of circular buffer
    unsigned long corrsize;     // cbsize/2 + 1
    unsigned long cbiwr;
    unsigned long cbord;
    float*        cbi;          // circular input buffer
    float*        cbo;          // circular output buffer
    float*        cbonorm;      // circular output buffer used to normalize signal
    float*        cbwindow;     // hann of length N/2, zeros for the rest
    float*        acwinv;       // inverse of autocorrelation of window
    float*        hannwindow;   // length-N hann
    int           noverlap;
    float*        ffttime;
    float*        fftfreqre;
    float*        fftfreqim;
    // VARIABLES FOR LOW-RATE SECTION
    float         aref;         // A tuning reference (Hz)
    float         pperiod;      // Pitch period (seconds)
    float         pitch;        // Pitch (semitones)
    float         pitchpers;    // Pitch persist (semitones)
    float         conf;         // Confidence of pitch period estimate (between 0 and 1)
    float         vthresh;      // Voiced speech threshold
    float         pmax;         // Maximum allowable pitch period (seconds)
    float         pmin;         // Minimum allowable pitch period (seconds)
    unsigned long nmax;         // Maximum period index for pitch prd est
    unsigned long nmin;         // Minimum period index for pitch prd est
    float         lrshift;      // Shift prescribed by low-rate section
    int           ptarget;      // Pitch target, between 0 and 11
    float         sptarget;     // Smoothed pitch target
    int           wasvoiced;    // 1 if previous frame was voiced
    float         persistamt;   // Proportion of previous pitch considered during next voiced period
    float         glidepersist;
    // VARIABLES FOR PITCH SHIFTER
    float         phprd;        // phase period
    float         phprdd;       // default (unvoiced) phase period
    float         phinc;        // input phase increment
    float         phincfact;    // factor determining output phase increment
    float         phasein;
    float         phaseout;
    float*        frag;         // windowed fragment of speech
    unsigned long fragsize;     // size of fragment in samples

  private:
    float fMix;
    float fShift;
    float fTune;
    float fA;
    float fBb;
    float fB;
    float fC;
    float fDb;
    float fD;
    float fEb;
    float fE;
    float fF;
    float fGb;
    float fG;
    float fAb;
    float fAmount;
    float fGlide;

  public:

    //----------

    void fft_forward(/*fft_vars* membvars,*/ float* input, float* output_re, float* output_im)
    {
      int ti;
      //int nfft;
      int hnfft;
      //int numfreqs;
      //nfft = membvars->nfft;
      hnfft = nfft/2;
      //numfreqs = membvars->numfreqs;
      for (ti=0; ti<nfft; ti++) { fft_data[ti] = input[ti]; }
      mayer_realfft(nfft, fft_data);
      output_im[0] = 0;
      for (ti=0; ti<hnfft; ti++)
      {
        output_re[ti]   = fft_data[ti];
        output_im[ti+1] = fft_data[nfft-1-ti];
      }
      output_re[hnfft] = fft_data[hnfft];
      output_im[hnfft] = 0;
    }

    //----------------------------------------------------------------------

    // Perform inverse FFT, returning real data
    // Accepts:
    //   membvars - pointer to struct of FFT variables
    //   input_re - pointer to an array of the real part of the output,
    //     size nfft/2 + 1
    //   input_im - pointer to an array of the imaginary part of the output,
    //     size nfft/2 + 1
    //   output - pointer to an array of (real) input values, size nfft

    void fft_inverse(/*fft_vars* membvars,*/ float* input_re, float* input_im, float* output)
    {
      int   ti;
      //int   nfft;
      int   hnfft;
      //int   numfreqs;
      //nfft      = membvars->nfft;
      hnfft     = nfft/2;
      //numfreqs  = membvars->numfreqs;
      for (ti=0; ti<hnfft; ti++)
      {
        fft_data[ti] = input_re[ti];
        fft_data[nfft-1-ti] = input_im[ti+1];
      }
      fft_data[hnfft] = input_re[hnfft];
      mayer_realifft(nfft, fft_data);
      for (ti=0; ti<nfft; ti++) { output[ti] = fft_data[ti]; }
    }

    //----------

    //myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    //: axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
    myPlugin(axHost* aHost, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(aHost,aNumProgs,aNumParams,aPlugFlags)
      {
        unsigned long ti;
        float SampleRate = updateSampleRate();
        aref        = 440;
        fs          = SampleRate;
        if (SampleRate>=88200) cbsize = 4096; else cbsize = 2048;
        corrsize    = cbsize / 2 + 1;
        pmax        = 1/(float)70;  // max and min periods (ms)
        pmin        = 1/(float)700; // eventually may want to bring these out as sliders
        pperiod     = pmax;
        nmax        = (unsigned long)(SampleRate * pmax);
        if (nmax > corrsize) { nmax = corrsize; }
        nmin        = (unsigned long)(SampleRate * pmin);
        cbi         = (float*)calloc(cbsize, sizeof(float));
        cbo         = (float*)calloc(cbsize, sizeof(float));
        cbonorm     = (float*)calloc(cbsize, sizeof(float));
        cbiwr       = 0;
        cbord       = 0;
        // Standard raised cosine window, max height at N/2
        hannwindow  = (float*)calloc(cbsize, sizeof(float));
        for (ti=0; ti<cbsize; ti++) { hannwindow[ti] = -0.5*cos(2*PI*ti/(cbsize - 1)) + 0.5; }
        // Generate a window with a single raised cosine from N/4 to 3N/4
        cbwindow    = (float*)calloc(cbsize, sizeof(float));
        for (ti=0; ti<(cbsize / 2); ti++) { cbwindow[ti+cbsize/4] = -0.5*cos(4*PI*ti/(cbsize - 1)) + 0.5; }
        noverlap    = 4;

        //fmembvars   = fft_con(cbsize);
        //nfft      = nfft;
        nfft      = cbsize;
        numfreqs  = nfft/2 + 1;
        fft_data  = (float*)calloc(nfft, sizeof(float));

        ffttime     = (float*)calloc(cbsize, sizeof(float));
        fftfreqre   = (float*)calloc(corrsize, sizeof(float));
        fftfreqim   = (float*)calloc(corrsize, sizeof(float));
        // Calculate autocorrelation of window
        acwinv      = (float*)calloc(cbsize, sizeof(float));
        for (ti=0; ti<cbsize; ti++) { ffttime[ti] = cbwindow[ti]; }
        fft_forward(/*fmembvars,*/ cbwindow, fftfreqre, fftfreqim);
        for (ti=0; ti<corrsize; ti++)
        {
          fftfreqre[ti] = (fftfreqre[ti])*(fftfreqre[ti]) + (fftfreqim[ti])*(fftfreqim[ti]);
          fftfreqim[ti] = 0;
        }
        fft_inverse(/*fmembvars,*/ fftfreqre, fftfreqim, ffttime);
        for (ti=1; ti<cbsize; ti++)
        {
          acwinv[ti] = ffttime[ti]/ffttime[0];
          if (acwinv[ti] > 0.000001) { acwinv[ti] = (float)1/acwinv[ti]; }
          else { acwinv[ti] = 0; }
        }
        acwinv[0]     = 1;
        // ---- END Calculate autocorrelation of window ----
        lrshift       = 0;
        ptarget       = 0;
        sptarget      = 0;
        wasvoiced     = 0;
        persistamt    = 0;
        glidepersist  = 100; // 100 ms glide persist
        vthresh       = 0.8;  //  The voiced confidence (unbiased peak) threshold level
        // Pitch shifter initialization
        phprdd        = 0.01; // Default period
        phprd         = phprdd;
        phinc         = (float)1/(phprd * SampleRate);
        phincfact     = 1;
        phasein       = 0;
        phaseout      = 0;
        frag          = (float*)calloc(cbsize, sizeof(float));
        fragsize      = 0;

        //mEditor = NULL;
        describe("port_autotalent","thomas a. baran","axonlib ported plugin",0,0);
        setupAudio(2,2);

        //hasEditor(AX_WIDTH,AX_HEIGHT);

        appendParameter(new parFloat(this, 0,"Mix",          "",     1 ));
        appendParameter(new parFloat(this, 1,"Output shift", "semi", 0, -12,12 ));
        appendParameter(new parFloat(this, 2,"Input tune",   "semi", 0, -1, 1  ));
        appendParameter(new parFloat(this, 3,"A",            "",     1 ));
        appendParameter(new parFloat(this, 4,"Bb",           "",     1 ));
        appendParameter(new parFloat(this, 5,"B",            "",     1 ));
        appendParameter(new parFloat(this, 6,"C",            "",     1 ));
        appendParameter(new parFloat(this, 7,"Db",           "",     1 ));
        appendParameter(new parFloat(this, 8,"D",            "",     1 ));
        appendParameter(new parFloat(this, 9,"Eb",           "",     1 ));
        appendParameter(new parFloat(this,10,"E",            "",     1 ));
        appendParameter(new parFloat(this,11,"F",            "",     1 ));
        appendParameter(new parFloat(this,12,"Gb",           "",     1 ));
        appendParameter(new parFloat(this,13,"G",            "",     1 ));
        appendParameter(new parFloat(this,14,"Ab",           "",     1 ));
        appendParameter(new parFloat(this,15,"Correction",   "",     1 ));
        appendParameter(new parFloat(this,16,"Glide",        "ms",   0, 0,500 ));
      //appendParameter(new parFloat(this,17,"Input",        "",     1 ));
      //appendParameter(new parFloat(this,18,"Output",       "",     1 ));

        processParameters();
      }

    //----------

    virtual ~myPlugin()
      {
        //fft_des(((Autotalent*)Instance)->fmembvars);
        free(cbi);
        free(cbo);
        free(cbonorm);
        free(cbwindow);
        free(hannwindow);
        free(acwinv);
        free(frag);
        free(ffttime);
        free(fftfreqre);
        free(fftfreqim);
      }

    //----------

    //virtual void onChange(axParameter* aParameter)
    //  {
    //    if(mEditor) mEditor->onChange(aParameter);
    //    doProcessParameter(aParameter);
    //  }

    //--------------------------------------------------

    //virtual axWindow* doCreateEditor(void)
    //  {
    //    axEditor* E = new axEditor(this,-1,axRect(0,0,AX_WIDTH,AX_HEIGHT),AX_FLAGS);
    //    E->setLayout(10,10,5,5);
    //    //E->setFlag(wfl_Vertical);
    //    E->appendWidget(new wdgKnobPar(E,0,axRect(0,0,100,32),wal_Stacked,mParameters[0]));
    //    E->updateWidgetValues();
    //    E->doRealign();
    //    mEditor = E;
    //    return mEditor;
    //  }

    //----------

    //virtual void doDestroyEditor(void)
    //  {
    //    axEditor* tempeditor = mEditor;
    //    mEditor = NULL;
    //    delete tempeditor;
    //  }

    //----------

    //virtual void doIdleEditor(void)
    //  {
    //    mEditor->redrawDirty();
    //  }

    //--------------------------------------------------

    //virtual void doProcessState(int aState)
    //  {
    //    TRACE("DoProcessState: %i\n",aState);
    //  }

    //----------

    // if has editor
    //virtual void doProcessTransport(int aState)
    //  {
    //    TRACE("doProcessTransport: %i\n",aState);
    //  }

    //----------

    //virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
    //  {
    //    TRACE("DoProcessMidi: (ofs:%i)  %i,%i,%i\n",ofs,msg1,msg2,msg3);
    //  }

    //----------

    virtual void doProcessParameter(axParameter* aParameter)
      {
        int  id = aParameter->mID;
        float f = aParameter->getValue();
        switch(id)
        {
          case  0: fMix   = f;  break;
          case  1: fShift = f;  break;
          case  2: fTune  = f;  break;
          case  3: fA     = f;  break;
          case  4: fBb    = f;  break;
          case  5: fB     = f;  break;
          case  6: fC     = f;  break;
          case  7: fDb    = f;  break;
          case  8: fD     = f;  break;
          case  9: fEb    = f;  break;
          case 10: fE     = f;  break;
          case 11: fF     = f;  break;
          case 12: fGb    = f;  break;
          case 13: fG     = f;  break;
          case 14: fAb    = f;  break;
          case 15: fAmount= f;  break;
          case 16: fGlide = f;  break;
          //case 17: fMix = f;  break;
          //case 18: fMix = f;  break;
        }
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

//void runAutotalent(LADSPA_Handle Instance, unsigned long SampleCount)

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];

//LADSPA_Data*  pfInput;
//LADSPA_Data*  pfOutput;

//      float         fMix;
//      float         fShift;
//      float         fTune;
//      float         fA;
//      float         fBb;
//      float         fB;
//      float         fC;
//      float         fDb;
//      float         fD;
//      float         fEb;
//      float         fE;
//      float         fF;
//      float         fGb;
//      float         fG;
//      float         fAb;
//      float         fGlide;
      float         fPersist;
//      float         fAmount;

      //Autotalent*   psAutotalent;
      //unsigned long lSampleIndex;

      unsigned long N;
      unsigned long Nf;
//      unsigned long fs;
//      float         pmin;
//      float         pmax;
//      unsigned long nmin;
//      unsigned long nmax;
      long int      ti;
      long int      ti2;
      long int      ti3;
      float         tf;
      float         tf2;
      float         tf3;
//      float         pperiod;
//      float         pitch;
//      float         conf;
//      float         aref;

//      //psAutotalent  = (Autotalent *)Instance;
//      pfInput       = m_pfInputBuffer1;
//      pfOutput      = psAutotalent->m_pfOutputBuffer1;
//      fMix          = (float) *(psAutotalent->m_pfMix);
//      fShift        = (float) *(psAutotalent->m_pfShift);
//      fTune         = (float) *(psAutotalent->m_pfTune);
//      fA            = (float) *(psAutotalent->m_pfA);
//      fBb           = (float) *(psAutotalent->m_pfBb);
//      fB            = (float) *(psAutotalent->m_pfB);
//      fC            = (float) *(psAutotalent->m_pfC);
//      fDb           = (float) *(psAutotalent->m_pfDb);
//      fD            = (float) *(psAutotalent->m_pfD);
//      fEb           = (float) *(psAutotalent->m_pfEb);
//      fE            = (float) *(psAutotalent->m_pfE);
//      fF            = (float) *(psAutotalent->m_pfF);
//      fGb           = (float) *(psAutotalent->m_pfGb);
//      fG            = (float) *(psAutotalent->m_pfG);
//      fAb           = (float) *(psAutotalent->m_pfAb);
//      fAmount       = (float) *(psAutotalent->m_pfAmount);
//      fGlide        = (float) *(psAutotalent->m_pfGlide);
        fPersist      = glidepersist;
        aref          = (float)440*pow(2,fTune/12);

        N             = cbsize;
        Nf            = corrsize;
        //fs            = fs;
        //pmax          = pmax;
        //pmin          = pmin;
        //nmax          = nmax;
        //nmin          = nmin;
        //aref          = aref;
        //pperiod       = pperiod;
        //pitch         = pitch;
        //conf          = conf;

      // MAIN DSP LOOP

      //for (lSampleIndex=0; lSampleIndex<SampleCount; lSampleIndex++)
      //{

        // load data into circular buffer

        tf = spl0; //(float) *(pfInput++);
        cbi[cbiwr] = tf;
        cbiwr++;
        if (cbiwr >= N) cbiwr = 0;

        // Low-rate section

        // Every N/noverlap samples, run pitch estimation / correction code
        if ((cbiwr)%(N/noverlap) == 0)
        {

          // Obtain autocovariance

          // Window and fill FFT buffer
          ti2 = cbiwr;
          for (ti=0; ti<N; ti++) { ffttime[ti] = (float)(cbi[(ti2-ti)%N]*cbwindow[ti]); }
          // Calculate FFT
          fft_forward(/*fmembvars,*/ ffttime, fftfreqre, fftfreqim);

          // Remove DC

          fftfreqre[0] = 0;
          fftfreqim[0] = 0;

          // Take magnitude squared

          for (ti=1; ti<Nf; ti++)
          {
            fftfreqre[ti] = (fftfreqre[ti])*(fftfreqre[ti]) + (fftfreqim[ti])*(fftfreqim[ti]);
            fftfreqim[ti] = 0;
          }
          // Calculate IFFT
          fft_inverse(/*fmembvars,*/ fftfreqre, fftfreqim, ffttime);
          // Normalize
          for (ti=1; ti<N; ti++) { ffttime[ti] = ffttime[ti] / ffttime[0]; }
          ffttime[0] = 1;

          // Calculate pitch and confidence

          // Calculate pitch period
          //   Pitch period is determined by the location of the max (biased)
          //     peak within a given range
          //   Confidence is determined by the corresponding unbiased height

          tf2 = 0;
          pperiod = pmin;
          for (ti=nmin; ti<nmax; ti++)
          {
            ti2 = ti-1;
            ti3 = ti+1;
            if (ti2<0)  { ti2 = 0; }
            if (ti3>Nf) { ti3 = Nf; }
            tf = ffttime[ti];
            if (tf>ffttime[ti2] && tf>=ffttime[ti3] && tf>tf2)
            {
              tf2 = tf;
              conf = tf*acwinv[ti];
              pperiod = (float)ti/fs;
            }
          }

          // Convert to semitones

          pitch = (float) -12*log10((float)aref*pperiod)*L2SC;
          pitch = pitch;
          pperiod = pperiod;
          conf = conf;

          // Determine pitch target

          // If voiced
          if (conf>=vthresh)
          {
            // TODO: Scale sliders
            // Determine pitch target
            tf = -1;
            tf2 = 0;
            tf3 = 0;
            for (ti=0; ti<12; ti++)
            {
              switch (ti)
              {
                case 0:   tf2 = fA;   break;
                case 1:   tf2 = fBb;  break;
                case 2:   tf2 = fB;   break;
                case 3:   tf2 = fC;   break;
                case 4:   tf2 = fDb;  break;
                case 5:   tf2 = fD;   break;
                case 6:   tf2 = fEb;  break;
                case 7:   tf2 = fE;   break;
                case 8:   tf2 = fF;   break;
                case 9:   tf2 = fGb;  break;
                case 10:  tf2 = fG;   break;
                case 11:  tf2 = fAb;  break;
              }
              // 	  if (ti==psAutotalent->ptarget) {
              // 	    tf2 = tf2 + 0.01; // add a little hysteresis
              // 	  }
              tf2 = tf2 - (float)fabs( (pitch-(float)ti)/6 - 2*floorf(((pitch-(float)ti)/12 + 0.5)) ); // like a likelihood function
              if (tf2>=tf)
              {                                                                           // that we're maximizing
                tf3 = (float)ti;                                                                       // to find the target pitch
                tf = tf2;
              }
            }
            ptarget = tf3;
            // Glide persist
            if (wasvoiced == 0)
            {
              wasvoiced = 1;
              tf = persistamt;
              sptarget = (1-tf)*ptarget + tf*sptarget;
              persistamt = 1;
            }
            // Glide on circular scale
            tf3 = (float)ptarget - sptarget;
            tf3 = tf3 - (float)12*floorf(tf3/12 + 0.5);
            if (fGlide>0) { tf2 = (float)1-pow((float)1/24, (float)N * 1000/ (noverlap*fs*fGlide)); }
            else { tf2 = 1; }
            sptarget = sptarget + tf3*tf2;
          }
          // If not voiced
          else
          {
            wasvoiced = 0;
            // Keep track of persist amount
            if (fPersist>0) { tf = pow((float)1/2, (float)N * 1000/ (noverlap*fs*fPersist)); }
            else { tf = 0; }
            persistamt = persistamt * tf; // Persist amount decays exponentially
          }

          // determine correction to feed to the pitch shifter

          tf = sptarget - pitch; // Correction amount
          tf = tf - (float)12*floorf(tf/12 + 0.5); // Never do more than +- 6 semitones of correction
          if (conf<vthresh) { tf = 0; }
          lrshift = fShift + fAmount*tf;  // Add in pitch shift slider
          // ---- Compute variables for pitch shifter that depend on pitch ---
          phincfact = (float)pow(2, lrshift/12);
          if (conf>=vthresh)
          {  // Keep old period when unvoiced
            phinc = (float)1/(pperiod*fs);
            phprd = pperiod*2;
          }
        }

        // Pitch Shifter

        // TODO: Pre-filter with some kind of filter (maybe cheby2 or just svf)
        // TODO: Use cubic spline interpolation
        // IMPROVE QUALITY OF PITCH SHIFTER!
        // what is the glitch at "lAaAack"? probably pitch shifter
        //   Better snippet management
        //   Pre-filter
        //   Cubic spline interp
        // Pitch shifter (overlap-add, pitch synchronous)
        //   Note: pitch estimate is naturally N/2 samples old

        phasein = phasein + phinc;
        phaseout = phaseout + phinc*phincfact;

        //   If it happens that there are no snippets placed at the output, grab a new snippet!
        //     if (psAutotalent->cbonorm[((long int)psAutotalent->cbord + (long int)(N/2*(1 - (float)1 / psAutotalent->phincfact)))%N] < 0.2) {
        //       fprintf(stderr, "help!");
        //       psAutotalent->phasein = 1;
        //       psAutotalent->phaseout = 1;
        //     }
        //   When input phase resets, take a snippet from N/2 samples in the past

        if (phasein >= 1)
        {
          phasein = phasein - 1;
          ti2 = cbiwr - (long int)N/2;
          for (ti=-((long int)N)/2; ti<(long int)N/2; ti++) { frag[ti%N] = cbi[(ti + ti2)%N]; }
        }
        //   When output phase resets, put a snippet N/2 samples in the future
        if (phaseout >= 1)
        {
          fragsize = fragsize*2;
          if (fragsize >= N) { fragsize = N; }
          phaseout = phaseout - 1;
          ti2 = cbord + N/2;
          ti3 = (long int)(((float)fragsize) / phincfact);
          for (ti=-ti3/2; ti<(ti3/2); ti++)
          {
            tf = hannwindow[(long int)N/2 + ti*(long int)N/ti3];
            cbo[(ti + ti2)%N] = cbo[(ti + ti2)%N] + frag[((int)(phincfact*ti))%N]*tf;
            cbonorm[(ti + ti2)%N] = cbonorm[(ti + ti2)%N] + tf;
          }
          fragsize = 0;
        }
        fragsize++;
        //   Get output signal from buffer
        tf = cbonorm[cbord];
        //   Normalize
        if (tf>0.5) { tf = (float)1/tf; }
        else { tf = 1; }
        tf = tf*cbo[cbord]; // read buffer
        tf = cbo[cbord];
        cbo[cbord] = 0; // erase for next cycle
        cbonorm[cbord] = 0;
        cbord++; // increment read pointer
        if (cbord >= N) { cbord = 0; }
        // Write audio to output of plugin
        // Mix (blend between original (delayed) =0 and shifted/corrected =1)

//        *(pfOutput++) = fMix*tf + (1-fMix)*cbi[(cbiwr - N + 1)%N];
        float out = fMix*tf + (1-fMix)*cbi[(cbiwr - N + 1)%N];
        *outs[0] = out;
        *outs[1] = out;
        //*outs[0] = spl0;
        //*outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
