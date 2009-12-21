#ifndef dspPitchDetect_included
#define dspPitchDetect_included
//----------------------------------------------------------------------

/*

Finalized Algorithm

     The first step is to take in the sound wave. The A/D
will take a signal from the microphone and output a 16bit
digital signal at 11kHz, which will be shifted into a shift
register the size of one block (330 samples). Every 330
samples, the contents of the shift register will be copied
into a buffer. The contents in this buffer are what we run
our algorithm on to determine a single value on the output
of the pitch graph.
     To run the algorithm, the first step is to perform a
half-autocorrelation on the data set in the input buffer.
The result of this autocorrelation will allow us to
determine the distance between the two peaks in a time block
because they will correspond to the two largest peaks in the
autocorrelation. Performing this process one the data from
the buffer will leave us with a new set of 330 points of
data, the “half-autocorrelation graph”. This data is then
processed as follows to find the distance between the peaks.

  1. Take the derivative of the data.
  2. Perform the sign function on the data from step 1.
  3. Take the derivative of data from step 2 and multiply by
     -1.
  4. Remove negative zero crossings.
  5. Multiply results from step 4 with data in half-
     autocorrelation graph one data point at a time and
     store in new buffer (or same buffer as step 4 to save
     space.)
  6. (May not be necessary:) Remove first few points from
     result in step 5.
  7. Find the first peak and record its index in the buffer.
  8. Use the index from step 7 and the block specifications
     to find the distance between nearest peaks and
     calculate value in time (period) and corresponding
     frequency (pitch):
     Period = T = (index / (samples per block)) * (block
     time),
     Pitch = f = 1 / T
  9. Output the pitch

Software Design

We used C to process the data that was passed to the
Microblaze from the SRAM. This process contained several
steps:

  1. Perform half-autocorrelation of data.
  2. Take the first derivative.
  3. Classify them into {-1, 0, 1} as the sign.
  4. Find out zero-crossing.
  5. Remove positive one’s.
  6. Flip the sign.
  7. Multiply them by the data from step 1.
  8. Find the maximum value and its location.
  9. Return the pitch of the processing block.
        a. (Sampling frequency) / (STEP * pitch location)

The C program proves to work like the Matlab program, but is
currently implemented using floating point. It must be
modified to work with the data stored in the SRAM (non-
floating point data). We need to reduce the input size of
the data by dividing it by a constant to ensure that
overflow of output data from half-autocorrelation does not
occur.

Block Size Calculation

     In order to calculate the pitch, we need at least two
peaks to be within the block we are measuring the pitch of.
Given that the block will not always perfectly align with
the waveform, the only way we can ensure that at least two
peaks are within the block is to set the block size to three
wavelengths of the lowest possible frequency. As mentioned
earlier we chose the lowest possible frequency to be 100Hz.

     1/(100Hz) = 10ms,
     3*10ms    = 30ms,
     At a sampling frequency of 11kHz:
     (11kHz)*(30ms) = 330 samples,
      Block Size = 30ms, 330 samples.

Note: The actual sampling rate of the codec was 44kHz, so we
had to decimate the input to simulate 11kHz (1 out of every
4 samples).

*/

//#define    BLOCKSIZE  330     /* size of one processing block */
//#define    STEP       4       /* */

#define MAX_BUF_SIZE  65536

#define    FREQ       44100   /* sampling frequency */
#define    Delta      0.001   /* for derivative */

class dspPitchDetect
{
  protected:
    int   i,j;
    int   index, position;
//    int   length;
//    int   k;
//    int   itr;
    float temp;
    int BLOCKSIZE,STEP;
    //float data[BLOCKSIZE];
    float* output;//[BLOCKSIZE];
    float* diff;//[BLOCKSIZE];
    float* zerocross;//[BLOCKSIZE];
    float* maxima;//[BLOCKSIZE];
    //float tempval;
    //float *tempdata;
    //char  ch[BLOCKSIZE * STEP];

  public:

    void setup(void)
      {
        output = new float[MAX_BUF_SIZE];
        diff = new float[MAX_BUF_SIZE];
        zerocross = new float[MAX_BUF_SIZE];
        maxima = new float[MAX_BUF_SIZE];
      }

    //----------

    void cleanup(void)
      {
        delete[] output;
        delete[] diff;
        delete[] zerocross;
        delete[] maxima;
      }

    //----------

    void process(float* data, int size, int stp)
      {
        BLOCKSIZE = size;
        STEP = stp;



        // k = itr = 0;

        // Half-autocorrelation
        // We only care about the distance from peak

        // loop: BLOCKSIZE * 3
        // ------
        //  -----
        //   ----
        //    ---
        //     --
        //      -

        // 300+299+298, etc.. 1 = 300*(300/2) = 300*150 = 45000
        // 300*300 square = 90000
        // half (triangle) = 45000

        for (i=0; i<BLOCKSIZE; i++)
        {
          j = 0;
          output[i] = 0;
          while ((j + i) < BLOCKSIZE)
          {
            temp = data[j] * data[j + i];
            output[i] = output[i] + temp;
            j = j + 1;
          }
        }

        // First derivative and sign function

        // loop: BLOCKSIZE

        temp = 0;
        for (i=0; i<(BLOCKSIZE-1); i++)
        {
          diff[i] = 0;
          diff[i] = ( output[i+1] - output[i] ) / Delta;
          if (diff[i] > 0) diff[i] = 1;
          else if (diff[i] < 0) diff[i] = -1;
        }
        diff[BLOCKSIZE - 1] = 0;

        // Zero Crossing, removing positive ones, flipping x-axis,
        // and impulses at maxima with magnitude values


        // loop: BLOCKSIZE

        for (i = 0; i < (BLOCKSIZE - 1); i++)
        {
          zerocross[i] = 0;
          zerocross[i] = (diff[i + 1] - diff[i]) / Delta;
          if (zerocross[i] > 0) zerocross[i] = 0;
          else zerocross[i] = zerocross[i] * (-1.0);
          maxima[i] = zerocross[i] * output[i];
          if (maxima[i] > temp)
          {
            temp = maxima[i];
            index = i;
          }
        }

// temp = largest maxima
// index = index into buffers (pos)

        // Need to bring it back to correct sampling rate

        position = index * STEP;
        zerocross[BLOCKSIZE - 1] = 0;
        maxima[BLOCKSIZE - 1] = zerocross[BLOCKSIZE - 1] * output[BLOCKSIZE - 1];

        //printf("\n*** %d'th Pitch ***\n", ++k);
        //++k;
        //printf("Pitch is at %d'th in the Maxima array\n\n", index);
        //printf("The value at that index: %e\n\n", maxima[index]);
        //printf("The (%d Hz / Index) is: %f\n\n", FREQ, FREQ/(double)position);
        //itr = itr + BLOCKSIZE * STEP;
        //fout = fopen("pitch_result.txt", "a");
        //fprintf(fout, "%f\n", FREQ / (double)position);
        //fclose(fout);
      }

};

//----------------------------------------------------------------------
#endif

