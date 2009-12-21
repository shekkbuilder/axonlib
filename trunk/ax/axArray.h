#ifndef axArray_included
#define axArray_included
//----------------------------------------------------------------------
#include <stdlib.h>

#define SIZE_INIT 16
#define SIZE_MULT 2

template<class _T>
class axArray
{
	private:

    _T* mArray;
    int mSize;
    int mRealSize;

  public:

		axArray()
			{
		  	mRealSize = SIZE_INIT;
			  mSize = 0;
			  mArray = (_T*)malloc(mRealSize*sizeof(_T));
			}

    //----------

    axArray(const axArray& aArray)
      {
        mArray = (_T*)malloc(sizeof(_T)*aArray.mRealSize);
        memcpy(mArray, aArray.mArray, sizeof(_T)*aArray.mRealSize);
        mRealSize = aArray.mRealSize;
        mSize = aArray.mSize;
      }

    //----------

    ~axArray()
      {
        if (mArray)
        {
          free(mArray);
          mArray = NULL;
        }
      }

    //----------

    _T& operator [] (int aIndex) { return mArray[aIndex]; }
    _T& item(int aIndex) { return mArray[aIndex]; }

    int size() { return mSize; }
    void* ptr() { return mArray; }


    //----------

    axArray& operator = (const axArray &aArray)
      {
        if (this==&aArray) return *this;
        if (aArray.mSize==0) clear();
        setSize(aArray.mSize);
        memcpy(mArray, aArray.mArray, sizeof(_T)*aArray.mSize);
        return *this;
      }

    //----------

//    void clear()
//      {
//        mSize = 0;
//        mArray = (_T*)realloc(mArray, sizeof(_T)*SIZE_INIT);
//        mRealSize = SIZE_INIT;
//      }
//
//    //----------
//
    void clear(bool aErase=true)
      {
        mSize = 0;
        if (aErase)
        {
          mArray = (_T*)realloc(mArray, sizeof(_T)*SIZE_INIT);
          mRealSize = SIZE_INIT;
        }
      }

    //----------

    void append(const _T& aItem)
      {
        mSize++;
        if (mSize>mRealSize)
        {
          mRealSize *= SIZE_MULT;
          mArray = (_T*)realloc(mArray, sizeof(_T)*mRealSize);
        }
        mArray[mSize-1] = aItem;
      }

    //----------

    void setSize(int aSize)
      {
        mSize = aSize;
        if (mSize!=0)
        {
          if ( (mSize>mRealSize) || (mSize<mRealSize/2) )
          //if ( (mSize>mRealSize) || (mSize<mRealSize/SIZE_MULT) )
          {
            mRealSize = mSize;
            mArray = (_T*)realloc(mArray, sizeof(_T)*mSize);
          }
        }
        else clear();
      }

    //----------

    void remove(int aPos)
      {
        if (mSize==1) clear();
        else
        {
          for( int i=aPos; i<mSize-1; i++ ) mArray[i] = mArray[i+1];
          mSize--;
        }
      }

};

//----------------------------------------------------------------------
#endif
