:: compile.cmd
:: ----------------------
@echo off
echo ---------------------------------------------------------------------------
echo @ axonlib compile script for windows
echo @ usage: compile.cmd [file.cpp]
echo ---------------------------------------------------------------------------
:: check for file input
if .%1==. goto nofile
	:: check for 'cpp' extention
	set tmpfile=%1
	if not "%tmpfile:~-4%"==".cpp" goto nocpp
		:: --------------------------
		:: set path mingw-bin, e.g: c:\mingw\bin\ (with dash at end)
		set mgwpath=
		:: --------------------------
		:: set target
		set infile=%1
		set rstring=.cpp
		call set target=%%infile:%rstring%=%%
		set target=%target%.dll
		set cmdpath=%~p0
		rem call set tgtpath=%tgtpath:\=/%
		:: --------------------------
		if exist %target% del %target%;
		:: --------------------------
		echo.
		echo * compiling windows binary for '%1'...
		echo.		
		%mgwpath%g++.exe -I%cmdpath%..\ax -I%cmdpath%..\..\vstsdk24 -shared -mwindows -O3 -Os -fstack-check -fdata-sections -ffunction-sections -pedantic -fpermissive -s -Wl,-gc-sections .\%1 -o .\%target% %2 %3 %4
		if exist %target% %mgwpath%strip --strip-all %target%
		:: optional upx
		rem upx --best %target%
		:: moving file
		echo.
		if not exist %~p0..\BIN md %~p0..\bin
		if not exist %target% echo # not compiled!		
		if exist %target% echo moving %target% to %~p0..\bin
		if exist %target%	move %target% %~p0..\bin
		:: --------------------------
		:: done
		echo.
		echo * done
		echo ---------------------------------------------------------------------------
		goto end
:: ----------------------------------------------------------------------------
:nocpp
echo.
echo ### input has to be a c++ file
goto end
:: ----------------------------------------------------------------------------
:nofile
echo.
echo ### no input file entered
goto end
:: ----------------------------------------------------------------------------
:end
