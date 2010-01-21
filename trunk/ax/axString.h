#ifndef axString_included
#define axString_included
//----------------------------------------------------------------------
// char* wrapper

#include <string.h>

//----------------------------------------------------------------------

/// char* wrapper

class axString
{
  private:
    char* mText;
  public:

    axString()                            { mText = (char*)""; };   ///< initialize empty string
    axString(char* txt)                   { mText = txt; };         ///< initalize string from char*
    axString(const char* txt)             { mText = (char*)txt; };  ///< initialize string with char*
  //axString(axString* str)               { mText = str->mText; };

    char*    ptr()                        { return mText; }         ///< return ptr to string
    void     set(char* txt)               { mText = txt; }          ///< set internal char* ptr
    bool     empty()                      { return mText[0]==0; }   ///< return bool if string is empty (element[0]==0)
    int      length()                     { return strlen(mText); } ///< return length of string

    char     operator [] (int i)          { return mText[i]; }      ///< access characters in string like an array

    void     operator = (axString str)    { mText = str.mText; }    ///< set string from another string
  //void     operator = (axString& str)   { mText = str.mText; }
    void     operator = (axString* str)   { mText = str->mText; }   ///< set string from ptr to another string
    void     operator = (char* txt)       { mText = txt; }          ///< set string from chars
    void     operator = (const char* txt) { mText = (char*)txt; }   ///< set string from chars (const)

};

//----------------------------------------------------------------------

//const char* NULLSTR = "\0";
static axString STR_EMPTY((char*)""); ///< const empty string
static axString STR_NONE((char*)"none");  ///< const 'none' string

#include "axArray.h"
typedef axArray<axString> axStrings;  ///< list of strings

//----------------------------------------------------------------------
#endif
