#pragma once
#include <Classes/Actor.h>
#include <Runtime/Animation/AnimationComponent.h>

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

	static FVector2 GetTilePositionAtIndex(int row, int col)
	{
		if (row % 2 == 0)
		{
			return FVector2(col * initialTileSizeX + initialTileSizeX / 2, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
		}
		else
		{
			return FVector2(col * initialTileSizeX + initialTileSizeX, row * ((initialTileSizeY * 2) / 3) + (initialTileSizeY / 2));
		}
	}

	static FVector2 GetIndexAtPosition(FVector2 position)
	{
		int row = (int)position.y / (int)((initialTileSizeY*2)/3);
		int col = 0;
		if (row % 2 == 0)
		{
			col = (int)position.x / initialTileSizeX;
		}
		else
		{
			col = ((int)position.x - initialTileSizeX/2) / initialTileSizeX;
		}
		
		return FVector2(row, col);
	}

public:
	FVector2 InitialTileSize = FVector2(98, 120);
	static const int initialTileSizeX = 98;
	static const int initialTileSizeY = 120;
};