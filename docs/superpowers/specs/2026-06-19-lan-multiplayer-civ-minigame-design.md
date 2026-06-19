# LAN Multiplayer Civilization Minigame Design

## Goal

Expand the existing GDI point-and-click turn game into a small Civilization-like tactical minigame with single-player AI, LAN/IP multiplayer, CSV-managed balance values, and a releasable Windows build.

## Current Project Context

The project is a Visual Studio C++17 Win32/GDI solution. The game code lives under `GDIProject/Games`, with reusable engine code under `GDIProject/Engine`.

The current playable loop is centered on:

- `Games/Scene/MenuScene.*`: title menu and scene transition.
- `Games/Scene/PlayScene.*`: tile creation, UI, next-turn button, victory checks, random tile deletion.
- `Games/Manager/TurnManager.*`: current player turn update.
- `Games/Player/PlayerController.*`: unit spawn, selection, movement, action count handling.
- `Games/Player/AIPlayer.*`: partially sketched AI behavior, not integrated into `TurnManager`.
- `Games/Character/PlayerCharacter.*`: unit type, hardcoded stats, animation, attack helper.
- `Games/Tile/Tile.*`: tile type, movement cost, hex-grid pathfinding, reachable tile highlighting.
- `Engine/Runtime/Core/FIleHelper.h`: existing resource file loading helper.

The implementation should preserve the existing GDI renderer, object model, scene system, camera behavior, and custom UI component style.

## Game Modes

The title menu will provide three buttons:

- `싱글플레이하기`: starts a local game against AI.
- `멀티플레이접속하기`: opens an IP input UI and connects to a host.
- `멀티플레이방만들기`: starts a host/server on a fixed LAN port and waits for one client.

The default LAN port is `43117`. The port value is configurable through `Resource/Config/NetworkRules.csv`.

Single-player and multiplayer both use the same `PlayScene`, `TurnManager`, unit rules, tile rules, and victory checks. The difference is the controller assigned to player slot 2:

- Single-player: slot 1 is local human, slot 2 is AI.
- Multiplayer host: slot 1 is local human and authoritative server, slot 2 is remote client.
- Multiplayer client: slot 1 is remote host, slot 2 is local human.

## Unit Roster

Each side starts with exactly three units:

- `Warrior`: melee combat unit. It attacks adjacent enemy units.
- `Archer`: ranged combat unit. It can attack enemy units within its configured attack range.
- `Scout`: fast exploration unit. It has higher movement/action capacity and weaker combat stats.

The existing `Settler` enum value may remain for compatibility while implementation migrates default starts and UI labels to `Scout`. It is not part of the default minigame roster.

Civilization-inspired default values are stored in CSV and may be tuned without recompiling. The initial defaults are:

```csv
UnitType,DisplayName,Health,AttackDamage,MoveRange,AttackRange,ActionMaxCount,CanMelee,CanRanged
Warrior,전사,100,20,1,1,3,1,0
Archer,원거리궁수,70,15,1,2,3,0,1
Scout,정찰병,60,8,2,1,5,1,0
```

`ActionMaxCount` is the practical per-turn movement/action budget used by the current code. `MoveRange` remains available as a semantic stat for UI and future rules.

## CSV Configuration

Create `Resource/Config` with the following files:

```text
Resource/Config/GameRules.csv
Resource/Config/UnitStats.csv
Resource/Config/TileRules.csv
Resource/Config/NetworkRules.csv
```

`GameRules.csv`:

```csv
Key,Value
MaxTurn,30
PvpTurnSeconds,30
RandomTilesRemovedPerTurn,5
StopRandomTileRemovalAfterTurn,25
StartingUnits,Warrior|Archer|Scout
```

`UnitStats.csv` uses the schema from the Unit Roster section.

`TileRules.csv`:

```csv
TileType,DisplayName,MoveCost,CanDisappear,CanSpawnCapital
Desert,사막,3,1,0
Grassland,초원,1,1,0
Hills,언덕,2,1,0
Plain,평야,1,1,0
Mountain,산,4,1,0
Capital,수도,1,0,1
```

`NetworkRules.csv`:

```csv
Key,Value
Port,43117
ConnectTimeoutMs,5000
ReceiveBufferBytes,4096
StateSyncEveryTurn,1
```

CSV parsing should be small and deterministic: split by comma, trim leading/trailing whitespace, ignore empty lines, and treat the first row as a header. If a file is missing or malformed, the game falls back to compiled defaults and shows a popup message in the current scene.

## Menu And Multiplayer UI

`UMenuscene_StartGuide` will replace the current start/end layout with:

- Single-player button.
- Multiplayer connect button.
- Multiplayer host button.
- IP text field area for connect mode.
- Status text for host/client state.
- Exit button remains at the lower-left corner.

The existing UI system has button and text components but no general text input component. The first implementation will support IP entry through keyboard input captured by the menu scene, displayed in a text component. It accepts digits, dots, backspace, and enter. The connect button or enter key attempts the connection.

Host mode shows local IP guidance as status text if it can be discovered; otherwise it shows the port and tells the player to share the machine's LAN IP.

## Turn And Command Model

Replace `ETurnState` with a slot-oriented model:

```cpp
enum class EMatchMode { SinglePlayer, MultiplayerHost, MultiplayerClient };
enum class EPlayerSlot { Player1, Player2 };
enum class ETurnPhase { WaitingForConnection, Player1Turn, Player2Turn, ResolvingAI, GameOver };
```

Each turn has:

- Active player slot.
- Remaining seconds for PvP turns.
- Per-unit remaining action counts.
- A list of submitted local commands.

Supported commands:

- Select unit locally for UI only.
- Move unit to tile.
- Attack target unit.
- Skip selected unit.
- End turn.
- Auto-move remaining units when timer expires.

Only gameplay-changing commands are sent over the network. Selection and camera movement stay local.

## PvP Timer And Penalty Auto-Move

In multiplayer, each player has `PvpTurnSeconds` seconds, default `30`.

If the active player does not finish before the timer reaches zero, the authoritative host moves each remaining-action unit automatically. The movement is intentionally bad for that player:

- Prefer reachable tiles farther from the nearest enemy if the unit can attack.
- Prefer tiles farther from enemy capital/victory targets.
- Prefer higher movement-cost tiles when distance scores are tied.
- Avoid illegal, invisible, occupied, and capital tiles.
- If no legal move exists, mark the unit skipped.

The score is deterministic and evaluated by the host:

```text
score = enemyDistance * 100 + targetDistance * 50 + tileMoveCost * 10 - remainingActionAfterMove
```

The highest score is selected. Ties use row, then column, ascending order so both machines can reproduce the choice when replaying state.

## AI Design

AI uses the same command model as a remote player. During `ResolvingAI`, the AI evaluates its units and submits commands to the host/local turn manager.

Initial AI behavior:

- Warriors move toward the nearest enemy and melee attack when adjacent.
- Archers move toward nearest enemy until in range, then ranged attack.
- Scouts move toward capitals or enemy units, prioritizing exploration and spacing.
- AI ends its turn automatically after all units have no useful action or are skipped.

The AI should not call raw movement helpers directly when a command path exists. It should produce the same command objects used by LAN clients.

## Networking Architecture

Use Winsock TCP with one host and one client.

The host is authoritative:

- Generates map and initial unit placement.
- Owns the random seed.
- Validates all gameplay commands.
- Advances turns and timers.
- Applies random tile deletion.
- Broadcasts state after accepted commands and at each turn transition.

The client:

- Connects to host IP and port.
- Sends local commands for player slot 2.
- Applies host state snapshots.
- Does not decide random outcomes.

Network messages are line-delimited UTF-8 or ASCII text. Each line begins with a message name and uses key-value fields separated by `|`.

Handshake:

```text
HELLO|version=1|role=client
WELCOME|slot=Player2|seed=123456|mapHash=8F6A21C0
START|mode=Multiplayer|active=Player1
```

Commands:

```text
COMMAND|turn=3|slot=Player2|type=Move|unit=Archer_1|row=4|col=8
COMMAND|turn=3|slot=Player2|type=Attack|unit=Archer_1|target=Warrior_1
COMMAND|turn=3|slot=Player2|type=Skip|unit=Scout_1
COMMAND|turn=3|slot=Player2|type=EndTurn
```

State sync:

```text
STATE|turn=4|active=Player1|timer=30|payload=T:0,0,Plain,1;T:0,1,Hills,1;U:P1_Warrior_0,Warrior,0,1,100,3,1;U:P2_Archer_0,Archer,9,13,70,3,1
ERROR|code=InvalidCommand|message=CommandRejectedForInactiveTurn
DISCONNECT|reason=PeerClosed
```

The first implementation serializes `payload` as compact semicolon-separated state records instead of JSON to avoid adding dependencies. The payload includes visible tiles, tile types, units, health, action counts, active player, and game-over state.

## Game State Serialization

State serialization must be deterministic and stable enough for reconnect-free LAN play:

- Tiles serialize in row-major order.
- Units serialize sorted by stable unit id.
- Random tile removal uses the host seed and host state.
- Commands include turn number and player slot.

A stable unit id is assigned at spawn time:

```text
P1_Warrior_0
P1_Archer_0
P1_Scout_0
P2_Warrior_0
P2_Archer_0
P2_Scout_0
```

Display names may be localized, but ids stay ASCII.

## Combat And Victory

Combat is added to the current movement minigame without introducing city production or diplomacy.

Rules:

- Warrior attacks adjacent targets only.
- Archer attacks within `AttackRange`.
- Scout can attack adjacent targets with low damage.
- Dead units are removed from tile occupancy and no longer receive actions.

Victory checks:

- A player wins if all enemy units are dead.
- A player wins if all surviving own units occupy enemy capital tiles at turn check.
- If `MaxTurn` is reached, the side with more surviving units wins.
- If surviving unit counts tie at max turn, lower total damage taken wins.
- If still tied, host wins in multiplayer and player wins in single-player to keep the result deterministic.

The existing random disappearing land pressure stays active using `GameRules.csv`.

## Build And Release

Keep the Visual Studio solution as the primary build system.

Add a release script:

```text
tools/build-release.ps1
```

The script will:

- Locate MSBuild or use `vswhere` when available.
- Build `GDIProject/GDIProject.sln` in `Release|x64`.
- Copy the executable and required `Resource` folder into `dist/GDIProject`.
- Preserve `Resource/Config/*.csv` beside the build so tuning remains editable after packaging.

## Testing Strategy

The existing repository has no dedicated automated test project. Add `GDIProject/Tests/GDIProjectCoreTests.vcxproj`, a console test project that can run without opening the GDI window.

Test targets:

- CSV parsing returns configured unit stats and falls back to defaults on missing files.
- Unit stat application sets Warrior, Archer, and Scout values from `UnitStats.csv`.
- Tile move cost reads from `TileRules.csv`.
- Network message parser accepts valid messages and rejects invalid ones.
- Command validator rejects out-of-turn commands and illegal moves.
- PvP timer auto-move chooses the deterministic unfavorable tile.
- State serialization orders tiles and units deterministically.

Manual verification:

- Single-player starts with three player units and three AI units.
- Host starts waiting on port `43117`.
- Client connects by LAN IP and receives the starting state.
- Player turns alternate one at a time.
- A 30-second timeout auto-moves remaining units.
- Release build launches from `dist/GDIProject`.

## Implementation Boundaries

Included:

- Menu mode selection and IP input.
- Single-player AI opponent.
- LAN host/client over TCP.
- Host-authoritative turn, command, timer, and state sync.
- Warrior, Archer, Scout default roster.
- CSV-managed balance and network values.
- Combat, death, and simple victory logic.
- Release build script.
- Documentation and tests for deterministic core logic.

Not included:

- Internet NAT traversal.
- Matchmaking or server browser.
- Reconnect after disconnect.
- More than two players.
- City production, diplomacy, tech tree, fog of war, or save games.

## Risks And Mitigations

The current gameplay code mixes UI, scene, turn, and movement concerns. The implementation should add small boundary classes instead of a large rewrite:

- `GameConfig` for CSV-loaded rules.
- `MatchSettings` for selected mode and host/client data.
- `NetworkSession` for Winsock host/client behavior.
- `GameCommand` and parser/serializer for network-safe actions.
- Turn manager changes kept focused around player slots and active phase.

Winsock networking can block the render loop if used directly. Socket work uses non-blocking sockets and a small polling layer called once per frame.

The host must remain authoritative to avoid desync from random tile deletion and timer auto-move.
