/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0 
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See LICENSE_AX for more details.
 *  
 * You should have received a copy of the Axonlib License 
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file axSymbol.h
  \brief symbol
*/

#ifndef ax_included
#define ax_included
//----------------------------------------------------------------------

/*
  - should this be a separate class containing a bitmap/surface with 16x16 small icons,
  and methods to blit part of the larger bitpas as a symbol/glyph
  - or a axBitmap subclass, with selectGlyph method
  - or something else?
  (todo: maker a decision and go with it..)
*/

/**
  \brief std symbols & icons
*/

class axSymbol// : public ax
{

  public:

    axSymbol() {}

    virtual ~ax() {}

    //----------------------------------------

};

//----------------------------------------------------------------------
#endif

/*

char* arrow_down =
  "          "
  "    XX    "
  "    XX    "
  "+XXXXXXXX+"
  " +XXXXXX+ "
  "  +XXXX+  "
  "   +XX+   "
  "    ++    "
  "          "

  axSymbol::createSymbol(0,arrow_down);
  axCanvas::drawSymbol(100,20,0);

*/
