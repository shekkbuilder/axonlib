:: compileall.cmd
:: ----------------------
@echo off
echo.
echo axonlib 'compileall' script for windows
echo ---------------------------------------------------------------------------
echo syntax: compileall [folder (no \ at end)] [compile flags (see compile.cmd)]
for %%f in (%1\*.cpp) do call compile.cmd %%f %2 %3 %4 %5 %6 %7 %8 %9 %10
echo.
echo ** all done!
