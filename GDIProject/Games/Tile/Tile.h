#pragma once
#include <Classes/Actor.h>
#include <Runtime/Animation/AnimationComponent.h>
#include "../Character/PlayerCharacter.h"

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

const int even_dy[6] = { 0, -1, -1, 0, 1, 1 };
const int even_dx[6] = { -1, -1, 0, 1, 0, -1 };
const int odd_dy[6] = { 0, -1, -1, 0, 1, 1 };
const int odd_dx[6] = { -1, 0, 1, 1, 1, 0 };

class ATile : public AActor
{
public:
	ATile();
	~ATile();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;

	void InitializeImage();

	ETileType m_etileType = ETileType::Desert;

	std::wstring GetTileName();
	int GetTileMoveCost();

	void SetHighlight(bool bhighlight);
	FVector2 originalPosition, highlightPosition;

	// static 함수
	static FVector2 GetTilePositionAtIndex(int row, int col);
	static FVector2 GetIndexAtPosition(FVector2 position);
	static int SearchCost(const int& startRow, const int& startCol, const int& targetRow, const int& targetCol, std::vector<std::vector<std::weak_ptr<ATile>>> map);
	static void GetReachableTiles(const int& startRow, const int& startCol, int maxCost,
		std::vector<std::vector<std::weak_ptr<ATile>>>& map,
		std::vector<std::pair<int, int>>& outReachableTiles);
	static bool IsValidIndex(int row, int col);

public:
	FVector2 InitialTileSize = FVector2(98, 120);
	static const int initialTileSizeX = 98;
	static const int initialTileSizeY = 120;

	// 이 타일 위에 있는 유닛.
	std::weak_ptr<APlayerCharacter> unit;
};