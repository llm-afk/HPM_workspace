@echo off
echo [1/3] Cleaning project...
call scons -c

echo [2/3] Building project (16 threads)...
call scons -j16

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Build failed! Please check the logs.
    pause
    exit /b %ERRORLEVEL%
)

echo [3/3] Flashing to board...
"D:\Program Files\SEGGER\JLink_V848\JLink.exe" -device HPM6E80XGNX -if JTAG -speed 4000 -jtagconf -1,-1 -autoconnect 1 -NoGui 1 -CommandFile flash.jlink

echo.
echo [DONE] Build and Flash sequence completed.
exit
