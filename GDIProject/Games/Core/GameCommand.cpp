#include "GameCommand.h"

#include "NetworkProtocol.h"

#include <cstdlib>
#include <sstream>

namespace
{
bool TryGetInt(const NetworkLine& line, const std::string& key, int& outValue)
{
	const auto it = line.Fields.find(key);
	if (it == line.Fields.end())
	{
		return false;
	}

	char* end = nullptr;
	const long parsed = std::strtol(it->second.c_str(), &end, 10);
	if (!end || *end != '\0')
	{
		return false;
	}
	outValue = static_cast<int>(parsed);
	return true;
}

std::string GetField(const NetworkLine& line, const std::string& key)
{
	const auto it = line.Fields.find(key);
	return it == line.Fields.end() ? "" : it->second;
}
}

GameCommand GameCommand::Move(int turn, EPlayerSlot slot, const std::string& unitId, int row, int col)
{
	GameCommand command;
	command.Turn = turn;
	command.Slot = slot;
	command.Type = EGameCommandType::Move;
	command.UnitId = unitId;
	command.Row = row;
	command.Col = col;
	return command;
}

GameCommand GameCommand::Attack(int turn, EPlayerSlot slot, const std::string& unitId, const std::string& targetUnitId)
{
	GameCommand command;
	command.Turn = turn;
	command.Slot = slot;
	command.Type = EGameCommandType::Attack;
	command.UnitId = unitId;
	command.TargetUnitId = targetUnitId;
	return command;
}

GameCommand GameCommand::Skip(int turn, EPlayerSlot slot, const std::string& unitId)
{
	GameCommand command;
	command.Turn = turn;
	command.Slot = slot;
	command.Type = EGameCommandType::Skip;
	command.UnitId = unitId;
	return command;
}

GameCommand GameCommand::EndTurn(int turn, EPlayerSlot slot)
{
	GameCommand command;
	command.Turn = turn;
	command.Slot = slot;
	command.Type = EGameCommandType::EndTurn;
	return command;
}

std::string SerializeCommand(const GameCommand& command)
{
	std::ostringstream out;
	out << "COMMAND|turn=" << command.Turn
		<< "|slot=" << ToString(command.Slot)
		<< "|type=" << ToString(command.Type);

	if (!command.UnitId.empty())
	{
		out << "|unit=" << command.UnitId;
	}
	if (command.Type == EGameCommandType::Move)
	{
		out << "|row=" << command.Row << "|col=" << command.Col;
	}
	else if (command.Type == EGameCommandType::Attack)
	{
		out << "|target=" << command.TargetUnitId;
	}

	return out.str();
}

CommandParseResult ParseCommand(const std::string& line)
{
	CommandParseResult result;
	const NetworkLine parsed = ParseNetworkLine(line);
	if (!parsed.Valid || parsed.Name != "COMMAND")
	{
		result.Error = "not a command line";
		return result;
	}

	if (!TryGetInt(parsed, "turn", result.Command.Turn))
	{
		result.Error = "invalid turn";
		return result;
	}

	if (!TryParsePlayerSlot(GetField(parsed, "slot"), result.Command.Slot))
	{
		result.Error = "invalid slot";
		return result;
	}

	if (!TryParseCommandType(GetField(parsed, "type"), result.Command.Type))
	{
		result.Error = "invalid type";
		return result;
	}

	result.Command.UnitId = GetField(parsed, "unit");
	result.Command.TargetUnitId = GetField(parsed, "target");

	if (result.Command.Type == EGameCommandType::Move)
	{
		if (!TryGetInt(parsed, "row", result.Command.Row) || !TryGetInt(parsed, "col", result.Command.Col))
		{
			result.Error = "invalid move destination";
			return result;
		}
		if (result.Command.UnitId.empty())
		{
			result.Error = "missing move unit";
			return result;
		}
	}
	else if (result.Command.Type == EGameCommandType::Attack)
	{
		if (result.Command.UnitId.empty() || result.Command.TargetUnitId.empty())
		{
			result.Error = "missing attack ids";
			return result;
		}
	}

	result.Success = true;
	return result;
}
