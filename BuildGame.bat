@echo off
REM ============================================================================
REM  Shards of Dawn - Build Script for Testing
REM  Usage: BuildGame.bat [build|cook|package|clean]
REM  Default: package (full build + cook + package)
REM ============================================================================

setlocal EnableDelayedExpansion

REM ── Auto-detect UE5 installation ──────────────────────────────────────────
set "UE_ROOT="

REM Check common installation paths
if exist "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.4"
)
if exist "C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.5"
)
if exist "D:\Epic Games\UE_5.4\Engine\Build\BatchFiles\RunUAT.bat" (
    set "UE_ROOT=D:\Epic Games\UE_5.4"
)
if exist "D:\Epic Games\UE_5.5\Engine\Build\BatchFiles\RunUAT.bat" (
    set "UE_ROOT=D:\Epic Games\UE_5.5"
)

REM Allow override via environment variable
if defined UE5_ROOT (
    set "UE_ROOT=%UE5_ROOT%"
)

if "%UE_ROOT%"=="" (
    echo [ERROR] Could not find Unreal Engine 5.4+
    echo Please set UE5_ROOT environment variable to your UE5 installation path
    echo Example: set UE5_ROOT=C:\Program Files\Epic Games\UE_5.4
    exit /b 1
)

echo [INFO] Using Unreal Engine at: %UE_ROOT%

set "UAT=%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat"
set "UBT=%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
set "EDITOR_CMD=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
set "PROJECT_DIR=%~dp0"
set "UPROJECT=%PROJECT_DIR%ShardsOfDawn.uproject"
set "BUILD_DIR=%PROJECT_DIR%Build\Packaged"
set "PLATFORM=Win64"
set "CONFIG=Development"

REM ── Parse command ─────────────────────────────────────────────────────────
set "COMMAND=%~1"
if "%COMMAND%"=="" set "COMMAND=package"

echo.
echo ============================================================================
echo  SHARDS OF DAWN - Build System
echo  Command: %COMMAND%
echo  Platform: %PLATFORM%
echo  Config: %CONFIG%
echo  Project: %UPROJECT%
echo ============================================================================
echo.

if /i "%COMMAND%"=="build" goto :BUILD
if /i "%COMMAND%"=="cook" goto :COOK
if /i "%COMMAND%"=="package" goto :PACKAGE
if /i "%COMMAND%"=="clean" goto :CLEAN
if /i "%COMMAND%"=="editor" goto :EDITOR

echo [ERROR] Unknown command: %COMMAND%
echo Usage: BuildGame.bat [build^|cook^|package^|clean^|editor]
exit /b 1

REM ── BUILD: Compile C++ only ───────────────────────────────────────────────
:BUILD
echo [STEP 1/1] Compiling C++ source...
"%UBT%" ShardsOfDawn %PLATFORM% %CONFIG% -Project="%UPROJECT%" -WaitMutex -FromMsBuild
if %ERRORLEVEL% NEQ 0 (
    echo [FAILED] C++ compilation failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] C++ compilation completed!
goto :END

REM ── COOK: Cook content only ───────────────────────────────────────────────
:COOK
echo [STEP 1/1] Cooking content for %PLATFORM%...
"%EDITOR_CMD%" "%UPROJECT%" -run=cook -targetplatform=%PLATFORM% -iterate -unversioned
if %ERRORLEVEL% NEQ 0 (
    echo [FAILED] Content cooking failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Content cooking completed!
goto :END

REM ── PACKAGE: Full build + cook + package ──────────────────────────────────
:PACKAGE
echo [STEP 1/1] Running full package pipeline...
echo   This will: Compile C++ -^> Cook Content -^> Package Game
echo   Output: %BUILD_DIR%
echo.

call "%UAT%" BuildCookRun ^
    -project="%UPROJECT%" ^
    -noP4 ^
    -platform=%PLATFORM% ^
    -clientconfig=%CONFIG% ^
    -cook ^
    -build ^
    -stage ^
    -pak ^
    -archive ^
    -archivedirectory="%BUILD_DIR%" ^
    -prereqs ^
    -nodebuginfo ^
    -utf8output ^
    -compile

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [FAILED] Package build failed! Check errors above.
    echo Common fixes:
    echo   1. Make sure Visual Studio 2022 is installed with C++ game dev workload
    echo   2. Run setup_editor.bat first
    echo   3. Check that .uproject opens correctly in UE5 Editor
    exit /b %ERRORLEVEL%
)

echo.
echo ============================================================================
echo  [SUCCESS] Game packaged successfully!
echo  Output: %BUILD_DIR%\WindowsNoEditor
echo.
echo  To run the game:
echo    %BUILD_DIR%\WindowsNoEditor\ShardsOfDawn.exe
echo ============================================================================
goto :END

REM ── CLEAN: Remove build artifacts ─────────────────────────────────────────
:CLEAN
echo [CLEANING] Removing build artifacts...
if exist "%PROJECT_DIR%Binaries" rmdir /s /q "%PROJECT_DIR%Binaries"
if exist "%PROJECT_DIR%Intermediate" rmdir /s /q "%PROJECT_DIR%Intermediate"
if exist "%PROJECT_DIR%Saved" rmdir /s /q "%PROJECT_DIR%Saved"
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
echo [SUCCESS] Build artifacts cleaned!
goto :END

REM ── EDITOR: Open in UE5 Editor ────────────────────────────────────────────
:EDITOR
echo [OPENING] Launching UE5 Editor...
start "" "%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe" "%UPROJECT%"
echo [INFO] Editor is starting...
goto :END

:END
echo.
echo Done.
endlocal
