# SHARDS OF DAWN — MASTER GDD (PRE-PRODUCTION)

## 1. Vision
Shards of Dawn là game co-op hành động phiêu lưu bắt buộc 2 người, nhấn mạnh phối hợp bất đối xứng, kể chuyện điện ảnh và nhịp cảm xúc sâu.

Mục tiêu chất lượng:
- Visual: AAA stylized realism
- Audio: adaptive score + high-clarity gameplay feedback
- Narrative: character-driven, chapter-based dramatic arc
- Gameplay: mỗi chương có mechanic co-op signature riêng

## 2. High Concept
- Genre: Co-op Action Adventure / Puzzle Platformer
- Camera: Dynamic split-screen + occasional merged frame
- Session length: 12–14 giờ cho full game
- Target platform: PC/Console current-gen

## 3. Player Fantasy
- “Tôi và bạn tôi thật sự cần nhau để chiến thắng.”
- “Mỗi chương đều làm tôi bất ngờ.”
- “Tôi chơi vì gameplay và ở lại vì cảm xúc.”

## 4. Core Design Pillars
1) Interdependence by design
2) Novel mechanics per chapter
3) Story through interaction
4) Cinematic readability
5) Emotional crescendo

## 5. Systems Overview
### 5.1 Traversal
- Contextual parkour
- Dual grappling actions (light tether / shadow hook)
- Assisted catch-up movement khi một người tụt lại xa

### 5.2 Puzzle Framework
- State machine 2 lớp: Light / Shadow
- Timed cooperative gates
- Relay puzzle (A triggers > B sustains > A executes)

### 5.3 Combat Framework
- Dual shield enemies
- Aggro choreography (one baits, one breaks)
- Co-op finishers with synchronized prompts

### 5.4 Boss Framework
- 3 phases minimum
- Phase transition bằng mechanical escalation
- Narrative reveal gắn trực tiếp với phase 2 hoặc 3

## 6. Chapter Structure (Full Game)
1. Cloud Market Ruins
2. Clock Reef
3. Glass Forest
4. Inverted Opera
5. Abyss Library
6. Citadel of Dawn

Mỗi chapter có:
- 1 signature mechanic set
- 2 side activities co-op
- 1 narrative memory reveal
- 1 major boss/encounter

## 7. Progression
- Unlock theo chapter
- Kỹ năng không tăng damage thuần; tăng chiều sâu phối hợp
- Memory Fragments mở cinematic + dialogue branches ở hub

## 8. UX / Accessibility
- Ping wheel theo ngữ cảnh
- Adaptive timing window
- Colorblind-safe VFX pairs
- Subtitles + speaker labeling + dynamic mix presets

## 9. Technical Requirements
- 60fps locked console / 90fps target PC mid-high
- Stable netcode for 120–180ms ping co-op online
- Deterministic puzzle states replicated server authoritative

## 10. Vertical Slice Definition
- 90–120 phút gameplay
- 2 chapter hoàn chỉnh
- 1 boss hoàn chỉnh production quality
- Full loop: puzzle + combat + chase + cinematic + hub return
