//----------------------------------------------------------------------

//TODO:
// a) hope the compiler will optimize this (probably)
// b) 'real' memcpy/memset (macros?)
inline void copy_cells(int* dst, int* src, int num)
  {
    //for (int i=0;i<num;i++) { mCells[dst+i] = mCells[src+i]; }
    memcpy(dst,src,num*4);
  }

//----------

inline void clear_cells(int* dst, int num)
  {
    //for (int i=0;i<num;i++) { mCells[dst+i] = 0; }
    memset(dst,0,num*4);
  }

//----------------------------------------------------------------------

inline void do_UpdateGrid(void)
  {
    //trace("do_UpdateGrid");
    int width  = (int)mWidth;
    int height = (int)mHeight;
    //trace("   width: " << dec << width << " height: " << height);

    //--- wraparound

    int* cell  = mCells;
    //int* cell2 = mCells2;
    if (mWrap==1)
    {
      copy_cells(cell,cell+SIZEh,width+2);
      copy_cells(cell+SIZEh+SIZE,cell+SIZE,width+2);
    }
    else
    {
      clear_cells( cell, width+2);
      clear_cells( cell+SIZEh+SIZE,width+2);
    }
    for( int i=0; i<height+2; i++ )
    {
      if (mWrap)
      {
        cell[0] = cell[width];
        cell[width+1] = cell[1];
      }
      else
      {
        cell[0] = 0;
        cell[width+1] = 0;
      }
      cell += SIZE;
    }

    //--- update grid

    cell  = mCells+SIZE+1;
    int* cell2 = mCells2+SIZE+1;
    int y = 0;
    for (int ih=0; ih<height; ih++)
    {
      if (mWrap)
      {
        cell[0] = cell[width];
        cell[width+1] = cell[1];
      }
      else
      {
        cell[0] = 0;
        cell[width+1] = 0;
      }
      int x = 0;
      for (int iw=0; iw<width; iw++)
      {
        int n = *cell & 1;
        int neighbours = 0;
        switch (mUpdateMode)
        {
          case 0:
            neighbours += (cell[-SIZE-1] & 1);
            neighbours += (cell[-SIZE  ] & 1);
            neighbours += (cell[-SIZE+1] & 1);
            neighbours += (cell[     -1] & 1);
            neighbours += (cell[     +1] & 1);
            neighbours += (cell[+SIZE-1] & 1);
            neighbours += (cell[+SIZE  ] & 1);
            neighbours += (cell[+SIZE+1] & 1);
            // life: B3/S23
            if (n==0)
            { // B3
              if (neighbours==3) *cell2=1; else *cell2=0;
            }
            if (n==1)
            { // S23
              if (neighbours<2) *cell2=0;
              else if (neighbours>3) *cell2=0;
              else *cell2=1;
            }
            break;
          case 1:
            *cell2 = cell[+1];
            break;
          case 2:
            *cell2 = cell[-1];
            break;
          case 3:
            *cell2 = cell[+SIZE];
            break;
          case 4:
            *cell2 = cell[-SIZE];
            break;
          // Life     = B3/S23
          // Gnarl    = B1/S1
          // Seeds    = B2/S
          // 34Life   = B34/S34
          // LongLife = B345/S5
          // HighLife = B36/S23
        } // switch updatemode
        //_CELLS += C2;
        //C2 = -C2;

//        //--- midi

        int index = (y*SIZE)+x + SIZE+1;
        if (mTriggers[index]&1)
        {
//          int note = /*notestart-*/y;
//          if (CELLS[cell+C2]==0) c==1 ?
//            midisend(offset-1,$x80, note | (0*256) );
//          cell[C2]==1 ?
//            midisend(offset,$x90+0, note | (127*256) );
        }
        if (mTriggers[index]&2)
        {
          //float value = (float)(height-y-1) * (128.0f/mHeight); // (h-y-1) ?
//          float value = (float)y * (128.0f/mHeight); // (h-y-1) ?
//          cell[C2]==1 ?
//            midisend(offset,$xb0+0, ccnum1  | (value*256) );
        }
        if (mTriggers[index]&4)
        {
          //float value = (float)(height-y-1) * (128/height); // (h-y-1) ?
//          float value = (float)(y) * (128/mHeight); // (h-y-1) ?
//          if (CELLS[cell+C2]==1)
//            midiSend(offset,$xb0+0, ccnum2  | (value*256) );
        }

        cell  += 1;
        cell2 += 1;
        x += 1;
      } // loop width
      cell  += (SIZE-width);
      cell2 += (SIZE-width);
      y += 1;
    } // loop height
//    CELLS += C2;
//    C2 = -C2;
    int* temp = mCells2;
    mCells2 = mCells;
    mCells = temp;
  }

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

inline void do_Timing(void)
  {
    //trace("do_Timing");
    if (mSpeedMode==0) // ms
    {
      mBeats = mMS * (mTempo/60);
      //TODO: update wudget
      //mBeats != slider6 ? ( slider6=beats; sliderchange(slider6); );
      speed = mMS * mSampleRate;
    }
    else
    {
      speed = mBeats * mSampleRate / (mTempo/60);
      //TODO: update widget
      //ms    = mBeats * (60/mTempo) * 1000;
      //ms != slider5 ? ( slider5=ms; sliderchange(slider5); );
    }
    // safeguarding
    if (speed<256) speed=256;

  }

//----------------------------------------------------------------------

inline void do_Transport(void)
  {
    //trace("do_Transport");
    if (mPlayState != prevstate)
    {
      if (mPlayState & ptr_playing)
      {
        trace("play");
        isplaying = true;
        countdown = 0;//speed;
        cycle = 0;//cyclecount;
      }
      else
      {
        trace("stop");
        isplaying = false;
      }
    }
    prevstate = mPlayState;
  }

//----------------------------------------------------------------------

inline void do_Cycle(void)
  {
    //trace("do_Cycle");
    if (mCycleMode>0)
    {
      cycle -= 1;
      if (cycle<=0)
      {
        //if (mCycleMode==1) memcpy(CELLS+C2,SAVED,SIZE2);
        if (mCycleMode==2)
        {
          clear_cells(mCells,SIZE2);
          if (mRandom>0)
          {
            int C = SIZE+1;
            int num = (mWidth*mHeight)*mRandom;
            for( int i=0; i<num; i++ )
            {
              int x = mWidth  * axRandom();
              int y = mHeight * axRandom();
              mCells[ C + (y*SIZE) + x] = 1;
            }
            //);
          } // random>0
        } // cyclemode=2
        cycle = mCycleCount;
      } // cycle<=0
    } // cyclemode>0
  }

//----------------------------------------------------------------------

inline void do_Sync(void)
  {
    //trace("do_Sync");
    offset = 0;
    block = mBlockSize;//sampleFrames;
    while (block>0)
    {
      if (countdown>=block)
      {
        countdown -= block;
        block = 0;
      } // count>=block
      else
      {
        offset += countdown;
        block -= countdown;
        do_UpdateGrid();
        do_Cycle();
        redrawgrid = true;
        countdown = speed;
      } // count<block
    }
  }

//----------------------------------------------------------------------


//----------------------------------------------------------------------

//inline void do_(void)
//  {
//  }

//----------------------------------------------------------------------

