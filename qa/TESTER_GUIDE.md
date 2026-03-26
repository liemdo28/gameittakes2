# Shards of Dawn - Tester Guide

## Quick Setup

### Requirements
- **Unreal Engine 5.4+** (via Epic Games Launcher)
- **Visual Studio 2022** or **Rider** (C++ compiler)
- **Windows 10/11** (64-bit)
- **16 GB RAM** minimum
- **GPU with DirectX 12** support

### Steps
1. Clone: `git clone https://github.com/liemdo28/gameittakes2.git`
2. Open `ShardsOfDawn.uproject` in Unreal Engine
3. Wait for C++ compilation (first time: 2-5 minutes)
4. Open map: `Content/Game/Maps/PrototypeRoom`
5. Set **Number of Players: 2** in Play settings
6. Press **Play** (split-screen co-op)

## Controls

### Player 1 (Keyboard + Mouse)
| Action | Key |
|--------|-----|
| Move | WASD |
| Look | Mouse |
| Light Pulse (Linh) | Left Click |
| Shadow Anchor (Nam) | Right Click |
| Interact | E |
| Ping | Q |
| Revive Partner | Hold F |

### Player 2 (Gamepad)
| Action | Button |
|--------|--------|
| Move | Left Stick |
| Look | Right Stick |
| Light Pulse / Shadow Anchor | RT |
| Interact | A |
| Ping | LB |
| Revive Partner | Hold X |

## Prototype Test Flow

```
START ZONE --> PUZZLE ZONE --> COMBAT ZONE --> END ZONE
    |              |               |              |
  Spawn     3 puzzles +      Dual-shield     Checkpoint
  here       1 door          enemy fight     = Level Done
```

### 1. Start Zone
- Both players spawn here
- Verify split-screen is working
- Test basic movement and camera

### 2. Puzzle Zone (3 puzzles)
- **Linh** uses Light Pulse on puzzle crystal to reveal it
- **Nam** uses Shadow Anchor within time window to lock it
- Both steps needed = puzzle solved
- Solve all 3 to open the door

### 3. Combat Zone
- Dual-shield enemy appears
- **Nam** breaks the Light Shield
- **Linh** breaks the Shadow Shield
- Both players perform Finisher together when vulnerable
- Enemy defeated

### 4. End Zone
- Reach the checkpoint
- Level complete summary displays

## Debug Commands

Press **`** (backtick/tilde) to open the console. Type these commands:

| Command | Effect |
|---------|--------|
| `SOD.SkipPuzzle` | Solve nearest unsolved puzzle |
| `SOD.SkipAllPuzzles` | Solve all puzzles, open doors |
| `SOD.SkipCombat` | Defeat current enemy instantly |
| `SOD.GodMode` | Toggle invincibility (both players) |
| `SOD.ResetLevel` | Reset entire level |
| `SOD.PrintTelemetry` | Print session metrics to log |
| `SOD.SetSyncScore 0.8` | Override co-op sync score (0-1) |
| `SOD.TeleportToZone Puzzle` | Teleport to zone (Start/Puzzle/Combat/End) |

## What to Test

### Priority 1: Core Loop
- [ ] Both players can move and look around
- [ ] Split-screen displays correctly
- [ ] Light Pulse reveals puzzle elements
- [ ] Shadow Anchor locks revealed elements
- [ ] Puzzle door opens after all puzzles solved
- [ ] Enemy shields break in correct order
- [ ] Co-op finisher works
- [ ] Level complete triggers at end checkpoint

### Priority 2: Co-op Feel
- [ ] Ping system works (mark locations for partner)
- [ ] Revive system works when a player is down
- [ ] Camera merges when players are close
- [ ] Camera splits when players separate
- [ ] Co-op sync score updates on success/failure

### Priority 3: Edge Cases
- [ ] What happens if only one player tries a puzzle?
- [ ] What happens if Shadow Anchor is too late (timeout)?
- [ ] What happens if both players die?
- [ ] What happens if you skip combat and go to end?

## Bug Reporting

When reporting a bug, include:
1. **Steps to reproduce** (exactly what you did)
2. **Expected result** (what should happen)
3. **Actual result** (what happened instead)
4. **Screenshot/video** if possible
5. **Output Log** (`Saved/Logs/ShardsOfDawn.log`)

### Log Location
- Windows: `%LOCALAPPDATA%/ShardsOfDawn/Saved/Logs/`
- Look for `[TELEMETRY]` lines for gameplay metrics

## Telemetry

The game automatically logs all gameplay events. At session end, a summary prints:
```
========== TELEMETRY SESSION SUMMARY ==========
  Duration: 342.5 seconds (5.7 minutes)
  Total Events: 47

  Puzzle.Solved: 3
  Combat.ShieldBreak: 4
  Combat.Finisher: 1
  Timer.End: 3
  CoOp.Ping: 12
  ...
================================================
```

Share this summary with your bug reports for context.
