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
#include "wdgPaintBox.h"

//#ifndef MAX_CURSORS
//  #define MAX_CURSORS 16
//#endif

//#ifndef MAX_AREAS
//  #define MAX_AREAS   16
//#endif

//#define wbf_Wave    1
//#define wbf_Slices  2
//#define wbf_Cursors 4
//#define wbf_Areas   8
//#define wbf_All     0xffff

//struct bufCursor
//{
//  float   pos;
//  //axColor color;
//  axPen*  pen;
//};

//struct bufArea
//{
//  float   start;
//  float   end;
//  //axColor color;
//  axBrush* brush;
//};

//----------

class wdgScope : public wdgPaintBox
{
  //private:
  //  axBrush* mFillBrush;
  public:
    int       mSize;    // num samples
    float*    mBuffer;  // mSize*2 samples (stereo)
    float     mScale;
//    int       mMode;    // left/right
//    bool      mUnipolar;
  public:
//    int       mDrawFlags;
//    int       mNumSlices;
//    int       mNumCursors;
//    int       mNumAreas;
//    bufCursor mCursors[MAX_CURSORS];
//    bufArea   mAreas[MAX_AREAS];
    axPen* mWavePen;
//    axPen* mSlicesPen;
//    axPen* mRectPen;

  public:

    wdgScope(axWidgetListener* aListener, int aID, axRect aRect, int aAlignment=wal_None)
    //: axWidget(aListener,aID,aRect,aAlignment)
    : wdgPaintBox(aListener,aID,aRect,aAlignment)
      {
        clearFlag(wfl_Active);
        mSize   = 0;
        mBuffer = NULL;
        mScale = 1;
//        mMode   = 0;
//        mUnipolar = false;
//        mDrawFlags        = wbf_Wave;
//        mNumSlices        = 0;
//        mNumCursors       = 0;
//        mNumAreas         = 0;
        //mFillBrush  = new axBrush(AX_GREY_DARK);
        //mWaveColor        = AX_GREY_LIGHT;
        //mSlicesColor      = AX_GREY_DARK;
        mWavePen = new axPen( AX_GREY_LIGHT );
//        mSlicesPen = new axPen( AX_GREY_DARK );
//        mRectPen = new axPen( AX_GREY_DARK );
//        int i;
//        for( i=0; i<MAX_CURSORS; i++)
//        {
//          mCursors[i].pos = 0;
//          mCursors[i].pen = new axPen(AX_WHITE);
//        }
//        for( i=0; i<MAX_AREAS; i++)
//        {
//          mAreas[i].start = 0;
//          mAreas[i].end = 0;
//          mAreas[i].brush = new axBrush( axColor(96,96,96));
//        }

      }

    virtual ~wdgScope()
      {
        //TODO delete everything
        //delete mFillBrush;
      }

    //--------------------------------------------------

    virtual void do_PrePaint(axCanvas* aCanvas, axRect aRect) {}
    virtual void do_PostPaint(axCanvas* aCanvas, axRect aRect) {}

    //--------------------------------------------------

    inline void setBuffer(float* aBuffer, int aSize)
      {
        mBuffer = aBuffer;
        mSize = aSize;
      }

    inline void setScale(float aScale) { mScale = aScale; }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        wdgPaintBox::doPaint(aCanvas,aRect);

        // --- areas

//        if( mDrawFlags&wbf_Areas )
//        {
//          for( int i=0; i<mNumAreas; i++ )
//          {
//            int ss = mAreas[i].start * mRect.w + mRect.x;
//            int se = mAreas[i].end   * mRect.w + mRect.x;
//            //aCanvas->setBrushColor( mAreas[i].color );
//            aCanvas->selectBrush( mAreas[i].brush );
//            aCanvas->fillRect( ss, mRect.y, se, mRect.y2() );
//          }
//        } //areas
        do_PrePaint(aCanvas, aRect);

        // --- wave

//        if( mDrawFlags&wbf_Wave )
//        {
          if( mBuffer && mSize>0 )
          {
            //aCanvas->setPenColor( mWaveColor );
            aCanvas->selectPen( mWavePen );
            int h2 = mRect.h * 0.5;
            int x   = mRect.x;
            int y1,y2;
            //if (mUnipolar) y2 = mRect.y2() - 1;
            //else
              y2  = mRect.y + h2;
            float pos = 0;
            float posadd = mSize / mRect.w;
            for( int i=0; i<mRect.w; i++ )
            {
              float n = mBuffer[ ((int)pos) * 2 ];
              n *= mScale;
              pos += posadd;
              //if (mUnipolar) y1 = y2 - (mRect.h*n);
              //else
                y1 = y2 - (h2*n);
              aCanvas->drawLine(x,y1,x,y2);
              x++;
            }
          }
//        } //wbm_Wave

        do_PostPaint(aCanvas, aRect);

        // --- grid/slices

//        if( mDrawFlags&wbf_Slices )
//        {
//          if( mNumSlices>1 )
//          {
//            //aCanvas->setPenColor( mSlicesColor );
//            aCanvas->selectPen( mSlicesPen );
//            float xadd = (float)mRect.w / (float)mNumSlices;
//            float x = mRect.x + xadd;
//            for( int i=1; i<mNumSlices; i++ )
//            {
//              aCanvas->drawLine( x, mRect.y, x, mRect.y2() );
//              x += xadd;
//            }
//          }
//        } //wbm_Slices

        // --- cursors

//        if( mDrawFlags&wbf_Cursors )
//        {
//          for( int i=0; i<mNumCursors; i++ )
//          {
//            //aCanvas->setPenColor( mCursors[i].color );
//            aCanvas->selectPen( mCursors[i].pen );
//            int rp = mCursors[i].pos * mRect.w + mRect.x;
//            aCanvas->drawLine( rp, mRect.y, rp, mRect.y2() );
//          }
//        } //wbf_Cursors
//        //aCanvas->setPenColor(AX_GREY_DARK);

//        aCanvas->selectPen( mRectPen );
//        aCanvas->drawRect(mRect.x, mRect.y,mRect.x2(),mRect.y2());

      }
};

//----------------------------------------------------------------------
#endif



