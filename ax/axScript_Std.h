#ifndef axScript_Std_included
#define axScript_Std_included
//----------------------------------------------------------------------

#include "axScript.h"

//----------------------------------------------------------------------

// ;
class opExit : public axOpcode
{
  public:
    opExit() : axOpcode(NULL,"EXIT") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Exit); }
    //virtual void execute(void) {}
};

//----------

// :
class opColon : public axOpcode
{
  public:
    opColon() : axOpcode(NULL,":") {}
    virtual void compile(int aIndex) { mOwner->appendWord(new axWord( mOwner->nextToken(), mOwner->codePos() )); }
    //virtual void execute(void) {)
};

//----------

// ;
class opSemiColon : public axOpcode
{
  public:
    opSemiColon() : axOpcode(NULL,";") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->popCall(); }
};

//----------------------------------------

// .
class opDot : public axOpcode
{
  public:
    opDot() : axOpcode(NULL,".") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { printf("%i\n",mOwner->popData()); }
};

//----------------------------------------

// +
class opAdd : public axOpcode
{
  public:
    opAdd() : axOpcode(NULL,"+") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->pushData( mOwner->popData() + mOwner->popData() ); }
};

//----------

// -
class opSub : public axOpcode
{
  public:
    opSub() : axOpcode(NULL,"-") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->pushData( mOwner->popData() - mOwner->popData() ); }
};

//----------

// *
class opMul : public axOpcode
{
  public:
    opMul() : axOpcode(NULL,"*") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->pushData( mOwner->popData() * mOwner->popData() ); }
};

//----------

// /
class opDiv : public axOpcode
{
  public:
    opDiv() : axOpcode(NULL,"/") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->pushData( mOwner->popData() / mOwner->popData() ); }
};

//----------------------------------------------------------------------
#endif
