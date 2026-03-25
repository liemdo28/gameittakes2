# CEO GAP CLOSURE PLAN — FROM DOCS TO PLAYABLE

## Executive Objective
Biến project từ trạng thái "paper-ready" sang "testable playable" trong 14 ngày, sau đó mở rộng lên vertical slice có thể đưa cho QA lớn và external testers.

## Gap Summary (from CEO review)
1. Chưa có playable build
2. Chưa có UE5 project skeleton thực thi
3. Chưa có testable test-case runtime
4. Chưa có QA onboarding pack cho quy mô lớn

## Non-Negotiable Success Criteria (Day 14)
- Có 1 build playable local co-op (2 player)
- Có 1 mechanic Light/Shadow chạy end-to-end
- Có 1 room puzzle hoàn chỉnh với fail/retry
- Có telemetry cơ bản (start/fail/complete)
- Có smoke test pass report

## 4 Workstreams (parallel)
### WS1 — Prototype Engineering
- UE5 project bootstrap
- Player movement + interaction
- Ability MVP: LightPulse + ShadowAnchor
- Puzzle actor + replication

### WS2 — Content & UX
- Graybox room
- Puzzle readability cues
- HUD + ping wheel v0
- Checkpoint + retry messaging

### WS3 — QA Readiness
- Smoke test suite
- Bug severity taxonomy
- Test runbook and reports

### WS4 — Production Control
- Daily war-room
- Scope lock & kill-list
- Risk burn-down dashboard

## Kill-List (to protect scope)
- Không làm chapter thứ 2 trước Day 14
- Không làm full cinematic pipeline
- Không làm advanced progression trees
- Không polish art beyond readability pass

## Decision Gates
- Day 3: technical feasibility gate
- Day 7: playable gate
- Day 10: testability gate
- Day 14: prototype acceptance gate

## Escalation Rules
- Blocker >24h chưa xử lý: escalate to Tech Director
- Scope mới không đổi trade-off: reject
- Build fail 2 ngày liên tiếp: freeze feature, fix pipeline
