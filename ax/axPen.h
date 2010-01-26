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
  \file axPen.h
  \brief pen
*/

#ifndef axPen_included
#define axPen_included
//----------------------------------------------------------------------
//TODO
#include "axCanvas.h"
#define DEF_PENWIDTH 0

class axPenBase
{
  protected:
    axColor mColor;
    int     mSize;
    int     mStyle;
  public:
    axPenBase(axColor aColor, int aSize, int aStyle)
      {
        mColor = aColor;
        mSize = aSize;
        mStyle = aStyle;
      }
    virtual ~axPenBase()
      {
      }
    virtual int handle(void)
      {
        return 0;
      }
    inline axColor color(void)  { return mColor; }
    inline int size(void)       { return mSize; }
    inline int style(void)      { return mStyle; }
};

//----------------------------------------------------------------------

#ifdef linux

  class axPen : public axPenBase
  {
    public:
      axPen(axColor aColor=AX_GREY_LIGHT, int aSize=DEF_PENWIDTH, int aStyle=0)
      : axPenBase(aColor,aSize,aStyle)
        {
        }
  };

#endif

//----------------------------------------------------------------------

#ifdef WIN32

  class axPen : public axPenBase
  {
    private:
      HPEN  mPen
      //HPEN  mOldPen;
    public:
      axPen(axColor aColor, int aSize, int aStyle)
      : axPenBase(aColor,aSize,aStyle)
        {
          mPen = CreatePen(PS_SOLID,aSize,aColor);
        }
      virtual ~axPen()
        {
          DeleteObject(mPen);
        }
      virtual int handle(void)
        {
          return mPen;
        }
  };

#endif

//----------------------------------------------------------------------
#endif


