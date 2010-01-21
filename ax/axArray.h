#ifndef axArray_included
#define axArray_included
//----------------------------------------------------------------------
#include <stdlib.h>

#define SIZE_INIT 16  ///< initial size of array
#define SIZE_MULT 2   ///< multiply factor when array needs to resize

/// templated dynamic array

template<class _T>
class axArray
{
	private:

    _T* mArray;       ///< the memory
    int mSize;        ///< array size
    int mRealSize;    ///< real (allocated) size of array

  public:

    /// empty constructor. initializes the array to a defined size (SIZE_INIT)

		axArray()
			{
		  	mRealSize = SIZE_INIT;
			  mSize = 0;
			  mArray = (_T*)malloc(mRealSize*sizeof(_T));
			}

    //----------

    /// copy constructor. initializes array from another array (copies data)

    axArray(const axArray& aArray)
      {
        mArray = (_T*)malloc(sizeof(_T)*aArray.mRealSize);
        memcpy(mArray, aArray.mArray, sizeof(_T)*aArray.mRealSize);
        mRealSize = aArray.mRealSize;
        mSize = aArray.mSize;
      }

    //----------

    /// destructor

    ~axArray()
      {
        if (mArray)
        {
          free(mArray);
          mArray = NULL;
        }
      }

    //----------

    /// [] operator, so that you can do things like
    /// int a = mArray[3] to get the 3rd element
    _T& operator [] (int aIndex) { return mArray[aIndex]; }

    /// direct access to elements, with index
    _T& item(int aIndex) { return mArray[aIndex]; }

    /// resutns size of array (number of elements in array)
    int size() { return mSize; }

    /// pointer to first element [0]
    void* ptr() { return mArray; }


    //----------

    /// ini/copy array from another array
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
    /// clear array. optionally deleting all elements
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

    /// append an item to the end of the array
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

    /// set new array size. re-allocates memory if needed
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

    /// remove an element from the array, at pos aPos
    void remove(int aPos)
      {
        if (mSize==1) clear();
        else
        {
          for( int i=aPos; i<mSize-1; i++ ) mArray[i] = mArray[i+1];
          mSize--;
        }
      }

    //----------

    /// push element to end of array
    /// so you can use it like a stack
    void push(const _T& aItem)
      {
        append(aItem);
      }

    //----------

    /// pops the last element off the array
    /// like a stack
    _T& pop(void)
      {
        if (mSize>0)
        {
          mSize--;
          return mArray[mSize];
        }
        else return NULL;
      }

};

//----------------------------------------------------------------------
#endif
