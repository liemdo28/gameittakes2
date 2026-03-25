# Shards of Dawn

**A premium co-op action-adventure game** featuring Light/Shadow dual-state mechanics.
Two players must cooperate to solve puzzles, defeat enemies, and rebuild a shattered world.

> "Only together can they rebuild the dawn."

## Project Status

| Area | Status |
|------|--------|
| Design & Documentation | Complete (GDD, Narrative Bible, Art/Audio Direction) |
| UE5 Project Structure | **Created** (.uproject, Source/, Config/, Content/) |
| Core C++ Gameplay Code | **Created** (Characters, Abilities, Puzzles, Combat, Co-op, Camera) |
| Build & Playtest | Pending (requires UE5 5.4+ to compile) |

## Quick Start (for developers)

1. Install **Unreal Engine 5.4+**
2. Open `ShardsOfDawn/ShardsOfDawn.uproject`
3. Compile the C++ source (Visual Studio 2022 or Rider recommended)
4. The project includes:
   - `SODGameMode` — Manages player spawning and role assignment (Light/Shadow)
   - `SODPlayerCharacter` — Base character with GAS integration and Enhanced Input
   - `SODLinhCharacter` — Linh "The Spark" (Light abilities)
   - `SODNamCharacter` — Nam "The Shade" (Shadow abilities)
   - `SODPuzzleActor` — Dual-state puzzle system (Neutral → LightRevealed → ShadowAnchored → Solved)
   - `SODPuzzleDoor` — Door that opens when linked puzzles are solved
   - `SODDualShieldEnemy` — Enemy requiring both players to defeat
   - `SODSplitScreenManager` — Dynamic split-screen camera system
   - `SODCoOpSyncSubsystem` — Tracks co-op quality, drives adaptive music/difficulty
   - `SODCheckpointActor` — Co-op checkpoint system

## Repository Structure

```
ShardsOfDawn/                    # <-- UE5 PROJECT (open this in Unreal)
├── ShardsOfDawn.uproject
├── Config/
│   ├── DefaultEngine.ini        # Lumen, Nanite, collision channels, networking
│   ├── DefaultGame.ini          # Project settings, packaging, localization
│   ├── DefaultInput.ini         # Enhanced Input config
│   └── DefaultGameplayTags.ini  # All gameplay tags (abilities, combat, chapters)
├── Source/
│   ├── ShardsOfDawn.Target.cs
│   ├── ShardsOfDawnEditor.Target.cs
│   ├── ShardsOfDawn/
│   │   ├── ShardsOfDawn.Build.cs
│   │   ├── Core/                # Game module, GameInstance, GameMode
│   │   ├── Characters/          # Player characters (Linh, Nam, base class)
│   │   ├── Abilities/           # GAS abilities (LightPulse, ShadowAnchor)
│   │   ├── Puzzle/              # Dual-state puzzle system + doors
│   │   ├── Combat/              # Dual-shield enemies
│   │   ├── Camera/              # Dynamic split-screen manager
│   │   ├── CoOp/                # Sync scoring, checkpoints
│   │   └── UI/                  # HUD system
│   └── ShardsOfDawnEditor/      # Editor module
└── Content/                     # UE5 content (assets created in-editor)

docs/                            # Project overview
game-design/                     # Pillars, gameplay systems, story script
art/                             # Art direction
audio/                           # Audio direction
tech/                            # UE5 architecture and system blueprints
prototype/                       # Prototype build roadmap and checklists
roadmap/                         # Sprint and long-term execution plans
qa/                              # QA and large-scale playtest planning
ops/                             # Live operations runbooks and readiness scorecards
production/                      # Governance, gates, master GDD, dashboards, templates
backlog/                         # Jira import datasets and backlog structures
pitch/                           # Investor/publisher-facing materials
```

## Architecture Overview

### Core Systems (C++ / GAS)
- **Gameplay Ability System (GAS):** All abilities use UE5's GAS with co-op validation
- **Enhanced Input:** Full gamepad + keyboard/mouse support for split-screen
- **Networking:** Server-authoritative replication for all gameplay state
- **Dynamic Split-Screen:** Auto-merges camera when players are close, splits when they separate

### Co-op Design Pillars
1. **Co-op is mandatory** — Neither player can solve puzzles or defeat enemies alone
2. **Light/Shadow duality** — Linh reveals, Nam anchors. Both roles are essential
3. **Adaptive experience** — Co-op sync score drives music layers, dialogue tone, and difficulty

### Puzzle Flow
```
Neutral → [Linh: Light Pulse] → LightRevealed → [Nam: Shadow Anchor] → Solved → Door Opens
                                     ↓ (timeout)
                                   Neutral (retry)
```

### Combat Flow
```
LightShield → [Nam breaks it] → ShadowShield → [Linh breaks it] → Vulnerable → [Both: Finisher] → Defeated
```

## Documentation Start Here
1. `production/MASTER_INDEX_AND_DEPENDENCY_MAP.md` — Navigation guide
2. `production/CEO_GAP_CLOSURE_PLAN.md` — 14-day prototype plan
3. `production/GDD_MASTER.md` — Master Game Design Document
4. `tech/UE5_TECH_ARCHITECTURE.md` — Technical architecture
5. `prototype/PROTOTYPE_BUILD_ROADMAP_14_DAYS.md` — Day-by-day build plan

## Next Steps (Execution Priority)
1. **Open the UE5 project** and compile C++ source
2. **Create greybox level** with basic geometry
3. **Test 2-player local co-op** with the existing character/puzzle code
4. **Validate Light/Shadow mechanic** with 1 puzzle room
5. **Build first playable** within 14 days (see CEO_GAP_CLOSURE_PLAN)
