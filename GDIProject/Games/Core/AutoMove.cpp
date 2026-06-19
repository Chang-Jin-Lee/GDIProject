#include "AutoMove.h"

#include <climits>
#include <cstdlib>
#include <queue>
#include <tuple>

namespace
{
const int evenDy[6] = { 0, -1, -1, 0, 1, 1 };
const int evenDx[6] = { -1, -1, 0, 1, 0, -1 };
const int oddDy[6] = { 0, -1, -1, 0, 1, 1 };
const int oddDx[6] = { -1, 0, 1, 1, 1, 0 };

bool IsValid(const AutoMoveRequest& request, int row, int col)
{
	return row >= 0 && col >= 0 && row < static_cast<int>(request.Tiles.size())
		&& !request.Tiles.empty() && col < static_cast<int>(request.Tiles[row].size());
}

int GridDistance(int rowA, int colA, int rowB, int colB)
{
	return std::abs(rowA - rowB) + std::abs(colA - colB);
}
}

AutoMoveChoice ChooseUnfavorableAutoMove(const AutoMoveRequest& request, const GameConfig& config)
{
	AutoMoveChoice best;
	if (!IsValid(request, request.UnitRow, request.UnitCol) || request.RemainingAction <= 0)
	{
		return best;
	}

	const int rows = static_cast<int>(request.Tiles.size());
	const int cols = static_cast<int>(request.Tiles[0].size());
	std::vector<std::vector<int>> cost(rows, std::vector<int>(cols, INT_MAX));
	std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> queue;
	cost[request.UnitRow][request.UnitCol] = 0;
	queue.push({ 0, request.UnitRow, request.UnitCol });

	while (!queue.empty())
	{
		const auto [currentCost, row, col] = queue.top();
		queue.pop();
		if (currentCost != cost[row][col])
		{
			continue;
		}

		const int* dx = (row % 2 == 0) ? evenDx : oddDx;
		const int* dy = (row % 2 == 0) ? evenDy : oddDy;
		for (int dir = 0; dir < 6; ++dir)
		{
			const int nextRow = row + dy[dir];
			const int nextCol = col + dx[dir];
			if (!IsValid(request, nextRow, nextCol))
			{
				continue;
			}

			const TerrainTile& tile = request.Tiles[nextRow][nextCol];
			if (!tile.Visible || tile.Occupied || tile.Capital)
			{
				continue;
			}

			const int moveCost = config.GetTile(tile.Type).MoveCost;
			const int nextCost = currentCost + moveCost;
			if (moveCost < 0 || nextCost > request.RemainingAction)
			{
				continue;
			}

			if (nextCost < cost[nextRow][nextCol])
			{
				cost[nextRow][nextCol] = nextCost;
				queue.push({ nextCost, nextRow, nextCol });
			}
		}
	}

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			if (row == request.UnitRow && col == request.UnitCol)
			{
				continue;
			}
			if (cost[row][col] == INT_MAX)
			{
				continue;
			}

			const TerrainTile& tile = request.Tiles[row][col];
			const int enemyDistance = GridDistance(row, col, request.NearestEnemyRow, request.NearestEnemyCol);
			const int targetDistance = GridDistance(row, col, request.TargetRow, request.TargetCol);
			const int tileMoveCost = config.GetTile(tile.Type).MoveCost;
			const int remainingAfterMove = request.RemainingAction - cost[row][col];
			const int score = enemyDistance * 100 + targetDistance * 50 + tileMoveCost * 10 - remainingAfterMove;

			if (!best.HasMove || score > best.Score || (score == best.Score && (row < best.Row || (row == best.Row && col < best.Col))))
			{
				best.HasMove = true;
				best.Row = row;
				best.Col = col;
				best.Cost = cost[row][col];
				best.Score = score;
			}
		}
	}

	return best;
}
