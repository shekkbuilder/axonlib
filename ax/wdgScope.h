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
            int yh  = mRect.y + h2;
            float pos = 0;
            float posadd = mSize / mRect.w;
            for( int i=0; i<mRect.w; i++ )
            {
              float n = mBuffer[ (int)pos ];
              pos += posadd;
              int   y = yh - (h2*n);
              aCanvas->drawLine(x,yh,x,y);
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



