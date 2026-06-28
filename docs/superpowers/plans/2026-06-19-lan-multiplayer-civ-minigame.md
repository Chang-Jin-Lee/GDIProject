# LAN Multiplayer Civilization Minigame Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the approved LAN/IP Civilization-like tactical minigame on top of the existing Win32/GDI project.

**Architecture:** Add deterministic, renderer-independent gameplay core modules under `GDIProject/Games/Core`, then connect them to the existing menu, play scene, player controller, turn manager, and AI. Winsock TCP remains behind `NetworkSession`, while the host owns authoritative state, timers, random tile removal, and state snapshots.

**Tech Stack:** C++17, Visual Studio v143, Win32/GDI, Winsock2 TCP, PowerShell release script, console-based C++ test project.

---

## File Structure

- Create `GDIProject/Games/Core/CsvTable.h/.cpp`: small CSV loader with trim, header parsing, and fallback-friendly error reporting.
- Create `GDIProject/Games/Core/GameConfig.h/.cpp`: compiled defaults plus loading from `Resource/Config/*.csv`.
- Create `GDIProject/Games/Core/GameTypes.h`: shared `EMatchMode`, `EPlayerSlot`, `ETurnPhase`, `EGameCommandType`, lightweight unit/tile/state structs.
- Create `GDIProject/Games/Core/GameCommand.h/.cpp`: command parsing/serialization and simple validation helpers.
- Create `GDIProject/Games/Core/AutoMove.h/.cpp`: deterministic unfavorable timeout move scoring.
- Create `GDIProject/Games/Core/StateSerializer.h/.cpp`: deterministic payload serialization and parsing.
- Create `GDIProject/Games/Network/NetworkSession.h/.cpp`: non-blocking Winsock host/client connection, line receive/send, status tracking.
- Create `GDIProject/Games/Core/MatchSettings.h/.cpp`: one shared selected-mode object set by the menu and consumed by `UPlayScene`.
- Modify `GDIProject/Games/Character/PlayerCharacter.*`: add Scout, stable id, slot, config-driven stat application.
- Modify `GDIProject/Games/Tile/Tile.*`: route tile names/move costs through `GameConfig`.
- Modify `GDIProject/Games/Player/PlayerController.*`: spawn Warrior/Archer/Scout for a given slot, expose command-style move/attack/skip helpers.
- Modify `GDIProject/Games/Player/AIPlayer.*`: produce commands for AI units instead of direct ad-hoc behavior.
- Modify `GDIProject/Games/Manager/TurnManager.*`: slot-oriented turn model, PvP timer, AI resolution, host/client command flow.
- Modify `GDIProject/Games/Scene/MenuScene.*` and `GDIProject/Games/UI/MenuScene_StartGuide.*`: three game-mode buttons, IP entry, status text.
- Modify `GDIProject/Games/Scene/PlayScene.*` and `GDIProject/Games/UI/PlayScene_Widget.*`: show active player/timer, route next-turn through turn manager, keep random tile pressure configurable.
- Modify `GDIProject/Games/Games.vcxproj` and `.filters`: include new source/header files.
- Modify `GDIProject/GDIProject.sln`: add `Tests` project.
- Create `GDIProject/Tests/GDIProjectCoreTests.vcxproj` plus `GDIProject/Tests/Main.cpp`: console tests for core logic.
- Create `GDIProject/Resource/Config/GameRules.csv`, `UnitStats.csv`, `TileRules.csv`, `NetworkRules.csv`.
- Create `docs/LAN_MULTIPLAYER.md`: user-facing host/client instructions and tuning notes.
- Create `tools/build-release.ps1`: Release|x64 build and dist packaging.

## Task 1: Core Tests Project

**Files:**
- Create: `GDIProject/Tests/GDIProjectCoreTests.vcxproj`
- Create: `GDIProject/Tests/GDIProjectCoreTests.vcxproj.filters`
- Create: `GDIProject/Tests/Main.cpp`
- Modify: `GDIProject/GDIProject.sln`

- [ ] **Step 1: Write the failing tests**

Add tests that include not-yet-existing headers and assert the desired API:

```cpp
#include "../Games/Core/GameConfig.h"
#include "../Games/Core/GameCommand.h"
#include "../Games/Core/AutoMove.h"
#include "../Games/Core/StateSerializer.h"

static void TestDefaultsContainScout();
static void TestCommandRoundTrip();
static void TestAutoMoveChoosesWorstReachableTile();
static void TestStateSerializationOrdersRecords();
```

- [ ] **Step 2: Run the test build and verify it fails**

Run:

```powershell
& "$msbuild" GDIProject/GDIProject.sln /t:GDIProjectCoreTests /p:Configuration=Debug /p:Platform=x64
```

Expected: compile failure because `GDIProject/Games/Core/*.h` does not exist.

- [ ] **Step 3: Add the console test project**

Create a Visual Studio C++ console project that compiles only the core `.cpp` files and `Main.cpp`, with `AdditionalIncludeDirectories` set to `$(SolutionDir)\Games;$(SolutionDir)\Engine`.

- [ ] **Step 4: Run the test build again**

Expected after Task 2 implementation: `GDIProjectCoreTests.exe` builds and returns exit code `0`.

## Task 2: CSV Config And Shared Types

**Files:**
- Create: `GDIProject/Games/Core/CsvTable.h`
- Create: `GDIProject/Games/Core/CsvTable.cpp`
- Create: `GDIProject/Games/Core/GameTypes.h`
- Create: `GDIProject/Games/Core/GameConfig.h`
- Create: `GDIProject/Games/Core/GameConfig.cpp`
- Create: `GDIProject/Resource/Config/GameRules.csv`
- Create: `GDIProject/Resource/Config/UnitStats.csv`
- Create: `GDIProject/Resource/Config/TileRules.csv`
- Create: `GDIProject/Resource/Config/NetworkRules.csv`

- [ ] **Step 1: Write failing config tests**

Assert:

```cpp
const GameConfig defaults = GameConfig::Defaults();
Require(defaults.GetUnit(EUnitType::Scout).DisplayName == L"정찰병", "Scout display name");
Require(defaults.GetUnit(EUnitType::Warrior).AttackDamage == 20, "Warrior damage");
Require(defaults.GetTile(ETileType::Mountain).MoveCost == 4, "Mountain move cost");
Require(defaults.GameRules.PvpTurnSeconds == 30, "PvP timer default");
```

- [ ] **Step 2: Verify RED**

Build tests. Expected: failure because `GameConfig` is undefined.

- [ ] **Step 3: Implement config defaults and CSV loading**

Implement exact type names from the tests:

```cpp
enum class EMatchMode { SinglePlayer, MultiplayerHost, MultiplayerClient };
enum class EPlayerSlot { Player1, Player2, None };
enum class ETurnPhase { WaitingForConnection, Player1Turn, Player2Turn, ResolvingAI, GameOver };
enum class EGameCommandType { Move, Attack, Skip, EndTurn, Invalid };
```

`GameConfig::LoadFromResource()` reads `../Resource/Config/*.csv` and falls back to `Defaults()` if any file is unavailable.

- [ ] **Step 4: Verify GREEN**

Run the console test executable. Expected: all config assertions pass.

## Task 3: Commands, Serialization, And Auto-Move

**Files:**
- Create: `GDIProject/Games/Core/GameCommand.h`
- Create: `GDIProject/Games/Core/GameCommand.cpp`
- Create: `GDIProject/Games/Core/AutoMove.h`
- Create: `GDIProject/Games/Core/AutoMove.cpp`
- Create: `GDIProject/Games/Core/StateSerializer.h`
- Create: `GDIProject/Games/Core/StateSerializer.cpp`

- [ ] **Step 1: Write failing command and state tests**

Assert:

```cpp
GameCommand cmd = GameCommand::Move(3, EPlayerSlot::Player2, "P2_Archer_0", 4, 8);
Require(SerializeCommand(cmd) == "COMMAND|turn=3|slot=Player2|type=Move|unit=P2_Archer_0|row=4|col=8", "Move serialize");
Require(ParseCommand(SerializeCommand(cmd)).Command.UnitId == "P2_Archer_0", "Move parse");
```

Assert auto-move chooses the highest score with row/col ascending tie break.

- [ ] **Step 2: Verify RED**

Expected: compile failure because command/serializer functions do not exist.

- [ ] **Step 3: Implement parser, serializer, and scorer**

Implement `ParseCommand`, `SerializeCommand`, `SerializeState`, `ParseState`, and `ChooseUnfavorableAutoMove` exactly as declared in the headers. Use ASCII protocol strings and deterministic sorting.

- [ ] **Step 4: Verify GREEN**

Expected: command, auto-move, and state serialization tests pass.

## Task 4: Runtime Unit/Tile Integration

**Files:**
- Modify: `GDIProject/Games/Character/PlayerCharacter.h`
- Modify: `GDIProject/Games/Character/PlayerCharacter.cpp`
- Modify: `GDIProject/Games/Tile/Tile.h`
- Modify: `GDIProject/Games/Tile/Tile.cpp`
- Modify: `GDIProject/Games/Player/PlayerController.h`
- Modify: `GDIProject/Games/Player/PlayerController.cpp`

- [ ] **Step 1: Write failing integration test coverage**

Add tests that call `GameConfig::Defaults().ApplyToUnitRecord(...)` and verify Warrior, Archer, Scout values.

- [ ] **Step 2: Verify RED**

Expected: failure because Scout and config stat application are not yet wired.

- [ ] **Step 3: Implement runtime integration**

Add `Scout` to `EUnitType`, add `StableId`, `OwnerSlot`, `CanMelee`, `CanRanged`, route `SetUnitType` through `GameConfig::Defaults()`, and add controller methods `InitializeForSlot`, `SpawnStartingUnits`, `TryMoveUnitToIndex`, `TryAttackUnit`, `HasLivingUnits`, and `GetLivingUnits`.

- [ ] **Step 4: Verify GREEN**

Expected: tests pass and existing `Games` project compiles.

## Task 5: Turn Manager, AI, And Play Scene

**Files:**
- Modify: `GDIProject/Games/Manager/TurnManager.h`
- Modify: `GDIProject/Games/Manager/TurnManager.cpp`
- Modify: `GDIProject/Games/Player/AIPlayer.h`
- Modify: `GDIProject/Games/Player/AIPlayer.cpp`
- Modify: `GDIProject/Games/Scene/PlayScene.h`
- Modify: `GDIProject/Games/Scene/PlayScene.cpp`
- Modify: `GDIProject/Games/UI/PlayScene_Widget.h`
- Modify: `GDIProject/Games/UI/PlayScene_Widget.cpp`

- [ ] **Step 1: Write failing command validator tests**

Assert out-of-turn commands are rejected and timeout auto-move returns deterministic move commands for units with remaining action.

- [ ] **Step 2: Verify RED**

Expected: validator functions missing.

- [ ] **Step 3: Implement slot turns and AI**

`TurnManager` owns player 1, player 2, optional AI, active phase, PvP timer, and networking hooks. Single-player resolves AI immediately after Player1 ends turn. Multiplayer alternates one timed turn at a time.

- [ ] **Step 4: Verify GREEN**

Expected: validator tests pass and `Games` compiles.

## Task 6: Menu And LAN Networking

**Files:**
- Create: `GDIProject/Games/Core/MatchSettings.h`
- Create: `GDIProject/Games/Core/MatchSettings.cpp`
- Create: `GDIProject/Games/Network/NetworkSession.h`
- Create: `GDIProject/Games/Network/NetworkSession.cpp`
- Modify: `GDIProject/Games/Scene/MenuScene.h`
- Modify: `GDIProject/Games/Scene/MenuScene.cpp`
- Modify: `GDIProject/Games/UI/MenuScene_StartGuide.h`
- Modify: `GDIProject/Games/UI/MenuScene_StartGuide.cpp`

- [ ] **Step 1: Write failing network parser tests**

Assert `NetworkSession` protocol helpers can parse `HELLO`, `WELCOME`, `START`, `STATE`, `ERROR`, and `DISCONNECT` lines without opening sockets.

- [ ] **Step 2: Verify RED**

Expected: protocol helper declarations missing.

- [ ] **Step 3: Implement mode selection and Winsock session**

Menu has `싱글플레이하기`, `멀티플레이접속하기`, `멀티플레이방만들기`, an IP text display, status text, and exit. `NetworkSession` supports host listen, client connect, non-blocking poll, send line, receive lines, and shutdown.

- [ ] **Step 4: Verify GREEN**

Expected: parser tests pass and `Games` links with `Ws2_32.lib`.

## Task 7: Build, Docs, And Release Packaging

**Files:**
- Create: `docs/LAN_MULTIPLAYER.md`
- Create: `tools/build-release.ps1`
- Modify: `GDIProject/Games/Games.vcxproj`
- Modify: `GDIProject/Games/Games.vcxproj.filters`
- Modify: `GDIProject/GDIProject.sln`

- [ ] **Step 1: Write release script smoke check**

Run:

```powershell
Test-Path .\tools\build-release.ps1
```

Expected before script creation: `False`.

- [ ] **Step 2: Create script and docs**

`tools/build-release.ps1` locates MSBuild via PATH or `vswhere`, builds `Release|x64`, copies the executable and `Resource` folder to `dist/GDIProject`, and leaves CSV files editable.

- [ ] **Step 3: Verify build and tests**

Run:

```powershell
& .\tools\build-release.ps1
& .\GDIProject\x64\Debug\GDIProjectCoreTests.exe
```

Expected: release package exists and tests pass.

## Self-Review

- Spec coverage: menu mode selection, Warrior/Archer/Scout starts, mirrored AI roster, PvP one-turn alternation, 30-second timeout, deterministic unfavorable auto-move, LAN/IP host/client, CSV tuning, docs, release script, and tests are mapped to Tasks 1-7.
- Completion-marker scan: no unfinished-marker words or undefined stand-in commands remain.
- Type consistency: shared enums are centralized in `GameTypes.h`; command, config, turn manager, network, and tests use the same names.
