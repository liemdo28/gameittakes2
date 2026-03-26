# First 15 Minutes Gate Test

**Purpose:** Strict pass/fail validation that the Shards of Dawn UE5 prototype is alive and functional on a clean machine. Every item must pass. Any single failure means the project is not test-ready.

---

## Prerequisites

- [ ] Unreal Engine 5.4 or later installed
- [ ] Visual Studio 2022 or Rider installed with UE C++ workload
- [ ] Minimum 16 GB RAM
- [ ] DirectX 12 compatible GPU with up-to-date drivers

---

## Phase 1 -- Project Opens (0-5 min)

- [ ] Repository cloned successfully (no LFS errors, no missing files)
- [ ] `.uproject` file opens in UE5 without crash
- [ ] No "missing plugin" dialogs appear on launch
- [ ] Editor reaches the main viewport without blocking errors

---

## Phase 2 -- Compilation and Assets (5-10 min)

- [ ] C++ compilation completes with **0 errors**
- [ ] `PrototypeRoom` map opens in the editor
- [ ] Output Log shows no asset-related errors (no missing references, no failed imports)

---

## Phase 3 -- Gameplay Verification (10-15 min)

### Spawn and Rendering
- [ ] Multiplayer > Number of Players set to **2**
- [ ] Press **Play** -- session starts without crash
- [ ] Both player characters spawn in the level
- [ ] Split-screen renders correctly (no black panels, no overlapping viewports)

### Input
- [ ] Player 1 can move using **WASD** (keyboard)
- [ ] Player 2 can move using **gamepad or second input device**

### Abilities
- [ ] **Light Pulse** ability fires when activated
- [ ] **Shadow Anchor** ability fires when activated

### Puzzle Interaction
- [ ] At least **1 puzzle element** responds to player interaction

---

## Verdict

**ALL** items above must be checked. Any single unchecked box means the build **fails** the gate and is **not test-ready**.

| Result | Criteria |
|--------|----------|
| **PASS** | Every box checked |
| **FAIL** | One or more boxes unchecked |

**Tester:** ___________________
**Date:** ___________________
**Build/Commit:** ___________________
