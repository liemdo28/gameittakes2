# Setup

Use this file as the entrypoint for local build validation.

## Prerequisites

- Unreal Engine 5.4 or later installed from Epic Games Launcher
- Visual Studio 2022 or Rider with the C++ toolchain
- Windows SDK installed

## First Run

1. Open `ShardsOfDawn.uproject`.
2. Generate local project files if Unreal prompts for them.
3. Run `automation\setup_editor.bat` from the repo root.
4. Build `ShardsOfDawnEditor` in `Development Editor`.
5. Open `Content/Game/Maps/PrototypeRoom/PrototypeRoom.umap`.

## Validation

- `automation\setup_editor.bat` checks the expected source tree and content folders.
- `.github/workflows/validate-source.yml` blocks case-insensitive filename collisions in CI.
- `qa/SMOKE_TEST_CHECKLIST.md` is the manual playtest gate after the editor build succeeds.

## References

- `automation/UE5_EDITOR_SETUP_GUIDE.md`
- `prototype/LOCAL_BUILD_AND_RUN_GUIDE.md`
- `qa/TESTER_GUIDE.md`
