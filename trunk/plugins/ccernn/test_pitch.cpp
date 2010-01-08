#define AX_PLUGIN     myPlugin
#define AX_NUMPROGS   1
#define AX_NUMPARAMS  1
//#define AX_AUTOSYNC
//#define AX_WIDTH      320
//#define AX_HEIGHT     240
//#define AX_FLAGS      (AX_EMBEDDED|AX_BUFFERED)
#define AX_DEBUG
#include "axDebug.h"
#include "axPlugin.h"
#include "parFloat.h"
//#include "parInteger.h"
//#include "axEditor.h"
//#include "wdgLabel.h"
//#include "wdgKnobPar.h"

//----------------------------------------------------------------------

#define    FREQ 44100               /* sampling frequency */
#define    BLOCKSIZE 4096           /* size of one processing block */
//#define    STEP 4                 /* */
#define    Delta (1.0f/1024.0f)
//0.001              /* for derivative */

//----------------------------------------------------------------------

class myPlugin : public axPlugin
{
  public:
    //axEditor* mEditor;
    float     mValue;
    parFloat* pValue;
    int pos;
    //
    int     i,j,index,length,position;
    float   tempval;
    float*  tempdata;
    float   data[BLOCKSIZE];
    float   temp;
    float   output[BLOCKSIZE];
    float   diff[BLOCKSIZE];
    float   zerocross[BLOCKSIZE];
    float   maxima[BLOCKSIZE];
//    char    ch[BLOCKSIZE * STEP];
//    int     k;// = 0;
//    int     itr;// = 0;


  public:

    myPlugin(audioMasterCallback audioMaster, int aNumProgs, int aNumParams, int aPlugFlags)
    : axPlugin(audioMaster,aNumProgs,aNumParams,aPlugFlags)
      {
//        k = 0;
//        itr = 0;
        pos = 0;
        //mEditor = NULL;
        describe("test_pitch","ccernn","product_string",0,0);
        //hasEditor(AX_WIDTH,AX_HEIGHT);
        //isSynth();
        appendParameter(pValue = new parFloat(this,0,"hz","",0 ));//, 0,44100 ));
        processParameters();
      }

    //----------

    //virtual ~myPlugin()
    //  {
    //  }

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
          case 0: mValue = f;  break;
        }
      }

    //--------------------------------------------------

    //virtual bool doProcessBlock(float** inputs, float** outputs, long sampleFrames)
    //  {
    //    //updateTimeInfo(); // AX_AUTOSYNC
    //    return false;
    //  }

    //----------

    void find_pitch(void)
      {

        // Half-autocorrelation
        // We only care about the distance from peak

        for (i=0; i<BLOCKSIZE; i++)
        {
          j = 0;
          output[i] = 0;
          while ((j+i)<BLOCKSIZE)
          {
            temp = data[j] * data[j + i];
            output[i] = output[i] + temp;
            j = j + 1;
          }
        }

        // First derivative and sign function

        temp = 0;
        for (i=0; i<(BLOCKSIZE-1); i++)
        {
          diff[i] = 0;
          diff[i] = (output[i + 1] - output[i]) / Delta;
          if (diff[i] > 0) diff[i] = 1;
          else if (diff[i] < 0) diff[i] = -1;
        }
        diff[BLOCKSIZE - 1] = 0;

        // Zero Crossing, removing positive ones, flipping x-axis,
        // and impulses at maxima with magnitude values

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

        // Need to bring it back to correct sampling rate

//        position = index / BLOCKSIZE;// * STEP;
        zerocross[BLOCKSIZE - 1] = 0;
        maxima[BLOCKSIZE - 1] = zerocross[BLOCKSIZE - 1] * output[BLOCKSIZE - 1];

        //    printf("\n*** %d'th Pitch ***\n", ++k);
        //    printf("Pitch is at %d'th in the Maxima array\n\n", index);
        //    printf("The value at that index: %e\n\n", maxima[index]);
        //    printf("The (%d Hz / Index) is: %f\n\n", FREQ, FREQ/(double)position);
        float hz = /*(float)FREQ * */ ( (float)BLOCKSIZE / (float)index );
        //hz = FREQ / (3*hz);

        //TRACE("hz: %f\n",hz);
        pValue->doSetValue(hz);
//        itr = itr + BLOCKSIZE * STEP;
        //    fout = fopen("pitch_result.txt", "a");
        //    fprintf(fout, "%f\n", FREQ / (double)position);
        //    fclose(fout);
      }

    virtual void doProcessSample(float** ins, float** outs)
      {
        float spl0 = *ins[0];
        float spl1 = *ins[1];
        data[pos] = spl0;
        pos++;
        if (pos>=BLOCKSIZE)
        {
          find_pitch();
          pos = 0;
        }
        *outs[0] = spl0;
        *outs[1] = spl1;
      }

};

//----------------------------------------------------------------------
#include "axMain.h"
