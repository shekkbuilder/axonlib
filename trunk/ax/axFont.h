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

/**
  \file axFont.h
  \brief font class
*/

#ifndef axFont_included
#define axFont_included
//----------------------------------------------------------------------
//TODO
#include "axCanvas.h"
#define DEF_PENWIDTH 0

class axFontBase
{
  protected:
    axString  mName;
    axColor   mColor;
    int       mSize;
    int       mStyle;
  public:
    axFontBase(axColor aColor, axString aName, int aSize, int aStyle)
      {
        mColor = aColor;
        mName  = aName;//"default";
        mSize = aSize;
        mStyle = aStyle;
      }
    virtual ~axFontBase() {}
    inline axString getName(void) { return mName; }
    inline axColor  getColor(void) { return mColor; }
    inline int      getSize(void)     { return mSize; }
    inline int      getStyle(void)    { return mStyle; }
    virtual int     getHandle(void)  { return 0; }
};

//----------------------------------------------------------------------

#ifdef linux

  class axFontX11 : public axFontBase
  {
    public:

      axFontX11(axColor aColor, axString aName="default", int aSize=-1, int aStyle=0)
      : axFontBase(aColor,aName,aSize,aStyle)
        {
        }

      virtual ~axFontX11()
        {
        }
  };

  typedef axFontX11 axFontImpl;

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  class axFontW32 : public axFontBase
  {
    public:

      axFontW32(axColor aColor, axString aName="default", int aSize=-1, int aStyle=0)
      : axFontBase(aColor,aName,aSize,aStyle)
        {
        }

      virtual ~axFontW32()
        {
        }

      virtual int handle(void)
        {
          return 0;
        }

  };

  typedef axFontW32 axFontImpl;

#endif

//----------------------------------------------------------------------

class axFont : public axFontImpl
{
  public:
    axFont(axColor aColor, axString aName="default", int aSize=-1, int aStyle=0)
    : axFontImpl(aColor,aName,aSize,aStyle)
      {}
};

//----------------------------------------------------------------------
#endif



