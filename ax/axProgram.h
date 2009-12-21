#ifndef axProgram_included
#define axProgram_included
//----------------------------------------------------------------------

class axProgram
{
  public:
    //axArray<axParameter*> mParameters;
    axParameters mParameters;
};

//----------

class axBank
{
  public:
    axArray<axProgram*> mPrograms;
};

//----------------------------------------------------------------------
#endif
