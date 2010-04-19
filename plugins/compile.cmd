:: compile.cmd
@echo off

:: -----------------------------------------------------------------------------
:: *** user settings here
:: -----------------------------------------------------------------------------
:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to the compile.cmd)
set vstpath=..\..\vstsdk24

:: set path to axonlib headers
:: e.g: c:\axonlib\ax (or relative to the compile.cmd)
set axpath=..\ax

:: set path to mingw-bin, e.g: c:\mingw\bin\ (with dash at end)
set mgwpath=

:: set warning flags
set warn=-pedantic -fpermissive -W -Wall -Wno-unused

:: set optimization flags
set opt=-msse -mfpmath=sse,387 -O3 -Os -fstack-check -fdata-sections -ffunction-sections

:: -----------------------------------------------------------------------------
:: *** end of user settings
:: -----------------------------------------------------------------------------

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

:: default values
set dstatus=OFF
set nvm=
set dbg=

:: check for not move 
if [%2]==[-nvm] set nvm=yes
if [%3]==[-nvm] set nvm=yes
if [%4]==[-nvm] set nvm=yes

:: check for debug
if [%2]==[-d] goto setdebug
if [%3]==[-d] goto setdebug
if [%4]==[-d] goto setdebug

:: get the tgt format
:getformat

if [%2]==[-dll] goto dlltarget
if [%3]==[-dll] goto dlltarget
if [%4]==[-dll] goto dlltarget
if [%2]==[-exe] goto exetarget
if [%3]==[-exe] goto exetarget
if [%4]==[-exe] goto exetarget

:: format is dll
:dlltarget
set ext=.dll
set tgtformat=AX_FORMAT_VST -shared
goto begin

:: format is exe
:exetarget
set ext=.exe
set tgtformat=AX_FORMAT_EXE
goto begin

:: set debug
:setdebug
set dbg=-DAX_DEBUG
set dstatus=ON
goto getformat

:: echo syntax 
:syntax
echo.
echo ---------------------------------------------------------------------------
echo * axonlib compile script for windows
echo.
echo usage: compile.cmd [file.cpp] [-h] [-exe or -dll] [-nvm] [-d]
echo -exe : create an executable
echo -dll : create a dll (default)
echo -nvm : do not move result to ..\bin
echo -d : enable debug mode
echo -h : show this help message
echo ---------------------------------------------------------------------------
goto end

:: --------------------------
:: begin
:: --------------------------
:begin
echo ---------------------------------------------------------------------------
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
echo * debug is: %dstatus%
echo.

:: call g++ / strip
%mgwpath%g++ -I%cmdpath%%axpath% -I%cmdpath%%vstpath% -mwindows -D%tgtformat% %warn% %opt% %dbg% -s -Wl,-gc-sections .\%infile% -o .\%target%
if exist %target% %mgwpath%strip --strip-all %target%

:: target missing -> error
if not exist %target% echo # ERR: not compiled!

:: check if '-nvm'
if not [%nvm%]==[] goto done
echo.
if not exist %~p0..\BIN md %~p0..\bin
if exist %target% echo moving '%target%' to '%~p0..\bin'
if exist %target%	move %target% %~p0..\bin
:: --------------------------
:: done
:done
echo.
echo * done
echo ---------------------------------------------------------------------------
goto end

:: errors
:: ----------------------------------------------------------------------------
:nocpp
echo.
echo ### ERR: first argument has to be a c++ file
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
:end
