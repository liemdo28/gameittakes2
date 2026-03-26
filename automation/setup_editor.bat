@echo off
:: ============================================================
:: ShardsOfDawn — UE5 Editor Setup & First-Run Automation
:: Run from project root after cloning/pulling
:: ============================================================
setlocal enabledelayedexpansion

set "PROJECT_ROOT=%~dp0.."
set "UPROJECT=%PROJECT_ROOT%\ShardsOfDawn.uproject"
set "UE_PATH=C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor.exe"
set "UE_FOUND=0"

echo ============================================================
echo  ShardsOfDawn — UE5 Editor Setup
echo ============================================================
echo.

:: Step 1: Check UE5 installation
echo [1/8] Checking Unreal Engine 5.4+...
if exist "%UE_PATH%" (
    set "UE_FOUND=1"
    echo     FOUND: %UE_PATH%
) else (
    echo     [WARN] UE5.4 default path not found.
    echo     Searching installed UE5 versions...
    for %%v in (5.7 5.6 5.5 5.4) do (
        if !UE_FOUND!==0 if exist "C:\Program Files\Epic Games\UE_%%v\Engine\Binaries\Win64\UnrealEditor.exe" (
            set "UE_PATH=C:\Program Files\Epic Games\UE_%%v\Engine\Binaries\Win64\UnrealEditor.exe"
            set "UE_FOUND=1"
        )
    )
    if !UE_FOUND!==0 (
        for /r "C:\Program Files\Epic Games" %%i in (UnrealEditor.exe) do (
            if !UE_FOUND!==0 (
                set "UE_PATH=%%i"
                set "UE_FOUND=1"
            )
        )
    )
    if !UE_FOUND!==0 (
        echo     [ERROR] UnrealEditor.exe not found under C:\Program Files\Epic Games.
        echo     Install Unreal Engine 5.4 or later, then rerun this script.
        exit /b 1
    )
    echo     FOUND: !UE_PATH!
)

:: Step 2: Generate project files
echo.
echo [2/8] Generating project files (.uproject)...
if exist "%UPROJECT%" (
    echo     OK: %UPROJECT%
    :: UE5 generates on double-click, but we can use UnrealBuildTool
    echo     [NOTE] Launch UE5 Editor to auto-generate project files.
) else (
    echo     [ERROR] ShardsOfDawn.uproject not found!
    exit /b 1
)

:: Step 3: Build Development Editor
echo.
echo [3/8] Build: ShardsOfDawnEditor (Development Editor)
echo.
set BUILD_CMD="%UE_PATH%" "%UPROJECT%" -Build -Target=ShardsOfDawnEditor -Configuration=Development -Platform=Win64 -WaitMutex -MutexName=UE4ShardsOfDawn
echo     Command: %BUILD_CMD%
echo     [INFO] If you want to build from command line, use:
echo     cd /d "%PROJECT_ROOT%"
echo     "%UE_PATH%" "%UPROJECT%" -build -Target=ShardsOfDawnEditor -Configuration=Development
echo.
echo     OR use Visual Studio:
echo     1. Open ShardsOfDawn.sln (after generation)
echo     2. Set ShardsOfDawnEditor as Startup Project
echo     3. Build ^> Configuration: Development Editor
echo.
set /p CONTINUE_BUILD="Open UE5 Editor now? (y/n): "
if /i "!CONTINUE_BUILD!"=="y" goto OPEN_EDITOR
goto SKIP_OPEN

:OPEN_EDITOR
echo.
echo [4/8] Launching UE5 Editor...
start "" "%UE_PATH%" "%UPROJECT%"
echo     Editor launching — wait for splash screen to load.

:SKIP_OPEN

:: Step 5: Check/create directory structure
echo.
echo [5/8] Validating Content directory structure...
set DIR_OK=1
for %%d in (
    "%PROJECT_ROOT%\Content\Game\Maps\PrototypeRoom"
    "%PROJECT_ROOT%\Content\Game\Blueprints\Player"
    "%PROJECT_ROOT%\Content\Game\Blueprints\Actors"
    "%PROJECT_ROOT%\Content\Game\GameModes"
    "%PROJECT_ROOT%\Content\Game\Input"
    "%PROJECT_ROOT%\Content\Game\Materials"
) do (
    if not exist "%%d" (
        echo     [CREATE] %%d
        mkdir "%%d" 2>nul
        set DIR_OK=0
    ) else (
        echo     [OK] %%d
    )
)
if !DIR_OK!==1 echo     All directories exist.

:: Step 6: Verify C++ header files
echo.
echo [6/8] Checking C++ source files...
set SRC_OK=1
for %%f in (
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Characters\SODPlayerCharacter.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Actors\PuzzleActors\SodPuzzleActorBase.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Actors\PuzzleActors\LightShardPuzzleActor.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Actors\PuzzleActors\ShadowShardPuzzleActor.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Actors\PuzzleActors\CoopBridgeActor.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Core\SODGameMode.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Controllers\SodPlayerController.h"
    "%PROJECT_ROOT%\Source\ShardsOfDawn\Public\Core\SODGameInstance.h"
) do (
    if not exist "%%f" (
        echo     [MISSING] %%f
        set SRC_OK=0
    ) else (
        echo     [OK] %%~nxf
    )
)
if !SRC_OK!==0 (
    echo.
    echo     [ERROR] Some C++ headers are missing!
    echo     Make sure you pulled from GitHub completely.
)

:: Step 7: Print Editor manual steps
echo.
echo ============================================================
echo  MANUAL STEPS IN UE5 EDITOR
echo ============================================================
echo.
echo STEP A: Generate Visual Studio project files
echo   - Right-click ShardsOfDawn.uproject ^> Generate Visual Studio project files
echo.
echo STEP B: Build C++ (Visual Studio)
echo   - Open ShardsOfDawn.sln
echo   - Build ^> Configuration: Development Editor
echo   - Wait for: 0 Error^(s^)
echo.
echo STEP C: Open PrototypeRoom map
echo   - Content Browser ^> Game ^> Maps ^> PrototypeRoom
echo   - Double-click PrototypeRoom.umap
echo.
echo STEP D: Create PlayerStarts
echo   - Place Actor ^> Game ^> PlayerStart
echo   - Tag 1st: PlayerStart.Light
echo   - Tag 2nd: PlayerStart.Shadow
echo.
echo STEP E: Place Shard actors
echo   - Place BP_LightShard x3 in Hall A
echo   - Place BP_ShadowShard x3 in Hall B
echo   - Place BP_CoopBridge x1 between Hall B and Exit
echo.
echo STEP F: Assign IMC to PlayerControllers
echo   - Edit ^> Project Settings ^> Input
echo   - Create IMC_ShardsOfDawn_LightWeaver
echo   - Create IMC_ShardsOfDawn_ShadowWalker
echo.
echo STEP G: PIE Smoke Test
echo   - Play ^> Selected Viewport ^(2 players^)
echo   - Run through smoke test checklist
echo.

:: Step 8: Summary
echo.
echo ============================================================
echo  SETUP CHECKLIST SUMMARY
echo ============================================================
echo.
echo [ ] Generate VS project files
echo [ ] Build: Development Editor ^> 0 errors
echo [ ] Open PrototypeRoom.umap
echo [ ] Create 2x PlayerStarts (Light / Shadow)
echo [ ] Place 6x Shard actors + 1x Bridge
echo [ ] Assign IMC
echo [ ] PIE smoke test
echo.
echo Run this script again after completing steps to verify.
echo.
pause
