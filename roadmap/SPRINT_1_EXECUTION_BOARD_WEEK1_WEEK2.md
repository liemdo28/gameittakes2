# SPRINT 1 EXECUTION BOARD (2 WEEKS)

## Sprint Goal
Thiết lập nền tảng playable đầu tiên cho co-op loop: session + movement + 2 abilities + 1 puzzle room hoàn chỉnh.

## Team Pods
- Pod A (Gameplay): movement, input, ability hooks
- Pod B (Network): session flow, replication sanity
- Pod C (Level Design): graybox room + puzzle logic
- Pod D (UI/UX): ping wheel v0 + cooldown HUD v0
- Pod E (Audio/Art support): placeholder feedback assets
- Pod F (QA): smoke plans + issue triage

---
## WEEK 1 (Day-by-Day)
### Day 1
- Kickoff sprint, xác nhận DoD
- Lock task ownership từng pod
- Setup branch strategy + CI smoke

### Day 2
- Gameplay: movement baseline cho 2 nhân vật
- Network: host/join local flow
- Level: blockout room #1

### Day 3
- Gameplay: interaction framework (usable actors)
- Network: reconnect prototype
- UI: ping wheel shell

### Day 4
- Abilities: LightPulse prototype
- Puzzle: dual-state door actor v0
- QA: smoke test checklist v1

### Day 5
- Abilities: ShadowAnchor prototype
- Integrate LightPulse + ShadowAnchor with door
- Sprint review mini-demo nội bộ

## WEEK 1 Exit Criteria
- Join session local thành công
- 2 ability prototype kích hoạt được
- 1 puzzle door chạy end-to-end local

---
## WEEK 2 (Day-by-Day)
### Day 6
- Network replication cho puzzle state
- UI cooldown + interaction prompt v0
- Add fail/retry messaging

### Day 7
- Camera dynamic split-screen v0
- Puzzle room pass readability
- Audio placeholder cues integrate

### Day 8
- Fix desync và timing inconsistencies
- Add checkpoint micro-state
- QA regression run #1

### Day 9
- Performance sanity pass (CPU/GPU quick profile)
- Polish animation timing cơ bản
- Internal playtest 1 (30–45 phút)

### Day 10
- Bug triage + hotfix
- Sprint demo build đóng gói
- Retro + Sprint 2 planning handoff

## WEEK 2 Exit Criteria
- Session create/join/reconnect hoạt động ổn định trong test nội bộ
- Puzzle replicated ổn định
- Sprint demo build đạt “playable first slice”

---
## Sprint 1 KPI
- Crash-free session >= 98% nội bộ
- 0 blocker bug mở khi kết sprint
- Puzzle completion success > 70% ở playtest nội bộ đầu tiên
