# LAN Multiplayer Guide

## Start Modes

- `싱글플레이하기`: starts a local match against AI.
- `멀티플레이접속하기`: enables IP entry. Type the host LAN IPv4 address and press Enter.
- `멀티플레이방만들기`: starts a host on the configured port and waits for one client.

The default port is `43117` and can be changed in `GDIProject/Resource/Config/NetworkRules.csv`.

## LAN Flow

1. On the host PC, choose `멀티플레이방만들기`.
2. On the client PC, choose `멀티플레이접속하기`.
3. Type the host PC's LAN IPv4 address, for example `192.168.0.12`.
4. Press Enter to connect.

The game uses Winsock TCP. The render loop polls sockets in non-blocking mode, so a failed or slow connection should not freeze the window.

## Gameplay Sync

After the host sends `START`, both peers exchange line-based `COMMAND` messages over TCP.

- Right-click an empty reachable tile to move the selected unit.
- Right-click an enemy unit to attack if the selected unit is in range.
- Skip, end-turn, and timeout auto-move results are also sent as commands.
- Only the active local player applies the 30-second timeout penalty; the other peer waits for the resulting move/end-turn commands.

## Tuning CSV Files

Runtime values are stored in `GDIProject/Resource/Config`:

- `GameRules.csv`: max turn, PvP turn seconds, random tile removal, starting roster.
- `UnitStats.csv`: Warrior, Archer, Scout stats.
- `TileRules.csv`: tile move cost and disappearance rules.
- `NetworkRules.csv`: LAN port and buffer values.

If a CSV file is missing or malformed, compiled defaults are used.

## Release Build

Run from the repository root:

```powershell
.\tools\build-release.ps1
```

The script builds `Release|x64` and creates `dist/GDIProject` with `Games.exe` and the editable `Resource` folder.
