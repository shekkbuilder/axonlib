:: compile.cmd
@echo off

:: -----------------------------------------------------------------------------
:: *** user settings here
:: -----------------------------------------------------------------------------
:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to compile.cmd)
set vstpath=..\..\vstsdk24

:: set path to axonlib headers
:: e.g: c:\axonlib\ax (or relative to the compile.cmd)
set axpath=..\ax

:: set path to mingw-bin, e.g: c:\mingw\bin\ (with dash at end)
set mgwpath=

:: set warning flags
set warn=-pedantic -fpermissive -W -Wall -Wextra -Wno-unused -Wno-long-long

:: set resource file (leave blank for no resources)
set resfile=rc_default.rc

:: set optimization flags
set opt=-msse -mfpmath=sse,387 -O3 -Os

:: linker options
set linker=-fstack-check -fdata-sections -ffunction-sections -Wl,-gc-sections -s

:: -----------------------------------------------------------------------------
:: *** end of user settings
:: -----------------------------------------------------------------------------

:: default values
set dstatus=OFF
set gccdstatus=OFF
set nmv=
set dbg=
set res=
set gccdbg=
set cmdline=

:: check for file input
if [%1]==[] goto syntax
if [%1]==[-h] goto syntax

:: check for cpp file
set infile=%1
set srcext=.cpp
if not "%infile:~-4%"=="%srcext%" goto nocpp

:: check includes
if not exist %axpath% goto noax
if not exist %vstpath% goto novstsdk

:: check for 'not move'
if [%2]==[-nmv] set nmv=yes
if [%3]==[-nmv] set nmv=yes
if [%4]==[-nmv] set nmv=yes
if [%5]==[-nmv] set nmv=yes

:: check for lib debug mode
if [%2]==[-d] goto setlibdebug
if [%3]==[-d] goto setlibdebug
if [%4]==[-d] goto setlibdebug
if [%5]==[-d] goto setlibdebug

:getgccdebug
:: check for gcc debug mode
if [%2]==[-g] goto setgccdebug
if [%3]==[-g] goto setgccdebug
if [%4]==[-g] goto setgccdebug
if [%5]==[-g] goto setgccdebug

:: get the tgt format
:getformat
if [%2]==[-dll] goto dlltarget
if [%3]==[-dll] goto dlltarget
if [%4]==[-dll] goto dlltarget
if [%5]==[-dll] goto dlltarget
if [%2]==[-exe] goto exetarget
if [%3]==[-exe] goto exetarget
if [%4]==[-exe] goto exetarget
if [%5]==[-exe] goto exetarget
goto :dlltarget

:: set lib debug
:setlibdebug
set dbg=-DAX_DEBUG
set dstatus=ON
goto getgccdebug

:: set gcc debug
:setgccdebug
set gccdbg=-DDEBUG -gstabs
set gccdstatus=ON
set resfile=
set linker=
set opt=-O3
goto getformat

:: format is dll
:dlltarget
echo ---------------------------------------------------------------------------
set ext=.dll
set tgtformat=-DAX_FORMAT_VST -shared
goto begin

:: format is exe
:exetarget
echo ---------------------------------------------------------------------------
if [%resfile%]==[] goto exenores
echo preparing resources...
if exist %resfile%.o del %resfile%.o
%mgwpath%windres -i %resfile% -o %resfile%.o
if not exist %resfile%.o goto nores
set res=%resfile%.o
:exenores
set ext=.exe
set tgtformat=-DAX_FORMAT_EXE
goto begin

:: echo syntax
:syntax
echo.
echo ---------------------------------------------------------------------------
echo * axonlib compile script for windows
echo.
echo usage: compile.cmd [file.cpp] [-h] [-exe or -dll] [-nmv] [-d]
echo  -exe : create an executable
echo  -dll : create a dll (default)
echo  -nmv : do not move result to ..\bin
echo  -d : enable library debug mode
echo  -g : enable gcc debug mode (-gstabs)
echo  -h : show this help message
echo ---------------------------------------------------------------------------
goto end

:: --------------------------
:: begin
:: --------------------------
:begin
:: set target
call set target=%%infile:%srcext%=%%
set target=%target%%ext%
set cmdpath=%~p0

:: delete old target
if exist %target% del %target%

:: echo settings
echo.
echo * compiling windows binary for '%infile%'...
echo * format is: %ext%
echo * lib debug is: %dstatus%
echo * gcc debug is: %gccdstatus%

:compile
echo.
set cmdline=%mgwpath%g++ -I%cmdpath%%axpath% -I%cmdpath%%vstpath% -mwindows %tgtformat% %opt% %warn% %gccdbg% %dbg% %linker% .\%infile% %res% -o .\%target%
:: show cmdline
echo command line is: %cmdline% && echo.
:: call g++
%cmdline%

:: target missing -> error
if not exist %target% echo. && echo # ERR: not compiled! && goto done
if not [%gccdstatus%]==[] goto printsize

:: call strip (no need if '-s' is passed to g++)
rem echo striping...
rem if exist %target% %mgwpath%strip --strip-all %target%

:printsize
:: target file size
for %%I in (%target%) do echo * filesize: %%~zI bytes

:: check if '-nmv'
if not [%nmv%]==[] goto done
if not exist %~p0..\bin md %~p0..\bin
if exist %target% echo. && echo moving '%target%' to '%~p0..\bin'
if exist %target%	move %target% %~p0..\bin
:: --------------------------
:: done
:done
echo. && echo * done
echo ---------------------------------------------------------------------------
goto end

:: errors
:: ----------------------------------------------------------------------------
:nocpp
echo.
echo ### ERR: first argument has to be a .cpp file
goto end
:: ----------------------------------------------------------------------------
:noax
echo.
echo ### ERR: cannot find axonlib headers in '%axpath%'
goto end
:: ----------------------------------------------------------------------------
:novstsdk
echo.
echo ### ERR: cannot find vst sdk in '%vstpath%'
goto end
:: ----------------------------------------------------------------------------
:nores
echo.
echo ### ERR: cannot create resource file '%resfile%.o'
goto end
:: ----------------------------------------------------------------------------
:end
