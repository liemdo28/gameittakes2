# PROTOTYPE BUILD ROADMAP — 14 DAYS (TESTABLE)

## Goal
Ship a testable prototype build for internal QA and pilot external testers.

## Deliverable at Day 14
- Windows packaged build
- 2-player co-op session (local/online dev mode)
- 1 puzzle room with LightPulse + ShadowAnchor
- basic UI feedback + retry loop
- smoke test report attached

---
## Day-by-Day Execution
### Day 1
- Create UE5 project structure
- Enable GAS + Enhanced Input + networking settings
- Setup CI smoke build

### Day 2
- Character base movement (2 archetypes)
- Input mapping
- Interaction component

### Day 3
- Session create/join flow
- Initial replication sanity check
- Gate 1: feasibility review

### Day 4
- Implement LightPulse MVP
- Add VFX/SFX placeholders
- Add telemetry event hooks

### Day 5
- Implement ShadowAnchor MVP
- Combine with LightPulse in one trigger chain
- Build candidate v0.1

### Day 6
- Create puzzle door actor (dual-state)
- Add fail/retry logic
- Add checkpoint micro-state

### Day 7
- Graybox room complete
- End-to-end gameplay loop pass
- Gate 2: playable review

### Day 8
- UI pass: cooldown + prompts + ping wheel shell
- Readability tuning for puzzle states
- QA smoke run #1

### Day 9
- Fix blockers from smoke run #1
- Camera split/merge basic tuning
- Net sync stability pass

### Day 10
- Build candidate v0.2
- Gate 3: testability review
- Freeze new features

### Day 11
- Performance sanity pass
- Crash/edge-case handling
- QA regression #2

### Day 12
- Polish pass: animation timing + audio cues
- Fix top critical/high bugs

### Day 13
- Final packaging rehearsal
- Release notes draft
- QA sign-off prep

### Day 14
- Prototype build release
- Gate 4: acceptance
- Post-mortem + next sprint planning

## Acceptance Checklist
- [ ] Build launches without crash on target test machines
- [ ] 2 players can complete room from start to finish
- [ ] Puzzle reset works reliably after failure
- [ ] Critical bugs = 0 at handoff
