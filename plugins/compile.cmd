:: compile.cmd
:: ----------------------
@echo off
:: check for file input
if .%1==. goto nofile
	:: check for 'cpp' extention
	set tmpfile=%1
	if not "%tmpfile:~-4%"==".cpp" goto nocpp
		:: --------------------------
		:: set path to mingw/bin
		set mgwpath=
		:: --------------------------	
		:: set target
		set infile=%1
		set rstring=.cpp
		call set target=%%infile:%rstring%=%%
		set target=%target%.dll
		:: --------------------------
		if exist %target% del %target%;		
		:: --------------------------
		echo.
		echo * compiling windows binary for '%1'...
		echo.
		%mgwpath%g++.exe -I../ax -I../../vstsdk24 -shared -mwindows -O3 -Os -fdata-sections -ffunction-sections -pedantic -fpermissive -Wl,-gc-sections %1 -s -o %target%
		if exist %target% %mgwpath%strip --strip-all %target%
		:: optional upx
		rem upx --best %target%
		:: moving file
		echo.
		if not exist ..\BIN md ..\bin		
		if not exist %target% echo # not compiled!		
		if exist %target% echo moving %target% to ..\bin
		if exist %target%	move %target% ..\bin
		:: --------------------------
		:: done
		echo.
		echo * done
		echo ---------------------------------------------------------------------------
		goto end
:: ----------------------------------------------------------------------------
:nocpp
echo.
echo ---------------------------------------------------------------------------
echo @ axonlib compile script for windows
echo @ usage: compile.cmd [file.cpp]
echo ---------------------------------------------------------------------------
echo ### input has to be a c++ file
goto end
:: ----------------------------------------------------------------------------
:nofile
echo.
echo ---------------------------------------------------------------------------
echo @ axonlib compile script for windows
echo @ usage: compile.cmd [file.cpp]
echo ---------------------------------------------------------------------------
echo ### no input file entered
goto end
:: ----------------------------------------------------------------------------
:end
