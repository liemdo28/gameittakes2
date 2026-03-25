# MASTER INDEX & DEPENDENCY MAP — SHARDS OF DAWN

## 1) Cách dùng tài liệu
- Bắt đầu từ file này.
- Đọc theo thứ tự đề xuất để team mới onboard trong 60–90 phút.
- Mọi thay đổi định hướng phải cập nhật tại đây trước.

## 2) Reading Order (Recommended)
1. `docs/PROJECT_OVERVIEW.md`
2. `production/GDD_MASTER.md`
3. `game-design/GAME_PILLARS.md`
4. `game-design/GAMEPLAY_SYSTEMS.md`
5. `production/NARRATIVE_BIBLE.md`
6. `game-design/STORY_SCRIPT.md`
7. `art/ART_DIRECTION.md`
8. `audio/AUDIO_DIRECTION.md`
9. `tech/UE5_TECH_ARCHITECTURE.md`
10. `tech/SYSTEM_BLUEPRINTS.md`
11. `production/VERTICAL_SLICE_PRODUCTION_BIBLE.md`
12. `roadmap/TEAM_AND_PRODUCTION_PLAN.md`
13. `roadmap/AAA_EXECUTION_ROADMAP.md`
14. `roadmap/KPI_AND_QA_STRATEGY.md`
15. `production/CONTENT_BENCHMARK_TARGETS.md`
16. `backlog/JIRA_EPICS_AND_FEATURES.md`
17. `backlog/JIRA_USER_STORIES.md`
18. `backlog/JIRA_IMPORT_TEMPLATE.csv`
19. `pitch/INVESTOR_PUBLISHER_PITCH_DECK.md`
20. `pitch/GO_TO_MARKET_STRATEGY.md`
21. `pitch/FINANCIAL_MODEL_OVERVIEW.md`

## 3) Dependency Map (High Level)
- `PROJECT_OVERVIEW` -> định nghĩa vision gốc cho toàn bộ hệ thống.
- `GDD_MASTER` -> phụ thuộc vision và khóa phạm vi gameplay.
- `GAME_PILLARS` + `GAMEPLAY_SYSTEMS` -> chi tiết hóa GDD cho design/gameplay loop.
- `NARRATIVE_BIBLE` + `STORY_SCRIPT` -> phụ thuộc GDD, tác động trực tiếp cinematic/audio.
- `ART_DIRECTION` + `AUDIO_DIRECTION` -> phụ thuộc GDD + narrative beats.
- `UE5_TECH_ARCHITECTURE` + `SYSTEM_BLUEPRINTS` -> phụ thuộc gameplay systems.
- `VERTICAL_SLICE_PRODUCTION_BIBLE` -> phụ thuộc design/art/audio/tech đã lock.
- `ROADMAP/*` + `KPI` -> phụ thuộc vertical slice scope + quality benchmark.
- `BACKLOG/*` -> chuyển hóa toàn bộ tài liệu thành hạng mục thực thi Jira.
- `PITCH/*` -> tiêu thụ output từ tất cả khối trên để đi fundraising/publishing.

## 4) Change Control Matrix
- Nếu đổi **core fantasy/co-op pillars**:
  - Bắt buộc cập nhật: GDD, Gameplay Systems, KPI, Backlog.
- Nếu đổi **narrative arc**:
  - Bắt buộc cập nhật: Narrative Bible, Story Script, Audio Direction, Cinematic backlog.
- Nếu đổi **tech stack/net model**:
  - Bắt buộc cập nhật: Tech Architecture, System Blueprints, KPI/QA, Jira Epics.
- Nếu đổi **scope milestone**:
  - Bắt buộc cập nhật: Vertical Slice Bible, Roadmap, Team Plan, Pitch.

## 5) Governance
- Owner file: Production Director.
- Cadence update: mỗi tuần (sau milestone review).
- Naming convention update: dùng tiền tố `vX.Y` cuối tài liệu nếu thay đổi lớn.
