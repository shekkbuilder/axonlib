#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------
#include "axDefines.h"

#define DEF_PENWIDTH 0

#ifdef AX_LINUX

struct axPen
{
  axColor mColor;
  int     mSize;
};

#endif


#ifdef AX_WIN32

struct axPen
{
  axColor mColor;
  int     mSize;
  HPEN    mHandle;
};

#endif


//class axPenBase
//{
//  protected:
//    axColor mColor;
//    int     mSize;
//  public:
//    axPenBase(axColor aColor, int aSize=DEF_PENWIDTH) { mColor=aColor; mSize=aSize; }
//    virtual ~axPenBase() {}
//    inline void setColor(axColor aColor) { mColor=aColor; }
//    inline void setSize(int aSize) { mSize=aSize; }
//    inline unsigned long getColor(void) { return mColor.get(); }
//    inline int getSize(void) { return mSize; }
//};
//
////----------------------------------------------------------------------
//
//#ifdef AX_WIN32
//
//class axPenWin32 : public axPenBase
//{
//  private:
//    HPEN  mHandle;
//  public:
//    axPenWin32(axColor aColor, int aSize=DEF_PENWIDTH)
//    : axPenBase(aColor,aSize)
//      {
//        mHandle = CreatePen(PS_SOLID,aSize,aColor.get());
//      }
//    virtual ~axPenWin32()
//      {
//        DeleteObject(mHandle);
//      }
//    inline HPEN getHandle(void) { return mHandle; }
//
//
//};
//typedef axPenWin32 axPenImpl;
//
//#endif
////----------------------------------------------------------------------
//#ifdef AX_LINUX
//
//class axPenLinux : public axPenBase
//{
//};
//typedef axPenLinux axPenImpl;
//
//#endif
////----------------------------------------------------------------------
//
//typedef axPenImpl axPen;
//
//////----------------------------------------------------------------------
////#ifdef AX_WIN32
////
////  struct axPen
////  {
////    axColor mColor;
////    int     mSize;
////    int     mHandle;
////    //
////    axPen() { mColor=0; mSize=DEF_PENWIDTH; }
////    axPen(axColor aColor, int aSize=DEF_PENWIDTH)
////      {
////        mColor=aColor;
////        mSize=aSize;
////        mHandle = (int)CreatePen(PS_SOLID,aSize,aColor.mColor);
////      }
////    ~axPen() {}
////
////    inline void setColor(axColor aColor) { mColor=aColor; }
////    inline void setSize(int aSize) { mSize=aSize; }
////
////    inline unsigned long getColor(void) { return mColor.get(); }
////    inline int getSize(void) { return mSize; }
////  };
////
////#endif
//////----------------------------------------------------------------------
////#ifdef AX_LINUX
////
////  struct axPen
////  {
////    axColor mColor;
////    int     mSize;
////    //
////    axPen() { mColor=0; mSize=DEF_PENWIDTH; }
////    axPen(axColor aColor, int aSize=DEF_PENWIDTH)
////      {
////        mColor=aColor;
////        mSize=aSize;
////      }
////    ~axPen() {}
////
////    inline void setColor(axColor aColor) { mColor=aColor; }
////    inline void setSize(int aSize) { mSize=aSize; }
////
////    inline unsigned long getColor(void) { return mColor.get(); }
////    inline int getSize(void) { return mSize; }
////  };
////
////#endif
//////----------------------------------------------------------------------

typedef axArray<axPen> axPens;

//----------------------------------------------------------------------
#endif
