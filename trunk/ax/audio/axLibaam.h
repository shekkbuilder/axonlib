/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axLibaam_included
#define axLibaam_included
//----------------------------------------------------------------------

#ifdef AX_LINUX
  #include <dlfcn.h>
#endif

#ifdef AX_WIN32
#endif

//----------

#include "core/axString.h"

//----------------------------------------------------------------------

#define STREAM_INIT         0
#define STREAM_OPEN         1
#define STREAM_CLOSE        2
//#define STREAM_PLAY         3
//#define STREAM_STOP         4
#define STREAM_EXIT         5
#define STREAM_ABOUT        6
#define STREAM_AUDIO_COUNT  7
#define STREAM_AUDIO_NAME   8
#define STREAM_FRAMES       9
#define STREAM_RATE         10
#define STREAM_LATENCY      17
#define STREAM_RELOAD       18
#define MIDI_IN_COUNT       30
#define MIDI_IN_NAME        31
#define MIDI_IN_ENABLE      32
#define MIDI_IN_ENABLED     33
#define AUDIO_IN_COUNT      40
#define AUDIO_IN_NAME       41
#define AUDIO_IN_ENABLE     42
#define AUDIO_IN_ENABLED    43
#define AUDIO_OUT_COUNT     50
#define AUDIO_OUT_NAME      51
#define AUDIO_OUT_ENABLE    52
#define AUDIO_OUT_ENABLED   53

//----------

typedef int (*libaam_dispatcher)(int index, int value1, int value2) asm ("libaam");
typedef int (*libaam_process)(float** inputs, float** outputs, int numins, int numouts, int frames, void* mididata, int mididatacount);

//----------------------------------------------------------------------

class axLibaam
{
  private:

    void*             mHandle;
    libaam_dispatcher mDispatcher;

  public:

    axLibaam()
      {
        mHandle     = NULL;
        mDispatcher = NULL;
      }

    //----------

    virtual ~axLibaam()
      {
      }

    //----------------------------------------
    // dll
    //----------------------------------------

    int load(axString aFileName)
      {
        #ifdef AX_LINUX
          mHandle = dlopen(aFileName.ptr(),RTLD_NOW/*RTLD_LAZY*/);  // crashed if 0
          if (!mHandle) return -2;
          mDispatcher = (libaam_dispatcher)dlsym(mHandle,(char*)"libaam");
          if (!mDispatcher) return -3;
        #endif
        #ifdef AX_WIN32
        #endif
        return 0;
      }

    //----------

    int close(void)
      {
        #ifdef AX_LINUX
          if (mHandle) dlclose(mHandle);
        #endif
        #ifdef AX_WIN32
        #endif
        mHandle = NULL;
        mDispatcher = NULL;
        return 0;
      }

    //----------

    int dispatch(int aIndex, int aValue1, int aValue2)
      {
        if (!mDispatcher) return -1;
        return mDispatcher(aIndex,aValue1,aValue2);
      }

    //----------------------------------------
    // stream
    //----------------------------------------

    // init library
    int streamInit(libaam_process process)
      {
        //case STREAM_INIT:
        //  process = (CProcess*)value1;
        return mDispatcher(STREAM_INIT,(int)process,0);   // !!! 32-bit (int)
      }

    //----------

    // open device
    int streamOpen(int num)
      {
        //case STREAM_OPEN:
        //  result = 1;
        return mDispatcher(STREAM_OPEN,num,0);
      }

    //----------

    // close device
    int streamClose(void)
      {
        //case STREAM_CLOSE:
        //  result = 1;
        return mDispatcher(STREAM_CLOSE,0,0);
      }

    //----------

    // STREAM_PLAY
    // STREAM_STOP

    //----------

    // exit library
    int streamExit(void)
      {
        //case STREAM_EXIT:
        return mDispatcher(STREAM_EXIT,0,0);
      }

    //----------

    // STREAM_ABOUT

    //----------

    // device count
    int streamAudioCount(void)
      {
        //case STREAM_AUDIO_COUNT:
        //  result = 1; // jack only
        return mDispatcher(STREAM_AUDIO_COUNT,0,0);
      }

    //----------

    // device name
    int streamAudioName(int num, char* str)
      {
        //case STREAM_AUDIO_NAME:
        //  strcpy((char*)value2, "JACK Audio");
        return mDispatcher(STREAM_AUDIO_NAME,num,(int)str);   // !!! 32-bit
      }

    //----------

    // get/set frames
    int streamFrames(int frames=0)
      {
        //case STREAM_FRAMES:
        //  if (value1 > 0) frames = value1;
        //  result = frames;
        return mDispatcher(STREAM_FRAMES,frames,0);
      }

    //----------

    // get/set rate
    int streamRate(int setRate, float* getRate)
      {
        //case STREAM_RATE:
        //  if (value1 == 0) *(float*)value2 = rate;
        //  else if (value1 > 0) rate = value1;
        return mDispatcher(STREAM_RATE,setRate,(int)getRate); // !!!
      }

    //----------

    // STREAM_LATENCY
    // STREAM_RELOAD

    //----------------------------------------
    // midi
    //----------------------------------------

    // midi in count
    int midiInCount(void)
      {
        //case MIDI_IN_COUNT:
        //  return midiDevices;
        return mDispatcher(MIDI_IN_COUNT,0,0);
      }

    //----------

    // midi in name
    int MidiInName(int num, char* str)
      {
        //case MIDI_IN_NAME:
        //  if (value1 >= 0 && value1 < midiDevices)
        //    strcpy((char*)value2, mdevs[value1]->id);
        return mDispatcher(MIDI_IN_NAME,num,(int)str);   // !!!
      }

    //----------

    // enable midi in
    int MidiInEnable(int num, int enabled)
      {
        //case MIDI_IN_ENABLE:
        //  if (value1 >= 0 && value1 < midiDevices) mdevs[value1]->enable(value2);
        return mDispatcher(MIDI_IN_ENABLE,num,enabled);
      }

    //----------

    // is midi in enabled?
    int MidiInEnabled(int num)
      {
        //case MIDI_IN_ENABLED:
        //  if (value1 >= 0 && value1 < midiDevices) return mdevs[value1]->enabled;
        return mDispatcher(MIDI_IN_ENABLED,num,0);
      }

    //----------------------------------------
    // audio in
    //----------------------------------------

    // audio in count
    int AudioInCount(void)
    {
      //case AUDIO_IN_COUNT:
      //  return aincount;
      return mDispatcher(AUDIO_IN_COUNT,0,0);
    }

    //----------

    // audio in name
    int AudioInName(int num, char* str)
    {
      //case AUDIO_IN_NAME:
      //  if (value1 >= 0 && value1 < aincount)
      //    strcpy((char*)value2, ain[value1].id);
      return mDispatcher(AUDIO_IN_NAME,num,(int)str);   // !!!
    }

    //----------

    // audio out enable
    int AudioInEnable(int num, int enabled)
    {
      //case AUDIO_IN_ENABLE:
      //  if (value1 >= 0 && value1 < maxChs)
      //    ain[value1].enabled = value2;
      return mDispatcher(AUDIO_IN_ENABLE,num,enabled);
    }

    //----------

    // audio in enabled
    int AudioInEnabled(int num)
    {
      //case AUDIO_IN_ENABLED:
      //  if (value1 >= 0 && value1 < maxChs)
      //    result = ain[value1].enabled;
      return mDispatcher(AUDIO_IN_ENABLED,num,0);
    }

    //----------------------------------------
    // audio out
    //----------------------------------------

    // audio out count
    int AudioOutCount(void)
    {
      //case AUDIO_OUT_COUNT:
      //  return aoutcount;
      return mDispatcher(AUDIO_OUT_COUNT,0,0);
    }

    //----------

    // audio out name
    int AudioOutName(int num, char* str)
    {
      //case AUDIO_OUT_NAME:
      //  if (value1 >= 0 && value1 < aoutcount)
      //    strcpy((char*)value2, aout[value1].id);
      return mDispatcher(AUDIO_OUT_NAME,num,(int)str);    // !!!
    }

    //----------

    // audio out enable
    int AudioOutEnable(int num, int enable)
    {
      //case AUDIO_OUT_ENABLE:
      //  if (value1 >= 0 && value1 < maxChs)
      //    aout[value1].enabled = value2;
      return mDispatcher(AUDIO_OUT_ENABLE,num,enable);
    }

    //----------

    // audio out enabled
    int AudioOutEnabled(int num)
    {
      //case AUDIO_OUT_ENABLED:
      //  if (value1 >= 0 && value1 < maxChs)
      //    result = aout[value1].enabled;
      return mDispatcher(AUDIO_OUT_ENABLED,num,0);
    }

};

//----------------------------------------------------------------------
#endif

/*

based on sources from http://svn.jacklab.net
and http://www.energy-xt.com/download/jack.cpp

you need to get a libaam.so/dll from
http://www.energy-xt.com/xt2.php
(or evenyually compile your own .so for linux)

http://www.kvraudio.com/forum/viewtopic.php?t=217002&postdays=0&postorder=asc&highlight=libaam&start=15

nexxusix:   "can the version of jack.cpp you have be used for other applications?"
drumfix:    "to my knowledge the code is public domain, so you can use it in your
             own projects. in general all your app has to do is to call procedure
             libaam with the appropriate dispatcher codes. if you understand how
             this procedure works you can easily add midiout as well."

http://www.energy-xt.com/forum/index.php?action=posts&forumId=2&topicId=453

jorgen:     "ok, heres the source code for using jack with XT2."
metasymbol: "under what licence you publicing your open source software?"
jorgen:     "the source is public domain"

- jack.cpp
http://www.energy-xt.com/download/jack.php
http://people.jacklab.net/drumfix/jack.cpp
http://svn.jacklab.net/listing.php?repname=eXT2&path=%2Flibaam%2F#_libaam_

- energy-xt & libaam
http://www.energy-xt.com/
http://www.energy-xt.com/xt2.php

*/
