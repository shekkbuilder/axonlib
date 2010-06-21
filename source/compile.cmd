:: compile.cmd
@echo off

:: -----------------------------------------------------------------------------
:: *** user settings here
:: -----------------------------------------------------------------------------
:: set path to axonlib headers
:: e.g: c:\axonlib\ax (or relative to the compile.cmd)
set axpath=..\ax

:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to compile.cmd)
set vstpath=..\..\vstsdk24

:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to compile.cmd)
set ladspapath=..\..\ladspa_sdk\src

:: set g++ path, e.g: c:\mingw\bin\g++
set mgwpath=g++

:: set windres path, e.g: c:\mingw\bin\windres
set windrespath=windres

:: set warning flags
set warn=-pedantic -fpermissive -W -Wall -Wextra -Wno-unused -Wno-long-long

:: set resource file (leave blank for no resources)
set resfile="rc_default.rc"

:: set optimization flags
set opt=-mfpmath=387 -O3

:: target & libraries
set tgtlib=-mwindows -lmsimg32

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
set v=
set suffix=
set ext_params=
set ext_p2=
set ext_p3=
set ext_p4=
set ext_p5=
set ext_p6=
set ext_p7=
set ext_p8=

:: check for file input
if [%1]==[] goto syntax
if [%1]==[-h] goto syntax

:: check for cpp file
set infile=%1
set srcext=.cpp

:: remove quotes
set infile=###%infile%###
set infile=%infile:"###=%
set infile=%infile:###"=%
set infile=%infile:###=%

:: check extesion
if not "%infile:~-4%"=="%srcext%" goto nocpp

:: check includes
if not exist %axpath% goto noax

:: check for additional params
if not [%2]==[] (if not [%2]==[-v] (if not [%2]==[-d] (if not [%2]==[-g] (if not [%2]==[-ladspa] (if not [%2]==[-vst] (if not [%2]==[-exe] (if not [%2]==[-ts] set ext_p2= %2)))))))
if not [%3]==[] (if not [%3]==[-v] (if not [%3]==[-d] (if not [%3]==[-g] (if not [%3]==[-ladspa] (if not [%3]==[-vst] (if not [%3]==[-exe] (if not [%3]==[-ts] set ext_p3= %3)))))))
if not [%4]==[] (if not [%4]==[-v] (if not [%4]==[-d] (if not [%4]==[-g] (if not [%4]==[-ladspa] (if not [%4]==[-vst] (if not [%4]==[-exe] (if not [%4]==[-ts] set ext_p4= %4)))))))
if not [%5]==[] (if not [%5]==[-v] (if not [%5]==[-d] (if not [%5]==[-g] (if not [%5]==[-ladspa] (if not [%5]==[-vst] (if not [%5]==[-exe] (if not [%5]==[-ts] set ext_p5= %5)))))))
if not [%6]==[] (if not [%6]==[-v] (if not [%6]==[-d] (if not [%6]==[-g] (if not [%6]==[-ladspa] (if not [%6]==[-vst] (if not [%6]==[-exe] (if not [%6]==[-ts] set ext_p6= %6)))))))
if not [%7]==[] (if not [%7]==[-v] (if not [%7]==[-d] (if not [%7]==[-g] (if not [%7]==[-ladspa] (if not [%7]==[-vst] (if not [%7]==[-exe] (if not [%7]==[-ts] set ext_p7= %7)))))))
if not [%8]==[] (if not [%8]==[-v] (if not [%8]==[-d] (if not [%8]==[-g] (if not [%8]==[-ladspa] (if not [%8]==[-vst] (if not [%8]==[-exe] (if not [%8]==[-ts] set ext_p8= %8)))))))
set ext_params=%ext_p2%%ext_p3%%ext_p4%%ext_p5%%ext_p6%%ext_p7%%ext_p8%

:: check for 'not move'
if [%2]==[-nmv] set nmv=yes
if [%3]==[-nmv] set nmv=yes
if [%4]==[-nmv] set nmv=yes
if [%5]==[-nmv] set nmv=yes
if [%6]==[-nmv] set nmv=yes
if [%7]==[-nmv] set nmv=yes

:: check for verbose
if [%2]==[-v] set v=yes
if [%3]==[-v] set v=yes
if [%4]==[-v] set v=yes
if [%5]==[-v] set v=yes
if [%6]==[-v] set v=yes
if [%7]==[-v] set v=yes

:: check for gcc debug mode
if [%2]==[-ts] set usets=yes
if [%3]==[-ts] set usets=yes
if [%4]==[-ts] set usets=yes
if [%5]==[-ts] set usets=yes
if [%6]==[-ts] set usets=yes
if [%7]==[-ts] set usets=yes

:: check for lib debug mode
if [%2]==[-d] goto setlibdebug
if [%3]==[-d] goto setlibdebug
if [%4]==[-d] goto setlibdebug
if [%5]==[-d] goto setlibdebug
if [%6]==[-d] goto setlibdebug
if [%7]==[-d] goto setlibdebug

:getgccdebug
:: check for gcc debug mode
if [%2]==[-g] goto setgccdebug
if [%3]==[-g] goto setgccdebug
if [%4]==[-g] goto setgccdebug
if [%5]==[-g] goto setgccdebug
if [%6]==[-g] goto setgccdebug
if [%7]==[-g] goto setgccdebug

:: get the tgt format
:getformat
if [%2]==[-ladspa] goto ladspatarget
if [%3]==[-ladspa] goto ladspatarget
if [%4]==[-ladspa] goto ladspatarget
if [%5]==[-ladspa] goto ladspatarget
if [%6]==[-ladspa] goto ladspatarget
if [%7]==[-ladspa] goto ladspatarget

if [%2]==[-vst] goto vsttarget
if [%3]==[-vst] goto vsttarget
if [%4]==[-vst] goto vsttarget
if [%5]==[-vst] goto vsttarget
if [%6]==[-vst] goto vsttarget
if [%7]==[-vst] goto vsttarget

if [%2]==[-exe] goto exetarget
if [%3]==[-exe] goto exetarget
if [%4]==[-exe] goto exetarget
if [%5]==[-exe] goto exetarget
if [%6]==[-exe] goto exetarget
if [%7]==[-exe] goto exetarget
goto :exetarget

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

:: format is vst
:ladspatarget
if not exist %ladspapath% goto noladspa
if not [%v%]==[] echo ---------------------------------------------------------------------------
set suffix=-ladspa
set ext=.dll
set tgtformat=-DAX_FORMAT_LADSPA -shared
set libfmt= LADSPA
goto begin

:: format is vst
:vsttarget
if not exist %vstpath% goto novstsdk
if not [%v%]==[] echo ---------------------------------------------------------------------------
set suffix=-vst
set ext=.dll
set tgtformat=-DAX_FORMAT_VST -shared
set libfmt= VST
goto begin

:: format is exe
:exetarget
if not [%v%]==[] echo ---------------------------------------------------------------------------
if [%resfile%]==[] goto exenores
echo preparing resources...
if exist %resfile%.o del %resfile%.o
%windrespath% -i %resfile% -o %resfile%.o
if not exist %resfile%.o goto nores
set res=%resfile%.o
:exenores
set libfmt=
set ext=.exe
set tgtformat=-DAX_FORMAT_EXE
goto begin

:: echo syntax
:syntax
echo.
echo ---------------------------------------------------------------------------
echo * axonlib compile script for windows
echo.
echo usage: compile.cmd [file.cpp] [-h] [format] [-nmv] [-d] [-g] [...]
echo  -exe : create an executable (default)
echo  -vst : create a vst dll
echo  -ladspa : create a ladspa dll
echo  -nmv : do not move result to ..\bin
echo  -ts : add time stamp to name e.g. plugin-vst-20153569.dll
echo  -d : enable library debug mode
echo  -g : enable gcc debug mode (-gstabs)
echo  -v : verbose
echo  -h : show this help message
echo  [...] : pass extra parameters e.g. -DSOME_DEF_1 -DSOME_DEF_2
echo ---------------------------------------------------------------------------
goto end

:: --------------------------
:: begin
:: --------------------------
:begin

:: get time stamp
if [%usets%]==[] goto nots
set td=%date%%time%
set td=%td:,=%
set td=%td:.=%
set td=%td: =%
set td=%td::=%
set td0=%td:~0,8%
set td1=%td:~9%
set td1=-%td1%
goto settarget
:nots
if not [%usets%]==yes set td1=

:: set target
:settarget
call set target=%%infile:%srcext%=%%
set target=%target%%suffix%%td1%%ext%
set cmdpath=%~p0

:: delete old target
if exist %target% del %target%

:: echo settings
echo.
echo * compiling windows binary for '%infile%'...
if not [%v%]==[] echo * binary format is:%libfmt% %ext%
if not [%v%]==[] echo * lib debug is: %dstatus%
if not [%v%]==[] echo * gcc debug is: %gccdstatus%
if not [%v%]==[] if exist %axpath% echo * found axonlib path '%axpath%'
if not [%v%]==[] if "%libfmt%" == " VST" echo * found vst sdk path '%vstpath%'
if not [%v%]==[] if "%libfmt%" == " LADSPA" echo * found ladpsa sdk path '%ladspapath%'

:compile
if not [%v%]==[] echo.
set cmdline=%mgwpath% -I%axpath% -I%vstpath% -I%ladspapath% %tgtformat% %opt% %warn% %gccdbg% %dbg% "%infile%" %res% -o "%target%" %ext_params% %tgtlib% %linker%
:: show cmdline
if not [%v%]==[] echo command line is: %cmdline% && echo.
:: call g++
%cmdline%

:: target missing -> error
if not exist "%target%" echo. && echo # ERR: not compiled! && goto done
if not [%gccdstatus%]==[] goto printsize

:: call strip (no need if '-s' is passed to g++)
rem echo striping...
rem if exist %target% %mgwpath%strip --strip-all %target%

:printsize
:: target file size
if not [%v%]==[] for %%I in ("%target%") do echo * filesize: %%~zI bytes

:: check if '-nmv'
if not [%nmv%]==[] goto done
if not exist "%~p0..\bin" md "%~p0..\bin"
if exist "%target%" if not [%v%]==[] echo. && echo moving '%target%' to '%~p0..\bin'
if exist "%target%"	move "%target%" "%~p0..\bin"
:: --------------------------
:: done
:done
if not [%v%]==[] echo. && echo * done
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
echo ### ERR: cannot find axonlib path '%axpath%'
goto end
:: ----------------------------------------------------------------------------
:novstsdk
echo.
echo ### ERR: cannot find vst sdk path '%vstpath%'
goto end
:: ----------------------------------------------------------------------------
:noladspa
echo.
echo ### ERR: cannot find ladspa sdk path '%ladspapath%'
goto end
:: ----------------------------------------------------------------------------
:nores
echo.
echo ### ERR: cannot create resource file '%resfile%.o'
goto end
:: ----------------------------------------------------------------------------
:end
if not [%v%]==[] echo ---------------------------------------------------------------------------
