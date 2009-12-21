#ifndef axList_included
#define axList_included
//----------------------------------------------------------------------

class axListNode
{
  public:
    axListNode* mPrev;
    axListNode* mNext;
};

//----------

class axList
{

  public:

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
