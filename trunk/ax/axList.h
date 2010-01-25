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
 * \brief desc
 */

/**
 * \brief desc
 *
 * long desc
 *
 */


#ifndef axList_included
#define axList_included
//----------------------------------------------------------------------

class axListNode
{
  protected:
  //public:
    axListNode* mPrev;
    axListNode* mNext;
};

//----------

class axList
{

  protected:
  //public:

    int mSize;
    axListNode* mHead;
    axListNode* mTail;

  public:

    axList() { clear(); }
    ~axList() {}

    void clear() { mSize=0; mHead=NULL; mTail=NULL; }
    axListNode* next(axListNode* aNode) { return aNode->mNext; }

    void insertHead(axListNode* aNode)
      {
//        aNode->mPrev = NULL;
//        aNode->mNext = mHead;
//        if (mHead) mHead->mPrev = aNode;
//        mHead = aNode;
//        mSize++;
      };

    void insertTail(axListNode* aNode) {}
    void insertBefore(axListNode* aNew, axListNode* aNode) {}
    void insertAfter(axListNode* aNew, axListNode* aNode) {}
    void remove(axListNode* aNode) {}

};

//----------------------------------------------------------------------
#endif
