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
 * \file doxygen_pages.h
 * \brief --- should hide this file from list 
 */

//----------------------------------------------------------------------
//
// MAIN PAGE
//
//----------------------------------------------------------------------
/**  
  \image html logo.png

  \mainpage axonlib mainpage 

  <h2>introduction:</h2>
  <b>axonlib</b> is a standalone, multiplatform (win32/linux) library for
  vst plugins and applications, which focuses on the ease of
  use, while rataining 'low-level' control. it is released under the axonlib
  license (AXL) which is based on the GNU LGPL.  
  <br>
  
  <h2>
  ### NOTE: the examples and methods here are outdated,
  but should be renewed soon.
  </h2>
  
  <h2>CONTENTS:</h2>
  - \subpage installation
  - \subpage examples
  - \subpage compile
  - \subpage debug
  - \subpage bin2h
  - \subpage screenshots  
  - \subpage license
  
*/
  
//----------------------------------------------------------------------
//
// INSTALL
//
//----------------------------------------------------------------------

/**
  \page installation installation
  <b>go to:</b> <br>
  http://axonlib.googlecode.com <br>
   
  you can get the latest revision from the svn reposotory or download the latest
  zip archive from the download section. <br>

  <b>get the vst sdk from:</b> <br>
  http://ygrabit.steinberg.de/~ygrabit/public_html/index.html <br>
  (the 2.4r1 link - it will actually forward to a registration page and you can get 2.4r2) <br>
  
  <br> please make sure you familiarize yourself with both axonlib and the vst sdk licenses. <br>

  <b>create the following folders:</b> <br>

  \code
  axonlib
  axonlib\axonlib <-- copy axonlib here (ax...bin...docs..etc)
  axonlib\vstsdk24 <-- copy vst sdk here (pluginterfaces...public.sdk...etc)
  \endcode

  <b>compiler requirements: </b>  
  
  <hr>
  
  <b>- linux:</b> <br>
  mingw32 - to cross-compile for win32 <br>
  g++ - to compile c++ code for linux <br>
  xorg-dev - required for gui-s. <br>
  <br>
  if you have ubuntu use: <br>
  \code
  sudo apt-get install mingw32
  sudo apt-get install g++
  sudo apt-get install xorg-dev
  \endcode
  
  <hr>
  
  <b>- win32:</b> <br>
  get mingw32 (the one included with dev-c++ will suffice) <br>
  set path to '[mingw32dir]/bin/g++.exe' <br>
  <br>
  dev-c++ - http://www.bloodshed.net/devcpp.html <br>
  mingw - http://www.mingw.org/ <br>
  
  <hr>
  
  <b>- cygwin:</b> <br> 
  -- to compile with cygwin for windows:: <br>
  download & install the standard mingw32, g++ packages <br>

  -- to compile with cygwin for linux:: <br>
  see bottom of 'axonlib/plugins/compile-cygwin' for instructions <br>
  
  you can get cygwin from: <br>
  http://www.cygwin.com/ <br>
  
  <br>
  
*/

//----------------------------------------------------------------------
//
// EXAMPLES
//
//----------------------------------------------------------------------

/**
  \page examples examples

  the absolutely simplest plugin you can make:

  \code
  // todo: new examples
  \endcode

  template for gui plugin:

  \code
  // todo: new examples
  \endcode
  
*/

//----------------------------------------------------------------------
//
// COMPILE
//
//----------------------------------------------------------------------

/**
  \page compile compilation

  there are ready to use compile scripts in the 'axonlib/plugins' folder.
  the compile scripts will move the resulted binaries to '../bin'.
  edit them to suit your needs.

  \note under win32 if you have a source file in a subfolder relative to 'compile.cmd'
  you should call the compile script like:
  \code
  cd mysubfolder
  ../compile.cmd myfx.cpp
  \endcode

  default compiler lines: <br>
  \code
  //win32::
  g++.exe -I../ax -I../../vstsdk24 -shared -mwindows -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //linux for linux::
  g++ -I../ax -I../../vstsdk24 -lX11 -lpthread -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //linux for win32::
  i586-mingw32msvc-g++ -I../ax -I../../vstsdk24 -mwindows -shared [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //cygwin for win32::
  g++ -I../ax -I../../vstsdk24 -shared -mwindows -mno-cygwin -lpthread [inputfile.cpp] -s -o [outputfile.cpp] -Wl,-gc-sections
  //cygwin for linux::
  (see bottom of 'axonlib/plugins/compile-cygwin')
  \endcode

  (see 'man gcc' for more info) <br>
  
  suggested optimization flags:
  \code
  -O3 -Os -fdata-sections -ffunction-sections -funroll-loops
  \endcode

  for monitoring warnings use (note that the vst sdk 2.4 may pop a warning or two):
  \code
  -pedantic -fpermissive -W -Wall
  \endcode

  enabling sse optimized math. this will enable gas to optimize the generated asm code with sse and fpu instructions.
  \code
  -msse -mfpmath=sse,387
  \endcode
  <br>
  see this page for more information: <br>
  http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/i386-and-x86_002d64-Options.html
  <br><br>
  enabling 'fast-math' will allow the compiler to cut some corner when the default c math functions are used (sinf, tanf etc).
  this will make such as fast as the included methods in axMath.h but with reduced accuracy.
  \code
   -ffast-math
  \endcode

  stripping the generated binary: <br>
  (see 'man strip' for more info)
  \code
  strip --strip-all [binaryfile]
  //or (i586-mingw32msvc-strip)
  \endcode

*/

//----------------------------------------------------------------------
//
// DEBUG
//
//----------------------------------------------------------------------

/**
 \page debug debug
 
 <b>debug info for axonlib compiled binaries </b>
 
 list of debugging commands with a short description:
 <br>
 \code
 assert() - same as C assert. will break if assert(false) [win32 & linux]
 trace() - output any type of variable and strings similar to std::cout [win32 & linux] 
 msg() - output a message/warning [win32 & linux]
 wdebug() - write output in text window (gui) [win32]
 wtrace() - trace for wine 
 \endcode
 
 <br> <hr> <br>
 
 <b>linux: </b> <br>
 to debug a shared library on linux simply start the program that loads the 
 library from a terminal. this will allow stdout to be routed to the terminal
 output. same for an executable.
 
 <br> <hr> <br>
 
 <b>windows: </b> <br> 
 debugging a dll on windows is a bit more complicated. the way to do it is 
 to create a console programmatically, route stdout to it and disable any
 buffering so that the output is shown immediately. alternatively the windows
 gui api can be used so that a text window is created and text is shown there.
  
 axonlib provides some methods for the above. 
 
 <br> <hr> <br>
 
 see axDebug.h for more info on syntax and examples. 

 */

//----------------------------------------------------------------------
//
// BIN2H
//
//----------------------------------------------------------------------

/**
 \page bin2h bin2h
 
  axonlib includes a simple applications called bin2h (tools/bin2h).
  this application will take any binary file and convert it into a byte
  array. it can be used to create a header file for any image that you
  wish to include in your binary.<br>
  
  usage:
  \code  
  bin2h myimage.png myimage.h myimage
  ...
  
  //the result in output.h:  
  const unsigned myimage_size = 1113;
  const unsigned char myimage[] =
  {
    0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x73, 0x74,
    0x68, 0x3e, 0x0a, 0x0a, 0x23, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20,
    ...
  }
  \endcode
  
  
  you can now include this header file in your source code and use it with the
  image loading procedure:
  \code  
  #include "axBitmapLoader.h"   
  #include "myimage.h" 
  ...
  axSurface* mysurface;
  ...
  mysurface = loadPng(myimage, myimage_size);
  
  \endcode
  
 */

//----------------------------------------------------------------------
//
// SCREENSHOTS
//
//----------------------------------------------------------------------

/**
 \page screenshots screenshots
 
 <!-- 
 <center>
 <b>some of the included fx:</b>
 <br><br>
 \image html screens/scr0.png
 <br>
 \image html screens/scr1.png
 <br>
 \image html screens/scr2.png
 <br> 
 \image html screens/scr3.png
 <br>
 \image html screens/scr4.png
 <br>
 \image html screens/scr5.png
 <br>
 \image html screens/scr6.png
 <br>
 \image html screens/scr7.png
 <br>
 \image html screens/scr8.png
 <br>
 \image html screens/scr9.png
 <br>
 \image html screens/scr10.png
 <br>
 \image html screens/scr11.png
 <br>
 <b>debugging vst fx on windows:</b>
 <br>
 \image html screens/debug.png
 <br>
 
 </center>
 
 -->
 
 */
 
//----------------------------------------------------------------------
//
// LICENSE
//
//----------------------------------------------------------------------
/**
\page license license

Axonlib is released under the Axonlib License (AXL):

\code
================================================================================
                  Axonlib License, Version 1.0 (AXL) 28-02-2010
================================================================================
I. COPYRIGHT NOTICE

    Project: Axonlib
    Website: http://axonlib.googlecode.com/
                            
    Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al

    -----------------------------------------------------------------------

    Everyone is permitted to copy and distribute verbatim copies
    of this license document, but changing it is not allowed.

  * From here below:
    - "the/this Library" or <the library> will be referring to the name of
    this project.
    - <library url> will be referring to this project's website.
    - "the/this License" or "AXL" will be referring to the Axonlib License.
    - "the/this Exception" will be referring to the exception notice in III.

================================================================================
II. TERMS AND CONDITIONS

                              AXONLIB LICENSE
        TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

    This Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3
    of the License, or (at your option) any later version.

    The Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with the Library.
    If not, see <http://www.gnu.org/licenses/>.

================================================================================
III. EXCEPTION NOTICE

                  EXCEPTION NOTICE IN THE CASE OF THE LIBRARY

 1. As a special exception, the copyright holders of the Library give
    permission for additional uses of the code contained in this release of
    the Library as licensed under the Axonlib License, applying either
    version 1.0 of the License, or (at your option) any later version of the
    License as published by the copyright holders of version 1.0 of the
    Axonlib License document (LICENSE_AX).

 2. You may use, copy, link, modify and distribute under your own terms,
    binary object code versions of works based on the Library, such as
    statically or dynamically linkable shared objects or executables,
    unless such are in conflict with the Axonlib License or any other used
    license.

 3. You are not allowed to include source code files which are part of
    the Library in COMMERCIAL projects, with the exception of files whose only
    purpose is to describe the Library interface.
    
 4. Acknowledgement of the use of the Library in closed-source projects is
    OBLIGATORY, with the following text in any documents or visuals that are
    part of your project distribution:
      " <my project name> is using <the library>.
        <library url> "

 5. Including the license files LICENSE_AX, LICENSE_AXDOCS, LGPL and GPL in
    closed-source project distributions is NOT OBLIGATORY, unless the Library
    source code is included as well.

 6. If you copy code from files distributed under the terms of the GNU
    General Public License or the GNU Lesser General Public License into a
    copy of the Library, as this License permits, this Exception does not
    apply to the code that you add in this way. To avoid misleading anyone as
    to the status of such modified files, you can change the licensing
    conditions notice in such files accordingly.

 7. If you write modifications of your own for the Library and decide to
    to release the modified version it must be protected by the Axonlib
    License. However it is your choice whether to permit this Exception to
    apply to your modifications. If you do not wish that, you must change the
    licensing conditions notice in any modified files accordingly.

 8. The following lines of text must remain present in header files that
    are part of the Library at all times and under any licensing conditions:
    "This file is part of <the library>."

                          /END OF EXCEPTION NOTICE

================================================================================
IV. QUESTIONS AND ANSWERS

Q: Can I use this Library for my commercial binaries?
A: Yes, you can, under the restrictions of the Axonlib License.
   The License adds a permissive Exception to the LGPL license. You can link
   library derivatives (binary) statically and/or dynamically to any program
   and distribute them commercially.

Q: What notable restrictions are there in the AXL ?
A: - For commercial binaries you cannot include any source code from the
     library. Please read 3. of the Exception notice.

   - Any modifications you have made on the Library should be released under
     the Axonlib License, the GPL or the LGPL (Please read 6. and 7. of the
     Exception notice).

   - If you are releasing the source code modifications under the Axonlib
     License the files LICENSE_AX, LICENSE_AXDOCS and PREAMBLE should
     also be present.

   - You must notify of the use of the Library for all projects. See III.4.
     for details.

Q: Can i use LGPL or GPL code with the Library?
A: Yes you can. You are allowed to modify the Library files to be released
   under GPL or LGPL, but you will have to change the licensing condition.
   Please read 6. and 7. of the Exception notice.

Q: Is it OK to use the VST SDK with this Library?
A: If you are using the Library with the VST SDK you should read the
   Steinberg VST PlugIns SDK Licensing Agreement as you are not permitted
   to modify the VST SDK or redistribute it. The VST SDK is also NOT FULLY
   COMPATIBLE with the GNU GPL and therefore with a version of the Library
   released under GPL (You cannot distribute binaries based on GPL code if you
   do not distribute ALL the source code for them). Same does not apply if the
   Library uses AXL.

Q: Can I use AXL with my own projects?
A: Yes, you can, but you have to:
   - Include copies of the files LGPL, GPL, PREAMBLE, LICENSE_AX and
     LICENSE_AXDOCS
   - Once in your own project, modify the top of the LICENSE_AX and
     LICENSE_AXDOCS and PREAMBLE to change the year, copyright holders and
     project name.
   - Add license notifications (section V.) in all source code files that
     are part of your library.

Q: Is AXL code compatible with MIT code for example?
A: Yes. As an extension of the LGPL, AXL is compatible with most known
   licenses. Please read more info on LGPL compatibility with other licenses.

Q: Where can I ask more questions?
A: Please visit http://axonlib.googlecode.com/

================================================================================
V. IN CODE COPYRIGHT NOTICE

  The following lines of text should be present in all headers that are part
  of your library:

    Copyright (c) <year> <authors>
    This file is part of <library name>.
    <website>

    <library name> is free software: you can redistribute it and/or modify
    it under the terms of the Axonlib License, either version 1.0
    of the License, or (at your option) any later version.

    <library name> is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See LICENSE_AX for more details.

    You should have received a copy of the Axonlib License
    If not, see <http://axonlib.googlecode.com/>.
    
================================================================================
\endcode

*/
