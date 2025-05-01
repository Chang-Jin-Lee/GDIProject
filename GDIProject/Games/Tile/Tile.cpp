#include <Runtime/Core/FIleHelper.h>
#include "Tile.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include "../Scene/PlayScene.h"
#include <queue>
#include <tuple>
#include <iostream>

ATile::ATile()
{
	bStatic = true;
}

ATile::~ATile()
{

}

void ATile::Initialize()
{
	__super::Initialize();

	bStatic = true;

	originalPosition = GetActorLocation();
	highlightPosition = GetActorLocation() + FVector2(0, -5);
	InitializeImage();
	//StaticMeshComponent->SetMeshSize(InitialTileSize.x, InitialTileSize.y);
}

void ATile::Update()
{
	__super::Update();
}

void ATile::LoadData()
{
	__super::LoadData();
}

void ATile::InitializeImage()
{
	if (g_TurnGameStateInstanceIsValid)
	{
		int** cloneInfo = g_TurnGameStateInstance->m_TileCloneInfo;
		int cloneInfoRowSize = g_TurnGameStateInstance->m_TileCloneInfoRowSize;
		int cloneInfoColsize = g_TurnGameStateInstance->m_TileCloneInfoColsize;
		Gdiplus::Bitmap* baseImage = g_TurnGameStateInstance->m_baseTileImage;

		//mesh = baseBitMap;
		if (cloneInfo)
		{
			int Type = static_cast<int>(m_etileType);
			if (cloneInfo[Type] && cloneInfoColsize > 3)
			{
				int left = cloneInfo[Type][0], top = cloneInfo[Type][1], right = cloneInfo[Type][2], bottom = cloneInfo[Type][3];
				int width = right - left;
				int height = bottom - top;

				StaticMeshComponent->SetMeshSize(width, height);
				//InitialTileSize = FVector2(width, height);
				StaticMeshComponent->mesh = baseImage->Clone(left, top, width, height, PixelFormat32bppARGB);
			}
		}
	}
}

FVector2 ATile::GetTilePositionAtIndex(int row, int col)
{
	if (row < 0 || col < 0 || row >= TILE_ROW_SIZE || col >= TILE_COL_SIZE) return FVector2(-1, -1);
	if (row % 2 == 0)
	{
		return FVector2(col * initialTileSizeX + initialTileSizeX / 2, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
	}
	else
	{
		return FVector2(col * initialTileSizeX + initialTileSizeX, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
	}
}

FVector2 ATile::GetIndexAtPosition(FVector2 position)
{
	if (position.x < 0 || position.y < 0) return FVector2(-1, -1);
	int row = (int)position.y / (int)((initialTileSizeY * 2) / 3);
	int col = 0;
	if (row % 2 == 0)
	{
		col = (int)position.x / initialTileSizeX;
	}
	else
	{
		col = ((int)position.x - initialTileSizeX / 2) / initialTileSizeX;
	}
	if (row >= TILE_ROW_SIZE || col >= TILE_COL_SIZE) return FVector2(-1, -1);

	return FVector2(row, col);
}

int ATile::SearchCost(const int& startRow, const int& startCol, const int& targetRow, const int& targetCol, std::vector<std::vector<std::shared_ptr<ATile>>> map)
{
	const int INF = 1e9;
	int cost[TILE_ROW_SIZE][TILE_COL_SIZE];
	for (int i = 0; i < TILE_ROW_SIZE; ++i)
		for (int j = 0; j < TILE_COL_SIZE; ++j)
			cost[i][j] = INF;

	//std::cout << "시작 row, col " << startRow << ' ' << startCol << '\n';
	//std::cout << "타겟 row, col " << targetRow << ' ' << targetCol << '\n';

	std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> pq;

	cost[startRow][startCol] = 0;
	pq.push({ 0, startRow, startCol });

	while (pq.empty() == false)
	{
		int currCost = std::get<0>(pq.top());
		int row = std::get<1>(pq.top());
		int col = std::get<2>(pq.top());
		pq.pop();

		//std::cout << "거쳐간 row, col " << row << ' ' << col << '\n';

		if (row == targetRow && col == targetCol)
			return currCost;

		const int* dx = (row % 2 == 0) ? even_dx : odd_dx;
		const int* dy = (row % 2 == 0) ? even_dy : odd_dy;

		for (int d = 0; d < 6; ++d)
		{
			int newRow = row + dy[d];
			int newCol = col + dx[d];

			//std::cout << "6방향 이동 newRow, newCol " << newRow << ' ' << newCol << '\n';
			if (!IsValidIndex(newRow, newCol)) continue;
			int tileCost = map[newRow][newCol]->GetTileMoveCost();
			if (tileCost == -1) continue; // 물 등 이동 불가

			//std::cout << "tile cost : " << tileCost << '\n';

			if (cost[newRow][newCol] > currCost + tileCost)
			{
				cost[newRow][newCol] = currCost + tileCost;
				//std::cout << "cost[newRow][newCol] : " << cost[newRow][newCol] << '\n';
				pq.push({ cost[newRow][newCol], newRow, newCol });
			}
		}
	}

	return -1; // 도달 불가
}

void ATile::GetReachableTiles(const int& startRow, const int& startCol, int maxCost,
	std::vector<std::vector<std::shared_ptr<ATile>>>& map,
	std::vector<std::pair<int, int>>& outReachableTiles)
{
	const int INF = 1e9;
	int cost[TILE_ROW_SIZE][TILE_COL_SIZE];
	for (int i = 0; i < TILE_ROW_SIZE; ++i)
		for (int j = 0; j < TILE_COL_SIZE; ++j)
			cost[i][j] = INF;

	std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> pq;

	cost[startRow][startCol] = 0;
	pq.push({ 0, startRow, startCol });

	while (!pq.empty())
	{
		int currCost = std::get<0>(pq.top());
		int row = std::get<1>(pq.top());
		int col = std::get<2>(pq.top());
		pq.pop();

		// 범위 내일 때만 추가
		if (currCost <= maxCost)
		{
			if (!(row == startRow && col == startCol))
			{
				outReachableTiles.emplace_back(row, col);
			}
		}
		else
		{
			continue;
		}

		const int* dx = (row % 2 == 0) ? even_dx : odd_dx;
		const int* dy = (row % 2 == 0) ? even_dy : odd_dy;

		for (int d = 0; d < 6; ++d)
		{
			int newRow = row + dy[d];
			int newCol = col + dx[d];

			if (!IsValidIndex(newRow, newCol)) continue;

			int tileCost = map[newRow][newCol]->GetTileMoveCost();
			if (tileCost == -1) continue;

			if (cost[newRow][newCol] > currCost + tileCost)
			{
				cost[newRow][newCol] = currCost + tileCost;
				pq.push({ cost[newRow][newCol], newRow, newCol });
			}
		}
	}
}


bool ATile::IsValidIndex(int row, int col)
{
	if (row < 0 || col < 0 || row >= TILE_ROW_SIZE || col >= TILE_COL_SIZE)
		return false;
	return true;
}

std::wstring ATile::GetTileName()
{
	switch (m_etileType)
	{
	case ETileType::Desert:
		return L"사막";
	case ETileType::Grassland:
		return L"초원";
	case ETileType::Hills:
		return L"언덕";
	case ETileType::Plain:
		return L"평지";
	case ETileType::Mountain:
		return L"산";
	case ETileType::Capital:
		return L"도시";
	case ETileType::MAX:
		return L"MAX";
	default:
		return L"default";
	}

}

int ATile::GetTileMoveCost()
{
	switch (m_etileType)
	{
	case ETileType::Plain: 
		return 1;
	case ETileType::Hills: 
		return 2;
	case ETileType::Mountain: 
		return 4;
	case ETileType::Desert:
		return 3;
	case ETileType::Grassland: 
		return 1;
	case ETileType::Capital: 
		return 1;
	default: 
		return -1; // 이동 불가
	}
}

void ATile::SetHighlight(bool bhighlight)
{
	if (bhighlight)
	{
		bSelected = true;
		SetActorLocation(highlightPosition);
	}
	else
	{
		bSelected = false;
		SetActorLocation(originalPosition);
	}
}
