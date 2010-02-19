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
 * \file axString.h
 * \brief axString
*/ 

/**
 * \brief desc
 *
 * long desc
 *
 */

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
