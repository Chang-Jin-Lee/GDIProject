#pragma once

#include <string>
#include <vector>

enum class EUnitType
{
	Settler,
	Warrior,
	Archer,
	Scout,
	MAX,
};

enum class ETileType
{
	Desert,
	Grassland,
	Hills,
	Plain,
	Mountain,
	Capital,
	MAX,
};

enum class EMatchMode
{
	SinglePlayer,
	MultiplayerHost,
	MultiplayerClient,
};

enum class EPlayerSlot
{
	None,
	Player1,
	Player2,
};

enum class ETurnPhase
{
	WaitingForConnection,
	Player1Turn,
	Player2Turn,
	ResolvingAI,
	GameOver,
};

enum class EGameCommandType
{
	Invalid,
	Move,
	Attack,
	Skip,
	EndTurn,
};

struct UnitStats
{
	EUnitType Type = EUnitType::Settler;
	std::wstring DisplayName = L"";
	int Health = 0;
	int AttackDamage = 0;
	int MoveRange = 0;
	int AttackRange = 0;
	int ActionMaxCount = 0;
	bool CanMelee = false;
	bool CanRanged = false;
};

struct TileStats
{
	ETileType Type = ETileType::Plain;
	std::wstring DisplayName = L"";
	int MoveCost = 1;
	bool CanDisappear = true;
	bool CanSpawnCapital = false;
};

struct GameRulesData
{
	int MaxTurn = 30;
	int PvpTurnSeconds = 30;
	int RandomTilesRemovedPerTurn = 5;
	int StopRandomTileRemovalAfterTurn = 25;
	std::vector<EUnitType> StartingUnits = { EUnitType::Warrior, EUnitType::Archer, EUnitType::Scout };
};

struct NetworkRulesData
{
	int Port = 43117;
	int ConnectTimeoutMs = 5000;
	int ReceiveBufferBytes = 4096;
	bool StateSyncEveryTurn = true;
};

struct TerrainTile
{
	int Row = 0;
	int Col = 0;
	ETileType Type = ETileType::Plain;
	bool Visible = true;
	bool Occupied = false;
	bool Capital = false;
};

struct UnitState
{
	std::string Id;
	EUnitType Type = EUnitType::Warrior;
	EPlayerSlot Slot = EPlayerSlot::None;
	int Row = 0;
	int Col = 0;
	int Health = 0;
	int ActionRemain = 0;
	bool Alive = true;
};

struct GameStateSnapshot
{
	int Turn = 0;
	EPlayerSlot ActiveSlot = EPlayerSlot::Player1;
	int TimerSeconds = 0;
	std::vector<TerrainTile> Tiles;
	std::vector<UnitState> Units;
	bool GameOver = false;
};

const char* ToString(EUnitType type);
const char* ToString(ETileType type);
const char* ToString(EPlayerSlot slot);
const char* ToString(EGameCommandType type);

bool TryParseUnitType(const std::string& text, EUnitType& outType);
bool TryParseTileType(const std::string& text, ETileType& outType);
bool TryParsePlayerSlot(const std::string& text, EPlayerSlot& outSlot);
bool TryParseCommandType(const std::string& text, EGameCommandType& outType);

std::wstring UnitTypeDisplayName(EUnitType type);
std::wstring TileTypeDisplayName(ETileType type);
