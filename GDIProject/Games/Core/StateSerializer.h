#pragma once

#include "GameTypes.h"

#include <string>

struct StateParseResult
{
	bool Success = false;
	GameStateSnapshot Snapshot;
	std::string Error;
};

std::string SerializeState(const GameStateSnapshot& snapshot);
StateParseResult ParseState(const std::string& line);
