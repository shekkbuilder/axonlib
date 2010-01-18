#ifndef axScript_Std_included
#define axScript_Std_included
//----------------------------------------------------------------------

#include "axScript.h"

//----------------------------------------------------------------------
// base
//----------------------------------------------------------------------

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

//----------

// :
class opDollar : public axOpcode
{
  public:
    opDollar() : axOpcode(NULL,"$") {}
    virtual void compile(int aIndex) { mOwner->appendLabel(new axWord( mOwner->nextToken(), mOwner->codePos() )); }
    //virtual void execute(void) {)
};

//----------------------------------------------------------------------
// stack (data)
//----------------------------------------------------------------------

// DUP
class opDup : public axOpcode
{
  public:
    opDup() : axOpcode(NULL,"DUP") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->dupData(); }
};

//----------

// DROP
class opDrop : public axOpcode
{
  public:
    opDrop() : axOpcode(NULL,"DROP") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { mOwner->dropData(); }
};

//----------------------------------------------------------------------
// conditionals
//----------------------------------------------------------------------

// =
class opEqual : public axOpcode
{
  public:
    opEqual() : axOpcode(NULL,"=") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n1==n2); }
};

//----------

// <>
class opNotEqual : public axOpcode
{
  public:
    opNotEqual() : axOpcode(NULL,"<>") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n1!=n2); }
};

//----------

// >
class opGreater : public axOpcode
{
  public:
    opGreater() : axOpcode(NULL,">") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2>n1); }
};

//----------

// <
class opLess : public axOpcode
{
  public:
    opLess() : axOpcode(NULL,"<") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { int n1=mOwner->popData(); int n2=mOwner->popData(); mOwner->pushData(n2<n1); }
};

//----------------------------------------------------------------------
// flow control
//----------------------------------------------------------------------

// EXIT
class opExit : public axOpcode
{
  public:
    opExit() : axOpcode(NULL,"EXIT") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Exit); }
    //virtual void execute(void) {}
};

//----------

// IF
class opIf : public axOpcode
{
  public:
    opIf() : axOpcode(NULL,"IF") {}
    virtual void compile(int aIndex)
      {
        mOwner->writeCode(op_Opcode);
        mOwner->writeCode(aIndex);
        mOwner->pushCond();
        mOwner->writeCode(0);   // placeholder
      }
    virtual void execute(void)
      {
        int cond = mOwner->popData();
        int pos  = mOwner->next();
        if (!cond) mOwner->codePos(pos);
      }
};

//TODO: ELSE
//----------

// ENDIF
class opEndif : public axOpcode
{
  public:
    opEndif() : axOpcode(NULL,"ENDIF") {}
    virtual void compile(int aIndex)
      {
        int pos = mOwner->popCond();
        mOwner->writeCodeAt(pos,mOwner->codePos());
      }
    //virtual void execute(void) {}
};


//----------------------------------------------------------------------
// arithmetic (int)
//----------------------------------------------------------------------

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
// io
//----------------------------------------------------------------------

// .
class opPrintInt : public axOpcode
{
  public:
    opPrintInt() : axOpcode(NULL,".") {}
    virtual void compile(int aIndex) { mOwner->writeCode(op_Opcode); mOwner->writeCode(aIndex); }
    virtual void execute(void) { printf("%i\n",mOwner->popData()); }
};

//----------------------------------------


//----------------------------------------




//----------------------------------------------------------------------
#endif
