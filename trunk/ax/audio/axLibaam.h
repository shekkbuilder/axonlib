#ifndef axLibaam_included
#define axLibaam_included
//----------------------------------------------------------------------

//const

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

//typedef float** ppsingle;
//int libaam(int index, int value1, int value2) asm ("libaam");
//typedef int (libaamDispatcher)(float** inputs, float** outputs, int numIns, int numOuts, int frames, void* midiData, int midiDataCount);

/*
class axLibBase
{
  private:
    void* mHandle;
    libaamDispatcher mDispatcher;
  public:
    axLibBase() {}
    virtual ~axLibBase() {}
    int LoadLib(char* aFilename) {}
    void CloseLib(void) {}
		int Dispatch(int aIndex,int aValue1,int aValue2:integer) {}
};
*/

//----------

/*
class axLibaam : public axLibBase
{
  public:
    axLibaam() {}
    ~axLibaam() {}
    //
    int StreamInit(libaamProcess proc) {return 0;}
    int StreamOpen(int num) {return 0;}
    int StreamClose(void) {return 0;}
    int StreamExit(void) {return 0;}
    int StreamAudioCount(void) {return 0;}
    int StreamAudioName(int num, char* str) {return 0;}
    int StreamFrames(int frames=0) {return 0;}
    int StreamRate(int setRate, float* getRate) {return 0;}
    //
    int MidiInCount(void) {return 0;}
    int MidiInName(int num, char* str) {return 0;}
    int MidiInEnable(int num, int enabled) {return 0;}
    int MidiInEnabled(int num) {return 0;}
    //
    int AudioInCount(void) {return 0;}
    int AudioInName(int num, char* str) {return 0;}
    int AudioInEnable(int num, int enabled) {return 0;}
    int AudioInEnabled(int num) {return 0;}
    //
    int AudioOutCount(void) {return 0;}
    int AudioOutName(int num; char* str) {return 0;}
    int AudioOutEnable(int num, int enable) {return 0;}
    int AudioOutEnabled(int num) {return 0;}
};
*/

//----------------------------------------------------------------------
#endif















#if 0
/*

unit plibaam;
{$mode objfpc}{$H+}

//----------------------------------------------------------------------
{
  ccernn / kkoagulaa
  cern.th.skei@gmail.com
  2008

  based on sources from http://svn.jacklab.net
  and http://www.energy-xt.com/download/jack.cpp

  you need to get a libaam.so/dll from
  http://www.energy-xt.com/xt2.php
}

{
v0.0.0.1
- initial (unofficial) release

v0.0.0.2
- officially public
- fixed MidiInEnable, MIDI_IN_ENABLE_D_
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Classes, SysUtils, dl;

const
	STREAM_INIT					= 0;
	STREAM_OPEN					= 1;
	STREAM_CLOSE				= 2;
//STREAM_PLAY					= 3;
//STREAM_STOP					= 4;
	STREAM_EXIT					= 5;
	STREAM_ABOUT				= 6;
	STREAM_AUDIO_COUNT	= 7;
	STREAM_AUDIO_NAME		= 8;
	STREAM_FRAMES				= 9;
	STREAM_RATE					= 10;

	STREAM_LATENCY			= 17;
	STREAM_RELOAD				= 18;

	MIDI_IN_COUNT				= 30;
	MIDI_IN_NAME				= 31;
	MIDI_IN_ENABLE			= 32;
	MIDI_IN_ENABLED			= 33;

	AUDIO_IN_COUNT			= 40;
	AUDIO_IN_NAME				= 41;
	AUDIO_IN_ENABLE			= 42;
	AUDIO_IN_ENABLED		= 43;

	AUDIO_OUT_COUNT			= 50;
	AUDIO_OUT_NAME			= 51;
	AUDIO_OUT_ENABLE		= 52;
	AUDIO_OUT_ENABLED		= 53;

type

  ppsingle  = ^psingle;

	//int libaam(int index, int value1, int value2) asm ("libaam");
  libaamDispatcher = function(aIndex,aValue1,aValue2:integer) : integer; cdecl;

	//typedef int (CProcess)(float** inputs, float** outputs, int numIns, int numOuts,
	//                       int frames, void* midiData, int midiDataCount);
	libaamProcess = function(	aInputs, aOutputs	: ppsingle;
 														aNumIns, aNumOuts	: integer;
               							aFrames						: integer;
                      			aMidiData					: pointer;
                         		aMidiDataCount		: integer) : integer; cdecl;

  //----------

  TLibBase = class
  private
    mHandle			: pointer;
    mDispatcher	: libaamDispatcher;
  public
    constructor Create;
    destructor  Destroy; override;
    function    LoadLib(aFilename:pchar) : integer;
    procedure   CloseLib;
		function		Dispatch(aIndex,aValue1,aValue2:integer) : integer;
  end;

  //----------

  TLibaam = class(TLibBase)
  public
    constructor Create;
    destructor  Destroy; override;
		function		StreamInit(proc:libaamProcess) : integer;
		function		StreamOpen(num:integer) : integer;
		function		StreamClose : integer;
		function		StreamExit : integer;
		function		StreamAudioCount : integer;
		function		StreamAudioName(num:integer; str:pchar) : integer;
		function		StreamFrames(frames:integer=0) : integer;
		function		StreamRate(setRate:integer; getRate:psingle) : integer;
    //
		function		MidiInCount : integer;
		function		MidiInName(num:integer; str:pchar) : integer;
		function		MidiInEnable(num:integer; enabled:integer) : integer;
		function		MidiInEnabled(num:integer) : integer;
    //
		function		AudioInCount : integer;
		function		AudioInName(num:integer; str:pchar) : integer;
		function		AudioInEnable(num:integer; enabled:integer) : integer;
		function		AudioInEnabled(num:integer) : integer;
    //
		function		AudioOutCount : integer;
		function		AudioOutName(num:integer; str:pchar) : integer;
		function		AudioOutEnable(num:integer; enable:integer) : integer;
		function		AudioOutEnabled(num:integer) : integer;
    //
  end;

//----------

var
  gLibaam : TLibaam;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//
//----------------------------------------------------------------------
//
// TLibaamBase
//
//----------------------------------------------------------------------
//

constructor TLibBase.Create;
begin
  mHandle := nil;
  mDispatcher := nil;
end;

//----------

destructor TLibBase.Destroy;
begin
  //inherited;
end;

//----------------------------------------------------------------------

function TLibBase.LoadLib(aFilename:pchar) : integer;
begin
  if Assigned(mHandle) then exit(-1);// CloseLib;
	mHandle := dlopen(aFilename,RTLD_NOW{RTLD_LAZY}); // CERN: crashed if 0
  if not Assigned(mHandle) then exit(-2);
	pointer(mDispatcher) := dlsym(mHandle,'libaam');
  if not Assigned(mDispatcher) then exit(-3);
  result := 0;
end;

//----------

procedure TLibBase.CloseLib;
begin
  if not Assigned(mHandle) then exit;
	dlclose(mHandle);
  mHandle := nil;
  mDispatcher := nil;
end;

//----------------------------------------------------------------------

function TLibBase.Dispatch(aIndex,aValue1,aValue2:integer) : integer;
begin
  if not Assigned(mDispatcher) then exit(-1);
  result := mDispatcher(aIndex,aValue1,aValue2);
end;

//
//----------------------------------------------------------------------
//
// TLibaam
//
//----------------------------------------------------------------------
//

constructor TLibaam.Create;
begin
  inherited;
end;

//----------

destructor TLibaam.Destroy;
begin
  //inherited;
end;

//----------------------------------------------------------------------
//int libaam (int index, int value1, int value2) {
//  int i, result = -1;
//  switch (index) {
//----------------------------------------------------------------------

// init library

function TLibaam.StreamInit(proc:libaamProcess) : integer;
begin
	//case STREAM_INIT:
	//  process = (CProcess*)value1;
  result := mDispatcher(STREAM_INIT,integer(proc),0);
end;

//----------

// open device

function TLibaam.StreamOpen(num:integer) : integer;
begin
	//case STREAM_OPEN:
	//  result = 1;
  result := mDispatcher(STREAM_OPEN,num,0);
end;

//----------

// close device

function TLibaam.StreamClose : integer;
begin
	//case STREAM_CLOSE:
	//  result = 1;
  result := mDispatcher(STREAM_CLOSE,0,0);
end;

//----------

// STREAM_PLAY
// STREAM_STOP

//----------

// exit library

function TLibaam.StreamExit : integer;
begin
	//case STREAM_EXIT:
  result := mDispatcher(STREAM_EXIT,0,0);
end;

//----------

// STREAM_ABOUT

//----------

// device count

function TLibaam.StreamAudioCount : integer;
begin
	//case STREAM_AUDIO_COUNT:
	//  result = 1; // jack only
  result := mDispatcher(STREAM_AUDIO_COUNT,0,0);;
end;

//----------

// device name

function TLibaam.StreamAudioName(num:integer; str:pchar) : integer;
begin
	//case STREAM_AUDIO_NAME:
	//  strcpy((char*)value2, "JACK Audio");
  result := mDispatcher(STREAM_AUDIO_NAME,num,integer(str));
end;

//----------

// get/set frames

function TLibaam.StreamFrames(frames:integer=0) : integer;
begin
	//case STREAM_FRAMES:
	//  if (value1 > 0) frames = value1;
	//  result = frames;
  result := mDispatcher(STREAM_FRAMES,frames,0);
end;

//----------

// get/set rate

function TLibaam.StreamRate(setRate:integer; getRate:psingle) : integer;
begin
	//case STREAM_RATE:
	//  if (value1 == 0) *(float*)value2 = rate;
	//  else if (value1 > 0) rate = value1;
  result := mDispatcher(STREAM_RATE,setRate,integer(getRate));
end;

//----------

// STREAM_LATENCY
// STREAM_RELOAD

//----------------------------------------------------------------------
// midi in
//----------------------------------------------------------------------

// midi in count

function TLibaam.MidiInCount : integer;
begin
	//case MIDI_IN_COUNT:
	//  return midiDevices;
  result := mDispatcher(MIDI_IN_COUNT,0,0);
end;

//----------

// midi in name

function TLibaam.MidiInName(num:integer; str:pchar) : integer;
begin
	//case MIDI_IN_NAME:
	//  if (value1 >= 0 && value1 < midiDevices)
	//    strcpy((char*)value2, mdevs[value1]->id);
  result := mDispatcher(MIDI_IN_NAME,num,integer(str));
end;

//----------

// enable midi in

function TLibaam.MidiInEnable(num:integer; enabled:integer) : integer;
begin
	//case MIDI_IN_ENABLE:
	//  if (value1 >= 0 && value1 < midiDevices) mdevs[value1]->enable(value2);
  result := mDispatcher(MIDI_IN_ENABLE,num,enabled);
end;

//----------

// is midi in enabled?

function TLibaam.MidiInEnabled(num:integer) : integer;
begin
	//case MIDI_IN_ENABLED:
	//  if (value1 >= 0 && value1 < midiDevices) return mdevs[value1]->enabled;
  result := mDispatcher(MIDI_IN_ENABLED,num,0);
end;

//----------------------------------------------------------------------
// audio in
//----------------------------------------------------------------------

// audio in count

function TLibaam.AudioInCount : integer;
begin
	//case AUDIO_IN_COUNT:
	//  return aincount;
  result := mDispatcher(AUDIO_IN_COUNT,0,0);
end;

//----------

// audio in name

function TLibaam.AudioInName(num:integer;str:pchar) : integer;
begin
	//case AUDIO_IN_NAME:
	//  if (value1 >= 0 && value1 < aincount)
	//    strcpy((char*)value2, ain[value1].id);
  result := mDispatcher(AUDIO_IN_NAME,num,integer(str));
end;

//----------

// audio out enable

function TLibaam.AudioInEnable(num:integer; enabled:integer) : integer;
begin
	//case AUDIO_IN_ENABLE:
	//  if (value1 >= 0 && value1 < maxChs)
	//    ain[value1].enabled = value2;
  result := mDispatcher(AUDIO_IN_ENABLE,num,enabled);
end;

//----------

// audio in enabled

function TLibaam.AudioInEnabled(num:integer) : integer;
begin
	//case AUDIO_IN_ENABLED:
	//  if (value1 >= 0 && value1 < maxChs)
	//    result = ain[value1].enabled;
  result := mDispatcher(AUDIO_IN_ENABLED,num,0);
end;

//----------------------------------------------------------------------
// audio out
//----------------------------------------------------------------------

// audio out count

function TLibaam.AudioOutCount : integer;
begin
	//case AUDIO_OUT_COUNT:
	//  return aoutcount;
  result := mDispatcher(AUDIO_OUT_COUNT,0,0);
end;

//----------

// audio out name

function TLibaam.AudioOutName(num:integer;str:pchar) : integer;
begin
	//case AUDIO_OUT_NAME:
	//  if (value1 >= 0 && value1 < aoutcount)
	//    strcpy((char*)value2, aout[value1].id);
  result := mDispatcher(AUDIO_OUT_NAME,num,integer(str));
end;

//----------

// audio out enable

function TLibaam.AudioOutEnable(num:integer; enable:integer) : integer;
begin
	//case AUDIO_OUT_ENABLE:
	//  if (value1 >= 0 && value1 < maxChs)
	//    aout[value1].enabled = value2;
  result := mDispatcher(AUDIO_OUT_ENABLE,num,enable);
end;

//----------

// audio out enabled

function TLibaam.AudioOutEnabled(num:integer) : integer;
begin
	//case AUDIO_OUT_ENABLED:
	//  if (value1 >= 0 && value1 < maxChs)
	//    result = aout[value1].enabled;
  result := mDispatcher(AUDIO_OUT_ENABLED,num,0);
end;

//return result; }

//----------------------------------------------------------------------

initialization
  gLibaam := nil;

finalization
  if Assigned(gLibaam) then gLibaam.Destroy;

//----------------------------------------------------------------------
end.


*/
#endif
