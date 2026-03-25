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
- Create `PrototypeRoom` map asset at `/Game/Maps/PrototypeRoom`
- Create 2 player archetype blueprints
- Implement first interaction + puzzle actor

## 6) Smoke Validation
- Launch PIE for 10–15 min
- Verify no startup crash
- Verify input and interaction path functional
