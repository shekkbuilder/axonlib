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
 * @file
 * \brief oscilloscope widget
 */

/**
 * \brief oscilloscope widget
 *
 * long desc
 *
 */

#ifndef wdgScope_included
#define wdgScope_included
//----------------------------------------------------------------------

// buffer/size dependent on each other,
// one of them wrong, and things go boom!
// if we set size from another thread...
//
// it has "direct access" to buffer (it's just a ptr)
// memcpy if we need to be sure
//
// only set in doIdleEditor?
// since we're in the gui thread then?

#include "axWidget.h"

#define MAX_CURSORS 256
#define MAX_AREAS   16

#define wbf_Wave    1
#define wbf_Slices  2
#define wbf_Cursors 4
#define wbf_Areas   8
#define wbf_All     0xffff

struct bufCursor
{
  float   pos;
  axColor color;
};

struct bufArea
{
  float   start;
  float   end;
  axColor color;
};

//----------

class wdgScope : public axWidget
{
  public:
    int       mSize;    // num samples
    float*    mBuffer;  // mSize*2 samples (stereo)
    int       mMode;    // left/right
    bool      mUnipolar;
  public:
    int       mDrawFlags;
    int       mNumSlices;
    int       mNumCursors;
    int       mNumAreas;
    bufCursor mCursors[MAX_CURSORS];
    bufArea   mAreas[MAX_AREAS];

    axColor mWaveColor;
    axColor mSlicesColor;

  public:

    wdgScope(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None/*, axParameter* aParameter=NULL*/)
    : axWidget(aListener,aID,aRect,aAlignment/*,aParameter*/)
      {
        clearFlag(wfl_Active);
        setBackground(true,AX_GREY_DARK);
        mSize   = 0;
        mBuffer = NULL;
        mMode   = 0;
        mUnipolar = false;
        //
        mDrawFlags        = wbf_Wave;
        mNumSlices        = 0;
        mNumCursors       = 0;
        mNumAreas         = 0;
        mWaveColor        = AX_GREY_LIGHT;
        mSlicesColor      = AX_GREY_DARK;
        int i;
        for( i=0; i<MAX_CURSORS; i++)
        {
          mCursors[i].pos = 0;
          mCursors[i].color = axColor(255,255,255);
        }
        for( i=0; i<MAX_AREAS; i++)
        {
          mAreas[i].start = 0;
          mAreas[i].end = 0;
          mAreas[i].color = axColor(96,96,96);
        }


      }
    virtual ~wdgScope()
      {
      }

    //----------

    void setup(float* aBuffer, int aSize, int aMode)
      {
        mBuffer = aBuffer;
        mSize = aSize;
        mMode = aMode;
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
//        axWidget::doPaint(aCanvas, aRect);
//        aCanvas->setPenColor( axColor(255,255,0) );
//        int w  = mRect.w;
//        int h2 = (mRect.h/2);
//        if (mBuffer && w>0 && mSize>0)
//        {
//          float x = 0;
//          float xadd = mSize / w;
//          int px = mRect.x;
//          int py = mRect.y +  h2;
//          for (int i=0; i<w; i++)
//          {
//            float n;
//            if (mMode==0) n=mBuffer[((int)x)*2];
//            else if (mMode==1) n=mBuffer[((int)x)*2+1];
//            int y = mRect.y +  h2 + (n*h2);
//            aCanvas->drawLine( px,py,mRect.x+i, y );
//            px = mRect.x+i;
//            py = y;
//            x+=xadd;
//          }
//        }

        if( mDrawFlags&wbf_Areas )
        {
          for( int i=0; i<mNumAreas; i++ )
          {
            int ss = mAreas[i].start * mRect.w + mRect.x;
            int se = mAreas[i].end   * mRect.w + mRect.x;
            aCanvas->setBrushColor( mAreas[i].color );
            aCanvas->fillRect( ss, mRect.y, se, mRect.y2() );
          }
        } //areas

        if( mDrawFlags&wbf_Wave )
        {
          if( mBuffer && mSize>0 )
          {
            aCanvas->setPenColor( mWaveColor );
            int h2 = mRect.h * 0.5;
            int x   = mRect.x;
            int y1,y2;
            if (mUnipolar) y2 = mRect.y2() - 1;
            else y2  = mRect.y + h2;
            float pos = 0;
            float posadd = mSize / mRect.w;
            for( int i=0; i<mRect.w; i++ )
            {
              float n = mBuffer[ (int)pos ];
              pos += posadd;
              if (mUnipolar) y1 = y2 - (mRect.h*n);
              else y1 = y2 - (h2*n);
              aCanvas->drawLine(x,y1,x,y2);
              x++;
            }
          }
        } //wbm_Wave

        if( mDrawFlags&wbf_Slices )
        {
          if( mNumSlices>1 )
          {
            aCanvas->setPenColor( mSlicesColor );
            float xadd = (float)mRect.w / (float)mNumSlices;
            float x = mRect.x + xadd;
            for( int i=1; i<mNumSlices; i++ )
            {
              aCanvas->drawLine( x, mRect.y, x, mRect.y2() );
              x += xadd;
            }
          }
        } //wbm_Slices

        if( mDrawFlags&wbf_Cursors )
        {
          for( int i=0; i<mNumCursors; i++ )
          {
            aCanvas->setPenColor( mCursors[i].color );
            int rp = mCursors[i].pos * mRect.w + mRect.x;
            aCanvas->drawLine( rp, mRect.y, rp, mRect.y2() );
          }
        } //wbf_Cursors

        aCanvas->setPenColor(AX_GREY_DARK);
        aCanvas->drawRect(mRect.x, mRect.y,mRect.x2(),mRect.y2());

      }
};

//----------------------------------------------------------------------
#endif



