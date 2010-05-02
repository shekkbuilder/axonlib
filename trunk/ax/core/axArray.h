#ifndef axArray_included
#define axArray_included
//----------------------------------------------------------------------

//#include <stdlib.h> // malloc, realloc, free
#include <axDefines.h>
#include <axStdlib.h>

#define SIZE_INIT 16  ///< initial size of array
#define SIZE_MULT 2   ///< amount to increase/decrease the array size when needed (multiplier)

template<class _T>
class axArray
{
	private:

    _T* mArray;                // the memory
    unsigned int mTsize;       // size of type
    unsigned int mSize;        // array size
    unsigned int mRealSize;    // real (allocated) size of array     

  public:

    // empty constructor.
    /*
      initializes the array to a defined size (SIZE_INIT, default 16)
    */

		axArray()
			{
		  	mTsize = sizeof(_T);
        mRealSize = SIZE_INIT;
			  mSize = 0;
			  mArray = (_T*)malloc(mRealSize*mTsize);
        //mArray = (_T*) operator new ( mRealSize*mTsize );
			}

    // copy constructor.
    /*
      initializes array from another array (copies data)
      \param aArray the other array you want to copy
    */

    axArray(const axArray& aArray)
      {
        mTsize = sizeof(_T);
        mArray = (_T*)malloc(mTsize*aArray.mRealSize);
        //mArray = (_T*) operator new ( mTsize*aArray.mRealSize );
        memcpy(mArray, aArray.mArray, mTsize*aArray.mRealSize);
        //axMemcpy(mArray, aArray.mArray, mTsize*aArray.mRealSize);
        mRealSize = aArray.mRealSize;
        mSize = aArray.mSize;
      }

    // destructor
    /*
      (deletes the buffer)
    */

    ~axArray()
      {
        if (mArray)
        {
          //delete [] mArray;
          free(mArray);
          mArray = NULL;
        }
      }

    // [] operator
    /*
      so that you can do things like int a = mArray[3] to get the 4th item
    */
    _T& operator [] (const unsigned int aIndex)
      {
        return mArray[aIndex];
      }

    // get item
    /*
      returns specified item.
      \param aIndex index of item you want (starting from 0)
      \return the item
    */
    _T& item(const unsigned int aIndex)
      {
        return mArray[aIndex];
      }

    // size of array
    /*
      returns size of array
      \return number of items in array
    */
    int size()
      {
        return mSize;
      }

    // return pointer
    /*
      \return pointer to first item [0]
    */
    void* ptr()
      {
        return mArray;
      }

    // = operator
    /*
      ini/copy array from another array
    */
    axArray& operator = (const axArray &aArray)
      {
        if (this==&aArray)
          return *this;
        if (aArray.mSize==0)
          clear();
        setSize(aArray.mSize);
        //axMmcpy(mArray, aArray.mArray, mTsize*aArray.mSize);
        memcpy(mArray, aArray.mArray, mTsize*aArray.mSize);
        return *this;
      }

    //void clear()
    //  {
    //    mSize = 0;
    //    mArray = (_T*)realloc(mArray, mTsize*SIZE_INIT);
    //    mRealSize = SIZE_INIT;
    //  }

    // clear array.
    /*
      optionally deleting all items
      \param aErase if true, also resize the array (reallocate the array memory).
      default is true.
    */
    void clear(bool aErase=true)
      {
        mSize = 0;
        if (aErase)
        {
          mArray = (_T*)realloc(mArray, mTsize*SIZE_INIT);
          //delete[] mArray;
          //mArray = (_T*) operator new ( mTsize*SIZE_INIT );
          mRealSize = SIZE_INIT;
        }
      }

    // append item
    /*
      append an item to the end of the array
      \param aItem item to append
    */
    void append(const _T& aItem)
      {
        mSize++;
        if (mSize > mRealSize)
        {
          mArray = (_T*)realloc(mArray, mTsize*mRealSize);
          /*
          mArray = (_T*)axRealloc(mArray, mTsize*mRealSize,
          mTsize*(mRealSize *= SIZE_MULT));
          */
          
        }
        mArray[mSize-1] = aItem;
      }

    // resize array
    /*
      set new array size. re-allocates memory if needed
    */
    void setSize(const unsigned int aSize)
      {
        if (aSize != 0)
        {
          if ( (aSize>mRealSize) || (aSize<mRealSize/2) )
          //if ( (mSize>mRealSize) || (mSize<mRealSize/SIZE_MULT) )
          {
            mRealSize = aSize;
            mArray = (_T*)realloc(mArray, mTsize*mSize);
            //mArray = (_T*)axRealloc(mArray, mTsize*mSize, mTsize*aSize);
            mSize = aSize;
          }
        }
        else clear();
      }

    // remove item
    /*
      remove an item from the array. (no realloc)
      \param aPos index of item to remove
    */
    void remove(const unsigned int aPos)
      {
        if (mSize==1) clear();
        else
        {
          for(unsigned int i=aPos; i<mSize-1; i++ )
            mArray[i] = mArray[i+1];
          mSize--;
        }
      }

    //----------

    // push item
    /*
      push item to the end of the array, so you can use it like a stack
    */
    //void push(const _T& aItem)
    void push(_T aItem)
      {
        append(aItem);
      }

    // pop item
    /*
      pops the last item off the array, like a stack
    */
    //_T& pop(void)
    _T pop(void)
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
