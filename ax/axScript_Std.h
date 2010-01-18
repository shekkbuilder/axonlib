#ifndef axScript_Std_included
#define axScript_Std_included
//----------------------------------------------------------------------

#include "axScript.h"

//----------------------------------------------------------------------

class opAdd : public axOpcode
{
  public:
    opAdd() : axOpcode(NULL,"+") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->pushData( mOwner->popData() + mOwner->popData() ); }
};

//----------------------------------------------------------------------

class opDot : public axOpcode
{
  public:
    opDot() : axOpcode(NULL,".") {}
    //virtual void compile(int aIndex) {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { printf("%i\n",mOwner->popData()); }
};

//----------------------------------------------------------------------

class opColon : public axOpcode
{
  public:
    opColon() : axOpcode(NULL,":") {}
    virtual void compile(int aIndex)
      {
        char* name = mOwner->nextToken();
        int pos = mOwner->codePos();
        mOwner->appendWord(new axWord(name,pos));
      }
    //virtual void execute(void)
    //  {
    //    mOwner->callWord( mOwner->popData() );
    //  }
};

//----------------------------------------------------------------------

class opSemiColon : public axOpcode
{
  public:
    opSemiColon() : axOpcode(NULL,";") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->popCall(); }
};

//----------------------------------------------------------------------
#endif
