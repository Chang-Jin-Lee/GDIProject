#include "StateSerializer.h"

#include "CsvTable.h"
#include "NetworkProtocol.h"

#include <algorithm>
#include <cstdlib>
#include <sstream>

namespace
{
bool ToInt(const std::string& text, int& outValue)
{
	char* end = nullptr;
	const long parsed = std::strtol(text.c_str(), &end, 10);
	if (!end || *end != '\0')
	{
		return false;
	}
	outValue = static_cast<int>(parsed);
	return true;
}

std::vector<std::string> Split(const std::string& text, char delimiter)
{
	std::vector<std::string> values;
	std::stringstream stream(text);
	std::string token;
	while (std::getline(stream, token, delimiter))
	{
		values.push_back(token);
	}
	return values;
}
}

std::string SerializeState(const GameStateSnapshot& snapshot)
{
	std::vector<TerrainTile> tiles = snapshot.Tiles;
	std::sort(tiles.begin(), tiles.end(), [](const TerrainTile& left, const TerrainTile& right)
	{
		if (left.Row != right.Row) return left.Row < right.Row;
		return left.Col < right.Col;
	});

	std::vector<UnitState> units = snapshot.Units;
	std::sort(units.begin(), units.end(), [](const UnitState& left, const UnitState& right)
	{
		return left.Id < right.Id;
	});

	std::ostringstream payload;
	bool first = true;
	for (const TerrainTile& tile : tiles)
	{
		if (!first) payload << ';';
		first = false;
		payload << "T:" << tile.Row << ',' << tile.Col << ',' << ToString(tile.Type) << ',' << (tile.Visible ? 1 : 0);
	}

	for (const UnitState& unit : units)
	{
		if (!first) payload << ';';
		first = false;
		payload << "U:" << unit.Id << ',' << ToString(unit.Type) << ',' << ToString(unit.Slot)
			<< ',' << unit.Row << ',' << unit.Col << ',' << unit.Health << ',' << unit.ActionRemain << ',' << (unit.Alive ? 1 : 0);
	}

	std::ostringstream line;
	line << "STATE|turn=" << snapshot.Turn
		<< "|active=" << ToString(snapshot.ActiveSlot)
		<< "|timer=" << snapshot.TimerSeconds
		<< "|payload=" << payload.str();
	return line.str();
}

StateParseResult ParseState(const std::string& line)
{
	StateParseResult result;
	const NetworkLine parsed = ParseNetworkLine(line);
	if (!parsed.Valid || parsed.Name != "STATE")
	{
		result.Error = "not a state line";
		return result;
	}

	const auto turnIt = parsed.Fields.find("turn");
	const auto activeIt = parsed.Fields.find("active");
	const auto timerIt = parsed.Fields.find("timer");
	const auto payloadIt = parsed.Fields.find("payload");
	if (turnIt == parsed.Fields.end() || activeIt == parsed.Fields.end() || timerIt == parsed.Fields.end() || payloadIt == parsed.Fields.end())
	{
		result.Error = "missing state fields";
		return result;
	}

	if (!ToInt(turnIt->second, result.Snapshot.Turn) || !ToInt(timerIt->second, result.Snapshot.TimerSeconds)
		|| !TryParsePlayerSlot(activeIt->second, result.Snapshot.ActiveSlot))
	{
		result.Error = "invalid state header";
		return result;
	}

	for (const std::string& record : Split(payloadIt->second, ';'))
	{
		if (record.rfind("T:", 0) == 0)
		{
			const std::vector<std::string> fields = Split(record.substr(2), ',');
			if (fields.size() != 4)
			{
				result.Error = "invalid tile record";
				return result;
			}

			TerrainTile tile;
			if (!ToInt(fields[0], tile.Row) || !ToInt(fields[1], tile.Col)
				|| !TryParseTileType(fields[2], tile.Type))
			{
				result.Error = "invalid tile values";
				return result;
			}
			tile.Visible = fields[3] == "1";
			result.Snapshot.Tiles.push_back(tile);
		}
		else if (record.rfind("U:", 0) == 0)
		{
			const std::vector<std::string> fields = Split(record.substr(2), ',');
			if (fields.size() != 8)
			{
				result.Error = "invalid unit record";
				return result;
			}

			UnitState unit;
			unit.Id = fields[0];
			if (!TryParseUnitType(fields[1], unit.Type) || !TryParsePlayerSlot(fields[2], unit.Slot)
				|| !ToInt(fields[3], unit.Row) || !ToInt(fields[4], unit.Col)
				|| !ToInt(fields[5], unit.Health) || !ToInt(fields[6], unit.ActionRemain))
			{
				result.Error = "invalid unit values";
				return result;
			}
			unit.Alive = fields[7] == "1";
			result.Snapshot.Units.push_back(unit);
		}
		else if (!CsvTable::Trim(record).empty())
		{
			result.Error = "unknown state record";
			return result;
		}
	}

	result.Success = true;
	return result;
}
