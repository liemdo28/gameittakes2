# Shards of Dawn

**A premium co-op action-adventure game** featuring Light/Shadow dual-state mechanics.
Two players must cooperate to solve puzzles, defeat enemies, and rebuild a shattered world.

> "Only together can they rebuild the dawn."

[![UE5 Source Validation](https://github.com/liemdo28/gameittakes2/actions/workflows/validate-source.yml/badge.svg)](https://github.com/liemdo28/gameittakes2/actions/workflows/validate-source.yml)

## Project Status

| Area | Status |
|------|--------|
| Design & Documentation | Complete (GDD, Narrative Bible, Art/Audio Direction) |
| UE5 Project (.uproject) | Complete (UE5 5.4+, GAS, Enhanced Input, Niagara) |
| C++ Gameplay Code | Complete (55+ source files across 12 systems) |
| Blueprint Stubs | Complete (Characters, GameMode, Puzzles, Materials, Maps) |
| Level Prototype | Complete (PrototypeRoom with puzzle/combat/checkpoint flow) |
| Telemetry | Complete (Session tracking, timed events, QA metrics) |
| CI/CD | Active (GitHub Actions source validation) |
| Automation | Complete (setup_editor.bat, setup_prototype.py, Editor Guide) |
| QA Artifacts | Complete (Smoke Test, Entry Gate, Playtest Plan, Tester Guide) |
| Build & Playtest | Ready (requires UE5 5.4+ installed and local project files generated) |

## Quick Start (for developers)

1. Install **Unreal Engine 5.4+**
2. Clone this repo
3. Open `ShardsOfDawn.uproject` in the root directory
4. Compile C++ source (Visual Studio 2022 or Rider recommended)
5. Open the `PrototypeRoom` map and press Play (2-player split-screen)

For detailed setup steps, see: `docs/SETUP.md`

## Repository Structure

```
/                                # Root = UE5 Project
├── ShardsOfDawn.uproject        # <-- Open this in Unreal Engine
├── Source/
│   ├── ShardsOfDawn/
│   │   ├── Public/
│   │   │   ├── Characters/      # SODPlayerCharacter, SODLinhCharacter, SODNamCharacter
│   │   │   ├── Abilities/       # GAS abilities (LightPulse, ShadowAnchor)
│   │   │   ├── Actors/PuzzleActors/  # Puzzle system (dual-state, doors, shards, bridge)
│   │   │   ├── Combat/          # SODDualShieldEnemy
│   │   │   ├── Camera/          # SODSplitScreenManager (dynamic merge/split)
│   │   │   ├── CoOp/            # SODCoOpSyncSubsystem, SODCheckpointActor
│   │   │   ├── Core/            # SODGameMode, SODGameInstance
│   │   │   ├── UI/              # SODHUD (ping markers, cooldowns)
│   │   │   ├── Controllers/     # SodPlayerController
│   │   │   ├── GameModes/       # SodGameMode
│   │   │   ├── GameInstance/    # SodGameInstance
│   │   │   └── Interfaces/      # Interface_Interactive
│   │   │   ├── Level/           # SODPrototypeLevel (test room manager)
│   │   │   └── Telemetry/       # SODTelemetrySubsystem (QA metrics)
│   │   ├── Private/             # .cpp implementations (mirrors Public/)
│   │   └── ShardsOfDawn.Build.cs
│   ├── ShardsOfDawnEditor/      # Editor module
│   ├── ShardsOfDawn.Target.cs
│   └── ShardsOfDawnEditor.Target.cs
│
├── Config/
│   ├── DefaultEngine.ini        # Lumen, Nanite, collision channels, networking
│   ├── DefaultGame.ini          # Project settings, packaging
│   ├── DefaultInput.ini         # Enhanced Input config
│   └── DefaultGameplayTags.ini  # All gameplay tags (abilities, combat, chapters)
│
├── Content/
│   └── Game/
│       ├── Blueprints/          # BP_LightWeaver, BP_ShadowWalker, BP_CoopBridge...
│       ├── GameModes/           # BP_SodGameMode
│       ├── Input/               # IMC per archetype
│       ├── Maps/PrototypeRoom/  # Playable test level
│       └── Materials/           # M_LightWeaver_Active, M_Bridge_Locked...
│
├── .github/workflows/           # CI: source validation
├── automation/                  # Setup scripts & guide
│   ├── setup_editor.bat         # Windows setup validator
│   ├── setup_prototype.py       # UE5 Python automation
│   └── UE5_EDITOR_SETUP_GUIDE.md
│
├── docs/                        # Project overview
├── game-design/                 # Pillars, gameplay systems, story script
├── art/                         # Art direction
├── audio/                       # Audio direction
├── tech/                        # UE5 architecture, system blueprints
├── prototype/                   # Prototype roadmap, build/run guide
├── roadmap/                     # Sprint plans, execution roadmap
├── qa/                          # QA checklists, smoke tests, playtest plan
├── ops/                         # Live operations runbooks
├── production/                  # GDD, Narrative Bible, governance, dashboards
├── backlog/                     # Jira-ready epics, stories, CSV imports
└── pitch/                       # Investor/publisher deck, GTM, financial model
```

## Architecture Overview

### Core C++ Systems
- **Gameplay Ability System (GAS):** All abilities use UE5 GAS with co-op validation
- **Enhanced Input:** Full gamepad + keyboard/mouse support for split-screen
- **Networking:** Server-authoritative replication for all gameplay state
- **Dynamic Split-Screen:** Auto-merges camera when players are close, splits when they separate
- **Co-op Sync Score:** Tracks cooperation quality, drives adaptive music and difficulty

### Gameplay Systems

**Puzzle Flow (Light/Shadow dual-state):**
```
Neutral → [Linh: Light Pulse] → LightRevealed → [Nam: Shadow Anchor] → Solved → Door Opens
                                      ↓ (timeout)
                                    Neutral (retry)
```

**Combat Flow (Dual-shield enemies):**
```
LightShield → [Nam breaks it] → ShadowShield → [Linh breaks it] → Vulnerable → [Both: Finisher] → Defeated
```

**Co-op Design Pillars:**
1. **Co-op is mandatory** — Neither player can solve puzzles or defeat enemies alone
2. **Light/Shadow duality** — Linh reveals, Nam anchors. Both roles are essential
3. **Adaptive experience** — Co-op sync score drives music layers, dialogue tone, and difficulty

## Key Documentation
1. `production/MASTER_INDEX_AND_DEPENDENCY_MAP.md` — Navigation guide
2. `production/GDD_MASTER.md` — Master Game Design Document
3. `production/CEO_GAP_CLOSURE_PLAN.md` — 14-day prototype plan
4. `tech/UE5_TECH_ARCHITECTURE.md` — Technical architecture
5. `automation/UE5_EDITOR_SETUP_GUIDE.md` — Step-by-step editor setup
6. `docs/SETUP.md` — Setup entrypoint and dependency validation
7. `qa/SMOKE_TEST_CHECKLIST.md` — QA smoke test suite
8. `prototype/LOCAL_BUILD_AND_RUN_GUIDE.md` — How to build and run locally

## For Testers

See `qa/TESTER_GUIDE.md` for full tester instructions. Quick start:

1. Install **Unreal Engine 5.4+** via Epic Games Launcher
2. Clone repo and open `ShardsOfDawn.uproject`
3. Compile and open `PrototypeRoom` map
4. Press Play with **2 players** (split-screen)
5. Use debug console commands (backtick key) for testing shortcuts
6. Follow `qa/SMOKE_TEST_CHECKLIST.md` for structured testing

## Next Steps
1. Open `ShardsOfDawn.uproject` in UE5 5.4+ and compile
2. Run `automation/setup_editor.bat` to validate environment
3. Open `PrototypeRoom` map and test 2-player split-screen
4. Validate Light/Shadow mechanic with puzzle shards
5. Run smoke test checklist (`qa/SMOKE_TEST_CHECKLIST.md`)
