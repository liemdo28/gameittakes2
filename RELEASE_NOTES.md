# v0.1.0-prototype — Tester-Ready Prototype

**Release Date:** 2026-03-26
**Tag:** v0.1.0-prototype
**Status:** Ready for QA testing

## What's Included

### Core Systems (55+ C++ source files)
- **Characters:** Linh (Light/The Spark) & Nam (Shadow/The Shade) with full GAS abilities
- **Puzzles:** Dual-state Light/Shadow system — 3 puzzle types + door mechanic
- **Combat:** Dual-shield enemy with co-op finisher mechanic
- **Camera:** Dynamic split-screen (auto merge/split based on player distance)
- **Co-op Sync:** Score subsystem driving adaptive difficulty and music layers
- **Level Manager:** SODPrototypeLevel orchestrating Start → Puzzle → Combat → End flow
- **Telemetry:** Session metrics logging for QA analysis
- **Debug Tools:** Console cheat commands for testers (SkipPuzzle, GodMode, etc.)
- **HUD:** Ping markers, cooldown display, checkpoint notifications

### Infrastructure
- **UE5 5.4+ Project:** Full .uproject with GAS, Enhanced Input, Niagara, Networking
- **CI/CD:** GitHub Actions source validation workflow
- **Config:** Engine (Lumen/Nanite), Input, GameplayTags, Game settings
- **Blueprint Stubs:** Characters, GameMode, Puzzles, Materials, Maps

### QA & Documentation
- **Tester Guide:** `qa/TESTER_GUIDE.md` — controls, test flow, debug commands
- **Smoke Test:** `qa/SMOKE_TEST_CHECKLIST.md` — 4-run smoke suite
- **Entry Gate:** `qa/QA_ENTRY_GATE_CHECKLIST.md` — mandatory criteria
- **Playtest Plan:** `qa/QA_PLAYTEST_PLAN_1000_GAMERS.md` — scaled testing

## How to Test

1. Install **Unreal Engine 5.4+** via Epic Games Launcher
2. Clone: `git clone https://github.com/liemdo28/gameittakes2.git`
3. Open `ShardsOfDawn.uproject` in Unreal Engine
4. Compile C++ (Visual Studio 2022 or Rider)
5. Open `Content/Game/Maps/PrototypeRoom`
6. Set **Number of Players: 2** → Press **Play**
7. Follow `qa/TESTER_GUIDE.md` for full instructions

## Debug Console Commands

Press **`** (backtick) in-game:

| Command | Effect |
|---------|--------|
| `SOD.SkipPuzzle` | Solve nearest puzzle |
| `SOD.SkipAllPuzzles` | Solve all puzzles |
| `SOD.SkipCombat` | Defeat enemy instantly |
| `SOD.GodMode` | Toggle invincibility |
| `SOD.ResetLevel` | Reset entire level |
| `SOD.PrintTelemetry` | Print session metrics |
| `SOD.SetSyncScore 0.8` | Override sync score |
| `SOD.TeleportToZone Combat` | Teleport to zone |

## Prototype Test Flow

```
START ZONE → PUZZLE ZONE (3 puzzles + door) → COMBAT ZONE (dual-shield enemy) → END ZONE (checkpoint)
```

## Known Limitations
- Greybox geometry only (no final art assets)
- Audio placeholders only
- Single prototype level
- Local split-screen only (no online co-op yet)
