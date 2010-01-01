/*
struct fft_vas
struct Autotalent

fft_vars* fft_con(int nfft)
void fft_des(fft_vars* membvars)
void fft_forward(fft_vars* membvars, float* input, float* output_re, float* output_im)
void fft_inverse(fft_vars* membvars, float* input_re, float* input_im, float* output)

LADSPA_Handle instantiateAutotalent(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate)
void connectPortToAutotalent(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
void runAutotalent(LADSPA_Handle Instance, unsigned long SampleCount)
void cleanupAutotalent(LADSPA_Handle Instance)
void deleteDescriptor(LADSPA_Descriptor * psDescriptor)
const LADSPA_Descriptor* ladspa_descriptor(unsigned long Index)

void _init()
void _fini()


*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

#include "ladspa.h"

//----------------------------------------------------------------------
//
// FFT
//
//----------------------------------------------------------------------

#include "mayer_fft.h"

// Variables for FFT routine

typedef struct
{
  int     nfft;        // size of FFT
  int     numfreqs;    // number of frequencies represented (nfft/2 + 1)
  float*  fft_data; // array for writing/reading to/from FFT function
} fft_vars;

//----------------------------------------------------------------------

// Constructor for FFT routine

fft_vars* fft_con(int nfft)
{
  fft_vars* membvars  = (fft_vars*) malloc(sizeof(fft_vars));
  membvars->nfft      = nfft;
  membvars->numfreqs  = nfft/2 + 1;
  membvars->fft_data  = (float*)calloc(nfft, sizeof(float));
  return membvars;
}

//----------------------------------------------------------------------

// Destructor for FFT routine

void fft_des(fft_vars* membvars)
{
  free(membvars->fft_data);
  free(membvars);
}

//----------------------------------------------------------------------

// Perform forward FFT of real data
// Accepts:
//   membvars - pointer to struct of FFT variables
//   input - pointer to an array of (real) input values, size nfft
//   output_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   output_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1

void fft_forward(fft_vars* membvars, float* input, float* output_re, float* output_im)
{
  int ti;
  int nfft;
  int hnfft;
  int numfreqs;
  nfft = membvars->nfft;
  hnfft = nfft/2;
  numfreqs = membvars->numfreqs;
  for (ti=0; ti<nfft; ti++) { membvars->fft_data[ti] = input[ti]; }
  mayer_realfft(nfft, membvars->fft_data);
  output_im[0] = 0;
  for (ti=0; ti<hnfft; ti++)
  {
    output_re[ti]   = membvars->fft_data[ti];
    output_im[ti+1] = membvars->fft_data[nfft-1-ti];
  }
  output_re[hnfft] = membvars->fft_data[hnfft];
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

void fft_inverse(fft_vars* membvars, float* input_re, float* input_im, float* output)
{
  int   ti;
  int   nfft;
  int   hnfft;
  int   numfreqs;
  nfft      = membvars->nfft;
  hnfft     = nfft/2;
  numfreqs  = membvars->numfreqs;
  for (ti=0; ti<hnfft; ti++)
  {
    membvars->fft_data[ti] = input_re[ti];
    membvars->fft_data[nfft-1-ti] = input_im[ti+1];
  }
  membvars->fft_data[hnfft] = input_re[hnfft];
  mayer_realifft(nfft, membvars->fft_data);
  for (ti=0; ti<nfft; ti++) { output[ti] = membvars->fft_data[ti]; }
}

//----------------------------------------------------------------------
//
// AutoTalent
//
//----------------------------------------------------------------------

// The port numbers

#define AT_MIX      0
#define AT_SHIFT    1
#define AT_TUNE     2
#define AT_A        3
#define AT_Bb       4
#define AT_B        5
#define AT_C        6
#define AT_Db       7
#define AT_D        8
#define AT_Eb       9
#define AT_E        10
#define AT_F        11
#define AT_Gb       12
#define AT_G        13
#define AT_Ab       14
#define AT_AMOUNT   15
#define AT_GLIDE    16
#define AT_INPUT1   17
#define AT_OUTPUT1  18

// THE MEMBER VARIABLES

typedef struct
{
  LADSPA_Data*  m_pfMix;
  LADSPA_Data*  m_pfShift;
  LADSPA_Data*  m_pfTune;
  LADSPA_Data*  m_pfA;
  LADSPA_Data*  m_pfBb;
  LADSPA_Data*  m_pfB;
  LADSPA_Data*  m_pfC;
  LADSPA_Data*  m_pfDb;
  LADSPA_Data*  m_pfD;
  LADSPA_Data*  m_pfEb;
  LADSPA_Data*  m_pfE;
  LADSPA_Data*  m_pfF;
  LADSPA_Data*  m_pfGb;
  LADSPA_Data*  m_pfG;
  LADSPA_Data*  m_pfAb;
  LADSPA_Data*  m_pfAmount;
  LADSPA_Data*  m_pfGlide;
  LADSPA_Data*  m_pfInputBuffer1;
  LADSPA_Data*  m_pfOutputBuffer1;
  fft_vars*     fmembvars;    // member variables for fft routine
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
} Autotalent;

//----------------------------------------------------------------------

// THE CONSTRUCTOR

LADSPA_Handle instantiateAutotalent(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate)
{
  unsigned long ti;
  Autotalent* membvars  = malloc(sizeof(Autotalent));
  membvars->aref        = 440;
  membvars->fs          = SampleRate;
  if (SampleRate>=88200) { membvars->cbsize = 4096; }
  else { membvars->cbsize = 2048; }
  membvars->corrsize    = membvars->cbsize / 2 + 1;
  membvars->pmax        = 1/(float)70;  // max and min periods (ms)
  membvars->pmin        = 1/(float)700; // eventually may want to bring these out as sliders
  membvars->pperiod     = membvars->pmax;
  membvars->nmax        = (unsigned long)(SampleRate * membvars->pmax);
  if (membvars->nmax > membvars->corrsize) { membvars->nmax = membvars->corrsize; }
  membvars->nmin        = (unsigned long)(SampleRate * membvars->pmin);
  membvars->cbi         = calloc(membvars->cbsize, sizeof(float));
  membvars->cbo         = calloc(membvars->cbsize, sizeof(float));
  membvars->cbonorm     = calloc(membvars->cbsize, sizeof(float));
  membvars->cbiwr       = 0;
  membvars->cbord       = 0;
  // Standard raised cosine window, max height at N/2
  membvars->hannwindow  = calloc(membvars->cbsize, sizeof(float));
  for (ti=0; ti<membvars->cbsize; ti++) { membvars->hannwindow[ti] = -0.5*cos(2*PI*ti/(membvars->cbsize - 1)) + 0.5; }
  // Generate a window with a single raised cosine from N/4 to 3N/4
  membvars->cbwindow    = calloc(membvars->cbsize, sizeof(float));
  for (ti=0; ti<(membvars->cbsize / 2); ti++) { membvars->cbwindow[ti+membvars->cbsize/4] = -0.5*cos(4*PI*ti/(membvars->cbsize - 1)) + 0.5; }
  membvars->noverlap    = 4;
  membvars->fmembvars   = fft_con(membvars->cbsize);
  membvars->ffttime     = calloc(membvars->cbsize, sizeof(float));
  membvars->fftfreqre   = calloc(membvars->corrsize, sizeof(float));
  membvars->fftfreqim   = calloc(membvars->corrsize, sizeof(float));
  // ---- Calculate autocorrelation of window ----
  membvars->acwinv      = calloc(membvars->cbsize, sizeof(float));
  for (ti=0; ti<membvars->cbsize; ti++) { membvars->ffttime[ti] = membvars->cbwindow[ti]; }
  fft_forward(membvars->fmembvars, membvars->cbwindow, membvars->fftfreqre, membvars->fftfreqim);
  for (ti=0; ti<membvars->corrsize; ti++)
  {
    membvars->fftfreqre[ti] = (membvars->fftfreqre[ti])*(membvars->fftfreqre[ti]) + (membvars->fftfreqim[ti])*(membvars->fftfreqim[ti]);
    membvars->fftfreqim[ti] = 0;
  }
  fft_inverse(membvars->fmembvars, membvars->fftfreqre, membvars->fftfreqim, membvars->ffttime);
  for (ti=1; ti<membvars->cbsize; ti++)
  {
    membvars->acwinv[ti] = membvars->ffttime[ti]/membvars->ffttime[0];
    if (membvars->acwinv[ti] > 0.000001) { membvars->acwinv[ti] = (float)1/membvars->acwinv[ti]; }
    else { membvars->acwinv[ti] = 0; }
  }
  membvars->acwinv[0]     = 1;
  // ---- END Calculate autocorrelation of window ----
  membvars->lrshift       = 0;
  membvars->ptarget       = 0;
  membvars->sptarget      = 0;
  membvars->wasvoiced     = 0;
  membvars->persistamt    = 0;
  membvars->glidepersist  = 100; // 100 ms glide persist
  membvars->vthresh       = 0.8;  //  The voiced confidence (unbiased peak) threshold level
  // Pitch shifter initialization
  membvars->phprdd        = 0.01; // Default period
  membvars->phprd         = membvars->phprdd;
  membvars->phinc         = (float)1/(membvars->phprd * SampleRate);
  membvars->phincfact     = 1;
  membvars->phasein       = 0;
  membvars->phaseout      = 0;
  membvars->frag          = calloc(membvars->cbsize, sizeof(float));
  membvars->fragsize      = 0;
  return membvars;
}

//----------------------------------------------------------------------

//  Connect port

void connectPortToAutotalent(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
  Autotalent * psAutotalent;
  psAutotalent = (Autotalent *)Instance;
  switch (Port)
  {
    case AT_MIX:      psAutotalent->m_pfMix           = DataLocation;   break;
    case AT_SHIFT:    psAutotalent->m_pfShift         = DataLocation;   break;
    case AT_TUNE:     psAutotalent->m_pfTune          = DataLocation;   break;
    case AT_A:        psAutotalent->m_pfA             = DataLocation;   break;
    case AT_Bb:       psAutotalent->m_pfBb            = DataLocation;   break;
    case AT_B:        psAutotalent->m_pfB             = DataLocation;   break;
    case AT_C:        psAutotalent->m_pfC             = DataLocation;   break;
    case AT_Db:       psAutotalent->m_pfDb            = DataLocation;   break;
    case AT_D:        psAutotalent->m_pfD             = DataLocation;   break;
    case AT_Eb:       psAutotalent->m_pfEb            = DataLocation;   break;
    case AT_E:        psAutotalent->m_pfE             = DataLocation;   break;
    case AT_F:        psAutotalent->m_pfF             = DataLocation;   break;
    case AT_Gb:       psAutotalent->m_pfGb            = DataLocation;   break;
    case AT_G:        psAutotalent->m_pfG             = DataLocation;   break;
    case AT_Ab:       psAutotalent->m_pfAb            = DataLocation;   break;
    case AT_AMOUNT:   psAutotalent->m_pfAmount        = DataLocation;   break;
    case AT_GLIDE:    psAutotalent->m_pfGlide         = DataLocation;   break;
    case AT_INPUT1:   psAutotalent->m_pfInputBuffer1  = DataLocation;   break;
    case AT_OUTPUT1:  psAutotalent->m_pfOutputBuffer1 = DataLocation;   break;
  }
}

//----------------------------------------------------------------------

// Called every time we get a new chunk of audio

void runAutotalent(LADSPA_Handle Instance, unsigned long SampleCount)
{
  LADSPA_Data*  pfInput;
  LADSPA_Data*  pfOutput;
  float         fMix;
  float         fShift;
  float         fTune;
  float         fA;
  float         fBb;
  float         fB;
  float         fC;
  float         fDb;
  float         fD;
  float         fEb;
  float         fE;
  float         fF;
  float         fGb;
  float         fG;
  float         fAb;
  float         fGlide;
  float         fPersist;
  float         fAmount;
  Autotalent*   psAutotalent;
  unsigned long lSampleIndex;
  unsigned long N;
  unsigned long Nf;
  unsigned long fs;
  float         pmin;
  float         pmax;
  unsigned long nmin;
  unsigned long nmax;
  long int      ti;
  long int      ti2;
  long int      ti3;
  float         tf;
  float         tf2;
  float         tf3;
  float         pperiod;
  float         pitch;
  float         conf;
  float         aref;

  psAutotalent  = (Autotalent *)Instance;
  pfInput       = psAutotalent->m_pfInputBuffer1;
  pfOutput      = psAutotalent->m_pfOutputBuffer1;
  fMix          = (float) *(psAutotalent->m_pfMix);
  fShift        = (float) *(psAutotalent->m_pfShift);
  fTune         = (float) *(psAutotalent->m_pfTune);
  fA            = (float) *(psAutotalent->m_pfA);
  fBb           = (float) *(psAutotalent->m_pfBb);
  fB            = (float) *(psAutotalent->m_pfB);
  fC            = (float) *(psAutotalent->m_pfC);
  fDb           = (float) *(psAutotalent->m_pfDb);
  fD            = (float) *(psAutotalent->m_pfD);
  fEb           = (float) *(psAutotalent->m_pfEb);
  fE            = (float) *(psAutotalent->m_pfE);
  fF            = (float) *(psAutotalent->m_pfF);
  fGb           = (float) *(psAutotalent->m_pfGb);
  fG            = (float) *(psAutotalent->m_pfG);
  fAb           = (float) *(psAutotalent->m_pfAb);
  fAmount       = (float) *(psAutotalent->m_pfAmount);
  fGlide        = (float) *(psAutotalent->m_pfGlide);
  fPersist      = psAutotalent->glidepersist;
  psAutotalent->aref = (float)440*pow(2,fTune/12);

  N             = psAutotalent->cbsize;
  Nf            = psAutotalent->corrsize;
  fs            = psAutotalent->fs;
  pmax          = psAutotalent->pmax;
  pmin          = psAutotalent->pmin;
  nmax          = psAutotalent->nmax;
  nmin          = psAutotalent->nmin;
  aref          = psAutotalent->aref;
  pperiod       = psAutotalent->pperiod;
  pitch         = psAutotalent->pitch;
  conf          = psAutotalent->conf;

  // MAIN DSP LOOP

  for (lSampleIndex=0; lSampleIndex<SampleCount; lSampleIndex++)
  {

    // load data into circular buffer

    tf = (float) *(pfInput++);
    psAutotalent->cbi[psAutotalent->cbiwr] = tf;
    psAutotalent->cbiwr++;
    if (psAutotalent->cbiwr >= N) { psAutotalent->cbiwr = 0; }

    // Low-rate section

    // Every N/noverlap samples, run pitch estimation / correction code
    if ((psAutotalent->cbiwr)%(N/psAutotalent->noverlap) == 0)
    {

      // Obtain autocovariance

      // Window and fill FFT buffer
      ti2 = psAutotalent->cbiwr;
      for (ti=0; ti<N; ti++) { psAutotalent->ffttime[ti] = (float)(psAutotalent->cbi[(ti2-ti)%N]*psAutotalent->cbwindow[ti]); }
      // Calculate FFT
      fft_forward(psAutotalent->fmembvars, psAutotalent->ffttime, psAutotalent->fftfreqre, psAutotalent->fftfreqim);

      // Remove DC

      psAutotalent->fftfreqre[0] = 0;
      psAutotalent->fftfreqim[0] = 0;

      // Take magnitude squared

      for (ti=1; ti<Nf; ti++)
      {
        psAutotalent->fftfreqre[ti] = (psAutotalent->fftfreqre[ti])*(psAutotalent->fftfreqre[ti]) + (psAutotalent->fftfreqim[ti])*(psAutotalent->fftfreqim[ti]);
        psAutotalent->fftfreqim[ti] = 0;
      }
      // Calculate IFFT
      fft_inverse(psAutotalent->fmembvars, psAutotalent->fftfreqre, psAutotalent->fftfreqim, psAutotalent->ffttime);
      // Normalize
      for (ti=1; ti<N; ti++) { psAutotalent->ffttime[ti] = psAutotalent->ffttime[ti] / psAutotalent->ffttime[0]; }
      psAutotalent->ffttime[0] = 1;

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
        tf = psAutotalent->ffttime[ti];
        if (tf>psAutotalent->ffttime[ti2] && tf>=psAutotalent->ffttime[ti3] && tf>tf2)
        {
          tf2 = tf;
          conf = tf*psAutotalent->acwinv[ti];
          pperiod = (float)ti/fs;
        }
      }

      // Convert to semitones

      pitch = (float) -12*log10((float)aref*pperiod)*L2SC;
      psAutotalent->pitch = pitch;
      psAutotalent->pperiod = pperiod;
      psAutotalent->conf = conf;

      // Determine pitch target

      // If voiced
      if (conf>=psAutotalent->vthresh)
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
          /* 	  if (ti==psAutotalent->ptarget) { */
          /* 	    tf2 = tf2 + 0.01; // add a little hysteresis */
          /* 	  } */
          tf2 = tf2 - (float)fabs( (pitch-(float)ti)/6 - 2*floorf(((pitch-(float)ti)/12 + 0.5)) ); // like a likelihood function
          if (tf2>=tf)
          {                                                                           // that we're maximizing
            tf3 = (float)ti;                                                                       // to find the target pitch
            tf = tf2;
          }
        }
        psAutotalent->ptarget = tf3;
        // Glide persist
        if (psAutotalent->wasvoiced == 0)
        {
          psAutotalent->wasvoiced = 1;
          tf = psAutotalent->persistamt;
          psAutotalent->sptarget = (1-tf)*psAutotalent->ptarget + tf*psAutotalent->sptarget;
          psAutotalent->persistamt = 1;
        }
        // Glide on circular scale
        tf3 = (float)psAutotalent->ptarget - psAutotalent->sptarget;
        tf3 = tf3 - (float)12*floorf(tf3/12 + 0.5);
        if (fGlide>0) { tf2 = (float)1-pow((float)1/24, (float)N * 1000/ (psAutotalent->noverlap*fs*fGlide)); }
        else { tf2 = 1; }
        psAutotalent->sptarget = psAutotalent->sptarget + tf3*tf2;
      }
      // If not voiced
      else
      {
        psAutotalent->wasvoiced = 0;
        // Keep track of persist amount
        if (fPersist>0) { tf = pow((float)1/2, (float)N * 1000/ (psAutotalent->noverlap*fs*fPersist)); }
        else { tf = 0; }
        psAutotalent->persistamt = psAutotalent->persistamt * tf; // Persist amount decays exponentially
      }

      // determine correction to feed to the pitch shifter

      tf = psAutotalent->sptarget - pitch; // Correction amount
      tf = tf - (float)12*floorf(tf/12 + 0.5); // Never do more than +- 6 semitones of correction
      if (conf<psAutotalent->vthresh) { tf = 0; }
      psAutotalent->lrshift = fShift + fAmount*tf;  // Add in pitch shift slider
      // ---- Compute variables for pitch shifter that depend on pitch ---
      psAutotalent->phincfact = (float)pow(2, psAutotalent->lrshift/12);
      if (conf>=psAutotalent->vthresh)
      {  // Keep old period when unvoiced
        psAutotalent->phinc = (float)1/(pperiod*fs);
        psAutotalent->phprd = pperiod*2;
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

    psAutotalent->phasein = psAutotalent->phasein + psAutotalent->phinc;
    psAutotalent->phaseout = psAutotalent->phaseout + psAutotalent->phinc*psAutotalent->phincfact;

    //   If it happens that there are no snippets placed at the output, grab a new snippet!
    /*     if (psAutotalent->cbonorm[((long int)psAutotalent->cbord + (long int)(N/2*(1 - (float)1 / psAutotalent->phincfact)))%N] < 0.2) { */
    /*       fprintf(stderr, "help!"); */
    /*       psAutotalent->phasein = 1; */
    /*       psAutotalent->phaseout = 1; */
    /*     } */
    //   When input phase resets, take a snippet from N/2 samples in the past

    if (psAutotalent->phasein >= 1)
    {
      psAutotalent->phasein = psAutotalent->phasein - 1;
      ti2 = psAutotalent->cbiwr - (long int)N/2;
      for (ti=-((long int)N)/2; ti<(long int)N/2; ti++) { psAutotalent->frag[ti%N] = psAutotalent->cbi[(ti + ti2)%N]; }
    }
    //   When output phase resets, put a snippet N/2 samples in the future
    if (psAutotalent->phaseout >= 1)
    {
      psAutotalent->fragsize = psAutotalent->fragsize*2;
      if (psAutotalent->fragsize >= N) { psAutotalent->fragsize = N; }
      psAutotalent->phaseout = psAutotalent->phaseout - 1;
      ti2 = psAutotalent->cbord + N/2;
      ti3 = (long int)(((float)psAutotalent->fragsize) / psAutotalent->phincfact);
      for (ti=-ti3/2; ti<(ti3/2); ti++)
      {
        tf = psAutotalent->hannwindow[(long int)N/2 + ti*(long int)N/ti3];
        psAutotalent->cbo[(ti + ti2)%N] = psAutotalent->cbo[(ti + ti2)%N] + psAutotalent->frag[((int)(psAutotalent->phincfact*ti))%N]*tf;
        psAutotalent->cbonorm[(ti + ti2)%N] = psAutotalent->cbonorm[(ti + ti2)%N] + tf;
      }
      psAutotalent->fragsize = 0;
    }
    psAutotalent->fragsize++;
    //   Get output signal from buffer
    tf = psAutotalent->cbonorm[psAutotalent->cbord];
    //   Normalize
    if (tf>0.5) { tf = (float)1/tf; }
    else { tf = 1; }
    tf = tf*psAutotalent->cbo[psAutotalent->cbord]; // read buffer
    tf = psAutotalent->cbo[psAutotalent->cbord];
    psAutotalent->cbo[psAutotalent->cbord] = 0; // erase for next cycle
    psAutotalent->cbonorm[psAutotalent->cbord] = 0;
    psAutotalent->cbord++; // increment read pointer
    if (psAutotalent->cbord >= N) { psAutotalent->cbord = 0; }
    // Write audio to output of plugin
    // Mix (blend between original (delayed) =0 and shifted/corrected =1)
    *(pfOutput++) = (LADSPA_Data) fMix*tf + (1-fMix)*psAutotalent->cbi[(psAutotalent->cbiwr - N + 1)%N];
  }
}

//----------------------------------------------------------------------

// THE DESTRUCTOR

void cleanupAutotalent(LADSPA_Handle Instance)
{
  fft_des(((Autotalent*)Instance)->fmembvars);
  free(((Autotalent*)Instance)->cbi);
  free(((Autotalent*)Instance)->cbo);
  free(((Autotalent*)Instance)->cbonorm);
  free(((Autotalent*)Instance)->cbwindow);
  free(((Autotalent*)Instance)->hannwindow);
  free(((Autotalent*)Instance)->acwinv);
  free(((Autotalent*)Instance)->frag);
  free(((Autotalent*)Instance)->ffttime);
  free(((Autotalent*)Instance)->fftfreqre);
  free(((Autotalent*)Instance)->fftfreqim);
  free((Autotalent*)Instance);
}

//----------------------------------------------------------------------
//
// LADSPA
//
//----------------------------------------------------------------------

LADSPA_Descriptor* g_psDescriptor;

//----------

// Called when first loaded

void _init()
{
  char**                  pcPortNames;
  LADSPA_PortDescriptor*  piPortDescriptors;
  LADSPA_PortRangeHint*   psPortRangeHints;
  g_psDescriptor          = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
  if (g_psDescriptor)
  {
    g_psDescriptor->UniqueID                    = 1000;                        // CHANGE ME!
    g_psDescriptor->Label                       = strdup("autotalent");
    g_psDescriptor->Properties                  = LADSPA_PROPERTY_HARD_RT_CAPABLE;
    g_psDescriptor->Name                        = strdup("Autotalent");
    g_psDescriptor->Maker                       = strdup("Thomas A. Baran");
    g_psDescriptor->Copyright                   = strdup("2009");
    g_psDescriptor->PortCount                   = 19;
    piPortDescriptors = (LADSPA_PortDescriptor *)calloc(19, sizeof(LADSPA_PortDescriptor));
    g_psDescriptor->PortDescriptors             = (const LADSPA_PortDescriptor *)piPortDescriptors;
    piPortDescriptors[AT_MIX]                   = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_SHIFT]                 = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_TUNE]                  = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_A]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_Bb]                    = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_B]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_C]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_Db]                    = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_D]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_Eb]                    = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_E]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_F]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_Gb]                    = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_G]                     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_Ab]                    = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_AMOUNT]                = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_GLIDE]                 = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
    piPortDescriptors[AT_INPUT1]                = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
    piPortDescriptors[AT_OUTPUT1]               = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
    pcPortNames                                 = (char **)calloc(19, sizeof(char *));
    g_psDescriptor->PortNames                   = (const char **)pcPortNames;
    pcPortNames[AT_MIX]                         = strdup("Mix");
    pcPortNames[AT_SHIFT]                       = strdup("Output shift (semitones)");
    pcPortNames[AT_TUNE]                        = strdup("Input tune (semitones)");
    pcPortNames[AT_A]                           = strdup("A");
    pcPortNames[AT_Bb]                          = strdup("Bb");
    pcPortNames[AT_B]                           = strdup("B");
    pcPortNames[AT_C]                           = strdup("C");
    pcPortNames[AT_Db]                          = strdup("Db");
    pcPortNames[AT_D]                           = strdup("D");
    pcPortNames[AT_Eb]                          = strdup("Eb");
    pcPortNames[AT_E]                           = strdup("E");
    pcPortNames[AT_F]                           = strdup("F");
    pcPortNames[AT_Gb]                          = strdup("Gb");
    pcPortNames[AT_G]                           = strdup("G");
    pcPortNames[AT_Ab]                          = strdup("Ab");
    pcPortNames[AT_AMOUNT]                      = strdup("Correction");
    pcPortNames[AT_GLIDE]                       = strdup("Glide (ms)");
    pcPortNames[AT_INPUT1]                      = strdup("Input");
    pcPortNames[AT_OUTPUT1]                     = strdup("Output");
    psPortRangeHints                            = ((LADSPA_PortRangeHint *)calloc(19, sizeof(LADSPA_PortRangeHint)));
    g_psDescriptor->PortRangeHints              = (const LADSPA_PortRangeHint *)psPortRangeHints;
    psPortRangeHints[AT_MIX].HintDescriptor     = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_MIX].LowerBound         = 0;
    psPortRangeHints[AT_MIX].UpperBound         = 1;
    psPortRangeHints[AT_SHIFT].HintDescriptor   = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0);
    psPortRangeHints[AT_SHIFT].LowerBound       = -12;
    psPortRangeHints[AT_SHIFT].UpperBound       = 12;
    psPortRangeHints[AT_TUNE].HintDescriptor    = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0);
    psPortRangeHints[AT_TUNE].LowerBound        = -1;
    psPortRangeHints[AT_TUNE].UpperBound        = 1;
    psPortRangeHints[AT_A].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_A].LowerBound           = 0;
    psPortRangeHints[AT_A].UpperBound           = 1;
    psPortRangeHints[AT_Bb].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_Bb].LowerBound          = 0;
    psPortRangeHints[AT_Bb].UpperBound          = 1;
    psPortRangeHints[AT_B].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_B].LowerBound           = 0;
    psPortRangeHints[AT_B].UpperBound           = 1;
    psPortRangeHints[AT_C].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_C].LowerBound           = 0;
    psPortRangeHints[AT_C].UpperBound           = 1;
    psPortRangeHints[AT_Db].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_Db].LowerBound          = 0;
    psPortRangeHints[AT_Db].UpperBound          = 1;
    psPortRangeHints[AT_D].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_D].LowerBound           = 0;
    psPortRangeHints[AT_D].UpperBound           = 1;
    psPortRangeHints[AT_Eb].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_Eb].LowerBound          = 0;
    psPortRangeHints[AT_Eb].UpperBound          = 1;
    psPortRangeHints[AT_E].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_E].LowerBound           = 0;
    psPortRangeHints[AT_E].UpperBound           = 1;
    psPortRangeHints[AT_F].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_F].LowerBound           = 0;
    psPortRangeHints[AT_F].UpperBound           = 1;
    psPortRangeHints[AT_Gb].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_Gb].LowerBound          = 0;
    psPortRangeHints[AT_Gb].UpperBound          = 1;
    psPortRangeHints[AT_G].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_G].LowerBound           = 0;
    psPortRangeHints[AT_G].UpperBound           = 1;
    psPortRangeHints[AT_Ab].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_Ab].LowerBound          = 0;
    psPortRangeHints[AT_Ab].UpperBound          = 1;
    psPortRangeHints[AT_AMOUNT].HintDescriptor  = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_1);
    psPortRangeHints[AT_AMOUNT].LowerBound      = 0;
    psPortRangeHints[AT_AMOUNT].UpperBound      = 1;
    psPortRangeHints[AT_GLIDE].HintDescriptor   = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_0);
    psPortRangeHints[AT_GLIDE].LowerBound       = 0;
    psPortRangeHints[AT_GLIDE].UpperBound       = 500;
    psPortRangeHints[AT_INPUT1].HintDescriptor  = 0;
    psPortRangeHints[AT_OUTPUT1].HintDescriptor = 0;
    g_psDescriptor->instantiate                 = instantiateAutotalent;
    g_psDescriptor->connect_port                = connectPortToAutotalent;
    g_psDescriptor->activate                    = NULL;
    g_psDescriptor->run                         = runAutotalent;
    g_psDescriptor->run_adding                  = NULL;
    g_psDescriptor->set_run_adding_gain         = NULL;
    g_psDescriptor->deactivate                  = NULL;
    g_psDescriptor->cleanup                     = cleanupAutotalent;
  }
}

//----------------------------------------------------------------------

void deleteDescriptor(LADSPA_Descriptor * psDescriptor)
{
  unsigned long lIndex;
  if (psDescriptor)
  {
    free((char *)psDescriptor->Label);
    free((char *)psDescriptor->Name);
    free((char *)psDescriptor->Maker);
    free((char *)psDescriptor->Copyright);
    free((LADSPA_PortDescriptor *)psDescriptor->PortDescriptors);
    for (lIndex = 0; lIndex < psDescriptor->PortCount; lIndex++) free((char *)(psDescriptor->PortNames[lIndex]));
    free((char **)psDescriptor->PortNames);
    free((LADSPA_PortRangeHint *)psDescriptor->PortRangeHints);
    free(psDescriptor);
  }
}

//----------------------------------------------------------------------

// Called when library is unloaded

void _fini()
{
  deleteDescriptor(g_psDescriptor);
}

//----------------------------------------------------------------------

// Return the plugin descriptor (there's only one in this file)

const LADSPA_Descriptor* ladspa_descriptor(unsigned long Index)
{
  switch (Index)
  {
    case 0:   return g_psDescriptor;
    default:  return NULL;
  }
}

