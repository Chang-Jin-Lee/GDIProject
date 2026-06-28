#pragma once

#include "GameTypes.h"

#include <string>

struct GameCommand
{
	int Turn = 0;
	EPlayerSlot Slot = EPlayerSlot::None;
	EGameCommandType Type = EGameCommandType::Invalid;
	std::string UnitId;
	std::string TargetUnitId;
	int Row = -1;
	int Col = -1;

	static GameCommand Move(int turn, EPlayerSlot slot, const std::string& unitId, int row, int col);
	static GameCommand Attack(int turn, EPlayerSlot slot, const std::string& unitId, const std::string& targetUnitId);
	static GameCommand Skip(int turn, EPlayerSlot slot, const std::string& unitId);
	static GameCommand EndTurn(int turn, EPlayerSlot slot);
};

struct CommandParseResult
{
	bool Success = false;
	GameCommand Command;
	std::string Error;
};

std::string SerializeCommand(const GameCommand& command);
CommandParseResult ParseCommand(const std::string& line);
