#pragma once

#include "GameConfig.h"

struct AutoMoveRequest
{
	int UnitRow = 0;
	int UnitCol = 0;
	int RemainingAction = 0;
	int NearestEnemyRow = 0;
	int NearestEnemyCol = 0;
	int TargetRow = 0;
	int TargetCol = 0;
	std::vector<std::vector<TerrainTile>> Tiles;
};

struct AutoMoveChoice
{
	bool HasMove = false;
	int Row = -1;
	int Col = -1;
	int Cost = 0;
	int Score = 0;
};

AutoMoveChoice ChooseUnfavorableAutoMove(const AutoMoveRequest& request, const GameConfig& config);
