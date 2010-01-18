#ifndef axScript_included
#define axScript_included
//----------------------------------------------------------------------
// forth inspired stack-based script
// TODO: eeor checking, memory safety...

#include <stdio.h>
#include <stdlib.h> // atoi..
#include <string.h>
#include "axString.h"
//#include "axScript_Std.h"

//----------------------------------------------------------------------

//  ! # % & ? $ @
//  _ ^ ~ '
//  () [] {} <>
//  .: ,;

#define MAX_CODESIZE  65536
#define MAX_STACKSIZE 65536

// opcodes
#define op_None   0
#define op_Opcode 1
#define op_Word   2
#define op_Int    3
#define op_Float  4

#define op_Exit   0xffff

//

// token types
#define tty_Unknown 0
//#define tty_Word    1
//#define tty_Int     2
//#define tty_Float   3
//#define tty_Ptr     4

//// script states
//#define sst_Compile 1
//#define sst_Execute 2

class axScript;

//----------------------------------------------------------------------
// token
//----------------------------------------------------------------------

class axToken
{
  protected:
    axScript* mOwner;
    char      mName[16];
    int       mType;
    union     mValue
    {
      int   i;
      float f;
      void* p;
    };
  public:
    axToken(axScript* aOwner, char* aName, int aType) { mOwner=aOwner; strcpy(mName,aName); mType=aType; }
    inline char* name(void) { return mName; }
};

typedef axArray<axToken*> axTokens;

//----------------------------------------------------------------------
// word
//----------------------------------------------------------------------

class axWord
{
  protected:
    char  mName[16];
    int   mPos;
  public:
    axWord(char* aName, int aPos) { strcpy(mName,aName); mPos=aPos; }
    inline char* name(void) { return mName; }
    inline void  pos(int aPos) { mPos=aPos; }
    inline int   pos(void) { return mPos; }
};

typedef axArray<axWord*> axWords;

//----------------------------------------------------------------------
// opcode
//----------------------------------------------------------------------

class axOpcode
{
  protected:
    axScript* mOwner;
    int       mIndex;
    char      mName[16];
  public:
    axOpcode(axScript* aOwner, axString aName) { mOwner=aOwner; strcpy(mName,aName.ptr()); }
    inline char* name(void) { return mName; }
    inline void  setOwner(axScript* aOwner) { mOwner=aOwner; }
    inline void  setIndex(int aIndex) { mIndex=aIndex; }
    virtual void compile(int aIndex) {}
    virtual void execute(void) {}
};

typedef axArray<axOpcode*> axOpcodes;

//----------------------------------------------------------------------
//
// script
//
//----------------------------------------------------------------------

class axScript
{
  protected:
    int       mSrcSize;
    char*     mSource;
    axTokens  mTokens;
    axOpcodes mOpcodes;
    int*      mCode;
    int       mCodeSize;
    int*      mDataStack;
    int*      mCallStack;
    int       mCodePos;
    int       mDataPos;
    int       mCallPos;
    axWords   mWords;
    //int       mState;
    int       mCurToken;

  public:

    axScript()
      {
        mSrcSize = 0;
        mSource = NULL;
        mTokens.clear();
        mOpcodes.clear();
        mCode = new int[MAX_CODESIZE];
        mDataStack = new int[MAX_STACKSIZE];
        mCallStack = new int[MAX_STACKSIZE];
        mCodePos = 0;
        mDataPos = 0;
        mCallPos = 0;
        mWords.clear();
        append_stdlib();
        mCurToken = 0;
      }

    //----------

    virtual ~axScript()
      {
        delete[] mCode;
        if (mSource) delete[] mSource;
        deleteTokens();
        deleteOpcodes();
      }

    //--------------------------------------------------

    void dumpCode(void)
      {
        printf("code size: %i\n",mCodeSize);
        for (int i=0; i<mCodeSize; i++)
        {
          int n = mCode[i];
          if ( ((i&7)==7) || (i==mCodeSize-1) ) printf("%08x\n",n);
          else printf("%08x ",n);
        }
      }

    //--------------------------------------------------

    inline int        codePos(void)       { return mCodePos; }
    inline void       codePos(int aPos)   { mCodePos = aPos; }
    inline int        next(void)          { return mCode[mCodePos++]; };
    inline char*      nextToken(void)     { return mTokens[mCurToken++]->name(); }
    inline int        numOpcodes(void)    { return mOpcodes.size(); }
    inline axOpcode*  opcode(int aIndex)  { return mOpcodes[aIndex]; }

    inline void   deleteTokens(void)  { for (int i=0; i<mTokens.size(); i++) delete mTokens[i]; }
    inline void   deleteOpcodes(void) { for (int i=0; i<mOpcodes.size(); i++) delete mOpcodes[i]; }

    inline void writeCode(int aCode)  { mCode[mCodePos++] = aCode; }
    inline void pushData(int aData)   { mDataStack[mDataPos++] = aData; }
    //inline void pushCall(int aCall)   { mCallStack[mCallPos++] = aCall; }
    inline void pushCall(void)        { mCallStack[mCallPos++] = mCodePos; }
    inline int  popData(void)         { return mDataStack[--mDataPos]; }
    //inline int  popCall(void)         { return mCallStack[--mCallPos]; }
    inline void popCall(void)         { mCodePos = mCallStack[--mCallPos]; }
    inline void callWord(int aWord)   { pushCall(/*mCodePos*/); mCodePos = mWords[aWord]->pos(); }
    //inline void returnWord(void)      { mCodePos = popCall(); }

    //----------

    void appendOpcode(axOpcode* aOpcode)
      {
        aOpcode->setOwner( this );
        aOpcode->setIndex( mOpcodes.size() );
        mOpcodes.append(aOpcode);
      }

    //----------

    int findOpcode(char* token)
      {
        for (int i=0; i<mOpcodes.size(); i++) { if (strcmp(token,mOpcodes[i]->name())==0) return i; }
        return -1;
      }

    //----------

    void appendWord(axWord* aWord)
      {
        aWord->pos( mCodePos );
        mWords.append(aWord);
      }

    //----------

    int findWord(char* token)
      {
        for (int i=0; i<mWords.size(); i++) { if (strcmp(token,mWords[i]->name())==0) return i; }
        return -1;
      }

    int wordPos(int num) { return mWords[num]->pos(); }

    //----------

    int parse(axString aSource)
      {
        int res = 0;
        mSrcSize = aSource.length();
        mSource = new char[mSrcSize];
        memcpy(mSource,aSource.ptr(),mSrcSize);
        for (int i=0; i<mSrcSize; i++) if (mSource[i]<32) mSource[i]=' ';
        char* temp;
        char* delimiters = (char*)" ";
        char* token = NULL;
        token  = strtok_r(mSource," ",&temp);
        do { mTokens.append(new axToken(this,token,tty_Unknown)); }
        while ((token=strtok_r(NULL,delimiters,&temp)));
        return res;
      }

    //--------------------------------------------------

    virtual void compileOpcode(int aOpcode)
      {
        //TRACE("  - [pos:%i] op_Opcode (%i), %i ('%s')\n",mCodePos,op_Opcode, aOpcode, mOpcodes[aOpcode]->name());
        mOpcodes[aOpcode]->compile(aOpcode);
      }

    //----------

    virtual void compileWord(int aWord)
      {
        //TRACE("  - [pos:%i] op_Word (%i), %i ('%s')\n",mCodePos,op_Word, aWord, mWords[aWord]->name());
        writeCode(op_Word);
        writeCode( aWord/*mWords[aWord]->pos()*/ );
      }

    //----------

    virtual void compileInt(int aValue)
      {
        //TRACE("  - [pos:%i] op_Int (%i), %i\n",mCodePos,op_Int, aValue);
        writeCode(op_Int);
        writeCode(aValue);
      }

    //----------

    virtual void compileFloat(float aValue)
      {
        int value = *(int*)(&aValue);
        //TRACE("  [pos:%i] op_Float (%i), %i (%f)\n",mCodePos,op_Float, value, aValue);
        writeCode(op_Float);
        writeCode(value);
      }

    //--------------------------------------------------

    virtual int compile(axString aSource)
      {
        //TRACE("compiling..........\n");
        //TRACE("- parsing\n");
        parse(aSource);
        mCurToken = 0;
        //for( int i=0; i<mTokens.size(); i++)
        do
        {
          //char* token = mTokens[i]->name();
          char* token = nextToken(); //mTokens[mCurToken]->name();
          //TRACE("  token:'%s'\n",token);
          int op = findOpcode(token);
          if (op>=0) compileOpcode(op);
          else
          {
            int wo = findWord(token);
            if (wo>=0) compileWord(wo);
            else
            {
              if (strchr(token,'.')!=NULL) compileFloat(atof(token));
              else compileInt(atoi(token));
            }
          }
        } while ( mCurToken<mTokens.size() );
        //TRACE("  exit\n");
        //TRACE("  - [pos:%i] op_Exit (%i)\n",mCodePos,op_Exit);
        writeCode(op_Exit);
        //TRACE("..........compiled ok\n");
        mCodeSize = mCodePos;
        return mCodeSize;
      }

    //--------------------------------------------------

    //#define NEXT mCode[mCodePos++]
    virtual int execute(int aStart=0)
      {
        //TRACE("executing..........\n");
        int result = 0;
        mCodePos = aStart;
        mDataPos = 0;
        mCallPos = 0;
        int val;
        int opcode = next();//NEXT;
        while (opcode!=op_Exit)
        {
          switch(opcode)
          {
            //case op_None:
            //  //TRACE("- op_None\n");
            //  break;
            case op_Opcode:
              val = next();//NEXT;
              //TRACE("- op_Opcode %i\n",val);
              mOpcodes[val]->execute();
              break;
            case op_Word:
              val = next();
              //TRACE("- op_Word %i\n",val);
              pushCall();
              mCodePos = mWords[val]->pos();    //TODO: save destination pos directly
              break;
            case op_Int:
              //TRACE("- op_Int\n");
              pushData(next());
              break;
            case op_Float:
              //TRACE("- op_Float\n");
              pushData(next());
              break;
            case op_Exit:
              //TRACE("- op_Exit\n");
              break;
            //default:
            //  break;
          }
          opcode = next();//NEXT;
        }
        //TRACE("..........executed ok\n");
        return result;
      }

    //--------------------------------------------------

    #ifdef AX_SCRIPT_STDLIB

    #include "axScript_Std.h"
    void append_stdlib(void)
      {
        appendOpcode( new opAdd() );
        appendOpcode( new opDot() );
        appendOpcode( new opColon() );
        appendOpcode( new opSemiColon() );
      }

    #endif

};


//----------------------------------------------------------------------
#endif
