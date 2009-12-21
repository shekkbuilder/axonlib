//This code still needs to be modified so that the input will be from
//the SRAM rather than a file. Also, all data will have to be converted
//to int or short type (instead of float).

#include <stdio.h>
#include <stdlib.h>

#define    FREQ 44100               /* sampling frequency */
#define    BLOCKSIZE 330                 /* size of one processing block */
#define    STEP 4                    /* */
#define    Delta 0.001              /* for derivative */

int main(void)
{
  FILE *fp, *fout;
  int i, j, index, length, position;
  int k = 0;
  float tempval;
  float *tempdata;
  float data[BLOCKSIZE];
  float  temp;
  float  output[BLOCKSIZE];
  float  diff[BLOCKSIZE];
  float  zerocross[BLOCKSIZE];
  float  maxima[BLOCKSIZE];
  char ch[BLOCKSIZE * STEP];
  int itr = 0;
  /* Read data from voice file */
  if ((fp = fopen("sample.txt", "r")) == NULL) fprintf(stderr, "Cannot open %s\n", "sample.txt");
  /* The whole voice data; Will be limited by the RAM size */
  tempdata = (float *) calloc(220500, sizeof(float));
  /* 220500 as (BLOCKSIZE * STEP)
  * One processing block.           */
  /* Write the voice file into tempdata */
  i = 0;
  if (fp != NULL)
  {
    while ((feof(fp) == 0) && (i < 10000000))
    {
      fscanf(fp, "%s", &ch);
      tempval = strtod(ch, NULL);
      tempdata[i] = tempval;
      i++;
    }
  }
  printf("%d\n",i);
  /* Return the total number of processing block */
  printf("\nThe length of the whole sample: %d\n", i-1);
  itr = 0;
  length = i - 1;
  while (itr < length)
  {
    /* Sample every STEP'th data from each BLOCKSIZE*STEP */
    for (i = 0; i < BLOCKSIZE; i++)
    {
      data[i] = tempdata[i * STEP + itr];
    }
    
    
    /* Processing begins */
    
    
    /* Half-autocorrelation
     * We only care about the distance from peak */
     
    for (i = 0; i < BLOCKSIZE; i++)
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
    
    /* First derivative and sign function */
    
    temp = 0;
    for (i = 0; i < (BLOCKSIZE - 1); i++)
    {
      diff[i] = 0;
      diff[i] = (output[i + 1] - output[i]) / Delta;
      if (diff[i] > 0) diff[i] = 1;
      else if (diff[i] < 0) diff[i] = -1;
    }
    diff[BLOCKSIZE - 1] = 0;
    
    /* Zero Crossing, removing positive ones, flipping x-axis,
     * and impulses at maxima with magnitude values */
     
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
    
    /* Need to bring it back to correct sampling rate */
    
    position = index * STEP;
    zerocross[BLOCKSIZE - 1] = 0;
    maxima[BLOCKSIZE - 1] = zerocross[BLOCKSIZE - 1] * output[BLOCKSIZE - 1];
    
    printf("\n*** %d'th Pitch ***\n", ++k);
    printf("Pitch is at %d'th in the Maxima array\n\n", index);
    printf("The value at that index: %e\n\n", maxima[index]);
    printf("The (%d Hz / Index) is: %f\n\n", FREQ, FREQ/(double)position);
    itr = itr + BLOCKSIZE * STEP;
    fout = fopen("pitch_result.txt", "a");
    fprintf(fout, "%f\n", FREQ / (double)position);
    fclose(fout);
  }
  printf("\nWe've detected %d pitches so far!\n\n", k);
  fclose(fp);
  free(tempdata);
  return 0;
}

