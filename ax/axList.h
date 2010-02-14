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

#ifndef axList_included
#define axList_included
//----------------------------------------------------------------------

class axListNode
{
  friend class axList;
  protected:
    axListNode* mPrev;
    axListNode* mNext;
  public:
    inline axListNode* getNext(void) { return mNext; }
    inline axListNode* getPrev(void) { return mPrev; }

};

//----------------------------------------------------------------------

class axList
{
  protected:

    int         mSize;
    axListNode* mHead;
    axListNode* mTail;

  public:

    axList()
      {
        reset();
      }

    //----------

    inline axListNode* getHead(void) { return mHead; }
    inline axListNode* getTail(void) { return mTail; }

    inline axListNode* getNext(axListNode* aNode) { return aNode->mNext; }
    inline axListNode* getPrev(axListNode* aNode) { return aNode->mPrev; }

    inline void reset(void)
      {
        mHead = NULL;
        mTail = NULL;
      }

    //----------------------------------------

    void appendHead(axListNode* aNode)
      {
        aNode->mPrev = NULL;
        if (mTail==NULL)
        {
          mTail = aNode;
          aNode->mNext = NULL;
        }
        else
        {
          mHead->mPrev = aNode;
          aNode->mNext = mHead;
        }
        mHead = aNode;
        mSize++;
      }

    //----------

    void appendTail(axListNode* aNode)
      {
        aNode->mNext = NULL;
        if (mHead==NULL)
        {
          mHead = aNode;
          aNode->mPrev = NULL;
        }
        else
        {
          mTail->mNext = aNode;
          aNode->mPrev = mTail;
        }
        mTail = aNode;
        mSize++;
      }

    //----------------------------------------

    void removeHead(void)
      {
        if (mHead==mTail) reset();
        else
        {
          mHead = mHead->mNext;
          mHead->mPrev = NULL;
        }
      }

    //----------

    void removeTail(void)
      {
        if (mHead==mTail) reset();
        else
        {
          mTail = mTail->mPrev;
          mTail->mNext = NULL;
        }
      }

    //----------------------------------------

    void append(axListNode* aNode)
      {
        appendTail(aNode);
      }

    //----------

    void remove(axListNode* aNode)
      {
        //if (aNode==NULL) return;
        if (aNode==mHead) removeHead();
        else if (aNode==mTail) removeTail();
        else
        {
          aNode->mNext->mPrev = aNode->mPrev;
          aNode->mPrev->mNext = aNode->mNext;
        }
      }

};

//----------------------------------------------------------------------
#endif


///*
// * This file is part of Axonlib.
// *
// * Axonlib is free software: you can redistribute it and/or modify
// * it under the terms of the GNU Lesser General Public License as
// * published by the Free Software Foundation, either version 3
// * of the License, or (at your option) any later version.
// *
// * Axonlib is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU Lesser General Public License for more details.
// *
// * You should have received a copy of the GNU Lesser General Public
// * License along with Axonlib.
// * If not, see <http://www.gnu.org/licenses/>.
// */
//
///**
// * @file
// * \brief desc
// */
//
///**
// * \brief desc
// *
// * long desc
// *
// */
//
//
//#ifndef axList_included
//#define axList_included
////----------------------------------------------------------------------
//
//class axListNode
//{
//  friend class axList;
//  protected:
//    axListNode* mPrev;
//    axListNode* mNext;
//  public:
//    inline axListNode* getPrev(void) { return mPrev; }
//    inline axListNode* getNext(void) { return mNext; }
//
//};
//
////----------------------------------------------------------------------
//
//class axList
//{
//
//  protected:
//
//    axListNode* mHead;
//    axListNode* mTail;
//
//  public:
//
//    axList()
//      {
//        clear();
//      }
//
//    ~axList()
//      {
//      }
//
//    //----------
//
//    void clear()
//      {
//        //mSize = 0;
//        mHead = NULL;
//        mTail = NULL;
//      }
//
//    inline axListNode* getHead(void) { return mHead; }
//    inline axListNode* getTail(void) { return mTail; }
//    inline axListNode* getPrev(axListNode* aNode) { return aNode->mPrev; }
//    inline axListNode* getNext(axListNode* aNode) { return aNode->mNext; }
//
//    void insertHead(axListNode* aNode) {}
//    void insertTail(axListNode* aNode) {}
//    void insertBefore(axListNode* aNew, axListNode* aNode) {}
//    void insertAfter(axListNode* aNew, axListNode* aNode) {}
//
//    void remove(axListNode* aNode) {}
//    void removeHead(void) {}
//    void removeTail(void) {}
//
//    void push(axListNode* aNode)
//      {
//        insertTail(aNode);
//      }
//    axListNode* pop(void)
//      {
//        axListNode* N = getTail();
//        removeTail();
//        return N;
//      }
//
//};
//
////----------------------------------------------------------------------
//#endif
