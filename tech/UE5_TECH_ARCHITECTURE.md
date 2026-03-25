# UE5 TECH ARCHITECTURE — SHARDS OF DAWN

## 1. Engine Stack
- Unreal Engine 5.4+
- Gameplay Ability System (GAS)
- Enhanced Input
- World Partition + Data Layers
- Niagara + Lumen + Nanite

## 2. Project Layout (proposed)
/Source
  /SODCore
  /SODGameplay
  /SODAI
  /SODUI
  /SODNet
  /SODCinematics

/Content
  /Characters
  /Environments
  /Abilities
  /Puzzles
  /Combat
  /UI
  /Audio
  /Cinematics

## 3. Gameplay Framework
- ASODGameModeCoop
- ASODGameState
- ASODPlayerState
- ASODCharacterBase
- USODAbilitySystemComponent
- USODCoopSyncComponent

## 4. Co-op Networking Model
- Dedicated authoritative server model for shipped build
- Listen server support in dev/test
- Client prediction for movement/ability startup
- Reliable RPC only for critical state (checkpoint, boss phase)

## 5. Puzzle Replication Contract
Each puzzle actor implements:
- Interface: ISODReplicatedPuzzle
- Functions:
  - GetPuzzleStateSnapshot()
  - ApplyPuzzleStateSnapshot()
  - OnPlayerContributed(PlayerId, ContributionType)

## 6. Combat Architecture
- AI Behavior Tree + EQS
- Boss controller with phase state machine asset-driven
- Damage model with dual-channel tags (Light/Shadow)

## 7. Split-screen / Camera
- Dynamic partition manager
- Merge mode when players are nearby
- Cinematic override tracks for authored moments

## 8. Build & Performance
- Target budgets:
  - Frame: 16.6ms console / 11.1ms performance mode
  - Draw calls: controlled by HLOD and instancing
  - Niagara budgets per scene tier

## 9. Tooling
- Automated perf captures every nightly build
- Net emulation test suite (60/120/180ms)
- Content validation pipeline (naming, references, texture budgets)
