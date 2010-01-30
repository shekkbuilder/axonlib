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
 * \file axBuffer.h
 * \brief buffer class
 */

#ifndef axBuffer_included
#define axBuffer_included
//----------------------------------------------------------------------
//TODO: consider lock/mutex stuff 

#include <stdlib.h>

/**
 * \brief buffer class
 *
 */

template<class _T>
class axBuffer
{
  //public: 
  protected:

    _T*   mData;
    int   mWidth;
    int   mHeight;
    //
    int   mReadPos;
    int   mWritePos;

  public:

    axBuffer()
      {
        mWidth  = 0;
        mHeight = 0;
        mData  = NULL;
        mReadPos = 0;
        mWritePos = 0;
      }

    //----------

    axBuffer(int aWidth, int aHeight=1)
      {
        create(aWidth,aHeight);
      }

    //----------

    ~axBuffer()
      {
        destroy();
      }

    //----------------------------------------

    inline int width(void)            { return mWidth; }
    inline int height(void)           { return mHeight; }
    inline int dataSize(void)         { return mWidth*mHeight*sizeof(_T); }
    inline char*  ptr(void)           { return mData; }
    inline char*  ptr(int aPos)       { return &(mData[aPos]); }            // 1d
    inline char*  ptr(int aX, int aY) { return &(mData[aY*mWidth+aX]); }    // 2d

    inline _T& operator [] (int aIndex)      { return mData[aIndex]; }
    inline _T& item(int aIndex)              { return mData[aIndex]; }

    //----------------------------------------
    
    /// buffer create
    bool create(int aWidth, int aHeight=1)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mData = (_T*)malloc( dataSize() );
        mReadPos = 0;
        mWritePos = 0;
        return (mData);
      }

    //----------
    /// buffer destroy
    void destroy(void)
      {
        if (mData) free(mData);// delete mData;
        mData = NULL;
        mWidth = 0;
        mHeight = 0;
      }

    //----------------------------------------
    /// clear buffer
    inline void clear(char aValue=0)
      {
        memset(mData,aValue,dataSize());
        mReadPos = 0;
        mWritePos = 0;
      }

    //----------
    /// buffer resize
    bool resize(int aWidth, int aHeight=1)
      {
        if(aWidth!=mWidth || aHeight!=mHeight )
        {
          mWidth = aWidth;
          mHeight = aHeight;
          mData = (_T*)realloc(mData,dataSize());
          mReadPos = 0;
          mWritePos = 0;
          return (mData);
        }
        return true;
      }

    //----------------------------------------
    /// check if readable
    inline int readable(void)
      {
        int res = mWritePos - mReadPos;
        if( res<0 ) res += mWidth;
        return res;
      }    
    /// check if writeable
    inline int writeable(void)
      {
        int res = mReadPos - mWritePos;
        if( res<0 ) res += mWidth;
        return res;
      }

    //----------
    /// read buffer
    inline _T read(void)
      {
        int rp = mReadPos+1;
        int ret = mData[rp-1];
        //if( rp>=mWidth ) rp=0;
        mReadPos=rp;
        return ret;
      }

    //----------
    /// write to buffer
    inline void write(_T aItem)
      {
        int wp = mWritePos+1;
        mData[wp-1] = aItem;
        //if( wp>=mWidth ) wp=0;
        mWritePos=wp;
      }

};

//----------------------------------------------------------------------
#endif
