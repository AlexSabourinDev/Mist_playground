@echo off

if not exist %~dp0/../Build/%2 mkdir %~dp0/../Build/%2

cd %~dp0/../Build/%2

call CMake -G %1 %~dp0/../Source
if errorlevel 1 (
	goto Error
)
if not errorLevel 1 (
	goto Success
)


:Success
echo "Generation Complete!"
timeout /t 3
goto End

:Error
echo "Failure!"
pause
goto End

:End
echo "Exiting..."