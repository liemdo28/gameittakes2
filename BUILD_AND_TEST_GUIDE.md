# Shards of Dawn - Build & Test Guide cho Tester

## Yeu cau he thong

- **OS:** Windows 10/11 (64-bit)
- **Unreal Engine:** 5.4+ (cai qua Epic Games Launcher)
- **Visual Studio:** 2022 voi C++ game development workload
- **RAM:** 16GB+ (khuyen nghi 32GB)
- **Disk:** 50GB free

## Cach 1: Build nhanh bang Script (Khuyen nghi)

### Buoc 1: Clone repo
```
git clone <repo-url>
cd gameittakes2
git checkout claude/build-game-for-testing-Ad2FN
```

### Buoc 2: Set UE5 path (neu can)
```
set UE5_ROOT=C:\Program Files\Epic Games\UE_5.4
```

### Buoc 3: Build game
```
BuildGame.bat package
```

Script se tu dong:
1. Compile C++ source
2. Cook content (materials, blueprints, maps)
3. Package thanh executable

### Buoc 4: Chay game
```
Build\Packaged\WindowsNoEditor\ShardsOfDawn.exe
```

## Cach 2: Build trong UE5 Editor

### Buoc 1: Mo project
- Mo `ShardsOfDawn.uproject` bang UE5 5.4+
- Cho compile C++ xong (thanh progress bar phia duoi)

### Buoc 2: Validate environment
- Chay `automation/setup_editor.bat` (kiem tra plugins, settings)

### Buoc 3: Test trong Editor (nhanh nhat)
1. Mo map: `Content/Game/Maps/PrototypeRoom`
2. World Settings > Game Mode = `BP_SodGameMode`
3. Bam **Play** (2 players split-screen):
   - Toolbar > Play > Advanced > **Number of Players = 2**
   - Hoac dung **Standalone Game** voi `-game -numplayers=2`

### Buoc 4: Package tu Editor
1. **Platforms > Windows > Package Project**
2. Chon thu muc output
3. Doi build xong (~10-20 phut)

## Cac lenh BuildGame.bat

| Lenh | Mo ta |
|------|-------|
| `BuildGame.bat package` | Full build + cook + package (cho tester) |
| `BuildGame.bat build` | Chi compile C++ (kiem tra loi nhanh) |
| `BuildGame.bat cook` | Chi cook content |
| `BuildGame.bat clean` | Xoa build artifacts |
| `BuildGame.bat editor` | Mo UE5 Editor |

## Test Plan cho Tester

### Quick Test (5 phut)
1. Chay game
2. Kiem tra 2 player spawn dung vi tri
3. WASD + Mouse di chuyen OK
4. Nhan E tuong tac voi Light/Shadow Shard
5. Puzzle co hoat dong (bridge ha xuong khi giai xong)

### Full Smoke Test
Xem file: `qa/SMOKE_TEST_CHECKLIST.md`

### Controls

| Key | Action |
|-----|--------|
| WASD | Di chuyen |
| Mouse | Xoay camera |
| Space | Nhay |
| Shift | Chay nhanh |
| E | Tuong tac |
| Q | Ability chinh |
| R | Ability phu |
| T | Ping vi tri cho dong doi |

## Troubleshooting

### Loi "Module not found"
- Kiem tra da cai UE5 5.4+ chua
- Chay `BuildGame.bat clean` roi build lai

### Loi compile C++
- Kiem tra Visual Studio 2022 voi "Game development with C++" workload
- Mo `.uproject` trong VS va build lai

### Game crash khi chay
- Kiem tra GPU driver moi nhat
- Thu tat Ray Tracing trong DefaultEngine.ini:
  ```
  r.RayTracing=False
  r.Lumen.DiffuseIndirectLighting.HardwareRayTracing=False
  ```

### 2 players khong spawn
- Kiem tra World Settings > Game Mode = BP_SodGameMode
- Kiem tra co 2 PlayerStart trong map (tag: Light va Shadow)
