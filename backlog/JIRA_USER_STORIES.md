# JIRA USER STORIES (SAMPLE READY)

## SOD-US-001 (EP01/FEAT-001)
**As** a player,
**I want** to create or join a co-op session in under 30 seconds,
**so that** I can start playing quickly with my partner.

**Acceptance Criteria**
- Given both players are online, when one hosts and the other joins by invite code, then session starts within 30s.
- Reconnect returns disconnected player to last safe checkpoint.
- Failure states provide retry guidance.

---
## SOD-US-002 (EP01/FEAT-002)
**As** a co-op player,
**I want** camera split to adapt based on distance,
**so that** I always see readable action.

**Acceptance Criteria**
- Split merges when player distance < threshold.
- Split separates smoothly with no hard pop.
- Camera avoids occlusion in 95% benchmark scenes.

---
## SOD-US-003 (EP02/FEAT-103)
**As** a player,
**I want** ability combos to react consistently,
**so that** teamwork feels reliable and skill-based.

**Acceptance Criteria**
- Light+Shadow combos trigger deterministic outcomes.
- Server-authoritative validation prevents desync.
- Combo feedback includes VFX, SFX, and UI confirmation.

---
## SOD-US-004 (EP03/FEAT-201)
**As** a level designer,
**I want** reusable puzzle actors,
**so that** I can build chapter content faster without code changes.

**Acceptance Criteria**
- Puzzle actor supports config-driven states.
- Supports replication snapshots.
- Supports analytics event hooks.

---
## SOD-US-005 (EP04/FEAT-302)
**As** a co-op team,
**I want** synchronized finishers,
**so that** boss takedowns feel rewarding and coordinated.

**Acceptance Criteria**
- Finisher prompt appears only when both conditions met.
- Grace window configurable per difficulty.
- Distinct fail and success outcomes.

---
## SOD-US-006 (EP05/FEAT-402)
**As** a player,
**I want** seamless transitions between gameplay and cinematic,
**so that** immersion is maintained.

**Acceptance Criteria**
- Transition duration < 1.5s.
- Correct camera ownership handoff.
- Subtitles and audio mix persist correctly.

---
## SOD-US-007 (EP06/FEAT-502)
**As** a player,
**I want** music to react to our teamwork quality,
**so that** emotional intensity reflects our performance.

**Acceptance Criteria**
- Co-op sync score drives at least 3 musical intensity tiers.
- Tier shifts avoid audible popping.
- Telemetry logs score and tier transitions.
