# UE5 EDITOR SETUP & PLAY — Shards of Dawn

## Prerequisites
- Unreal Engine 5.4 installed (from Epic Games Launcher)
- Visual Studio 2022 with C++ toolchain
- Windows 10/11
- Git

---

## PHASE 1 — Clone / Pull Project

```bash
cd E:\Project\Personal
git clone https://github.com/liemdo28/gameittakes2.git
cd gameittakes2
git checkout main
```

Or if already cloned:
```bash
cd E:\Project\Personal\gameittakes2
git pull origin main
```

---

## PHASE 2 — Build C++ (Required Before Running Editor)

### Option A: Visual Studio (Recommended)

```
1. Double-click ShardsOfDawn.uproject
   → UE5 will ask: "Generate Visual Studio project files?"
   → Click YES

2. Visual Studio opens with ShardsOfDawn.sln

3. Build Configuration dropdown → Select: "Development Editor"

4. Build > Build Solution (or Ctrl+Shift+B)

5. Wait for: ========== Build: 0 Error(s), 0 Warning(s) ==========
```

### Option B: Command Line (Advanced)

```powershell
cd E:\Project\Personal\gameittakes2
"C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat"
    ShardsOfDawn.uproject Development Win64
```

### Option C: UE5 Editor (Easiest)

```
1. Double-click ShardsOfDawn.uproject
   → UE5 opens and auto-compiles C++ on first load
   → Wait for compilation in bottom-right progress bar
   → "Compiling C++..." must finish
```

---

## PHASE 3 — Open & Setup Map

### 3.1 — Open Project

```
1. UE5 Editor is already open (from Phase 2)
2. Content Browser should show:
   Game/ Maps/PrototypeRoom
   Game/ Blueprints/Player/
   Game/ Blueprints/Actors/
   Game/ GameModes/
   Game/ Input/
   Game/ Materials/
```

### 3.2 — Open PrototypeRoom Map

```
Content Browser > Game > Maps > PrototypeRoom
→ Double-click PrototypeRoom.umap
→ Map opens in viewport
```

### 3.3 — Create PlayerStarts

**Light Weaver Start:**
```
1. Modes Panel > Place Mode > All Classes
2. Search: PlayerStart
3. Drag into viewport at position (-800, 0, 0)
4. In Details Panel:
   - Actor Label: PlayerStart.Light
   - Tags: + Add "PlayerStart.Light"
```

**Shadow Walker Start:**
```
1. Drag another PlayerStart into viewport at position (-800, 400, 0)
2. In Details Panel:
   - Actor Label: PlayerStart.Shadow
   - Tags: + Add "PlayerStart.Shadow"
```

### 3.4 — Place Shard Actors

**Hall A — Light Shards (3x):**
```
1. Content Browser > Game > Blueprints > Actors > BP_LightShard
2. Drag 3x into viewport:
   - Instance 1: Location (-400, 200, 0)
   - Instance 2: Location (-200, 400, 0)
   - Instance 3: Location (0, 600, 0)
3. Rename each:
   - Shard_Light_01
   - Shard_Light_02
   - Shard_Light_03
```

**Hall B — Shadow Shards (3x):**
```
1. Content Browser > Game > Blueprints > Actors > BP_ShadowShard
2. Drag 3x into viewport:
   - Instance 1: Location (0, -200, 0)
   - Instance 2: Location (200, -400, 0)
   - Instance 3: Location (400, -600, 0)
3. Rename each:
   - Shard_Shadow_01
   - Shard_Shadow_02
   - Shard_Shadow_03
```

**Co-op Bridge (1x):**
```
1. Content Browser > Game > Blueprints > Actors > BP_CoopBridge
2. Drag into viewport at location (600, -200, 0)
3. Rotation Y: 90°
4. Rename: BP_CoopBridge
5. In Details > CoopBridge Config:
   - RequiredActivationCount = 6
   - MonitoredShardIDs = [Shard_Light_01, Shard_Light_02, Shard_Light_03,
                          Shard_Shadow_01, Shard_Shadow_02, Shard_Shadow_03]
```

### 3.5 — Configure IMC (Input Mapping Context)

**IMC_LightWeaver:**
```
1. Content Browser > Game > Input > Right-click
2. Import or create: IMC_ShardsOfDawn_LightWeaver
3. Open IMC asset > Mappings:
   - Action Mappings:
     * Interact  [E]
     * Jump      [Space]
     * Sprint    [Left Shift]
   - Axis Mappings:
     * MoveForward  [W] scale=1.0
     * MoveRight    [D] scale=1.0
     * LookUp       [Mouse Y] scale=1.0
     * Turn         [Mouse X] scale=1.0
```

**IMC_ShadowWalker:** (same bindings, different name)

**Assign to PlayerControllers:**
```
1. Open BP_SodPlayerController
2. Details > Input:
   - LightWeaver_IMC = IMC_ShardsOfDawn_LightWeaver
   - ShadowWalker_IMC = IMC_ShardsOfDawn_ShadowWalker
```

### 3.6 — Assign GameMode

```
World Settings > GameMode Override:
  GameMode Override = BP_SodGameMode

Or in DefaultEngine.ini (already set):
  GlobalDefaultGameMode=/Game/Game/GameModes/BP_SodGameMode.BP_SodGameMode_C
```

---

## PHASE 4 — Build Map Geometry (Minimum)

### Floor
```
1. Modes > Place Mode > Basic > Box
2. Drag into viewport: Size X=3000, Y=3000, Z=50
3. Location: (0, 0, -25)
4. Material: M_Floor (create or assign default)
```

### Hall Separators (optional visual)
```
Hall A: Box at (-400, 400, 100), Size: 800x10x200
Hall B: Box at (200, -400, 100), Size: 800x10x200
```

### Exit Platform
```
Box at (900, -200, 200), Size: 400x400x50
```

---

## PHASE 5 — PIE Smoke Test

### Start PIE (2 Players)
```
1. Top toolbar: Play button dropdown >
2. Select: "Selected Viewport" (or "New Editor Window")
3. Number of players: 2
4. Net Mode: "Play Standalone" or "Play in Process" (for LAN: Listen Server)

→ Both characters spawn, one at each PlayerStart
```

### Run Smoke Test
Follow `qa/SMOKE_TEST_CHECKLIST.md`

**Quick smoke (5 min):**
```
[P1] Walk to Light Shard 1 → Press E → verify glow + sound
[P2] Walk to Shadow Shard 1 → Press E → verify glow + sound
[Both] Activate all 6 shards → bridge lowers → cross bridge
```

**Full smoke (15 min):** Run all sections in `qa/SMOKE_TEST_CHECKLIST.md`

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| "Module not found" on build | Run Step 2 again, ensure VS has UE5 toolchain |
| PlayerStart not spawning correct character | Check GameMode DefaultPawnClass or BP_SodGameMode |
| Shard not responding to E key | Check InteractionTraceChannel = ECC_GameTraceChannel1 |
| Bridge not lowering | Check RequiredActivationCount=6 and MonitoredShardIDs match |
| IMC not binding keys | Ensure IMC assigned to PlayerController BP |
| No sound on shard activation | Assign ActivationSound in BP_LightShard/BP_ShadowShard |
| NetMode errors in PIE | Enable "Run Under One Process" for 2-player PIE |

---

## Automation Scripts

| Script | Usage |
|--------|-------|
| `automation/setup_editor.bat` | Run from Windows Explorer to verify setup |
| `automation/setup_prototype.py` | Run from UE5: File > Execute Python |

---

## Next After Smoke Test Passes

```
✓ PIE 15-min stability pass
✓ All shards interactive
✓ Bridge lowers correctly
✓ Exit trigger fires
→ Sprint 1 complete → Proceed to Chapter 1 full level design
→ See: roadmap/SPRINT_1_EXECUTION_BOARD_WEEK1_WEEK2.md
```
