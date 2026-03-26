# LOCAL BUILD & RUN GUIDE (UE5 SKELETON)

## 1) Prerequisites
- Unreal Engine 5.4 installed
- Visual Studio with C++ toolchain
- Windows SDK installed

## 2) Open Project
- Open `ShardsOfDawn.uproject`
- If prompted, generate project files

## 3) Build
- Build target: `ShardsOfDawnEditor`
- Configuration: Development Editor

## 4) Verify
- Project opens without module errors
- Default map path resolves
- Play-in-editor launches successfully

## 5) Next Immediate Steps
- [x] C++ PlayerCharacter (`ASODPlayerCharacter`) — role-based co-op character, GAS-enabled, replicated
- [x] ✅ C++ PuzzleActors (`SodPuzzleActorBase` + `LightShard` + `ShadowShard`)
- [x] ✅ C++ GameMode (`ASodGameMode`) + PlayerController + GameInstance
- [x] ✅ Input Mapping Context stubs + Blueprint stubs (Light/Shadow archetypes)
- [x] ✅ PrototypeRoom map layout doc
- [ ] Import character meshes + set ArchetypeMaterial
- [ ] Create actual `.uasset` for `BP_LightWeaver`, `BP_ShadowWalker`, `BP_SodGameMode`
- [ ] Place 6x shard actors in PrototypeRoom (3x Light, 3x Shadow)
- [ ] Create co-op bridge actor (requires all shards activated)
- [ ] PIE smoke validation 10–15 min

## 6) Smoke Validation
- Launch PIE for 10–15 min
- Verify no startup crash
- Verify input and interaction path functional
