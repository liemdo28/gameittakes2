# LIVE PLAYTEST OPERATIONS RUNBOOK

## Purpose
Điều phối test live quy mô lớn mà không làm gián đoạn trải nghiệm người chơi.

## Command Structure
- Incident Commander
- QA Lead
- Engineering On-Call
- Community Ops Lead
- Data Analyst

## Operational Cadence
- T-7 days: environment dry run
- T-2 days: build freeze + go/no-go
- T-0: live playtest launch
- T+1 day: incident and KPI review

## Live Monitoring Panels
- Concurrent sessions
- Crash rate by minute
- Reconnect success rate
- Match completion funnel
- Top error signatures

## Incident Severity
- Sev0: service outage / unplayable
- Sev1: major feature failure
- Sev2: degraded experience
- Sev3: minor issue

## Incident Playbook
1. Detect
2. Classify
3. Assign owner
4. Mitigate
5. Communicate to players
6. Post-incident review

## Communication Templates
- Launch message
- Known issue update
- Hotfix deployed note
- Test closing statement

## Post-Test Outputs
- KPI summary
- Top defects and root causes
- Retention and completion insights
- Next build action plan
