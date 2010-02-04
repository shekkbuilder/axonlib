
// 0 =
// 1 =
// 2 = save
// 3 = r
// 4 = g
// 5 = b

//----------

//TODO:
// a) hope the compiler will optimize this (probably)
// b) 'real' memcpy/memset (macros?)
inline void copy_cells(int dst, int src, int num)
  {
    for (int i=0;i<num;i++) { CELLS[dst+i] = CELLS[src+i]; }
  }

//----------

inline void clear_cells(int dst, int num)
  {
    for (int i=0;i<num;i++) { CELLS[dst+i] = 0; }
  }

//----------------------------------------------------------------------

inline void do_Tick(void)
  {
    trace("do_Tick");
    int width  = (int)mWidth;
    int height = (int)mHeight;

    //--- wraparound

    int cell = 0;//_CELLS;
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
        CELLS[cell] = CELLS[cell+width];
        CELLS[cell+width+1] = CELLS[cell+1];
      }
      else
      {
        CELLS[cell] = 0;
        CELLS[cell+width+1] = 0;
      }
      cell += SIZE;
    }

    //--- update grid

    cell = SIZE+1;
    int y = 0;
    for (int ih=0; ih<height; ih++)
    {
      if (mWrap)
      {
        CELLS[cell] = CELLS[cell+width];
        CELLS[cell+width+1] = CELLS[cell+1];
      }
      else
      {
        CELLS[cell] = 0;
        CELLS[cell+width+1] = 0;
      }
      int x = 0;
      for (int iw=0; iw<width; iw++)
      {
        int n = CELLS[cell];
        int neighbours = 0;
        switch (mUpdateMode)
        {
          case 0:
            neighbours += CELLS[cell-SIZE-1];
            neighbours += CELLS[cell-SIZE  ];
            neighbours += CELLS[cell-SIZE+1];
            neighbours += CELLS[cell     -1];
            neighbours += CELLS[cell     +1];
            neighbours += CELLS[cell+SIZE-1];
            neighbours += CELLS[cell+SIZE  ];
            neighbours += CELLS[cell+SIZE+1];
            // life: B3/S23
            if (n==0)
            { // B3
              if (neighbours==3) CELLS[cell+C2]=1; else CELLS[cell+C2]=0;
            }
            if (n==1)
            { // S23
              if (neighbours<2) CELLS[cell+C2]=0;
              else if (neighbours>3) CELLS[cell+C2]=0;
              else CELLS[cell+C2]=1;
            }
            break;
          case 1:
            CELLS[cell+C2] = CELLS[cell+1];
            break;
          case 2:
            CELLS[cell+C2] = CELLS[cell-1];
            break;
          case 3:
            CELLS[cell+C2] = CELLS[cell+SIZE];
            break;
          case 4:
            CELLS[cell+C2] = CELLS[cell-SIZE];
          // Life     = B3/S23
          // Gnarl    = B1/S1
          // Seeds    = B2/S
          // 34Life   = B34/S34
          // LongLife = B345/S5
          // HighLife = B36/S23
        }
        _CELLS += C2;
        C2 = -C2;


//        //--- midi

        int index = (y*SIZE)+x + SIZE+1;
        if ( CELLS[RTRIG+index]>0)
        {
//          int note = /*notestart-*/y;
//          if (CELLS[cell+C2]==0) c==1 ?
//            midisend(offset-1,$x80, note | (0*256) );
//          cell[C2]==1 ?
//            midisend(offset,$x90+0, note | (127*256) );
        }
        if (CELLS[GTRIG+index]>0)
        {
          //float value = (float)(height-y-1) * (128.0f/mHeight); // (h-y-1) ?
//          float value = (float)y * (128.0f/mHeight); // (h-y-1) ?
//          cell[C2]==1 ?
//            midisend(offset,$xb0+0, ccnum1  | (value*256) );
        }
        if (CELLS[BTRIG+index]>0)
        {
          //float value = (float)(height-y-1) * (128/height); // (h-y-1) ?
//          float value = (float)(y) * (128/mHeight); // (h-y-1) ?
//          if (CELLS[cell+C2]==1)
//            midiSend(offset,$xb0+0, ccnum2  | (value*256) );
        }

        cell += 1;
        x += 1;
      } // loop width
      cell += (SIZE-width);
      y += 1;
    } // loop height
//    CELLS += C2;
//    C2 = -C2;
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
        do_Tick();
        redrawgrid = true;
        countdown = speed;
      } // count<block
    }
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
          clear_cells(C2,SIZE2);

          if (mRandom>0)
          {
            int C = C2+SIZE+1;
            int num = (mWidth*mHeight)*mRandom;
            for( int i=0; i<num; i++ )
            {
              int x = mWidth  * axRandom();
              int y = mHeight * axRandom();
              CELLS[C+(y*SIZE)+x] = 1;
            }
            //);
          } // random>0
        } // cyclemode=2
        cycle = mCycleCount;
      } // cycle<=0
    } // cyclemode>0
  }

//----------------------------------------------------------------------

//inline void do_(void)
//  {
//  }

//----------------------------------------------------------------------

