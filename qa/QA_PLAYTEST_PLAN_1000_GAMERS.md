# QA & PLAYTEST PLAN — SCALE TO 1,000+ GAMERS

## Objective
Chuẩn bị hạ tầng và quy trình để test game với quy mô lớn, từ internal QA đến external community playtest.

## Test Program Phases
### Phase 0 — Internal QA (20–50 testers)
- Mục tiêu: ổn định core loop, loại blocker
- Thời lượng: 2–3 tuần

### Phase 1 — Closed External Alpha (100–200 players)
- Mục tiêu: xác nhận fun loop + network reliability cơ bản
- Thời lượng: 1–2 tuần

### Phase 2 — Expanded Closed Beta (500 players)
- Mục tiêu: stress test backend/gameplay systems
- Thời lượng: 2 tuần

### Phase 3 — Large-Scale Beta (1,000+ players)
- Mục tiêu: validate retention, completion, stability at scale
- Thời lượng: 2–4 tuần

## QA Test Pillars
1. Session stability (create/join/reconnect)
2. Gameplay correctness (puzzle/combat progression)
3. Network sync (desync, lag, packet loss)
4. Performance (fps, spikes, memory)
5. UX clarity and accessibility

## Environment Matrix
- Hardware tiers: low/mid/high
- Network conditions: 40ms/100ms/180ms + packet loss profiles
- Input devices: keyboard/mouse + controller variants

## Defect Process
- Severity: Blocker / Critical / High / Medium / Low
- SLA fix windows:
  - Blocker: 24h
  - Critical: 48h
  - High: 72h

## Telemetry Events (minimum)
- SessionStart, SessionDrop, SessionReconnect
- PuzzleStart, PuzzleFail, PuzzleComplete
- BossAttempt, BossFail, BossComplete
- CrashEvent with context snapshot

## Go / No-Go Criteria for 1,000+ Test
- Crash-free sessions >= 99%
- Blocker count = 0
- Critical open defects <= threshold
- Match/session success rate >= target
- Average completion of target slice >= target
