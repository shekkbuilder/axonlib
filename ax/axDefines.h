#ifndef axDefines_included
#define axDefines_included
//----------------------------------------------------------------------

#ifndef NULL
  #define NULL 0
#endif

#ifdef linux
  #define AX_LINUX
#endif

#ifdef WIN32
  #define AX_WIN32
#endif

//
// 0x40    0x48    0x50    0x58
// |       |       |       |
// @ABCDEFGHIJKLMNOPQRSTUVWXYZ
//

#define AX_MAGIC 0x41580000

#define SPL float
#define STR axString
#define REC axRect
#define COL axColor

//----------------------------------------------------------------------
#endif
