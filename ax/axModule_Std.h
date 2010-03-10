#ifndef axModule_Std_included
#define axModule_Std_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// extern, io
//----------------------------------------------------------------------

class modAudioIn : public axModule
{
  private:
    axPin*  mLeft;
    axPin*  mRight;
  public:
    modAudioIn(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Const;
        appendOutput( mLeft  = new axPin(this,"left", mpt_Data,mpm_Const) );
        appendOutput( mRight = new axPin(this,"right",mpt_Data,mpm_Const) );
      }
    inline void setInputs(float aLeft, float aRight)
      {
        mLeft->writeValue(aLeft);
        mRight->writeValue(aRight);
      }
};

//----------

class modAudioOut : public axModule
{
  private:
    axPin*  mLeft;
    axPin*  mRight;
  public:
    modAudioOut(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Const;
        appendInput( mLeft  = new axPin(this,"left", mpt_Data,mpm_Const) );
        appendInput( mRight = new axPin(this,"right",mpt_Data,mpm_Const) );
      }
};

//----------

class modConst : public axModule
{
  private:
    axPin*  mOut;
  public:
    modConst(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Const;
        appendOutput( mOut = new axPin(this,"out",mpt_Data,mpm_Const,1) );
      }
    inline void setValue(float aValue)
      {
        mOut->writeValue(aValue);
      }
};

//----------------------------------------------------------------------
// math
//----------------------------------------------------------------------

class modNoise : public axModule
{
  private:
    axPin*  mOut0;
  public:
    modNoise(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendOutput( mOut0 = new axPin(this,"output",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        mOut0->writeValue( axRandomSigned() );
      }
};

//----------------------------------------------------------------------
// math
//----------------------------------------------------------------------

class modAdd : public axModule
{
  private:
    axPin *mIn0, *mIn1;
    axPin *mOut0;
  public:
    modAdd(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in0",mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1",mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in0 = mIn0->readValue();
        float in1 = mIn1->readValue();
        mOut0->writeValue( in0+in1 );
      }
};

//----------

class modSub : public axModule
{
  private:
    axPin *mIn0, *mIn1;
    axPin *mOut0;
  public:
    modSub(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in0",mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1",mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in0 = mIn0->readValue();
        float in1 = mIn1->readValue();
        mOut0->writeValue( in0-in1 );
      }
};

//----------

class modMul : public axModule
{
  private:
    axPin *mIn0, *mIn1;
    axPin *mOut0;
  public:
    modMul(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in0",mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1",mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in0 = mIn0->readValue();
        float in1 = mIn1->readValue();
        mOut0->writeValue( in0*in1 );
      }
};

//----------

class modDiv : public axModule
{
  private:
    axPin *mIn0, *mIn1;
    axPin *mOut0;
  public:
    modDiv(axModuleListener* aOwner, axString aName)
    : axModule(aOwner,aName)
      {
        mMode = mmo_Dynamic;
        appendInput(  mIn0  = new axPin(this,"in0",mpt_Data) );
        appendInput(  mIn1  = new axPin(this,"in1",mpt_Data) );
        appendOutput( mOut0 = new axPin(this,"out",mpt_Data) );
      }
    virtual void doProcess(void)
      {
        float in0 = mIn0->readValue();
        float in1 = mIn1->readValue();
        mOut0->writeValue( in0/in1 );
      }
};

//----------------------------------------------------------------------
#endif
