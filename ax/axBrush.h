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
  \file axBrush.h
  \brief brush
*/

#ifndef axBrush_included
#define axBrush_included
//----------------------------------------------------------------------

#include "axCanvas.h"

class axBrushBase
{
  protected:
    axColor mColor;
    int     mStyle;
  public:
    axBrushBase(axColor aColor, int aStyle)
      {
        mColor = aColor;
        mStyle = aStyle;
      }
    virtual ~axBrushBase()
      {
      }
    virtual int handle(void)
      {
        return 0;
      }
    inline axColor color(void)  { return mColor; }
    inline int style(void)      { return mStyle; }
};

//----------------------------------------------------------------------

#ifdef linux

  class axBrushX11 : public axBrushBase
  {
    public:
      axBrushX11(axColor aColor, int aStyle)
      : axBrushBase(aColor,aStyle)
        {
        }
  };

  typedef axBrushX11 axBrushImpl;

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  class axBrushW32 : public axBrushBase
  {
    private:
      HBRUSH  mBrush;
      //HBRUSH  mOldBrush;
    public:
      axBrushW32(axColor aColor, int aStyle=0)
      : axBrushBase(aColor,aStyle)
        {
          mBrush = CreateSolidBrush(aColor.mColor);
        }
      virtual ~axBrushW32()
        {
          DeleteObject(mBrush);
        }
      virtual int handle(void)
        {
          return (int)mBrush;
        }
  };

  typedef axBrushW32 axBrushImpl;

#endif

//----------------------------------------------------------------------

class axBrush : public axBrushImpl
{
  public:
    axBrush(axColor aColor, int aStyle=0) : axBrushImpl(aColor,aStyle) {}
};

//----------------------------------------------------------------------

#define DEF_BRUSH_BLACK axBrush(AX_BLACK)
#define DEF_BRUSH_DARK  axBrush(AX_GREY_DARK)

//----------------------------------------------------------------------
#endif


