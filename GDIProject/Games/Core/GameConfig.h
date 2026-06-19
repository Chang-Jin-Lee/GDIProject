#pragma once

#include "GameTypes.h"

#include <map>
#include <string>

class GameConfig
{
public:
	static GameConfig Defaults();
	static GameConfig LoadFromResource();
	static GameConfig LoadFromDirectory(const std::wstring& resourceConfigDir, bool* usedFallback = nullptr);

	const UnitStats& GetUnit(EUnitType type) const;
	const TileStats& GetTile(ETileType type) const;

	bool TryApplyCsvDirectory(const std::wstring& resourceConfigDir);

	GameRulesData GameRules;
	NetworkRulesData NetworkRules;

private:
	std::map<EUnitType, UnitStats> m_units;
	std::map<ETileType, TileStats> m_tiles;
};
