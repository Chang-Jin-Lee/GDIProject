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
	//Capital,
	MAX,
};

class ATile : public AActor
{
public:
	ATile();
	~ATile();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;

	ETileType m_etileType = ETileType::Desert;

	static FVector2 GetTilePositionAtIndex(int row, int col);
	static FVector2 GetIndexAtPosition(FVector2 position);
	std::wstring GetTileName();

public:
	FVector2 InitialTileSize = FVector2(98, 120);
	static const int initialTileSizeX = 98;
	static const int initialTileSizeY = 120;

	// 이 타일 위에 있는 유닛.
	std::shared_ptr<APlayerCharacter> unit = nullptr;
};