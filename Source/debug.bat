@echo off
REM =====================
REM STM32 Debug Batch Script (No Build / Flash)
REM Handles spaces in paths safely
REM =====================

REM Get the folder where this batch file is located
SET "PROJECT_DIR=%~dp0"

REM OpenOCD & GDB paths
SET "OPENOCD_PATH=C:\Tool\xpack-openocd-0.12.0-7\bin\openocd.exe"
SET "STLINK_CFG=interface/stlink.cfg"
SET "TARGET_CFG=target/stm32f1x.cfg"
SET "GDB_PATH=C:\Tool\Arm-GNU-Toolchain\10_2021.10\bin\arm-none-eabi-gdb.exe"
SET "ELF_FILE=%PROJECT_DIR%build\main.elf"

REM Change to project directory
cd /d "%PROJECT_DIR%"

REM =====================
REM Start OpenOCD in background
REM =====================
echo Starting OpenOCD...
start "" "%OPENOCD_PATH%" -f "%STLINK_CFG%" -f "%TARGET_CFG%" -c "gdb_port 3333"

REM Wait a few seconds for OpenOCD to initialize
timeout /t 3 /nobreak > nul

REM =====================
REM Start GDB and connect to OpenOCD
REM =====================
echo Starting GDB...
echo %ELF_FILE%
start "" cmd /k "arm-none-eabi-gdb -x debug.gdb build/main.elf"

echo Debug session started. OpenOCD is running in the background.
echo Use Task Manager to close OpenOCD if needed.
pause
