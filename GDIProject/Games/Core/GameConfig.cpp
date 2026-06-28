#include "GameConfig.h"

#include "CsvTable.h"

#include <cstdlib>
#include <sstream>

namespace
{
std::wstring WidenUtf8(const std::string& value)
{
	return std::wstring(value.begin(), value.end());
}

int ToInt(const std::string& text, int fallback)
{
	char* end = nullptr;
	const long value = std::strtol(text.c_str(), &end, 10);
	return end && *end == '\0' ? static_cast<int>(value) : fallback;
}

bool ToBool(const std::string& text, bool fallback)
{
	if (text == "1" || text == "true" || text == "True")
	{
		return true;
	}
	if (text == "0" || text == "false" || text == "False")
	{
		return false;
	}
	return fallback;
}

std::vector<EUnitType> ParseStartingUnits(const std::string& text)
{
	std::vector<EUnitType> result;
	std::stringstream stream(text);
	std::string token;
	while (std::getline(stream, token, '|'))
	{
		EUnitType type = EUnitType::Settler;
		if (TryParseUnitType(CsvTable::Trim(token), type))
		{
			result.push_back(type);
		}
	}
	return result.empty() ? std::vector<EUnitType>{ EUnitType::Warrior, EUnitType::Archer, EUnitType::Scout } : result;
}

template <typename TKey, typename TValue>
const TValue& FindOrFirst(const std::map<TKey, TValue>& values, TKey key)
{
	const auto it = values.find(key);
	if (it != values.end())
	{
		return it->second;
	}
	return values.begin()->second;
}
}

const char* ToString(EUnitType type)
{
	switch (type)
	{
	case EUnitType::Settler: return "Settler";
	case EUnitType::Warrior: return "Warrior";
	case EUnitType::Archer: return "Archer";
	case EUnitType::Scout: return "Scout";
	default: return "Unknown";
	}
}

const char* ToString(ETileType type)
{
	switch (type)
	{
	case ETileType::Desert: return "Desert";
	case ETileType::Grassland: return "Grassland";
	case ETileType::Hills: return "Hills";
	case ETileType::Plain: return "Plain";
	case ETileType::Mountain: return "Mountain";
	case ETileType::Capital: return "Capital";
	default: return "Unknown";
	}
}

const char* ToString(EPlayerSlot slot)
{
	switch (slot)
	{
	case EPlayerSlot::Player1: return "Player1";
	case EPlayerSlot::Player2: return "Player2";
	default: return "None";
	}
}

const char* ToString(EGameCommandType type)
{
	switch (type)
	{
	case EGameCommandType::Move: return "Move";
	case EGameCommandType::Attack: return "Attack";
	case EGameCommandType::Skip: return "Skip";
	case EGameCommandType::EndTurn: return "EndTurn";
	default: return "Invalid";
	}
}

bool TryParseUnitType(const std::string& text, EUnitType& outType)
{
	if (text == "Settler") { outType = EUnitType::Settler; return true; }
	if (text == "Warrior") { outType = EUnitType::Warrior; return true; }
	if (text == "Archer") { outType = EUnitType::Archer; return true; }
	if (text == "Scout") { outType = EUnitType::Scout; return true; }
	return false;
}

bool TryParseTileType(const std::string& text, ETileType& outType)
{
	if (text == "Desert") { outType = ETileType::Desert; return true; }
	if (text == "Grassland") { outType = ETileType::Grassland; return true; }
	if (text == "Hills") { outType = ETileType::Hills; return true; }
	if (text == "Plain") { outType = ETileType::Plain; return true; }
	if (text == "Mountain") { outType = ETileType::Mountain; return true; }
	if (text == "Capital") { outType = ETileType::Capital; return true; }
	return false;
}

bool TryParsePlayerSlot(const std::string& text, EPlayerSlot& outSlot)
{
	if (text == "Player1") { outSlot = EPlayerSlot::Player1; return true; }
	if (text == "Player2") { outSlot = EPlayerSlot::Player2; return true; }
	if (text == "None") { outSlot = EPlayerSlot::None; return true; }
	return false;
}

bool TryParseCommandType(const std::string& text, EGameCommandType& outType)
{
	if (text == "Move") { outType = EGameCommandType::Move; return true; }
	if (text == "Attack") { outType = EGameCommandType::Attack; return true; }
	if (text == "Skip") { outType = EGameCommandType::Skip; return true; }
	if (text == "EndTurn") { outType = EGameCommandType::EndTurn; return true; }
	return false;
}

std::wstring UnitTypeDisplayName(EUnitType type)
{
	return GameConfig::Defaults().GetUnit(type).DisplayName;
}

std::wstring TileTypeDisplayName(ETileType type)
{
	return GameConfig::Defaults().GetTile(type).DisplayName;
}

GameConfig GameConfig::Defaults()
{
	GameConfig config;
	config.GameRules.MaxTurn = 30;
	config.GameRules.PvpTurnSeconds = 30;
	config.GameRules.RandomTilesRemovedPerTurn = 5;
	config.GameRules.StopRandomTileRemovalAfterTurn = 25;
	config.GameRules.StartingUnits = { EUnitType::Warrior, EUnitType::Archer, EUnitType::Scout };

	config.NetworkRules.Port = 43117;
	config.NetworkRules.ConnectTimeoutMs = 5000;
	config.NetworkRules.ReceiveBufferBytes = 4096;
	config.NetworkRules.StateSyncEveryTurn = true;

	config.m_units[EUnitType::Settler] = UnitStats{ EUnitType::Settler, L"\uac1c\ucc99\uc790", 50, 0, 2, 1, 4, false, false };
	config.m_units[EUnitType::Warrior] = UnitStats{ EUnitType::Warrior, L"\uc804\uc0ac", 100, 20, 1, 1, 3, true, false };
	config.m_units[EUnitType::Archer] = UnitStats{ EUnitType::Archer, L"\uc6d0\uac70\ub9ac\uad81\uc218", 70, 15, 1, 2, 3, false, true };
	config.m_units[EUnitType::Scout] = UnitStats{ EUnitType::Scout, L"\uc815\ucc30\ubcd1", 60, 8, 2, 1, 5, true, false };

	config.m_tiles[ETileType::Desert] = TileStats{ ETileType::Desert, L"\uc0ac\ub9c9", 3, true, false };
	config.m_tiles[ETileType::Grassland] = TileStats{ ETileType::Grassland, L"\ucd08\uc6d0", 1, true, false };
	config.m_tiles[ETileType::Hills] = TileStats{ ETileType::Hills, L"\uc5b8\ub355", 2, true, false };
	config.m_tiles[ETileType::Plain] = TileStats{ ETileType::Plain, L"\ud3c9\uc57c", 1, true, false };
	config.m_tiles[ETileType::Mountain] = TileStats{ ETileType::Mountain, L"\uc0b0", 4, true, false };
	config.m_tiles[ETileType::Capital] = TileStats{ ETileType::Capital, L"\uc218\ub3c4", 1, false, true };

	return config;
}

GameConfig GameConfig::LoadFromResource()
{
	return LoadFromDirectory(L"../Resource/Config");
}

GameConfig GameConfig::LoadFromDirectory(const std::wstring& resourceConfigDir, bool* usedFallback)
{
	GameConfig config = Defaults();
	const bool loaded = config.TryApplyCsvDirectory(resourceConfigDir);
	if (usedFallback)
	{
		*usedFallback = !loaded;
	}
	return config;
}

const UnitStats& GameConfig::GetUnit(EUnitType type) const
{
	return FindOrFirst(m_units, type);
}

const TileStats& GameConfig::GetTile(ETileType type) const
{
	return FindOrFirst(m_tiles, type);
}

bool GameConfig::TryApplyCsvDirectory(const std::wstring& resourceConfigDir)
{
	bool allLoaded = true;

	CsvTable gameRules;
	if (gameRules.LoadFromFile(resourceConfigDir + L"/GameRules.csv"))
	{
		for (const CsvRow& row : gameRules.Rows())
		{
			const std::string key = row.Get("Key");
			const std::string value = row.Get("Value");
			if (key == "MaxTurn") GameRules.MaxTurn = ToInt(value, GameRules.MaxTurn);
			else if (key == "PvpTurnSeconds") GameRules.PvpTurnSeconds = ToInt(value, GameRules.PvpTurnSeconds);
			else if (key == "RandomTilesRemovedPerTurn") GameRules.RandomTilesRemovedPerTurn = ToInt(value, GameRules.RandomTilesRemovedPerTurn);
			else if (key == "StopRandomTileRemovalAfterTurn") GameRules.StopRandomTileRemovalAfterTurn = ToInt(value, GameRules.StopRandomTileRemovalAfterTurn);
			else if (key == "StartingUnits") GameRules.StartingUnits = ParseStartingUnits(value);
		}
	}
	else
	{
		allLoaded = false;
	}

	CsvTable unitStats;
	if (unitStats.LoadFromFile(resourceConfigDir + L"/UnitStats.csv"))
	{
		for (const CsvRow& row : unitStats.Rows())
		{
			EUnitType type = EUnitType::Settler;
			if (!TryParseUnitType(row.Get("UnitType"), type))
			{
				allLoaded = false;
				continue;
			}

			UnitStats current = GetUnit(type);
			current.Type = type;
			current.DisplayName = WidenUtf8(row.Get("DisplayName", std::string(ToString(type))));
			current.Health = ToInt(row.Get("Health"), current.Health);
			current.AttackDamage = ToInt(row.Get("AttackDamage"), current.AttackDamage);
			current.MoveRange = ToInt(row.Get("MoveRange"), current.MoveRange);
			current.AttackRange = ToInt(row.Get("AttackRange"), current.AttackRange);
			current.ActionMaxCount = ToInt(row.Get("ActionMaxCount"), current.ActionMaxCount);
			current.CanMelee = ToBool(row.Get("CanMelee"), current.CanMelee);
			current.CanRanged = ToBool(row.Get("CanRanged"), current.CanRanged);
			m_units[type] = current;
		}
	}
	else
	{
		allLoaded = false;
	}

	CsvTable tileRules;
	if (tileRules.LoadFromFile(resourceConfigDir + L"/TileRules.csv"))
	{
		for (const CsvRow& row : tileRules.Rows())
		{
			ETileType type = ETileType::Plain;
			if (!TryParseTileType(row.Get("TileType"), type))
			{
				allLoaded = false;
				continue;
			}

			TileStats current = GetTile(type);
			current.Type = type;
			current.DisplayName = WidenUtf8(row.Get("DisplayName", std::string(ToString(type))));
			current.MoveCost = ToInt(row.Get("MoveCost"), current.MoveCost);
			current.CanDisappear = ToBool(row.Get("CanDisappear"), current.CanDisappear);
			current.CanSpawnCapital = ToBool(row.Get("CanSpawnCapital"), current.CanSpawnCapital);
			m_tiles[type] = current;
		}
	}
	else
	{
		allLoaded = false;
	}

	CsvTable networkRules;
	if (networkRules.LoadFromFile(resourceConfigDir + L"/NetworkRules.csv"))
	{
		for (const CsvRow& row : networkRules.Rows())
		{
			const std::string key = row.Get("Key");
			const std::string value = row.Get("Value");
			if (key == "Port") NetworkRules.Port = ToInt(value, NetworkRules.Port);
			else if (key == "ConnectTimeoutMs") NetworkRules.ConnectTimeoutMs = ToInt(value, NetworkRules.ConnectTimeoutMs);
			else if (key == "ReceiveBufferBytes") NetworkRules.ReceiveBufferBytes = ToInt(value, NetworkRules.ReceiveBufferBytes);
			else if (key == "StateSyncEveryTurn") NetworkRules.StateSyncEveryTurn = ToBool(value, NetworkRules.StateSyncEveryTurn);
		}
	}
	else
	{
		allLoaded = false;
	}

	return allLoaded;
}
