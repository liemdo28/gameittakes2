# System Confidence Maps

Isolated test maps for validating individual subsystems independently.
When a bug appears in PrototypeRoom, these maps help isolate whether the issue
is in the subsystem itself or in subsystem interaction.

## Map List

### 1. TestMap_Camera (`Content/Game/Maps/TestMaps/TestMap_Camera`)

**Purpose:** Validate SODSplitScreenManager in isolation.

**Setup:**
- Empty room 5000x5000 units
- 2 player starts, 500 units apart
- No puzzles, no enemies, no checkpoints

**Test Cases:**
| # | Test | Expected |
|---|------|----------|
| 1 | Both players spawn | Two viewports render correctly |
| 2 | Walk toward each other | Camera merges at threshold distance |
| 3 | Walk apart | Camera splits smoothly |
| 4 | Rapid in/out of merge zone | No flickering, no viewport corruption |
| 5 | One player stops, other runs away | Split activates, idle camera stable |
| 6 | Merged camera rotation | Both players visible, no clipping |
| 7 | Vertical split vs horizontal split | Both modes render correctly |

---

### 2. TestMap_Puzzle (`Content/Game/Maps/TestMaps/TestMap_Puzzle`)

**Purpose:** Validate SODPuzzleActor state machine and SODPuzzleDoor in isolation.

**Setup:**
- Small room with 3 puzzle actors and 1 door
- No enemies, no camera tricks (fixed camera OK)
- HUD enabled for state display

**Test Cases:**
| # | Test | Expected |
|---|------|----------|
| 1 | Light Pulse on Neutral puzzle | State → LightRevealed |
| 2 | Shadow Anchor on LightRevealed | State → Solved |
| 3 | Timeout after LightRevealed | State → Neutral (reset) |
| 4 | Shadow Anchor on Neutral (wrong order) | No state change |
| 5 | Light Pulse on LightRevealed (duplicate) | No state change |
| 6 | Solve all 3 puzzles | Door opens |
| 7 | Solve puzzles in different orders | Door still opens |
| 8 | Race condition: anchor at timeout edge | Clean state, no stuck |
| 9 | Reset puzzle (if bResettable) | Back to Neutral, re-solvable |
| 10 | OnPuzzleStateChanged fires correctly | HUD/VFX update on every transition |

---

### 3. TestMap_Combat (`Content/Game/Maps/TestMaps/TestMap_Combat`)

**Purpose:** Validate SODDualShieldEnemy phase transitions and co-op finisher.

**Setup:**
- Arena with 1 dual-shield enemy
- Both players spawn nearby
- No puzzles, no doors

**Test Cases:**
| # | Test | Expected |
|---|------|----------|
| 1 | Enemy spawns in LightShield phase | Correct VFX/HUD indicator |
| 2 | Shadow player damages LightShield | Shield HP decreases |
| 3 | Light player tries to damage LightShield | No effect (wrong element) |
| 4 | LightShield breaks → ShadowShield | Phase transition, VFX changes |
| 5 | Light player damages ShadowShield | Shield HP decreases |
| 6 | ShadowShield breaks → Vulnerable | Phase transition, VFX changes |
| 7 | Both players finisher in time window | Enemy defeated |
| 8 | Only one player finisher (timeout) | Finisher resets, enemy still alive |
| 9 | Player dies during finisher | No state corruption |
| 10 | Enemy with no BehaviorTree | Graceful fallback, no crash |

---

### 4. TestMap_Network (`Content/Game/Maps/TestMaps/TestMap_Network`)

**Purpose:** Validate replication of gameplay state.

**Setup:**
- 1 puzzle + 1 enemy + 1 checkpoint
- Run as Listen Server + Client

**Test Cases:**
| # | Test | Expected |
|---|------|----------|
| 1 | Puzzle state replicates | Both players see same ESODPuzzleState |
| 2 | Enemy phase replicates | Both players see same ESODShieldPhase |
| 3 | Checkpoint triggers replicate | Both players get notification |
| 4 | Co-op sync score replicates | GetSyncScore() same on both |
| 5 | Ping marker replicates | P1 ping visible to P2 and vice versa |
| 6 | OnRep callbacks fire | Client-side UI updates correctly |

---

### 5. TestMap_Input (`Content/Game/Maps/TestMaps/TestMap_Input`)

**Purpose:** Validate Enhanced Input bindings for both players.

**Setup:**
- Empty room, 2 player starts
- HUD showing last input action name + timestamp

**Test Cases:**
| # | Test | Expected |
|---|------|----------|
| 1 | P1 WASD movement | Character moves in correct directions |
| 2 | P1 mouse look | Camera rotates smoothly |
| 3 | P1 LMB/RMB abilities | Correct ability fires |
| 4 | P1 E/Q/F interactions | Interact, ping, revive work |
| 5 | P2 left stick movement | Character moves correctly |
| 6 | P2 right stick look | Camera rotates smoothly |
| 7 | P2 RT/A/LB/X buttons | Ability, interact, ping, revive work |
| 8 | Gamepad dead zones | No drift at rest position |
| 9 | Simultaneous P1+P2 input | No input stealing or conflicts |

---

## Implementation Priority

1. **TestMap_Puzzle** — Highest ROI, puzzle state bugs are P0
2. **TestMap_Combat** — Combat phase transitions need isolation
3. **TestMap_Camera** — Split-screen is unique risk for co-op
4. **TestMap_Network** — Replication bugs caught early save weeks
5. **TestMap_Input** — Lowest risk but needed for gamepad validation

## How to Create

Each test map is a minimal UE5 level:
```
1. File → New Level → Empty Level
2. Add Player Start x2 (for 2-player)
3. Add the specific actors being tested
4. Add basic floor (plane mesh) and lighting
5. Save to Content/Game/Maps/TestMaps/
```

Keep these maps as simple as possible. No art, no extra systems.
The point is isolation, not aesthetics.
