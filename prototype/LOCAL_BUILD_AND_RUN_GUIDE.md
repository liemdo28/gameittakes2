# LOCAL BUILD & RUN GUIDE (UE5 — Unified Architecture)

## Architecture Summary (v2 — After Cleanup)

```
Canonical Classes (SOD prefix):
  Source/ShardsOfDawn/
  ├── Public/
  │   ├── Characters/SODPlayerCharacter.h       ← PlayerRole replicated, GAS, IMC
  │   ├── Actors/PuzzleActors/SodPuzzleActorBase.h ← IInterface_Interactive
  │   ├── Actors/PuzzleActors/LightShardPuzzleActor.h
  │   ├── Actors/PuzzleActors/ShadowShardPuzzleActor.h
  │   ├── Actors/PuzzleActors/CoopBridgeActor.h     ← Polls ASODGameMode
  │   ├── Core/SODGameMode.h                ← SetShardActivationState, AssignPlayerRole
  │   ├── Core/SODGameInstance.h           ← IMC helpers, session, telemetry
  │   ├── Controllers/SodPlayerController.h ← IMC switching by role
  │   ├── Interfaces/Interface_Interactive.h ← ResetPuzzleState added
  │   └── Abilities/SODAbility_*.h
  └── Private/ [corresponding .cpp]

DELETED (duplicate / zombie files):
  Source/ShardsOfDawn/Public/GameModes/SodGameMode.h   ← merged into Core/SODGameMode
  Source/ShardsOfDawn/Private/GameModes/SodGameMode.cpp
  Source/ShardsOfDawn/Public/GameInstance/SodGameInstance.h ← merged into Core/SODGameInstance
  Source/ShardsOfDawn/Private/GameInstance/SodGameInstance.cpp
```

## 1) Prerequisites
- Unreal Engine 5.4+ (5.7 recommended)
- Visual Studio 2022 with C++ toolchain
- Windows SDK installed
- Git

## 2) Open Project
```bash
cd E:\Project\Personal\It-Takes-Two-Inspired-Game
git pull origin main
```
Then double-click `ShardsOfDawn.uproject` — UE5 auto-generates project files.

## 3) Build
```
Visual Studio: Build > Build Solution (Ctrl+Shift+B)
Configuration: Development Editor
Expected: 0 Error(s)
```

Or via command line:
```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"
    ShardsOfDawn.uproject Development Win64 -WaitMutex -MutexName=UE4ShardsOfDawn
```

## 4) Verify Build Success
- [x] Project opens without module errors
- [x] Default map path resolves
- [x] Play-in-editor launches successfully

## 5) Next Immediate Steps

### Already Implemented (C++ — Production Ready)
- [x] ✅ `ASODPlayerCharacter` — role-based (Light/Shadow), GAS-enabled, replicated `PlayerRole`, `SetPlayerRole()`
- [x] ✅ `ASodPuzzleActorBase` — implements `IInterface_Interactive`, `ResetPuzzleState()`, shard registry integration
- [x] ✅ `ALightShardPuzzleActor` — Light only, float animation, point light
- [x] ✅ `AShadowShardPuzzleActor` — Shadow only, pulse animation, void light
- [x] ✅ `ACoopBridgeActor` — polls `ASODGameMode::GetActivatedShardCount()`, implements `IInterface_Interactive`
- [x] ✅ `ASODGameMode` (Core/) — `SetShardActivationState()`, `ActivatedShardIDs` replicated, `AssignPlayerRole()`
- [x] ✅ `ASodPlayerController` — IMC switching via `RefreshInputMapping()` by `ESODPlayerRole`
- [x] ✅ `USODGameInstance` (Core/) — merged IMC helpers + session + telemetry
- [x] ✅ `IInterface_Interactive` — `ResetPuzzleState()` added for GameMode reset
- [x] ✅ `SODAbility_LightPulse`, `SODAbility_ShadowAnchor` — GAS abilities

### Still Needs UE5 Editor (Blueprint / Asset work)
- [ ] Import character skeletal meshes (Linh + Nam)
- [ ] Create `BP_LightWeaver` extending `SODPlayerCharacter` (PlayerRole=Light default)
- [ ] Create `BP_ShadowWalker` extending `SODPlayerCharacter` (PlayerRole=Shadow default)
- [ ] Create `BP_SODGameMode` extending `SODGameMode` (Core/)
- [ ] Place 6x shard actors in PrototypeRoom (3 Light, 3 Shadow) — set `ShardID` on each
- [ ] Place `BP_CoopBridge` — set `MonitoredShardIDs` = [all 6 shard IDs]
- [ ] Create IMC assets and assign to `SodPlayerController`
- [ ] PIE smoke validation 10–15 min

## 6) Smoke Validation Checklist

See: `qa/SMOKE_TEST_CHECKLIST.md`

**Quick smoke (5 min):**
```
[P1 = Linh/Light] Walk to LightShard → press E → glow activates
[P2 = Nam/Shadow] Walk to ShadowShard → press E → glow activates
[Both] Activate all 6 shards → bridge lowers → cross bridge
```

**Full smoke (15 min):** Run all sections in `qa/SMOKE_TEST_CHECKLIST.md`
