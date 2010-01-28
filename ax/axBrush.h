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

/// define brush base
class axBrushBase
{
  protected:
    axColor mColor;
    int     mStyle;
  public:
    /// brush base constructor
    axBrushBase( axColor aColor=AX_GREY_DARK, int aStyle=0 )
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
/// brush for linux
#ifdef linux

  class axBrush : public axBrushBase
  {
    public:
      /// brush constructor for linux
      axBrush(axColor aColor, int aStyle)
      : axBrushBase(aColor,aStyle)
        {
        }
  };

#endif

//----------------------------------------------------------------------
/// brush for win32
#ifdef WIN32

  class axBrush : public axBrushBase
  {
    private:
      HBRUSH  mBrush;
      //HBRUSH  mOldBrush;
    public:
      /// brush constructor for win32
      axBrush(axColor aColor, int aStyle)
      : axBrushBase(aColor,aStyle)
        {
          mBrush = CreateSolidBrush(aColor.mColor);
        }
      virtual ~axBrush()
        {
          DeleteObject(mBrush);
        }
      virtual int handle(void)
        {
          return (int)mBrush;
        }
  };

#endif

//----------------------------------------------------------------------
#endif


