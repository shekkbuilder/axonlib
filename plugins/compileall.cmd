:: compileall.cmd
:: ----------------------
@echo off
echo.
echo ---------------------------------------------------------------------------
echo @ axonlib 'compileall' for windows
echo ---------------------------------------------------------------------------
for %%a in (*.cpp) do call compile.cmd %%a
echo.
echo ** all done!
