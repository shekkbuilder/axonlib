/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
  \file axFont.h
  \brief font
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
    inline axString name(void) { return mName; }
    inline axColor color(void) { return mColor; }
    inline int size(void)     { return mSize; }
    inline int style(void)    { return mStyle; }
    virtual int handle(void)  { return 0; }
};

//----------------------------------------------------------------------

#ifdef linux

  class axFont : public axFontBase
  {
    public:

      axFont(axColor aColor, axString aName="default", int aSize=-1, int aStyle=0)
      : axFontBase(aColor,aName,aSize,aStyle)
        {
        }

      virtual ~axFont()
        {
        }
  };

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  class axFont : public axFontBase
  {
    public:

      axFont(axColor aColor, axString aName="default", int aSize=-1, int aStyle=0)
      : axFontBase(aColor,aName,aSize,aStyle)
        {
        }

      virtual ~axFont()
        {
        }

      virtual int handle(void)
        {
          return 0;
        }

  };

#endif

//----------------------------------------------------------------------
#endif



