@echo off
echo Compiling Producer-Consumer program (C version)...
gcc -Wall -Wextra -O2 -pthread -o producer_consumer.exe producer_consumer.c

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo.
    echo Running the program...
    echo.
    producer_consumer.exe
) else (
    echo Compilation failed!
    pause
)
