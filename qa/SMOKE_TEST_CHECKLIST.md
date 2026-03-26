# SMOKE TEST CHECKLIST — Shards of Dawn Prototype
> **Date:** 2026-03-25 | **Map:** PrototypeRoom | **Config:** Development Editor
> **Owner:** QA | **Criteria:** Must pass ALL items to proceed to next sprint

---

## Pre-flight

- [ ] `ShardsOfDawn.uproject` opens without module error
- [ ] VS build `ShardsOfDawnEditor` → `Development Editor` succeeds
- [ ] `PrototypeRoom` map loads in Editor viewport
- [ ] `BP_SodGameMode` assigned as GameMode (check World Settings)
- [ ] Both PlayerStarts present: `PlayerStart.Light` + `PlayerStart.Shadow`
- [ ] 3x `BP_LightShard` placed in Hall A
- [ ] 3x `BP_ShadowShard` placed in Hall B
- [ ] 1x `BP_CoopBridge` placed between Halls and Exit
- [ ] Collision channels: `ECC_GameTraceChannel1` = "Interaction"
- [ ] IMC_LightWeaver + IMC_ShadowWalker assigned to respective PlayerControllers

---

## Smoke Run A — P1 Solo (Light Weaver)

### A1 — Spawn
- [ ] P1 spawns at `PlayerStart.Light`
- [ ] Player character is `BP_LightWeaver`
- [ ] HUD shows "Light Weaver" archetype notification

### A2 — Movement
- [ ] `WASD` moves forward/left/back/right
- [ ] `Mouse` rotates camera (orbit around character)
- [ ] `Space` jumps (character clears 150cm obstacle)
- [ ] `Shift` sprint activates (speed visually faster)
- [ ] Character does not fall through floor

### A3 — Interaction
- [ ] Walk within 400cm of `BP_LightShard` → interaction prompt appears
- [ ] Walk within 400cm of `BP_ShadowShard` → no prompt (wrong archetype)
- [ ] Press `[E]` near `BP_LightShard` → shard activates (glow + sound)
- [ ] Press `[E]` near same shard again → shard deactivates (toggle)
- [ ] Wrong archetype attempt on Shadow Shard → failure sound + visual feedback

### A4 — Puzzle State
- [ ] 3x Light Shards all activatable by P1
- [ ] Activation count in HUD or log increments per shard
- [ ] Log output: `LogTemp: [SodGameMode] ActivatedShardIDs` updates

---

## Smoke Run B — P2 Solo (Shadow Walker)

### B1 — Spawn
- [ ] P2 spawns at `PlayerStart.Shadow`
- [ ] Player character is `BP_ShadowWalker`
- [ ] HUD shows "Shadow Walker" archetype notification

### B2 — Movement
- [ ] `WASD` + `Mouse` movement functional
- [ ] `Space` jump functional
- [ ] `Shift` sprint functional

### B3 — Interaction
- [ ] Walk near `BP_ShadowShard` → prompt appears
- [ ] Walk near `BP_LightShard` → no prompt
- [ ] Press `[E]` on `BP_ShadowShard` → shard activates
- [ ] Toggle off works

---

## Smoke Run C — Co-op (Both Players)

### C1 — Session Join
- [ ] P1 + P2 in same LAN session
- [ ] Both players visible to each other
- [ ] No desync crash within 5 minutes

### C2 — Co-op Puzzle
- [ ] P1 activates Light Shard 1, 2, 3 → progress 50%
- [ ] P2 activates Shadow Shard 1, 2, 3 → progress 100%
- [ ] At 100%: bridge lowers after 1.5s delay (audio + animation)
- [ ] Bridge is walkable (collision allows passage)

### C3 — Exit
- [ ] Both players walk across lowered bridge
- [ ] Both reach exit platform
- [ ] `RestartRound` event triggers on overlap (reset puzzle state)

---

## Smoke Run D — 15-Minute Stability

| Minute | Check | Result |
|--------|-------|--------|
| 0–2    | Launch PIE, both players join | Pass/Fail |
| 2–5    | Rapid shard toggle (E key mash) | Pass/Fail |
| 5–8    | Walk bridge up/down repeatedly | Pass/Fail |
| 8–12   | Leave PIE idle (no input) | Pass/Fail |
| 12–15  | Respawn / restart round stress | Pass/Fail |

**Final verdict:** ☐ PASS | ☐ FAIL

---

## Network / Replication

- [ ] Shard activation replicates to remote client
- [ ] Bridge state replicates to remote client
- [ ] No `CheatManager` errors in log
- [ ] Player replication defaults verified on `ASODPlayerCharacter`
- [ ] No `GetLifetimeReplicatedProps` warnings

---

## Telemetry / Logging

- [ ] `LogGameEvent` fires on shard activation
- [ ] `LogTemp` shows archetype notification text
- [ ] `OnPlayerJoined` fires on second controller join

---

## Exit Criteria Summary

```
All boxes checked → QA SIGN-OFF → Sprint 2 gate unlocked
Any FAIL → Blocker logged in Jira → Return to Engineering
```

| Blocker Severity | Action |
|---|---|
| Crash / hang | P0 — Engineering hotfix |
| Interaction broken | P1 — Sprint 2 must fix |
| Visual/audio glitch | P2 — Backlog |
| Cosmetic | P3 — Nice to have |
