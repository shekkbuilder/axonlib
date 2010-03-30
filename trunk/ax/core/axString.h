#ifndef axString_included
#define axString_included
//----------------------------------------------------------------------
// char* wrapper

#include <string.h>

//----------------------------------------------------------------------

class axString
{
  private:
    char* mText;
  public:

    axString()                            { mText = (char*)""; }
    axString(char* txt)                   { mText = txt; }
    axString(const char* txt)             { mText = (char*)txt; }
  //axString(axString* str)               { mText = str->mText; }

    char*    ptr()                        { return mText; }
    void     set(char* txt)               { mText = txt; }
    bool     empty()                      { return mText[0]==0; }
    int      length()                     { return strlen(mText); }

    char     operator [] (int i)          { return mText[i]; }

    void     operator = (axString str)    { mText = str.mText; }
  //void     operator = (axString& str)   { mText = str.mText; }
    void     operator = (axString* str)   { mText = str->mText; }
    void     operator = (char* txt)       { mText = txt; }
    void     operator = (const char* txt) { mText = (char*)txt; }

};

//----------------------------------------------------------------------

static axString STR_EMPTY((char*)"");
static axString STR_NONE((char*)"none");

#include "axArray.h"
typedef axArray<axString> axStrings;

//----------------------------------------------------------------------
#endif
